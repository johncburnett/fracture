#include "main.h"
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	shader.load("shadersGL2/shader");
    
    ofSetFrameRate(30);
    
    xscale = 0.0;
    yscale = 0.0;
    
    img1.load("img/rock.jpg");
    img2.load("img/cloud.jpg");
    
    w = img1.getWidth();
    h = img2.getHeight();
    
    fbo1.allocate(w, h, GL_RGBA);
    fbo2.allocate(w, h, GL_RGBA);
    fbo3.allocate(w, h, GL_RGBA);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    yscale += 8;
    xscale += 0;
    
    fbo1.begin();
    ofSetColor(255);
    img1.draw(0, 0);
    fbo1.end();
    
    fbo2.begin();
    ofSetColor(255);
    img2.draw(0, 0);
    fbo2.end();
    
    ofTexture tex1 = fbo1.getTexture();
    ofTexture tex2 = fbo2.getTexture();

    fbo3.begin();
	ofClear(0, 0, 0, 1);
    
    shader.begin();
    
    shader.setUniformTexture("tex0",   tex1, 0);
    shader.setUniformTexture("tex1",  tex2, 1);
    shader.setUniform1i("w", w);
    shader.setUniform1i("h", h);
    shader.setUniform1f("xscale", xscale);
    shader.setUniform1f("yscale", yscale);
    
    glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(w, 0); glVertex3f(w, 0, 0);
	glTexCoord2f(w, h); glVertex3f(w, h, 0);
	glTexCoord2f(0,h);  glVertex3f(0,h, 0);
	glEnd();
    
    shader.end();
    fbo3.end();
    
    fbo3.draw(0, 0);
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
