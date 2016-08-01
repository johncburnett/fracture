(
/**
*
* palo_duro - John Burnett + Sage Jenson (c)2016
*
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
					LinLin.kr( SinOsc.kr(rrand(1/16, 1/32) * mod_scale), -1.0, 1.0, rrand(4.0, 6.0), rrand(8.0, 12.0) ),
					10),
				rrand(200, 10000),
				0.3)
		} ).sum / 2;

		d2 = Array.fill(6, {
			BPF.ar(
				CombC.ar(
					in[1],
					15.0,
					LinLin.kr( SinOsc.kr(rrand(1/16, 1/32) * mod_scale), -1.0, 1.0, rrand(4.0, 6.0), rrand(8.0, 12.0) ),
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