/*
 * fracture/src/osc_server.h
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

#ifndef osc_server_h
#define osc_server_h

#include "ofxOsc.h"
#include "main.h"

#define NUM_MSG_STRINGS 20
#define OSC_IN 7771
#define OSC_OUT 57120

//========================================================================
class OSC_Server {
public:
    OSC_Server(int);
    void update(void);
    
    ofxOscReceiver receiver;
    
    int current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
    
    int PORT;
    float click, sines, bass, noise;
    float mod0, mod1;
    float rms;
    
    vector< ofEvent<float>* > pulses;
    vector< ofEvent<float>* > mods;
};

#endif /* osc_server_h */
