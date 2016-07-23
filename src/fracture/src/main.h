//
//  main.h
//  fracture
//
//  Created by John Burnett on 7/23/16.
//
//

#ifndef main_h
#define main_h

// group of similar images with probability distribution
struct category {
    char *c_name;
    vector<ofImage> images;
    vector<float> distribution;
    int n;
};

// holds set of fbos for image textures
struct composite_fbo {
    struct category *cat;
    vector<ofFbo> fbo_v;
    ofShader *shader;
    vector<float> params;
    int n;
    ofFbo composite;
};

// holds a cateogry and a combination shader
struct node {
    struct category *set;
    ofShader *shader;
    int n;
    vector<struct edge> edges;
};

// holds interpolation shader
struct edge {
    vector<ofShader> shaders;
    vector<ofShader> params;
    float threshold;
    int n;
    vector<struct node> nodes;
};

// pointers to all categories, nodes, edges, and shaders
vector<struct category *> categories;
vector<struct node *> nodes;
vector<struct edge *> edges;
vector<ofShader *> shaders;

void directories_to_categories(char *dir_name);
struct composite_fbo gen_composite_image(struct node, ofShader);
void interpolate(struct composite_fbo, struct edge, float amount);
struct node traverse_edge(struct node, struct edge);


#endif /* main_h */
