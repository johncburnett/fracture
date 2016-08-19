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
    ofBackground(0);
    
    blur.setup(WIDTH, HEIGHT, 10, .4, 4);
    
    fboi = new ofFbo();
    fboi->allocate(WIDTH, HEIGHT, GL_RGBA);
    
    img0 = new Still("img/emory.jpg");
    img1 = new Still("img/stone.jpg");
    img2 = new Still("img/bw.jpg");
    img3 = new Still("img/emory.jpg");
    img4 = new Still("img/rock.jpg");
    img5 = new Still("img/landscape.jpg");
    
    //vid0 = new Video("lapses/pano_lapse.mov");
    
    twirl = new Twirl();
    twirl->set_scale(0.15);

    //transform = new DisplayImage(vid0);
    smear = new SmearInner(img1, 0.0);
    //swarm = new Swarm();
    mirror = new Mirror();
    
    stream0 = new Stream();
    stream0->add_transform(twirl);
    stream0->add_transform(mirror);
    //stream0->add_transform(smear);
    //stream0->add_transform(swarm);
    
    kernel = new Kernel();
    kernel->add_stream(stream0, 0);

    kernel->toggle_loop(true);
    
    server = new OSC_Server(OSC_IN);
    
    run_supercollider();
}

//--------------------------------------------------------------
void ofApp::update(){
	vol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    //vid0->update();
    stream0->set_init_img(img4);
    kernel->update();
    
    /*
    cout << ofMap(mouseX, 0, ofGetWidth(), 0, 10) << endl;
    cout << ofMap(mouseY, 0, ofGetHeight(), -PI, PI) << endl;
    blur.setScale(ofMap(mouseX, 0, ofGetWidth(), 0, 10));
    blur.setRotation(ofMap(mouseY, 0, ofGetHeight(), -PI, PI));
    */
    blur.setScale(0.26);
    blur.setRotation(0);
    
    
    smear->set_scale(ofGetMouseX()*vol);
//    vid0->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate()));
    
    blur.begin();
    ofSetColor(255);
    kernel->draw();
    blur.end();
    
    blur.draw();
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
