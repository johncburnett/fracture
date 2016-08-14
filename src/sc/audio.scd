(
/*
* fracture/src/sc/audio.sc
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

/**************************************************************************************************/
//_Init

//_GUI
~width = 1000;
~height = 600;


//_Server
s = Server.local;
o = s.options;
o.memSize_(2e6); // 2GB of RAM
o.numOutputBusChannels = 4;


//_OSC
~osc = { |msg, time, addr|
	if(msg[0] != '/status.reply') {
		"time: % sender: %\nmessage: %\n".postf(time, addr, msg);
	}
};
thisProcess.addOSCRecvFunc(~osc);
OSCFunc.trace(true);


//_Utility
~scale = { |x, inputMin, inputMax, outputMin, outputMax|
	var numer, denom, scaled;
	numer = (outputMax-outputMin) * (x-inputMin);
	denom = inputMax - inputMin;
	scaled = (numer / denom) + outputMin;
	scaled;
};


s.waitForBoot({

	/**************************************************************************************************/
	//_Synths

	SynthDef(\Input,{ |amp = 1.0|
		var in, mix;
		in = SoundIn.ar(0)!2;
		mix = in * amp;
		Out.ar(10, mix);
	}).load(s);


	SynthDef(\Process,{ |amp = 1.0, mod_scale = 1.0|
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
		mix = ([d1, d2] + (in*0.3)) * amp;

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
		Out.ar(0, snd * amp);
	}).load(s);

	SynthDef(\bass, { |f1=52.8, f2=740, amp=0.8|
		var bass, env, mix;
		bass = SinOsc.ar([52.8, 740]*0.8, 0, [2, 0.05]).mean.tanh;
		env = EnvGen.ar(Env([0, 0.5, 0.3, 0],
			[0, 0.2, 0.1]),
		doneAction:2
		)!2;
		mix = bass * env * amp;
		Out.ar(0, mix);
	}).load(s);

	SynthDef(\hNoise, { |amp=0.8|
		var hNoise, env, mix;
		hNoise = BPF.ar(
			WhiteNoise.ar(Decay2.ar(LFNoise1.ar(8, 0.5, 0.5), 0.02, 0.1) * 0.05),
			TRand.ar(12000, 15000, 1),
			0.9
		)!2;
		env = EnvGen.ar(Env([0, 0.002, 0.002, 0], [0.001, 0.2, 0.001]), doneAction:2);
		mix = hNoise * env * amp;
		Out.ar(0, mix);
	}).load(s);

	//--------------------------------------------------------------
	//_Mixer

	SynthDef(\Mixer,{ |amp = 1.0|
		var in, mix;
		in = In.ar(20, 2);
		mix = in * amp;
		Out.ar(0, mix);
	}).load(s);


	/**************************************************************************************************/
	//_Main

	~mix = Synth(\Mixer);
	~proc = Synth(\Process);
	~input = Synth(\Input);

	//_OSC
	OSCdef.new(
		\temperature, { |msg, time, addr, port|
			var val = ~scale.(msg[1], 0.0, 1.0, 0.5, 1.5);
			~proc.set(\mod_scale, val);
		},
		'sensors/temperature'
	);
	OSCdef.new(
		\piezo, { |msg, time, addr, port|
			~proc.set(\amp, msg[1]);
		},
		'sensors/sensors'
	);


	/**************************************************************************************************/
	//_GUI


});
)