/*
 * fracture/src/events.h
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

#ifndef events_h
#define events_h

#include "main.h"

class OSC_Server;

//========================================================================
class EventObject {
public:
    EventObject(OSC_Server *);
    ~EventObject(void);

    void enable(void);
    void disable(void);
    void update(ofEventArgs & args);

	ofEvent<float> newFloatEvent;
	ofEvent<int> newIntEvent;
    
    OSC_Server *server;
    vector< ofEvent<int>* > pulses;
    vector< ofEvent<float>* > modulations;
};

#endif /* events_h */
