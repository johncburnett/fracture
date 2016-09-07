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

    //init_stream0();
    init_stream0();
    
    //_OSC
    server = new OSC_Server(OSC_IN);
    set_listeners();
    
    //_supercollider
    //run_supercollider();
}

//--------------------------------------------------------------
void ofApp::update(){

    //update_stream0();
//    stream1->set_init_img(img7);
//    stream1->evaluate();
    update_stream0();
    server->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate()));

    //stream0->draw();
    stream0->draw();
    if (ofGetMousePressed()){
        

    }
//    stream1->draw();
}

void ofApp::init_stream0(){
    
    mode = 2;
    
    mirror = new Mirror();
    //vid0->update();
    smear = new SmearInner(img6);
    swarm = new Swarm();
    invert = new Invert(1.0);
    blur = new ofxBlur();
    mirror->set_mode(mode);
    blur->setScale(0.1);
    
    stream0 = new Stream();
    
    stream0->add_transform(smear);
    stream0->add_transform(mirror);
    stream0->add_transform(invert);
    stream0->add_transform(swarm);
    stream0->add_transform(blur);
}


void ofApp::init_stream2(){
    
    mode = 1;
    
    mirror = new Mirror();
    
    smear = new SmearInner(img6);
    swarm = new Swarm();
    invert = new Invert(1.0);
    blur = new ofxBlur();
    mirror->set_mode(mode);
    blur->setScale(0.1);
    
    stream2 = new Stream();
    
    stream2->add_transform(smear);
    stream2->add_transform(mirror);
    stream2->add_transform(invert);
    stream2->add_transform(swarm);

    stream2->add_transform(blur);
}

void ofApp::update_stream0(){
    //if (ofGetFrameNum() %2) stream0->num_nodes = (stream0->num_nodes) % 4 + 2;
    //smear->set_scale(ofMap(mouseX, 0, WIDTH, 0, 10000));
    smear->set_scale(macro);
    //stream0->set_init_img(vid0);
    stream0->set_init_img(sources[source_index]);
    stream0->evaluate();
}


void ofApp::update_stream2(){
    //if (ofGetFrameNum() %2) stream0->num_nodes = (stream0->num_nodes) % 4 + 2;
    //smear->set_scale(ofMap(mouseX, 0, WIDTH, 0, 10000));
    //vid0->update();
    smear->set_scale(macro);
    stream2->set_init_img(sources[source_index]);
    stream2->evaluate();
}


void ofApp::init_stream1(){
    
    heat = new HeatDistort(img7, img7mask);
    
    stream1 = new Stream();
    stream1->add_transform(heat);
}


void ofApp::load_media(){
    img0 = new Still("img/IMG_0639.jpg");
    img1 = new Still("img/stone.jpg");
    img2 = new Still("img/IMG_3006.jpg");
    img3 = new Still("img/emory.jpg");
    img4 = new Still("img/rock.jpg");
    img5 = new Still("img/IMG_0644.jpg");
    img6 = new Still("img/sludge.jpg");
    img7 = new Still("img/IMG_1734.png");
    img8 = new Still("img/IMG_1734.png");
    img7mask = new Still("img/IMG_6140.jpg");
    
    sources.push_back(img0);
    sources.push_back(img1);
    sources.push_back(img2);
    sources.push_back(img3);
    sources.push_back(img4);
    sources.push_back(img5);
    sources.push_back(img6);
    sources.push_back(img7);
    sources.push_back(img8);
    
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
    //mirror->set_mode((mirror->mode + 1) % 2);

    
}

//--------------------------------------------------------------
void ofApp::noise(float &f) {
    ;
}

//--------------------------------------------------------------
void ofApp::click(float &f) {
    smear->set_scale(ofMap(ofGetFrameNum() % 1000 / 1000.f, 0.0, 1.0, 0, 1000));
    //invert->scale = abs(1 - invert->scale);
    
    mode = (int) ofRandom(1.99);
    mirror->set_mode(mode);
}

//--------------------------------------------------------------
void ofApp::bass(float &f) {
    ;
}

//--------------------------------------------------------------
void ofApp::mod0(float &f) {
    //macro = ofMap(f, 0.0, 1.0, 0.0, 10000);
}

//--------------------------------------------------------------
void ofApp::mod1(float &f) {
    ;
}

//--------------------------------------------------------------
void ofApp::rms(float &f) {
    macro = ofMap(f, 0.0, 1.0, 0, 4000);
    //cout << ofGetFrameNum() << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 'f'){
        stream0->num_nodes = (stream0->num_nodes) % stream0->nodes.size() + 1;
    }
    
    if (key == ' '){
        source_index = (source_index + 1) % sources.size();
    }
    
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
