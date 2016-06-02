//
//  osc_server.hpp
//  palo_duro
//
//  Created by John Burnett on 6/2/16.
//
//

#ifndef osc_server_hpp
#define osc_server_hpp

#include <stdio.h>
#include "ofxOsc.h"
#include "ofMain.h"

#define NUM_MSG_STRINGS 20

class OSC_Server {
public:
		ofTrueTypeFont font;
		ofxOscReceiver receiver;

		int current_msg_string;
		string msg_strings[NUM_MSG_STRINGS];
		float timers[NUM_MSG_STRINGS];

		int temperature, vibration;
        int PORT;
};

#endif /* osc_server_hpp */
