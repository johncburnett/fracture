//
//  osc_server.cpp
//  palo_duro
//
//  Created by John Burnett on 6/2/16.
//
//

#include "osc_server.hpp"


//--------------------------------------------------------------
OSC_Server::OSC_Server(int osc_port) {
    PORT = osc_port;
    
    temperature = 0;
    vibration = 0;
    
    cout << "listening on port " << PORT << endl;
    receiver.setup(PORT);
    
	current_msg_string = 0;
    ofBackground(30, 30, 130);
}


//--------------------------------------------------------------
OSC_Server::update(void) {
	// hide old messages
	for(int i = 0; i < NUM_MSG_STRINGS; i++){
		if(timers[i] < ofGetElapsedTimef()){
			msg_strings[i] = "";
		}
	}

	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(m);
        
        // check for temperature sensor
		if(m.getAddress() == "/sensors/temperature"){
            temperature = m.getArgAsInt32(0);
		}
		// check for piezo sensor
		if(m.getAddress() == "/sensors/piezo"){
            vibration = m.getArgAsInt32(0);
		}
		else{
			// unrecognized message: display on the bottom of the screen
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for(int i = 0; i < m.getNumArgs(); i++){
				// get the argument type
				msg_string += m.getArgTypeName(i);
				msg_string += ":";
				// display the argument - make sure we get the right type
				if(m.getArgType(i) == OFXOSC_TYPE_INT32){
					msg_string += ofToString(m.getArgAsInt32(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
					msg_string += ofToString(m.getArgAsFloat(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
					msg_string += m.getArgAsString(i);
				}
				else{
					msg_string += "unknown";
				}
			}
			// add to the list of strings to display
			msg_strings[current_msg_string] = msg_string;
			timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
			current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
			// clear the next line
			msg_strings[current_msg_string] = "";
		}
	}
}