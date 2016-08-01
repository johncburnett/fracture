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
        Image(ofFbo);
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
        virtual void process_image(void) =0;
        virtual void update(void) =0;
    
        // inherited methods
        void draw(void);
        ofFbo get_fbo(void);
        Image *to_image(void);
};

class Smear : public virtual Transform {
    public:
        ofShader shader;
        float x_scale, y_scale;
        float dx, dy;
    
        Smear(Image *, Image *, float, float, float, float);
        ~Smear(void);
    
        void set_img1(Image);
        void set_img2(Image);
        
        // virtual methods
        void process_image(void);
        void update(void);
};

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

#endif /* main_h */
