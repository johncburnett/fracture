/*
 * fracture/src/ofApp.cpp
 *
 * John Burnett + Sage Jenson (c) 2016
 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    
    img1 = new Image("img/stone.jpg");
    img2 = new Image("img/emory.jpg");
    smear = new Smear(img1, img2, 0, 0, 0, 1);
    
    // initialize audio server
    int bufferSize = 256;
    audio.assign(bufferSize, 0.0);
    soundStream.setup(this, 0, 1, 44100, bufferSize, 2);
    soundStream.stop();
}

//--------------------------------------------------------------
void ofApp::update(){
    smear->update();
    
	vol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
}

//--------------------------------------------------------------
void ofApp::draw(){
    smear->draw();
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
