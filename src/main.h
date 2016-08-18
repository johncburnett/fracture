/*
 * fracture/src/main.h
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef main_h
#define main_h


#define WIDTH 1920
#define HEIGHT 1080

/*
#define WIDTH 2880
#define HEIGHT 1620
*/

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
    Image(ofFbo *);
    ~Image(void);
    
    void overwrite_fbo(ofFbo *);
    
    void display(void);
    ofFbo getFbo(void);
};



//========================================================================
/*
 * The Image class stores an image / video
 */
class BaseImage {
public:
    ofFbo fbo;
    
    //virtual methods
    virtual void update(void) =0;
    virtual ofTexture get_texture(void) =0;
    virtual void display(void) =0;
    
    
    //inhereted methods
    void overwrite_fbo(ofFbo *);
    ofFbo get_fbo(void);
};

//========================================================================
/*
 * The Still class holds an image from file in an ofImage.
 * The Constructor takes in the file name of the image.
 */
class Still : public virtual BaseImage {
public:
    ofImage img;
    
    Still(const char *);
    ~Still(void);
    
    ofTexture get_texture(void);
    void display(void);
    void update(void);
    
};


//========================================================================
/*
 * The Timelapse class
 */
class Video : public virtual BaseImage {
public:
    ofVideoPlayer mov;
    int cur_frame;
    
    Video(const char *);
    ~Video(void);
    
    ofTexture get_texture(void);
    void display(void);
    void update(void);
    
};

//========================================================================
/*
 * Transform is an abstract class that processes one or more images.
 * It serves as a parent class to various other transforms.
 *
 * void draw(void) draws fbo to the screen.
 * void draw_quad(void) maps a texture to a quad.
 */
class NewTransform {
public:
    ofFbo *fbo;
    BaseImage *img1;
    BaseImage *img2;
    
    // virtual methods
    virtual void process_image(void) =0;
    virtual void update(void) =0;
    
    // inherited methods
    void draw(void);
    void init_fbo(void);
    ofFbo *get_fbo(void);
    void set_fbo(ofFbo *);
    Image *to_image(void);
    void draw_quad(void);
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
    ofFbo *fbo;
    Image *img1;
    Image *img2;

    // virtual methods
    virtual void process_image(void) =0;
    virtual void update(void) =0;

    // inherited methods
    void draw(void);
    ofFbo get_fbo(void);
    void set_fbo(ofFbo *);
    Image *to_image(void);
    void draw_quad(void);
};

//========================================================================
/* 
 * DisplayImage displays an image with no processing.
 */
class DisplayImage : public virtual NewTransform {
public:
    DisplayImage(BaseImage *);
    ~DisplayImage(void);

    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
/*
 *  Mirror an image.
 */
class Mirror : public virtual NewTransform {
public:
    ofShader shader;
    Mirror(BaseImage *);
    ~Mirror(void);
    
    //virtual methods
    void update(void);
    void process_image(void);
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
class Smear : public virtual NewTransform {
public:
    ofShader shader;
    float x_scale, y_scale;
    float dx, dy;

    Smear(BaseImage *, BaseImage *, float, float, float, float);
    ~Smear(void);

    void update_delta(float,float);

    // virtual methods
    void update(void);
    void process_image(void);
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
class SmearInner : public virtual NewTransform {
public:
    ofShader shader;
    float scale;

    SmearInner(BaseImage *, BaseImage *, float);
    ~SmearInner(void);

    void set_scale(float);

    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
/*
 * Invert an image.
 */
class Invert : public virtual NewTransform{
public:
    ofShader shader;
    float scale;
    
    Invert(BaseImage*, float);
    ~Invert(void);
    
    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
/*
 * Make an image grayscale.
 */
class Grayscale : public virtual NewTransform{
public:
    ofShader shader;
    float scale;
    
    Grayscale(BaseImage*, float);
    ~Grayscale(void);
    
    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
/*
 * ShadowMask removes pixels that are below a given threshold.
 */
class ShadowMask : public virtual NewTransform {
public:
    ofShader shader;
    float threshold;
    
    ShadowMask(BaseImage*, float);
    ~ShadowMask(void);
    
    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
/*
 * Colormap maps one image's color to another image. Unfinished.
 */
class ColorMap : public virtual Transform{
public:
    ofShader shader;
    float scale;
    ofImage processed;
    
