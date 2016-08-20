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
    // openFrameworks mumbojumbo
    ofSetFrameRate(FRAMERATE);
    
    load_media();

    init_stream0();
    //init_stream1();
    
    //_OSC
    server = new OSC_Server(OSC_IN);
    set_listeners();
    
    //_supercollider
//    run_supercollider();
}

//--------------------------------------------------------------
void ofApp::update(){

    update_stream0();
//    stream1->set_init_img(img7);
//    stream1->evaluate();
    
    server->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate()));

    stream0->draw();
    if (ofGetMousePressed()){
        

    }
//    stream1->draw();
}

void ofApp::init_stream0(){
    
    mode = 0;
    
    mirror = new Mirror();
    
    smear = new SmearInner(img6);
    swarm = new Swarm();
    invert = new Invert(1.0);
    blur = new ofxBlur();
    mirror->set_mode(mode);
    blur->setScale(0.1);
    
    stream0 = new Stream();
    
    stream0->add_transform(mirror);
    stream0->add_transform(smear);
    stream0->add_transform(swarm);
    stream0->add_transform(invert);
    stream0->add_transform(blur);
}

void ofApp::update_stream0(){
    //if (ofGetFrameNum() %2)stream0->num_nodes = (stream0->num_nodes) % 4 + 2;
    vid0->update();
    smear->set_scale(ofMap(mouseX, 0, WIDTH, 0, 10000));
    stream0->set_init_img(vid0);
    stream0->evaluate();
}

void ofApp::init_stream1(){
    
    heat = new HeatDistort(img7, img7mask);
    
    stream1 = new Stream();
    stream1->add_transform(heat);
}

void ofApp::init_stream2(){
    ;
}

void ofApp::load_media(){
    img0 = new Still("img/emory.jpg");
    img1 = new Still("img/stone.jpg");
    img2 = new Still("img/IMG_3006.jpg");
    img3 = new Still("img/emory.jpg");
    img4 = new Still("img/rock.jpg");
    img5 = new Still("img/landscape.jpg");
    img6 = new Still("img/sludge.jpg");
    img7 = new Still("img/IMG_1734.png");
    img7mask = new Still("img/IMG_1734_mask.png");
    
    //_videos
    vid0 = new Video("lapses/pano_lapse.mov");
}

//--------------------------------------------------------------
void ofApp::set_listeners(void) {
	ofAddListener(*server->pulses[0], this, &ofApp::sines);
	ofAddListener(*server->pulses[1], this, &ofApp::noise);
	ofAddListener(*server->pulses[2], this, &ofApp::click);
	ofAddListener(*server->pulses[3], this, &ofApp::bass);
	ofAddListener(*server->mods[0], this, &ofApp::mod0);
	ofAddListener(*server->mods[1], this, &ofApp::mod1);
	ofAddListener(*server->mods[2], this, &ofApp::rms);
}

//--------------------------------------------------------------
void ofApp::sines(float &f) {
    smear->set_scale(ofRandom(100.0));
    mirror->set_mode(mode);
    mode++;
    mode %= 4;
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
void ofApp::mod0(float &f) {
    ;
}

//--------------------------------------------------------------
void ofApp::mod1(float &f) {
    ;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    
    /*ofToggleFullscreen();*/}
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
