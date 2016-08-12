/*
 * fracture/src/ofApp.cpp
 *
 * fracture
 * Copyright (C) 2016 - epistrata (John Burnett + Sage Jenson)
 * <http://epistrata.xyz/>
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    
    img0 = new Image("img/IMG_2242.jpg");
    img0_heat = new Image("img/IMG_2242_mask.jpg");
    img2 = new Image("img/IMG_3006.jpg");
    img3 = new Image("img/rock_v.jpg");
    img4 = new Image("img/sludge.jpg");
    
    /*
    invert = new Invert(img1, 1.0);
    invert->process_image();
    Image *i = new Image(invert->fbo);
    
    noise_mask = new NoiseMask(img1, img2);
    
    mask = new ShadowMask(noise_mask->to_image(), 0.4);
    mask->process_image();
    smear = new Smear(mask->to_image(), img3, 0, 0, 0 ,1);
    
    heat = new HeatDistort(img1);
    
    c = 0;
    */
    
    nm = new NoiseMaker();
    
    // initialize audio server
    int bufferSize = 256;
    audio.assign(bufferSize, 0.0);
    soundStream.setup(this, 0, 1, 44100, bufferSize, 2);
}

//--------------------------------------------------------------
void ofApp::update(){
    nm->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate()));
    nm->draw();
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){	
	
	float curVol = 0.0;
	int numCounted = 0;

	for (int i = 0; i < bufferSize; i++){
		audio[i] = input[i*2]*0.5;
		curVol += audio[i] * audio[i];
		numCounted+=2;
	}
	
	curVol /= (float)numCounted;
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
	bufferCounter++;
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
