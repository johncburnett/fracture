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
    ofSetVerticalSync(true);
    
    blur.setup(WIDTH, HEIGHT, 10, .4, 4);
    
    img0 = new Still("img/emory.jpg");
    img1 = new Still("img/stone.jpg");
    img2 = new Still("img/bw.jpg");
    img3 = new Still("img/emory.jpg");
    img4 = new Still("img/rock.jpg");
    img5 = new Still("img/landscape.jpg");
    
    //vid0 = new Video("lapses/pano_lapse.mov");
    
    twirl = new Twirl();
    twirl->set_scale(0.15);
    smear = new SmearInner(img1, 0.0);
    mirror = new Mirror();
    
    stream0 = new Stream();
    stream0->add_transform(twirl);
    stream0->add_transform(mirror);
    
    kernel = new Kernel();
    kernel->add_stream(stream0, 0);

    kernel->toggle_loop(true);
    
    server = new OSC_Server(OSC_IN);
    
    run_supercollider();
}

//--------------------------------------------------------------
void ofApp::update(){
    stream0->set_init_img(img4);
    kernel->update();
    
    blur.setScale(0.26);
    blur.setRotation(0);
    
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
//    vid0->draw()
}

//--------------------------------------------------------------
void ofApp::set_listeners(void) {
//	ofAddListener(*event->pulses[0], this, &ofApp::sines);
//	ofAddListener(*event->pulses[1], this, &ofApp::noise);
//	ofAddListener(*event->pulses[2], this, &ofApp::click);
//	ofAddListener(*event->pulses[3], this, &ofApp::bass);
}

//--------------------------------------------------------------
void ofApp::sines(float &f) {
    ;
}

//--------------------------------------------------------------
void ofApp::noise(float &f) {
    ;
}

//--------------------------------------------------------------
void ofApp::click(float &f) {
    ;
}

//--------------------------------------------------------------
void ofApp::bass(float &f) {
    ;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}
void ofApp::keyReleased(int key) {}
void ofApp::mouseMoved(int x, int y) {}
void ofApp::mouseDragged(int x, int y, int button) {}
void ofApp::mousePressed(int x, int y, int button) {}
void ofApp::mouseReleased(int x, int y, int button) {}
void ofApp::mouseEntered(int x, int y) {}
void ofApp::mouseExited(int x, int y) {}
void ofApp::windowResized(int w, int h) {}
void ofApp::gotMessage(ofMessage msg) {}
void ofApp::dragEvent(ofDragInfo dragInfo) {}
