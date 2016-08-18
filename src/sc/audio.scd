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
// o.sampleRate_(96e3); 		// 96k Hz
o.memSize_(2e6); 			// 2GB of RAM
o.numOutputBusChannels_(2);	// 2ch

//_Params
~bpm = 100.0;

//_OSC
~ip = "127.0.0.1";
~port = 7771;

//_GUI
~width = 1000;
~height = 600;
~display_gui = false;

//_dev
~debug_osc = false;

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
	s.sync;

	/**************************************************************************************************/
	//_Synths

	//--------------------------------------------------------------
	//_Sampling

	SynthDef(\sampler_1ch, { |bufnum=0, rate=1, pan=0, amp=0.8|
		var in, mix;
		in = PlayBuf.ar(1, bufnum, rate, 1, doneAction:2);
		mix = Pan2.ar(in, pan) * amp;
		Out.ar(10, in); 	// to processing
		Out.ar(20, mix);	// to mixer
	}).load(s);


	SynthDef(\sampler_2ch, { |bufnum=0, rate=1, amp=0.8|
		var in, mix;
		in = PlayBuf.ar(2, bufnum, rate, 1, doneAction:2);
		mix = in * amp;
		Out.ar(10, in); 	// to processing
		Out.ar(20, mix);	// to mixer
	}).load(s);

	SynthDef(\looper_1ch, { |bufnum=0, rate=1, pan=0, amp=0.8|
		var in, mix;
		in = PlayBuf.ar(1, bufnum, rate, 1, loop:1);
		mix = Pan2.ar(in, pan) * amp;
		Out.ar(10, in); 	// to processing
		Out.ar(20, mix);	// to mixer
	}).load(s);


	SynthDef(\looper_2ch, { |bufnum=0, rate=1, amp=0.8|
		var in, mix;
		in = PlayBuf.ar(2, bufnum, rate, 1, loop:1);
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

		Out.ar(20, mix);
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
		Out.ar(20, mix);
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
		Out.ar(20, mix);
	}).load(s);

	//--------------------------------------------------------------
	//_Rhythm

	SynthDef(\snd, { |f1=44, f2=90, pan=0, amp=0.8|
		var snd = LPF.ar(
			SinOsc.ar(f1.midicps, 0, 0.5) + SinOsc.ar(f2.midicps, 0, 0.6),
			32.midicps * 2
		) + HPF.ar(LPF.ar(WhiteNoise.ar(0.008), 12000), 2400);
		snd = Pan2.ar(
			snd * EnvGen.ar(Env([0, 1, 0.6, 0], [0.0001, 0.4, 0.01]), doneAction:2),
			pan
		);
		Out.ar(20, snd * amp * 2);
	}).load(s);


	SynthDef(\bass, { |f1=52.8, f2=740, amp=0.8|
		var bass, env, mix;
		bass = SinOsc.ar([52.8, 740]*0.8, 0, [2, 0.05]).mean.tanh;
		env = EnvGen.ar(Env([0, 0.5, 0.3, 0],
			[0, 0.2, 0.1]),
		doneAction:2
		)!2;
		mix = bass * env * amp * 5;
		Out.ar(20, mix);
	}).load(s);


	SynthDef(\hNoise, { |amp=0.8|
		var hNoise, env, mix;
		env = EnvGen.ar(Env([0, 1.0, 0], [0.0005, 0.01]), doneAction:2);
		hNoise = BPF.ar(
			WhiteNoise.ar(Decay2.ar(env * LFNoise1.ar(8, 0.5, 0.5), 0.02, 0.1) * 0.05),
			TRand.ar(12000, 15000, 1),
			0.9
		)!2;
		mix = hNoise * amp;
		Out.ar(20, mix);
	}).load(s);


	SynthDef(\click, { |pan=0, amp=0.8|
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
		Out.ar(20, mix);
	}).load(s);

	//--------------------------------------------------------------
	//_Mixer

	SynthDef(\mixer,{ |amp = 1.0|
		var in, mix;
		in = In.ar(20, 2);
		mix = Limiter.ar(MidEQ.ar(in, 15000, 0.7, 8)) * amp;
		Out.ar(0, mix);
	}).load(s);

	/**************************************************************************************************/
	//_Sequencing

	TempoClock.default.tempo = ~bpm/60.0;

	//--------------------------------------------------------------
	//_Streams

	~machine_drone = Routine({
		var drone = Synth(\sampler_2ch, [\bufnum, ~b4.bufnum, \amp, 0.2]);
	});

	~grains = Routine({
		var grains = ~grain = Synth(\vGrain, [\bufnum, ~b1.bufnum, \len, 0.2, \vary, 1.0]);
	});

	//--------------------------------------------------------------
	//_Percussive

	~r1 = Routine({
		Pbind(
			\instrument, \click,
			\dur, Prand([Pseq((1/4)!4), Pseq([1/8, 1/8, 1/8])], inf),
			\pan, Pseq([-1, 0, 1], inf)
		).play;

		Pbind(
			\instrument, \snd,
			\dur, Prand([Pseq((1/2)!4), Pseq([1/4, 1/4, 1/4])], inf),
			\pan, Prand([-1, 0, 1], inf)
		).play;
	});

	~r2 = Routine({
		Pbind(
			\instrument, \sampler_1ch,
			\bufnum, Pseq([~b1.bufnum, ~b2.bufnum], inf),
			\dur, Prand([1/2, 1, 3/2, 3/4]*2, inf),
			\pan, Pseq([-1, 0, 1], inf)
		).play;
	});

	~r3 = Routine({
		Pbind(
			\instrument, \bass,
			\dur, Prand([1/2, 1, 3/2, 3/4], inf),
			\pan, Pseq([-1, 0, 1], inf)
		).play;
	});

	~r4 = Routine({
		Pbind(
			\instrument, \hNoise,
			\dur, Prand([Pseq((1)!4), Pseq([1/2, 1/2, 1/2])], inf),
			\pan, Pseq([-1, 0, 1], inf)
		).play;
	});

	~r5 = Routine({
		Pbind(
			\instrument, \sampler_1ch,
			\bufnum, ~b2.bufnum,
			\dur, Prand([Pseq((1/2)!4), Pseq([3/4, 1/2, 3/4])], inf),
		).play;
	});

	//--------------------------------------------------------------
	//_Modulations

	~mod_melt = Routine({
		var val = 0.8;
		inf.do({ |i|
			val = 1 - (val * rrand(0.0, 0.99));
			~melt.set(\mod_scale, val);
			rrand(1.0, 4.0).wait;
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
		spec = ControlSpec(0.2, 0.00001, \lin, 0.0001);
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
	//_Sound Worlds

	~world1 = Routine({
		~r1.play;
		~machine_drone.play;
		~mod_melt.play;
		~deep.play;
	});

	//--------------------------------------------------------------
	//_Utilities

	~routines = [
		~machine_drone, ~grains,	// streams
		~r1, ~r2, ~r3, ~r4, ~r5,	// percussive
		~mod_melt,					// modulations
		~deep, ~burst, ~splatter,	// gestures
		~world1						// worlds
	];

	~reset = { for(0, ~routines.size-1, { |i| ~routines[i].reset }) };

	/**************************************************************************************************/
	//_Main

	~mix = Synth(\mixer);
	~melt = Synth(\melt, [\mod_scale, 0.2, \amp, 0.2]);

	~world1.play;

	//--------------------------------------------------------------
	//_OSC

	~osc = NetAddr.new(~ip, ~port);

	if( (~debug_osc),
		{ OSCFunc.trace(true) },
		{ OSCFunc.trace(false) });

	OSCdef.new(
		\temperature, { |msg, time, addr, port|
			var spec, val;
			spec = ControlSpec(0.5, 1.5, \lin);
			val = spec.(msg[1]);
			~proc.set(\mod_scale, val);
		},
		'sensors/temperature'
	);

	OSCdef.new(
		\piezo, { |msg, time, addr, port|
			~proc.set(\amp, msg[1]);
		},
		'sensors/piezo'
	);

	/**************************************************************************************************/
	//_GUI

	w = Window("GUI", Rect(
		Window.screenBounds.width/2 - (~width/2),
		Window.screenBounds.height/2 - (~height/2),
		~width,
		~height
	))
	.background_(Color(0.1, 0.1, 0.1))
	.alwaysOnTop_(true)
	.onClose_({ Server.killAll });

	if( (~display_gui), { w.front });

});
)
