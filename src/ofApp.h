/*
 * fracture/src/ofApp.h
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

#pragma once

#include "main.h"

#include "transform.h"
#include "kernel.h"
#include "osc_server.h"

class ofApp : public ofBaseApp {
public:
    //========================================================================
    //_openFrameworks functions
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    //========================================================================
    //media
    // TODO: put this stuff in a list and parallelize loading
    Still *img0, *img1, *img2, *img3, *img4, *img5, *img6, *img7, *img7mask, *img8;
    Video *vid0, *vid1;
    ofImage img;
    vector<BaseImage*> sources;
    void load_media(void);
    int source_index;
    
    //========================================================================
    //transforms
    NoiseMask *noise_mask;
    DisplayImage *transform;
    SmearInner *smear;
    Swarm *swarm;
    Invert *invert;
    Grayscale *grayscale;
    Twirl *twirl;
    Mirror *mirror;
    ofxBlur *blur;
    DisplayImage *pass_image;
    HeatDistort *heat;
    Pan *pan;
    
    //========================================================================
    //Specific scenes
    void init_stream0(void);
    void update_stream0(void);
    
    void init_stream1(void);
    void update_stream1(void);
    
    void init_stream2(void);
    void update_stream2(void);
    
    int mode;
    float macro;
    
    //========================================================================
    // status
    bool status[4];
    float m0;
    float m1;
    float vol;
    
    //========================================================================
    //control
    Kernel *kernel;
    Stream *stream0, *stream1, *stream2;
    OSC_Server *server;
    
    //========================================================================
    //_event functions
    void set_listeners(void);
    void sines(float &f);
    void noise(float &f);
    void click(float &f);
    void bass(float &f);
    void mod0(float &f);
    void mod1(float &f);
    void rms(float &f);
};
