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
    
    click = 0;
    sines = 0;
    
    cout << "listening on port " << PORT << "\n";
    receiver.setup(PORT);
    
    current_msg_string = 0;
}

//--------------------------------------------------------------
void OSC_Server::update(void) {
    
    while(receiver.hasWaitingMessages()) {

        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        if(m.getAddress() == "/click") {
            click = m.getArgAsInt32(0);
        }
        else if(m.getAddress() == "/sines") {
            sines = m.getArgAsInt32(0);
        }
        else if(m.getAddress() == "/bass") {
            bass = m.getArgAsInt32(0);
        }
        else if(m.getAddress() == "/noise") {
            noise = m.getArgAsInt32(0);
        }
        else {
            ;
        }
    }
}