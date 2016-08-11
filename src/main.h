/*
 * fracture/src/main.h
 *
 * fracture
 * Copyright (C) 2016 - epistrata (John Burnett + Sage Jenson)
 * <http://epistrata.xyz/>
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef main_h
#define main_h

#define WIDTH 1920
#define HEIGHT 1080

#include "ofMain.h"

#include <vector>
using namespace std;

//========================================================================
/* 
 * The Image class holds an image from file in an ofImage and ofFbo.
 * The Constructor takes in the file name of the image.
 * Display outputs the image to the screen
 */
class Image {
    public:
        ofImage img;
        ofFbo fbo;
        
        Image(const char *);
        Image(ofFbo);
        ~Image();
        
        void display(void);
        ofFbo getFbo(void);
};

//========================================================================
/* 
 * Transform is an abstract class that processes one or more images.
 * It serves as a parent class to various other transforms.
 * 
 * void draw(void) draws fbo to the screen.
 * void draw_quad(void) maps a texture to a quad.
 */
class Transform {
    public:
        ofFbo fbo;
        Image *img1;
        Image *img2;
    
        // virtual methods
        virtual void process_image(void) =0;
        virtual void update(void) =0;
    
        // inherited methods
        void draw(void);
        ofFbo get_fbo(void);
        Image *to_image(void);
        void draw_quad(void);
};

//========================================================================
/* 
 * Smear is a Transform that distorts img1 based on the color of img2
 * Smear(img1, img2, xi, yi, init_dx, init_dy):
 *     img1 : source image
 *     img2 : displacement image
 *     xi : initial x-axis displacement
 *     yi : initial y- axis displacement
 *     init_dx : x-displacement movement
 *     init_dy : y-displacement movement
 *
 * update_delta(new_dx, new_dy) changes dx and dy values.
 *
 */
class Smear : public virtual Transform {
public:
    ofShader shader;
    float x_scale, y_scale;
    float dx, dy;

    Smear(Image *, Image *, float, float, float, float);
    ~Smear(void);

    void update_delta(float,float);

    // virtual methods
    void update(void);
    void process_image(void);
};


//========================================================================
/*
 * Invert an image.
 */
class Invert : public virtual Transform {
public:
    ofShader shader;
    float scale;
    
    Invert(Image *, float);
    ~Invert(void);
    
    void update();
    void process_image();
};

//========================================================================
/*
 * Make an image grayscale.
 */
class Grayscale: public virtual Transform {
public:
    ofShader shader;
    float scale;
    
    Grayscale(Image *, float);
    ~Grayscale(void);
    
    void update();
    void process_image();
};

//========================================================================
/*
 * ShadowMask removes pixels that are below a given threshold.
 */
class ShadowMask : public virtual Transform {
public:
    ofShader shader;
    float threshold;
    
    ShadowMask(Image *, float);
    ~ShadowMask(void);
    
    void update();
    void process_image(void);
};

//========================================================================
/*
 * Colormap maps one image's color to another image. Unfinished.
 */
class ColorMap : public virtual Transform {
public:
    ofShader shader;
    float scale;
    ofImage processed;
    
    ColorMap(Image *, Image *);
    ~ColorMap(void);
    
    void update();
    void process_image();
};

//========================================================================
/*
 * Twist distorts a section of the image around a point radially.
 * center is the center of the distortion
 */
class Twirl: public virtual Transform {
public:
    ofShader shader;
    float scale;
    ofVec2f center;
    
    Twirl(Image *, float s);
    ~Twirl(void);
    
    void set_center(float, float);
    
    void update();
    void process_image();
};

//========================================================================
/*
 * NoiseMask interpolates between two images based on ofImage noise.
 *
 */
class NoiseMask: public virtual Transform {
public:
    ofShader shader;
    ofImage noise;
    int x0, y0;
    float frequency;
    float time;
    
    NoiseMask(Image *, Image *);
    ~NoiseMask(void);
    
    void update();
    void process_image();
};

//========================================================================
/*
 * HeatDistort uses a depth map to distort an image
 * Currently working on this...
 *
 */
class HeatDistort: public virtual Transform {
public:
    ofShader shader;
    int x0, y0;
    Image * img1, * img2;
    float frequency;
    float time;
    float distort;
    float rise;
    
    HeatDistort(Image *, Image *);
    ~HeatDistort(void);
    
    void update();
    void process_image();
};

//========================================================================
class Graph {
    struct node *root;
    vector<struct node *> nodes;
    
    void update(void);
    ofFbo eval(void);
};

struct node {
    Image *img;
    vector<struct edge *> edges;
};

struct edge {
    Transform *transform;
    struct node *destination;
};

//========================================================================
//_Utilities
double mean(vector<double> A);
double std_dev(vector<double> A);

#endif /* main_h */
