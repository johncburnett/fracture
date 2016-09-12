/*
 * fracture/src/main.cpp
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

#include <math.h>
#include "ofApp.h"
#include "main.h"

//========================================================================
int main( ){
    // OF_GAME_MODE seems to alleviate our fullscreen woes
	ofSetupOpenGL(WIDTH, HEIGHT,OF_WINDOW);
	ofRunApp(new ofApp());
}

//========================================================================
//_Utilities

double mean(std::vector<double> A) {
    double avg, sum;
    sum = 0;
    for(int i = 0; i < A.size(); i++) {
        sum += A[i];
    }
    avg = sum / A.size();
    return avg;
}

double std_dev(std::vector<double> A) {
    double sd = 0;
    double avg = mean(A);
    for(int i = 0; i < A.size(); i++) {
        sd += abs(A[i] - avg);
    }
    return sd;
}

void run_supercollider(void) {
    pid_t pid = fork();

    if(pid == 0) {
        std::system("../../../../sc \
               ../../../../src/sc/audio.scd \
               > ../../../../log/sc_log.txt");
    }
}