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

//========================================================================
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
class Smear : public virtual Transform {
    public:
        ofShader shader;
        float x_scale, y_scale;
        float dx, dy;
    
        Smear(Image *, Image *, float, float, float, float);
        ~Smear(void);
    
        // virtual methods
        void update(void);
        void process_image(void);
};

//========================================================================
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
class Twirl: public virtual Transform {
public:
    ofShader shader;
    float scale;
    
    Twirl(Image *, float s);
    ~Twirl(void);
    
    void update();
    void process_image();
};

//========================================================================
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
class HeatDistort: public virtual Transform {
public:
    ofShader shader;
    ofImage noise;
    int x0, y0;
    float frequency;
    float time;
    float distort;
    float rise;
    
    HeatDistort(Image *);
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
