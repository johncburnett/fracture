//
//  image.h
//  fracture
//
//  Created by John Burnett on 9/2/16.
//
//

#ifndef image_h
#define image_h

#define WIDTH 1920
#define HEIGHT 1080

#include "ofMain.h"

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
    Still(ofFbo *);
    Still(void);
    
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

#endif /* image_h */
