/*
 * fracture/src/main.cpp
 *
 * John Burnett + Sage Jenson (c) 2016
 */

#include "ofMain.h"
#include "ofApp.h"
#include "main.h"

//========================================================================
int main( ){
	ofSetupOpenGL(WIDTH, HEIGHT, OF_WINDOW);
	ofRunApp(new ofApp());
}

//========================================================================
Image::Image(const char *fname) {
    img.load(fname);
    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
    
    // write img to fbo
    fbo.begin();
    ofSetColor(255);
    img.draw(0, 0);
    fbo.end();
}

void Image::display(void) {
    fbo.draw(0, 0);
}

ofFbo Image::getFbo(void) {
    return fbo;
}

//========================================================================
void Transform::draw(void) {
    fbo.draw(0, 0);
}

//========================================================================
Smear::Smear(Image *i1, Image *i2, float x, float y) {
    shader.load("shadersGL2/smear");
    img1 = i1;
    img2 = i2;
    x_scale = x;
    y_scale = y;
    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
    
}

void Smear::update(void) {
//    x_scjale++;
    y_scale++;
}

void Smear::draw() {
    ofTexture tex0 = img1->fbo.getTexture();
    ofTexture tex1 = img2->fbo.getTexture();

    fbo.begin();
	ofClear(0, 0, 0, 1);
    
        shader.begin();
    
            shader.setUniformTexture("tex0",   tex0, 0);
            shader.setUniformTexture("tex1",  tex1, 1);
            shader.setUniform1i("w", WIDTH);
            shader.setUniform1i("h", HEIGHT);
            shader.setUniform1f("xscale", x_scale);
            shader.setUniform1f("yscale", y_scale);
            
            glBegin(GL_QUADS);
        	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        	glTexCoord2f(WIDTH, 0); glVertex3f(WIDTH, 0, 0);
        	glTexCoord2f(WIDTH, HEIGHT); glVertex3f(WIDTH, HEIGHT, 0);
        	glTexCoord2f(0, HEIGHT);  glVertex3f(0, HEIGHT, 0);
        	glEnd();
        
        shader.end();
    
    fbo.end();
    fbo.draw(0, 0);
}
