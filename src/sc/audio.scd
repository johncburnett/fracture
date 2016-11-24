(
/*
* fracture/src/sc/audio.scd
*
* fracture
* Copyright (C) 2016 - epistrata (John Burnett + Sage Jenson)
* <http://www.epistrata.xyz/>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//_Server
s = Server.local;
o = s.options;
o.sampleRate_(44.1e3); 		// 44.1k Hz
o.memSize_(2e6); 			// 2GB of RAM
o.numOutputBusChannels_(2);	// 2ch

//_Params
~bpm = 100.0;

//_OSC
 ~ip = "127.0.0.1";
//~ip = "192.168.1.66";
~port = 7771;
~osc = NetAddr.new(~ip, ~port);

//_GUI
~display_gui = true;

//_dev
~debug_ide  = true;  // debug from scide
~debug_osc  = false; // post all osc
~run_server = true;  // start osc server

s.waitForBoot({

	/**************************************************************************************************/
	//_Init

	//_Buffers
	~cwd = thisProcess.nowExecutingPath.dirname +/+ "samples";
	~b1 = Buffer.read(s, ~cwd +/+ "1.wav");
	~b2 = Buffer.read(s, ~cwd +/+ "2.wav");
	~b3 = Buffer.read(s, ~cwd +/+ "3.wav");
	~b4 = Buffer.read(s, ~cwd +/+ "machine_drone.wav");
	~b5 = Buffer.read(s, ~cwd +/+ "boiling.wav");
	~b6 = Buffer.read(s, ~cwd +/+ "foam.aiff");
	~b7 = Buffer.read(s, ~cwd +/+ "superball.wav");
	~g1 = Buffer.alloc(s, s.sampleRate * 16); // for live granulation
	s.sync;

	/**************************************************************************************************/
	//_Synths

	//--------------------------------------------------------------
	//_Live Input

	SynthDef(\input, { |amp=0.8|
		var in, mix, chain, onsets;
		in = SoundIn.ar(0);
		mix = in!2 * amp;
		Out.ar(10, in); 	// to processing
		Out.ar(20, mix);	// to mixer

		chain = FFT(LocalBuf(2048), in);
		onsets = Onsets.kr(chain, 0.1, \rcomplex);
		SendTrig.kr(onsets);
		SendPeakRMS.kr(in, 60 , 3,  "/replyAddress");
	}).load(s);

	//--------------------------------------------------------------
	//_Sampling

	SynthDef(\sampler_1ch, { |bufnum=0, rate=1, pan=0, amp=0.8|
		var in, mix;
		in = PlayBuf.ar(1, bufnum, rate, doneAction:2);
		mix = Pan2.ar(in, pan) * amp;
		Out.ar(10, in); 	// to processing
		Out.ar(20, mix);	// to mixer
	}).load(s);


	SynthDef(\sampler_2ch, { |bufnum=0, rate=1, amp=0.8|
		var in, mix;
		in = PlayBuf.ar(2, bufnum, rate, doneAction:2);
		mix = in * amp;
		Out.ar(10, in); 	// to processing
		Out.ar(20, mix);	// to mixer
	}).load(s);

	SynthDef(\looper_1ch, { |bufnum=0, rate=1, pan=0, amp=0.8|
		var in, mix;
		in = PlayBuf.ar(1, bufnum, rate, loop:1);
		mix = Pan2.ar(in, pan) * amp;
		Out.ar(10, in); 	// to processing
		Out.ar(20, mix);	// to mixer
	}).load(s);


	SynthDef(\looper_2ch, { |bufnum=0, rate=1, amp=0.8|
		var in, mix;
		in = PlayBuf.ar(2, bufnum, rate, loop:1);
		mix = in * amp;
		Out.ar(10, in); 	// to processing
		Out.ar(20, mix);	// to mixer
	}).load(s);

	//--------------------------------------------------------------
	//_Processing

	SynthDef(\melt,{ |amp = 1.0, mod_scale = 1.0|
		var in, d1, d2, pan, mix;

		in = In.ar(10, 2);

		d1 = Array.fill(6, {
			BPF.ar(
				CombC.ar(
					in[0],
					15.0,
					LinLin.kr(
						SinOsc.kr(rrand(1/16, 1/32) * mod_scale),
						-1.0,
						1.0,
						rrand(4.0, 6.0),
						rrand(8.0, 12.0) ),
					10),
				rrand(200, 10000),
				0.3)
		} ).sum / 2;

		d2 = Array.fill(6, {
			BPF.ar(
				CombC.ar(
					in[1],
					15.0,
					LinLin.kr(
						SinOsc.kr(rrand(1/16, 1/32) * mod_scale),
						-1.0,
						1.0,
						rrand(4.0, 6.0),
						rrand(8.0, 12.0) ),
					10),
				rrand(200, 10000),
				0.3)
		} ).sum / 2;

		pan = LinLin.kr(SinOsc.kr(0.125), -1.0, 1.0, -0.6, 0.6);
		d1 = Pan2.ar(d1, pan);
		d2 = Pan2.ar(d2, -1.0*pan);
		mix = ([d1, d2]*4) * amp;

		Out.ar(20, mix);	// to mixer
	}).load(s);

	//--------------------------------------------------------------
	//_Grains

	SynthDef(\vGrain, { |bufnum=0, startPos=0.0, amp=0.3, len=0.4, rate=1.0, vary=0.2, scale=1.0|
		var in, grains, mix;

		in = PlayBuf.ar(1, bufnum, loop:1) * (1.0 - scale) * 0.5;

		grains = Array.fill(2, {
			Array.fill(8, {
				PlayBuf.ar(
					1,
					bufnum,
					BufRateScale.kr(bufnum) * ((Rand(0.0, 1.0)*vary) + 0.5) * rate,
					1,
					BufFrames.ir(bufnum)*startPos,
					1,
				) * (EnvGen.kr(Env.perc(0.01, len * ((Rand(0.0, 1.0)*vary) + 0.5)).circle)-0.001) * scale;
			});
		});

		mix = (in + grains) * amp;
		Out.ar(20, mix);	// to mixer
	}).load(s);


	SynthDef(\grainFlurry, { |bufnum=0, startPos=0.0, amp=0.3, len=0.4, rate=1.0, vary=0.2, scale=1.0|
		var in, grains, env, mix;

		in = PlayBuf.ar(1, bufnum, loop:1) * (1.0 - scale) * 0.5;

		grains = Array.fill(2, {
			Array.fill(8, {
				PlayBuf.ar(
					1,
					bufnum,
					BufRateScale.kr(bufnum) * ((Rand(0.0, 1.0)*vary) + 0.5) * rate,
					1,
					BufFrames.ir(bufnum)*startPos,
					1,
				) * (EnvGen.kr(Env.perc(0.01, len * ((Rand(0.0, 1.0)*vary) + 0.5)))-0.001) * scale;
			});
		});

		env = EnvGen.ar(Env([1, 1], [len*vary*rate]), doneAction:2);

		mix = (in + grains) * amp * env * 0.5;
		Out.ar(20, mix);	// to mixer
	}).load(s);

	// code by Martin Hünniger
	// start
	SynthDef( \liveGrains, { |amp=0.0, bufnum, pdisp = 0.01, tdisp = 0.02|
		var frames, sRate, recHead, rec, in, overlap, trig,
		out, eff, rate, dur, pan, center;

		in      = In.ar( 10 );

		frames  = BufFrames.kr( bufnum );
		sRate   = BufSampleRate.kr( bufnum );
		recHead = Phasor.ar( 1, BufRateScale.kr( bufnum), 0, frames, 0 );
		rec     = BufWr.ar( in, bufnum, recHead );
		// overlap = MouseY.kr( 0.8, 10, 1 );
		overlap = 0.9;
		dur     = 0.6 + SinOsc.kr( 0.1, mul:0.4 );
		pdisp   = pdisp + 1;
		trig    = Impulse.ar( overlap/dur );
		// rate    = MouseX.kr( -6, 6, 0 ) * TExpRand.kr( pdisp.reciprocal, pdisp, trig );
		rate    = -4 * TExpRand.kr( pdisp.reciprocal, pdisp, trig );
		center  = Wrap.ar( recHead - (dur * rate * 0.5), 0, frames-1 ) - TRand.kr( 0, tdisp, trig );
		pan     = WhiteNoise.kr(0.7);

		out     = TGrains.ar( 2, trig, bufnum, rate, center/sRate, dur, pan, 1.0, 2 );
		eff     = out;
		6.do({
			eff = LPF.ar( AllpassC.ar( eff, 0.5, 0.1.rand, 1), 8000 );
		});

		out = (out + eff) * amp;
		Out.ar( 20, out );
	}).send(s);
	// end

	//--------------------------------------------------------------
	//_Rhythm

	SynthDef(\snd, { |f1=44, f2=90, pan=0, amp=0.0|
		var snd = LPF.ar(
			SinOsc.ar(f1.midicps, 0, 0.5) + SinOsc.ar(f2.midicps, 0, 0.6),
			32.midicps * 2
		) + HPF.ar(LPF.ar(WhiteNoise.ar(0.008), 12000), 2400);
		snd = Pan2.ar(
			snd * EnvGen.ar(Env([0, 1, 0.6, 0], [0.0001, 0.4, 0.01]), doneAction:2),
			pan
		);
		Out.ar(20, snd * amp * 3);	// to mixer
	}).load(s);


	SynthDef(\bass, { |f1=52.8, f2=740, amp=0.0|
		var bass, env, mix;
		bass = SinOsc.ar([52.8, 740]*0.8, 0, [2, 0.05]).mean.tanh;
		env = EnvGen.ar(Env([0, 0.5, 0.3, 0],
			[0, 0.2, 0.1]),
		doneAction:2
		)!2;
		mix = bass * env * amp * 5;
		Out.ar(20, mix);	// to mixer
	}).load(s);


	SynthDef(\hNoise, { |amp=0.0|
		var hNoise, env, mix;
		env = EnvGen.ar(Env([0, 1.0, 0], [0.0005, 0.01]), doneAction:2);
		hNoise = BPF.ar(
			WhiteNoise.ar(Decay2.ar(env * LFNoise1.ar(8, 0.5, 0.5), 0.02, 0.1) * 0.05),
			TRand.ar(12000, 15000, 1),
			0.9
		)!2;
		mix = hNoise * amp;
		Out.ar(20, mix);	// to mixer
	}).load(s);


	SynthDef(\click, { |pan=0, amp=0.0|
		var click, env, mix;
		env = EnvGen.ar(Env([0, 1, 0], [0.001, 0.001]), doneAction:2);
		click = BPF.ar(
			PinkNoise.ar(Decay.ar(env, 0.001 * LFNoise1.ar(4).abs)),
			15000,
			0.9,
			25 * LFNoise1.ar(8).range(0, 1)
		).fold(-1, 1);
		click = Pan2.ar(click, pan);
		mix = click * amp;
		Out.ar(20, mix);	// to mixer
	}).load(s);

	//--------------------------------------------------------------
	//_Mixer

	SynthDef(\mixer,{ |amp = 0.0|
		var in, mix;
		in = In.ar(20, 2);
		mix = Limiter.ar(MidEQ.ar(in, 15000, 0.7, 8)) * amp;
		SendPeakRMS.kr(in, 61, 3, '/rms');
		Out.ar(0, mix);
	}).load(s);

	/**************************************************************************************************/
	//_Sequencing

	TempoClock.default.tempo = ~bpm/60.0;

	//--------------------------------------------------------------
	//_Percussive

	~r1 = Pbind(
		\instrument, \snd,
		\dur, Prand([Pseq((1/2)!2), Pseq([1/4, 1/4, 1/4])], inf),
		\pan, Prand([-1, 0, 1], inf),
		\sendMsg, Pfunc({|ev|  ~osc.sendMsg("/sines", 1.0)}),
	).asEventStreamPlayer;

	~r2 = Pbind(
		\instrument, \hNoise,
		\dur, Prand([Pseq((1)!4), Pseq([1/2, 1/2, 1/2]), Pseq([1/8, 1/8])], inf),
		\pan, Pseq([-1, 0, 1], inf),
		\sendMsg, Pfunc({|ev|  ~osc.sendMsg("/noise", 1.0)}),
	).asEventStreamPlayer;

	~r3 = Pbind(
		\instrument, \click,
		\dur, Prand([Pseq((1/4)!4), Pseq([1/8, 1/8, 1/8])], inf),
		\pan, Pseq([-1, 0, 1], inf),
		\sendMsg, Pfunc({|ev|  ~osc.sendMsg("/click", 1.0)}),
	).asEventStreamPlayer;

	~r4 = Pbind(
		\instrument, \bass,
		\dur, Prand([Pseq([1/8, 1/2, 1/2]), 1, 3/2, 3/4, 1/4, 1/8], inf),
		\pan, Pseq([-1, 0, 1], inf),
		\sendMsg, Pfunc({|ev|  ~osc.sendMsg("/bass", 1.0)}),
	).asEventStreamPlayer;

	~r5 = Pbind(
		\instrument, \sampler_1ch,
		\bufnum, Pseq([~b1.bufnum], inf),
		\dur, Prand([1, 4, 3/2, 6], inf),
		\pan, Pseq([-1, 0, 1], inf),
		\amp, 0.2,
	).asEventStreamPlayer;

	~r6 = Pbind(
		\instrument, \sampler_1ch,
		\bufnum, ~b2.bufnum,
		\dur, Prand([Pseq((1/2)!2), Pseq([3/4, 1/2, 3/4])]*8, inf),
	).asEventStreamPlayer;

	~flurry = Routine({
		Pbind(
			\instrument, \snd,
			\dur, Prand([Pseq((1/2)!2), Pseq([1/4, 1/4, 1/4])], 5),
			\pan, Prand([-1, 0, 1]),
			\sendMsg, Pfunc({|ev|  ~osc.sendMsg("/sines", 1.0)}),
		).play;

		Pbind(
			\instrument, \hNoise,
			\dur, Prand([Pseq([3/2]), Pseq([1/2])], 2),
			\pan, Pseq([-1, 0, 1]),
			\sendMsg, Pfunc({|ev|  ~osc.sendMsg("/noise", 1.0)}),
		).play;

		Pbind(
			\instrument, \click,
			\dur, Prand([Pseq([1/4]), Pseq([1/8, 1/8, 1/8])], 5),
			\pan, Pseq([-1, 0, 1]),
			\sendMsg, Pfunc({|ev|  ~osc.sendMsg("/click", 1.0)}),
		).play;

		Pbind(
			\instrument, \bass,
			\dur, Prand([1/2], 1),
			\pan, Pseq([-1, 0, 1]),
			\sendMsg, Pfunc({|ev|  ~osc.sendMsg("/bass", 1.0)}),
		).play;
	});

	//--------------------------------------------------------------
	//_Modulations

	~mod_melt = Routine({
		var v1 = 0.8;
		inf.do({ |i|
			v1 = 1 - (v1 * rrand(0.0, 0.99));
			~melt.set(\mod_scale, v1);
			~osc.sendMsg("/mod0", v1);
			rrand(1.0, 4.0).wait;
		});
	});

	~mod_grains = Routine({
		var spec1, spec2;
		spec1 = ControlSpec(0.2, 0.00001, \lin, 0.0001);
		spec2 = ControlSpec(0.5, 1, \lin, 0.0001);
		100.do({ |i|
			var v1, v2;
			v1 = spec1.map(i/100.0);
			v2 = spec2.map(i/100.0);
			~grains.set(\len, v1);
			// ~grains.set(\vary, v2);
			~osc.sendMsg("/mod1", i/100.0);
			(30.0/100.0).wait;
		});
	});

	//--------------------------------------------------------------
	//_Gestures

	~deep = Routine({
		var deep = Synth(\sampler_1ch, [\bufnum, ~b2.bufnum, \amp, 0.8]);
	});

	~burst = Routine({
		Synth(\grainFlurry, [\bufnum, ~b1.bufnum, \dur, 0.5, \vary, 2.0]);
	});

	~splatter = Routine({
		var grains, spec;
		grains = Synth(\vGrain, [\bufnum, ~b1.bufnum, \dur, 0.2, \vary, 1.0, \rate, 0.6]);
		spec = ControlSpec(0.2, 0.00001, \lin, 0.000001);
		10.do({ |i|
			var v1 = spec.map(i/10.0);
			grains.set(\len, v1);
			(3.0/10.0).wait;
		});
		grains.free;
		~burst.play;
		5.wait;
		~burst.reset;
	});

	//--------------------------------------------------------------
	//_Utilities

	~routines = [
		~r1, ~r2, ~r3, ~r4, ~r5, ~r6, // patterns
		~mod_melt,					  // modulations
		~deep, ~burst, ~splatter,	  // gestures
		~world1, ~world2, ~world3	  // worlds
	];

	~reset = {
		for(0, ~routines.size-1, { |i|
			~routines[i]
			.stop
			.reset;
		});
	};

	~reset_amp = {
		~melt.set([\amp, 0.0]);
		~machine.set([\amp, 0.0]);
		~grains.set([\amp, 0.0]);
	};

	~fadein = { |synth, amp=0.8, len=10.0, res=50|
		Routine({
			res.do({ |i|
				synth.set(\amp, i/res*amp);
				(len/res).wait;
			});
		}).play;
	};

	~fadeout = { |synth, amp=0.8, len=10.0, res=50|
		Routine({
			res.do({ |i|
				synth.set(\amp, amp - (i/res*amp));
				(len/res).wait;
			});
		}).play;
	};

	~interp = { |synth, param, start, end, len=10.0, res=100|
		Routine({
			var spec = ControlSpec(start, end, \lin, 0.00001);
			res.do({ |i|
				var v1 = spec.map(i/res);
				synth.set(param, v1);
				~osc.sendMsg("/mod0", i/res * 0.1);
				(len/res).wait;
			});
		}).play;
	};

	//--------------------------------------------------------------
	//_Worlds

	~world1 = Routine({
		~reset.();

		~r1.play;
		// ~r3.play;
		~mod_melt.play;
		// ~deep.play;

		Routine({
			30.wait;
			~r5.play;
		});

		~fadein.(~machine, amp:0.2, len:10.0);
		~fadein.(~melt, amp:0.4, len:30.0);
		// ~fadeout.(~grains, amp:0.8, len:10.0);

		"entering world 1".postln;
	});

	~world2 = Routine({
		~reset.();

		// ~r1.play;
		// ~r3.play;

		~interp.(~grains, \dur, 0.2, 0.00001, len:20);
		~interp.(~grains, \vary, 1, 0.5, len:20);
		~interp.(~grains, \rate, 0.8, 0.25, len:20);

		~fadein.(~grains, amp:0.6, len:20.0);
		// ~fadeout.(~machine, amp:0.2, len:10.0);
		~fadeout.(~melt, amp:0.4, len:10.0);

		"entering world 2".postln;
	});

	~world3 = Routine({
		~reset.();

		~r1.play;
		~r2.play;
		~r3.play;
		~r4.play;

		~interp.(~grains, \dur, 0.2, 0.00001, len:60);

		~burst.play;
		~burst.reset;

		~fadeout.(~grains, amp:0.6, len:60.0);

		"entering world 3".postln;
	});

	// grains
	~world4 = Routine({
		~reset.();

		~fadein.(~machine,    amp:0.2, len:10.0);
		~fadein.(~liveGrains, amp:0.5, len:10.0);
		// ~fadeout.(~grains, amp:0.6, len:60.0);

		"entering world 4".postln;
	});

	/**************************************************************************************************/
	//_Main

	//_wait for server
	s.sync;

	//_mixer
	~mix = Synth(\mixer);

	//_processes
	~melt = Synth(\melt, [\mod_scale, 0.2, \amp, 0.0]);
	~liveGrains = Synth(\liveGrains, [\bufnum, ~g1.bufnum, \amp, 0.0]);

	//_textures
	// ~machine = Synth(\looper_2ch, [\bufnum, ~b4.bufnum, \amp, 0.0]);
	~machine = Synth(\looper_2ch, [\bufnum, ~b7.bufnum, \amp, 0.2]);
	~grains = Synth(\vGrain, [\bufnum, ~b5.bufnum, \dur, 0.2, \vary, 0.5, \rate, 0.25, \amp, 0.0]);

	//--------------------------------------------------------------
	//_OSC

	if( (~debug_osc),
		{ OSCFunc.trace(true) },
		{ OSCFunc.trace(false) });
`
	OSCFunc({ |msg, time|
		~osc.sendMsg('/rms', msg[4]);
	},'/rms', s.addr);

	/**************************************************************************************************/
	//_GUI

	// window config
	w = Window("audio", Rect(
		Window.screenBounds.width - 200,
		Window.screenBounds.height,
		200,
		400
	))
	.background_(Color(0.1, 0.1, 0.1));

	// audio controls
	~audio_toggle = Button(w, Rect(10, 10, 180, 80))
	.states_([
		["audio off", Color.black, Color.red],
		["audio on", Color.black, Color.green]
	])
	.action_({ |state|
		if(state.value == 0, { ~mix.set(\amp, 0.0) });
		if(state.value == 1, { ~mix.set(\amp, 1.0) });
	});

	~close_button = Button(w, Rect(10, 110, 180, 80))
	.states_([
		["quit", Color.black, Color.blue]
	])
	.action_({ |state|
		"exiting fracture audio\n".postln;
		s.quit;
		~osc.disconnect;
		w.close;
		if(not(~debug_ide), {
			thisProcess.shutdown;
			0.exit;
		});
	});

	// world triggers
	~toggle_w1 = Button(w, Rect(10, 210, 180, 40))
	.states_([
		[" world 1: inactive", Color.black, Color.white],
		[" world 1: active  ", Color.black, Color.cyan]
	])
	.action_({ |state|
		if(state.value == 0, { ~reset.(); });
		if(state.value == 1, {
			~world1.play;
			~toggle_w2.value_(0);
			~toggle_w3.value_(0);
			~toggle_w4.value_(0);
		});
	});

	~toggle_w2 = Button(w, Rect(10, 250, 180, 40))
	.states_([
		[" world 2: inactive", Color.black, Color.white],
		[" world 2: active  ", Color.black, Color.cyan]
	])
	.action_({ |state|
		if(state.value == 0, { ~reset.(); });
		if(state.value == 1, {
			~world2.play;
			~toggle_w1.value_(0);
			~toggle_w3.value_(0);
			~toggle_w4.value_(0);
		});
	});

	~toggle_w3 = Button(w, Rect(10, 290, 180, 40))
	.states_([
		[" world 3: inactive", Color.black, Color.white],
		[" world 3: active  ", Color.black, Color.cyan]
	])
	.action_({ |state|
		if(state.value == 0, { ~reset.(); });
		if(state.value == 1, {
			~world3.play;
			~toggle_w1.value_(0);
			~toggle_w2.value_(0);
			~toggle_w4.value_(0);
		});
	});

	~toggle_w4 = Button(w, Rect(10, 330, 180, 40))
	.states_([
		[" world 4: inactive", Color.black, Color.white],
		[" world 4: active  ", Color.black, Color.cyan]
	])
	.action_({ |state|
		if(state.value == 0, { ~reset.(); });
		if(state.value == 1, {
			~world4.play;
			~toggle_w1.value_(0);
			~toggle_w2.value_(0);
			~toggle_w3.value_(0);
		});
	});

	// display gui
	if( (~display_gui), { w.front });

	//--------------------------------------------------------------
	//_Status

	"".postln;
	"fracture audio is now running\n".postln;
	"audio".postln;
	("sampling rate: " + o.sampleRate).postln;
	("allocated RAM: " + (o.memSize/1000000) + "GB").postln;
	("# output channels: " + o.numOutputBusChannels + "\n").postln;
	"OSC".postln;
	("ip: " + ~ip).postln;
	("port: " + ~port + "\n").postln;
});
)