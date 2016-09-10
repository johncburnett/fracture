//
//  kernel.h
//  fracture
//
//  Created by John Burnett on 9/2/16.
//
//

#ifndef kernel_h
#define kernel_h

#include "ofMain.h"
#include "image.h"
#include "transform.h"

//========================================================================
class Stream {
public:
    ofFbo fbo;
    Still * img;
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

#endif /* kernel_h */
