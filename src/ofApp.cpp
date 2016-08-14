/*
 * fracture/src/ofApp.cpp
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

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    smoothedVol = 0;
    
    img0 = new Image("img/rock.jpg");
    img1 = new Image("img/rock_b.jpg");
    img2 = new Image("img/rock_v.jpg");
    img3 = new Image("img/emory.jpg");
    img4 = new Image("img/stone.jpg");
    img5 = new Image("img/landscape.jpg");

    noise_mask = new NoiseMask(img0, img1);
    smear0 = new Smear(img0, img2, 0, 0, 0, 8);
    mask = new ShadowMask(img0, 0.2);
    heat0 = new HeatDistort(img2, img2);
    heat1 = new HeatDistort(img4, img3);
    twirl = new Twirl(img3, 0.2);
    smear1 = new Smear(img5, img0, 0, 0, 0, 2);

    stream0 = new Stream();
    stream0->add_transform(noise_mask);

    stream1 = new Stream();
    stream1->add_transform(heat0);
    stream1->add_transform(smear0);
    
    stream2 = new Stream();
    stream2->add_transform(twirl);
    
    stream3 = new Stream();
    stream3->add_transform(heat1);
    
    stream4 = new Stream();
    stream4->add_transform(smear1);

    kernel = new Kernel();
    kernel->add_stream(stream0, 0);
    kernel->add_stream(stream1, 0);
    
    kernel->add_frame(2.0f);
    kernel->add_stream(stream2, 1);
    
    kernel->add_frame(2.0f);
    kernel->add_stream(stream3, 2);
    
    kernel->add_frame(2.0f);
    kernel->add_stream(stream4, 3);
    
    kernel->toggle_loop(true);
    
    // initialize audio server
//    int bufferSize = 256;
//    audio.assign(bufferSize, 0.0);
//    soundStream.setup(this, 0, 1, 44100, bufferSize, 2);
}

//--------------------------------------------------------------
void ofApp::update(){
//	vol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    kernel->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate()));
    kernel->draw();
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
