//
//  image.cpp
//  fracture
//
//  Created by John Burnett on 9/2/16.
//
//

#include "image.h"

//========================================================================
void BaseImage::overwrite_fbo(ofFbo * f){
    fbo.begin();
    ofClear(0,0,0,0);
    ofSetColor(255);
    f->draw(0, 0);
    fbo.end();
}

ofFbo BaseImage::get_fbo(void) {
    return fbo;
}

//========================================================================
Still::Still(const char *fname){
    load_media(fname);
}

Still::Still(ofFbo * _input){
    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
    
    fbo.begin();
    ofClear(0,0,0,0);
    _input->draw(0,0);
    fbo.end();
}


Still::Still(void){
    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
    
    fbo.begin();
    ofClear(0,0,0,0);
    fbo.end();
}

void Still::update(void){
    ;
}

void Still::load_media(const char *fname) {
    img.load(fname);
    img.resize(WIDTH, HEIGHT);

    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);

    fbo.begin();
    ofClear(0,0,0,0);
    img.draw(0,0);
    fbo.end();
}

ofTexture Still::get_texture(void){
    return fbo.getTexture();
}

void Still::display(){
    fbo.draw(0, 0);
}

//========================================================================
Video::Video(const char *fname){
    load_media(fname);
}

Video::Video(void) {
    ;
}

void Video::update(void){
    if (mov.getPosition() == 1.0) {
        mov.setPosition(0.0);
    }
    mov.nextFrame();
    mov.update();

    fbo.begin();
    ofClear(0,0,0,0);
    mov.draw(0,0,WIDTH,HEIGHT);
    fbo.end();
}

ofTexture Video::get_texture(){
    return mov.getTexture();
}

void Video::load_media(const char *fname) {
    mov.load(fname);
    sleep(5);

    mov.play();
    mov.setPaused(true);

    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);

    update();
}

void Video::display(){
    fbo.draw(0,0);
}
