/*
 * fracture/src/main.h
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef main_h
#define main_h

#define FRAMERATE 60

#include "ofMain.h"
#include "ofEvents.h"

#include <libiomp/omp.h>
#include <math.h>
#include <vector>
using namespace std;

//========================================================================
//_Utilities
double mean(vector<double> A);
double std_dev(vector<double> A);

void run_supercollider(void);

#endif /* main_h */
