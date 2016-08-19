/*
 * fracture/src/events.cpp
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

#include "events.h"

//========================================================================
EventObject::EventObject(OSC_Server *_server) {
    server = _server;
    
    for(int i = 0; i < 4; i++) {
        pulses.push_back(new ofEvent<int>());
    }
}

void EventObject::enable(void) {
    ofAddListener(ofEvents().update, this, &EventObject::update);
}

void EventObject::disable(void) {
    ofRemoveListener(ofEvents().update, this, &EventObject::update);
}

void EventObject::update(ofEventArgs &args) {
    ofNotifyEvent(*pulses[0], server->sines, this);
    ofNotifyEvent(*pulses[1], server->noise, this);
    ofNotifyEvent(*pulses[2], server->click, this);
    ofNotifyEvent(*pulses[3], server->bass,  this);
}
