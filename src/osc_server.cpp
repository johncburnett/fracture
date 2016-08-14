/*
 * fracture/src/osc_server.cpp
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

#include "osc_server.h"

//--------------------------------------------------------------
OSC_Server::OSC_Server(int osc_port) {
    PORT = osc_port;
    
    temperature = 0.0;
    vibration = 0.0;
    
    cout << "listening on port " << PORT << endl;
    receiver.setup(PORT);
    
    current_msg_string = 0;
}


//--------------------------------------------------------------
void OSC_Server::update(void) {
    // hide old messages
    for(int i = 0; i < NUM_MSG_STRINGS; i++) {
        if(timers[i] < ofGetElapsedTimef()) {
            msg_strings[i] = "";
        }
    }
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()) {
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        // check for temperature sensor
        if(m.getAddress() == "/sensors/temperature") {
            temperature = m.getArgAsInt32(0);
        }
        // check for piezo sensor
        if(m.getAddress() == "/sensors/piezo") {
            vibration = m.getArgAsInt32(0);
        }
        else {
            // unrecognized message: display on the bottom of the screen
            string msg_string;
            msg_string = m.getAddress();
            msg_string += ": ";
            for(int i = 0; i < m.getNumArgs(); i++) {
                // get the argument type
                msg_string += m.getArgTypeName(i);
                msg_string += ":";
                // display the argument
                if(m.getArgType(i) == OFXOSC_TYPE_INT32){
                    msg_string += ofToString(m.getArgAsInt32(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT) {
                    msg_string += ofToString(m.getArgAsFloat(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_STRING) {
                    msg_string += m.getArgAsString(i);
                }
                else {
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
