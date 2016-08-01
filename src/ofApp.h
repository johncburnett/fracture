/*
 * fracture/src/ofApp.h
 *
 * John Burnett + Sage Jenson (c) 2016
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

    Image *img1, *img2;
    Smear *smear;
    
    // audio server
    ofSoundStream soundStream;
    vector<float> audio;
    float smoothedVol;
    float vol;
    int bufferCounter;
};
