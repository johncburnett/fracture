/*
 * palo_duro/src/fracture/src/ofApp.h
 *
 * John Burnett + Sage Jenson (c) 2016
 */

#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
    	void setup();
		void update();
		void draw();

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
    

	ofImage img1;
	ofImage img2;
    ofFbo fbo1, fbo2, fbo3, fbo4, fbo5;
	ofShader shader;
    
    int w, h;
    float xscale;
    float yscale;
};
