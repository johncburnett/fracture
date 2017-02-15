 /*
 * fracture/src/kernel.h
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

    void add_transform(Transform *);
    void set_init_img(BaseImage *);

    void evaluate(void);
    void draw(void);
};

struct node {
    Transform *transform;
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
    
    void to_frame(int i);

    void update(void);
    void draw(void);
};

struct frame {
    vector<Stream *> streams;
    int n;
    int t;
};

#endif /* kernel_h */
