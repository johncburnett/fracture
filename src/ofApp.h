/*
 * fracture/src/ofApp.h
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

#pragma once

#include "ofMain.h"
#include "main.h"


class ofApp : public ofBaseApp{

	public:
    	void setup();
		void update();
		void draw();

        void audioIn(float *input, int buffersize, int nChannels);
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    Kernel *kernel;
    Stream *stream0, *stream1;
    ofxBlur blur;
    ofFbo * fboi;
    
    Still *img0, *img1, *img2, *img3, *img4, *img5;
    Video *vid0, *vid1;
    NoiseMask *noise_mask;
//    HeatDistort *heat0, *heat1;
//    Swarm *swarm;
//    Smear *smear0, *smear1;
//    ShadowMask *mask;
//    Invert *invert;
//    ColorMap *color_map;
//    Stream *stream0;
//    Stream *stream1;
//    Stream *stream2;
//    Stream *stream3;
//    Stream *stream4;
//    Kernel *kernel;
    DisplayImage *transform;
    SmearInner * smear;
    Swarm * swarm;
    Grayscale * grayscale;
    Mirror * mirror;
    // audio server
    ofSoundStream soundStream;
    vector<float> audio;
    float smoothedVol;
    float vol;
    int bufferCounter;
};
