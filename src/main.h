/*
 * fracture/src/main.h
 *
 * John Burnett + Sage Jenson (c) 2016
 */

#ifndef main_h
#define main_h

#define WIDTH 1920
#define HEIGHT 1080

#include "ofMain.h"

#include <vector>
using namespace std;

class Image {
    public:
        ofImage img;
        ofFbo fbo;
        
        Image(const char *);
        ~Image();
        
        void display(void);
        ofFbo getFbo(void);
};

class Transform {
    public:
        ofFbo fbo;
        Image *img1;
        Image *img2;
    
        // virtual methods
        virtual void update(void) =0;
    
        // inherited methods
        void draw(void);
};

class Smear : public virtual Transform {
    public:
        ofShader shader;
        float x_scale, y_scale;
        
        Smear(Image *, Image *, float, float);
        ~Smear(void);
    
        void set_uniforms(void);
        void set_img1(Image);
        void set_img2(Image);
        
        // virtual methods
        void update(void);
        void process_image(void);
    
        // inherited methods
        void draw(void);
};

#endif /* main_h */
