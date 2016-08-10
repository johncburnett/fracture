/*
 * fracture/src/ofApp.cpp
 *
 * John Burnett + Sage Jenson (c) 2016
 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    
    img1 = new Image("img/rock.jpg");
    img2 = new Image("img/rock_b.jpg");
    img3 = new Image("img/rock_v.jpg");
    img4 = new Image("img/sludge.jpg");
    
    invert = new Invert(img1, 1.0);
    invert->process_image();
    Image *i = new Image(invert->fbo);
    
    noise_mask = new NoiseMask(img1, img2);
    
    mask = new ShadowMask(noise_mask->to_image(), 0.4);
    mask->process_image();
    smear = new Smear(mask->to_image(), img3, 0, 0, 0 ,1);
    
    heat = new HeatDistort(img1);
    
    c = 0;
    
    // initialize audio server
    int bufferSize = 256;
    audio.assign(bufferSize, 0.0);
    soundStream.setup(this, 0, 1, 44100, bufferSize, 2);
    soundStream.start();
}

//--------------------------------------------------------------
void ofApp::update(){
    noise_mask->update();
    
    smear->y_scale = c;
    smear->update();
    
    heat->update();
    
    c += 8;
}

//--------------------------------------------------------------
void ofApp::draw(){
//    heat->draw();
    noise_mask->draw();
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
