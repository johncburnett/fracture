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

//#include <libiomp/omp.h>
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // init starting scene
    current_frame = 0;
    
    //_OpenGL init
    ofSetFrameRate(FRAMERATE);
    
    //_Media
    load_media();
    
    //_Streams
//    init_stream0();
    init_stream1();
    init_stream2();
    init_stream3();
    
    //_Kernel
    kernel = new Kernel();
    kernel->add_frame(10);
    kernel->add_frame(10);
    
//    kernel->add_stream(stream0, 0);
    kernel->add_stream(stream1, 0);
    kernel->add_stream(stream2, 1);
    kernel->add_stream(stream3, 2);

    //_OSC
    server = new OSC_Server(OSC_IN);
    set_listeners();
    
    //_supercollider
    run_supercollider();
    
    // Syphon
	mainOutputSyphonServer.setName("Screen Output");
	mClient.setup();
    mClient.set("","Simple Server");
}

//--------------------------------------------------------------
void ofApp::update(){
    if( current_frame == 0 ) { update_stream1(); kernel->to_frame(0); }
    else if( current_frame == 1 ) { update_stream2(); kernel->to_frame(1); }
    else if( current_frame == 2 ) { update_stream3(); kernel->to_frame(2); }
   
    kernel->update();
    server->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetWindowTitle("FPS: " + ofToString(ofGetFrameRate()));

    kernel->draw();
    
    // Syphon
    mClient.draw(50, 50);    
	mainOutputSyphonServer.publishScreen();
}

//--------------------------------------------------------------
void ofApp::init_stream0(){
    
    mode = 2;
    
    mirror = new Mirror();
    smearIn = new SmearInner(img6);
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

    stream0->set_init_img(img2);
}

void ofApp::update_stream0(){
    //if (ofGetFrameNum() %2) stream0->num_nodes = (stream0->num_nodes) % 4 + 2;
    //smear->set_scale(ofMap(mouseX, 0, WIDTH, 0, 10000));
    smearIn->set_scale(macro);
    //stream0->set_init_img(vid0);
    stream0->set_init_img(sources[source_index]);
    stream0->evaluate();
//    vid0->update();
}

void ofApp::init_stream1(){
    img.load("img/IMG_0644_BIG.jpg");
    img8 = new Still();
    img8->load_media("img/IMG_0644_BIG.jpg");
    
    pan = new Pan(&img);
    mirror = new Mirror();
    mirror->set_mode(2);
    smearIn = new SmearInner(img6);
    
    stream1 = new Stream();
    stream1->add_transform(pan);
    stream1->add_transform(mirror);
    stream1->add_transform(smearIn);
}

void ofApp::update_stream1(){
    pan->set_corners(0, -600-(m0 * 2000));
    smearIn->set_scale(vol * 16000);
}

void ofApp::init_stream2(){
    fm = new FrameMover(vid0);
    
    stream2 = new Stream();
    stream2->add_transform(fm);
}

void ofApp::update_stream2(){
    fm->scale = m0 + (vol*3.0f);
}

void ofApp::init_stream3(){
    smear3a = new Smear(img2, 0, 0, 0, 0);
    smear3a->set_mod(8);
    smear3b = new Smear(img0, 0, 0, 0, 0);
    smear3b->set_mod(8);
    
    stream3 = new Stream();
    stream3->add_transform(smear3b);
    stream3->add_transform(smear3a);
    
    stream3->set_init_img(img3);
}

void ofApp::update_stream3(){
    smear3a->update_delta(0, (-1) * vol * m0 * 0.01);
    smear3b->update_delta(0, (-1) * vol * 2 * m0 * 0.01);
}

//--------------------------------------------------------------
void ofApp::load_media(){
    
    //_images
    img0 = new Still();
    img1 = new Still();
    img2 = new Still();
    img3 = new Still();
    img4 = new Still();
    img5 = new Still();
    img6 = new Still();
    img7 = new Still();
    img7mask = new Still();
    
    //_videos
    vid0 = new Video();
    
    sources.push_back(img0);
    sources.push_back(img1);
    sources.push_back(img2);
    sources.push_back(img3);
    sources.push_back(img4);
    sources.push_back(img5);
    sources.push_back(img6);
    sources.push_back(img7);
    sources.push_back(img7mask);
    sources.push_back(vid0);
    
    vector<const char *> fnames = {
        "textures/IMG_0668.jpg",
        "img/stone.jpg",
        "textures/IMG_6012.jpg",
        "textures/IMG_0613.jpg",
        "img/rock.jpg",
        "img/IMG_0644.jpg",
        "img/sludge.jpg",
        "img/IMG_1734.png",
        "textures/IMG_4783.jpg",
        "lapses/pano_lapse.mov",
    };
    
    //_multithreaded loading
//    #pragma omp parallel for
    for(int i = 0; i < sources.size(); i++) {
        sources[i]->load_media(fnames[i]);
    }
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
	ofAddListener(*server->scene, this, &ofApp::scene);
}

//--------------------------------------------------------------
// OSC callbacks

void ofApp::sines(float &f) { }

void ofApp::noise(float &f) { }

void ofApp::click(float &f) {
    smearIn->set_scale(ofMap(ofGetFrameNum() % 1000 / 1000.f, 0.0, 1.0, 0, 1000));
    //invert->scale = abs(1 - invert->scale);
    
    mode = (int) ofRandom(1.99);
    mirror->set_mode(mode);
}

void ofApp::bass(float &f) { }

void ofApp::mod0(float &f) {
    //macro = ofMap(f, 0.0, 1.0, 0.0, 10000);
    m0 = f;
}

void ofApp::mod1(float &f) {
    m1 = f;
}

void ofApp::rms(float &f) {
//    macro = ofMap(f, 0.0, 1.0, 0, 4000);
    vol = f * 10.0;
}

void ofApp::scene(int &i) {
    current_frame = i;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 'f'){
        stream1->num_nodes = (stream1->num_nodes) % stream1->nodes.size() + 1;
    }
    
    if (key == ' '){
        source_index = (source_index + 1) % sources.size();
    }
}

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