    ColorMap(Image*, Image*);
    ~ColorMap(void);
    
    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
/*
 * Twist distorts a section of the image around a point radially.
 * center is the center of the distortion
 */
class Twirl : public virtual NewTransform{
public:
    ofShader shader;
    float scale;
    ofVec2f center;
    
    Twirl(BaseImage*, float s);
    ~Twirl(void);
    
    void set_center(float, float);
    
    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
/*
 * NoiseMask interpolates between two images based on ofImage noise.
 *
 */
class NoiseMask : public virtual NewTransform {
public:
    ofShader shader;
    float frequency, time, scale;
    
    NoiseMask(BaseImage*, BaseImage*);
    ~NoiseMask(void);
    
    void set_scale(float);
    
    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
/*
 * HeatDistort uses a depth map to distort an image
 *
 */
class HeatDistort : public virtual NewTransform {
public:
    ofShader shader;
    int x0, y0;
    BaseImage *img1, *img2;
    float frequency;
    float time;
    float distort;
    float rise;
    
    HeatDistort(BaseImage*, BaseImage*);
    ~HeatDistort(void);
    
    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
class Aberration : public virtual Transform {
    ofShader shader;
    float amount;
    
    Aberration(void);
    ~Aberration(void);
    
    //virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
class NoiseMaker : public virtual Transform {
    ofShader shader;
    
    NoiseMaker(void);
    ~NoiseMaker(void);
    
    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
class Swarm : public virtual NewTransform {
public:
    ofFbo particleFbo;
    ofShader updateShader, drawShader;
    ofVboMesh mesh, quadMesh;
    float opacity;
    
    // dim of particle location texture
    int w, h;

    Swarm(BaseImage *);
    ~Swarm(void);
    
    void createFbo(void);
    void createMesh(void);
    void createPoints(void);
    void setOpacity(float);
    
    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
class Particle {
public:
    ofVec2f location, original_location;
    ofColor color, bg_color;
    float d;
    
    Particle(float, float, float, ofColor, ofColor);
    
    void reset_location(void);
    void up(void);
    void draw_original(void);
    void draw(void);
};

//========================================================================
class Disintegrate : public virtual NewTransform{
public:
    ofTexture color;
    ofVboMesh mesh, quadMesh;
    vector<Particle*> particles;
    
    BaseImage *source, *mask, *delta;
    
    Disintegrate(BaseImage *, BaseImage *, BaseImage *);
    ~Disintegrate(void);
    
    void create_particles(void);
    bool in_bounds(Particle *);
    
    // virtual methods
    void update(void);
    void process_image(void);
};

//========================================================================
class Stream {
public:
    ofFbo fbo;
    int num_nodes;
    vector<struct node> nodes;
    
    Stream(void);
    ~Stream(void);
    
    void add_transform(NewTransform *);
    void set_init_img(BaseImage *);
    
    void evaluate(void);
    void draw(void);
};

//========================================================================
class Kernel {
public:
    int current_frame;
    int num_frames;
    float start_time;
    float target_time;
    bool loop;
    ofFbo fbo;
    Image *img;
    vector<struct frame> frames;
    
    Kernel(void);
    ~Kernel(void);
    
    void add_stream(Stream *, int frame_index);
    void add_frame(float l);
    ofFbo get_stream_fbo(int frame_index, int stream_index);
    ofFbo get_frame_fbo(int frame_index);
    void get_frame_image(int frame_index);
    void set_frame_length(int frame_index, float l);
    void toggle_loop(bool);
    
    void update(void);
    void draw(void);
};

struct node {
    NewTransform *transform;
};

struct frame {
    vector<Stream *> streams;
    int n;
    int t;
};

// Thanks kylemcdonald for this blur¡¡!!
class ofxBlur {
protected:
	ofFbo base;
	vector<ofFbo> ping, pong;

	ofShader blurShader, combineShader;
	float scale, rotation;
	float downsample;
	float brightness;
public:
	ofxBlur();

	void setup(int width, int height, int radius = 100, float shape = .2, int passes = 1, float downsample = .5);

	void setScale(float scale);
	void setRotation(float rotation);
	void setBrightness(float brightness); // only applies to multipass

	void begin();
	void end();
	void draw();
    void draw(ofRectangle rect);

	ofTexture& getTextureReference();
};

// kylemcdonald end

//========================================================================
//_Utilities
double mean(vector<double> A);
double std_dev(vector<double> A);

#endif /* main_h */
