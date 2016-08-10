/*
 * fracture/src/main.cpp
 *
 * John Burnett + Sage Jenson (c) 2016
 */

#include <math.h>
#include "ofMain.h"
#include "ofApp.h"
#include "main.h"

//========================================================================
int main( ){
	ofSetupOpenGL(WIDTH, HEIGHT, OF_WINDOW);
	ofRunApp(new ofApp());
}

//========================================================================
Image::Image(const char *fname) {
    img.load(fname);
    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
    img.resize(WIDTH, HEIGHT);
    
    // write img to fbo
    fbo.begin();
    ofSetColor(255);
    img.draw(0, 0);
    fbo.end();
}

Image::Image(ofFbo f) {
    fbo = f;
}

void Image::display(void) {
    fbo.draw(0, 0);
}

ofFbo Image::getFbo(void) {
    return fbo;
}

//========================================================================
void Transform::draw(void) {
    fbo.draw(0, 0);
}

ofFbo Transform::get_fbo(void) {
    return fbo;
}

Image *Transform::to_image(void) {
    return new Image(fbo);
}

void Transform::draw_quad(void) {
    glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(WIDTH, 0); glVertex3f(WIDTH, 0, 0);
	glTexCoord2f(WIDTH, HEIGHT); glVertex3f(WIDTH, HEIGHT, 0);
	glTexCoord2f(0, HEIGHT);  glVertex3f(0, HEIGHT, 0);
	glEnd();
}

//========================================================================
Smear::Smear(Image *i1, Image *i2, float xi, float yi, float init_dx, float init_dy) {
    shader.load("shadersGL2/smear");
    img1 = i1;
    img2 = i2;
    x_scale = xi;
    y_scale = yi;
    dx = init_dx;
    dy = init_dy;
    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
}

void Smear::update(void) {
    x_scale += dx;
    y_scale += dy;
    
    process_image();
}

void Smear::process_image(void) {
    ofTexture tex0 = img1->fbo.getTexture();
    ofTexture tex1 = img2->fbo.getTexture();

    fbo.begin();
	ofClear(0, 0, 0, 1);
    
        shader.begin();
    
            shader.setUniformTexture("tex0",   tex0, 0);
            shader.setUniformTexture("tex1",  tex1, 1);
            shader.setUniform1i("w", WIDTH);
            shader.setUniform1i("h", HEIGHT);
            shader.setUniform1f("xscale", x_scale);
            shader.setUniform1f("yscale", y_scale);
            
            draw_quad();
    
        shader.end();
    
    fbo.end();
}

//========================================================================
Invert::Invert(Image *img, float t) {
    shader.load("shadersGL2/invert");
    img1 = img;
    scale = t;
    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
}

void Invert::update(void) {
    process_image();
}

void Invert::process_image(void) {
    ofTexture tex0 = img1->fbo.getTexture();
    
    fbo.begin();
    ofClear(0, 0, 0, 1);
    
        shader.begin();
    
            shader.setUniformTexture("tex0", tex0, 0);
            shader.setUniform1f("scale", scale);
            
            draw_quad();
    
        shader.end();
    
    fbo.end();
}
//========================================================================
Grayscale::Grayscale(Image *img, float t) {
    shader.load("shadersGL2/bw");
    img1 = img;
    scale = t;
    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
}

void Grayscale::update(void) {
    process_image();
}

void Grayscale::process_image(void) {
    ofTexture tex0 = img1->fbo.getTexture();
    
    fbo.begin();
    ofClear(0, 0, 0, 1);
    
        shader.begin();
    
            shader.setUniformTexture("tex0", tex0, 0);
            shader.setUniform1f("scale", scale);
            
            draw_quad();
    
        shader.end();
    
    fbo.end();
}

//========================================================================
ShadowMask::ShadowMask(Image *img, float t) {
    shader.load("shadersGL2/shadow_mask");
    img1 = img;
    threshold = t;
    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
}

void ShadowMask::update(void) {
    process_image();
}

void ShadowMask::process_image(void) {
    ofTexture tex0 = img1->fbo.getTexture();
    
    fbo.begin();
    ofClear(0, 0, 0, 1);
    
        shader.begin();
    
            shader.setUniformTexture("tex0", tex0, 0);
            shader.setUniform1f("threshold", threshold);
            
            draw_quad();
    
        shader.end();
    
    fbo.end();
}

//========================================================================
ColorMap::ColorMap(Image *source, Image *target) {
    shader.load("shadersGL2/color_map");
    img1 = source;
    img2 = target;
    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
    processed.allocate(WIDTH, HEIGHT, OF_IMAGE_COLOR);
}

void ColorMap::update(void) {
    process_image();
}

void ColorMap::process_image(void) {
    vector<double> sr(WIDTH * HEIGHT);
    vector<double> sg(WIDTH * HEIGHT);
    vector<double> sb(WIDTH * HEIGHT);
    vector<double> tr(WIDTH * HEIGHT);
    vector<double> tg(WIDTH * HEIGHT);
    vector<double> tb(WIDTH * HEIGHT);
    
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            ofColor s_color = img1->img.getColor(j, i);
            ofColor t_color = img2->img.getColor(j, i);
            int index = j + (i*WIDTH);
//            sr[index] = s_color.r;
//            sg[index] = s_color.g;
//            sb[index] = s_color.b;
//            tr[index] = t_color.r;
//            tg[index] = t_color.g;
//            tb[index] = t_color.b;
            
            sr[index] = s_color.getHue();
            sg[index] = s_color.getSaturation();
            sb[index] = s_color.getBrightness();
            tr[index] = t_color.getHue();
            tg[index] = t_color.getSaturation();
            tb[index] = t_color.getBrightness();
        }
    }
    
    double source_mean_r = mean(sr);
    double source_mean_g = mean(sg);
    double source_mean_b = mean(sb);
    double target_mean_r = mean(tr);
    double target_mean_g = mean(tg);
    double target_mean_b = mean(tb);
    
    double source_std_r = std_dev(sr);
    double source_std_g = std_dev(sg);
    double source_std_b = std_dev(sb);
    double target_std_r = std_dev(tr);
    double target_std_g = std_dev(tg);
    double target_std_b = std_dev(tb);
    
    double coef_r = target_std_r / source_std_r;
    double coef_g = target_std_g / source_std_g;
    double coef_b = target_std_b / source_std_b;
    
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            ofColor color = img2->img.getColor(j, i);
            double r_star = ((color.r - target_mean_r) * coef_r) + target_mean_r;
            double g_star = ((color.g - target_mean_g) * coef_g) + target_mean_g;
            double b_star = ((color.b - target_mean_b) * coef_b) + target_mean_b;
//            ofColor p_color = ofColor(r_star, g_star, b_star, 1);
            ofColor p_color = ofColor(0);
            p_color.setHsb(r_star, g_star, b_star);
            processed.setColor(j, i, p_color);
        }
    }
    processed.update();
    
    fbo.begin();
    ofClear(0, 0, 0, 1);
    processed.draw(0, 0);
    fbo.end();
}

//========================================================================
Twirl::Twirl(Image *img, float s) {
    shader.load("shadersGL2/twirl");
    img1 = img;
    scale = s;
    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
}

void Twirl::update(void) {
    process_image();
}

void Twirl::process_image(void) {
    ofTexture tex0 = img1->fbo.getTexture();
    
    fbo.begin();
    ofClear(0, 0, 0, 1);
    
        shader.begin();
    
            shader.setUniformTexture("tex0", tex0, 0);
            shader.setUniform1f("scale", scale);
            shader.setUniform1i("width", WIDTH);
            shader.setUniform1i("height", HEIGHT);
            shader.setUniform1i("time", ofGetFrameNum());
    
            draw_quad();
    
        shader.end();
    
    fbo.end();
}

//========================================================================
NoiseMask::NoiseMask(Image *i1, Image *i2) {
    shader.load("shadersGL2/noise_mask");
    img1 = i1;
    img2 = i2;
    x0 = y0 = 0;
    frequency = 500.0f;
    time = 0.0f;
    noise.allocate(WIDTH, HEIGHT, OF_IMAGE_GRAYSCALE);
    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
    update();
}

void NoiseMask::update(void) {
    time = ofGetElapsedTimef() * 0.5;
    
    ofPixelsRef pixels = noise.getPixels();
    
    int tmpIndex = 0;
    for( int y = 0; y < noise.getHeight(); y++ ) {
        for( int x = 0; x < noise.getWidth(); x++ ) {
            float tmpNoise = ofNoise( (x0 + x) / frequency, (y0 + y) / frequency, time );
            
            pixels[tmpIndex] = tmpNoise * 255.0f;
            tmpIndex++;
        }
    }
    
    noise.update();
    
    process_image();
}

void NoiseMask::process_image(void) {
    ofTexture tex0 = img1->fbo.getTexture();;
    ofTexture tex1 = img2->fbo.getTexture();
    ofTexture tex2 = noise.getTexture();
    
    fbo.begin();
    ofClear(0, 0, 0, 1);
    
    shader.begin();
    
    shader.setUniformTexture("tex0", tex0, 0);
    shader.setUniformTexture("tex1", tex1, 1);
    shader.setUniformTexture("noise", tex2, 2);
    shader.setUniform1i("width", WIDTH);
    shader.setUniform1i("height", HEIGHT);
    
    draw_quad();
    
    shader.end();
    
    fbo.end();
}

//========================================================================
HeatDistort::HeatDistort(Image *i1) {
    shader.load("shadersGL2/heat");
    img1 = i1;
    x0 = y0 = 0;
    frequency = 40.0f;
    time = 0.0f;
    distort = 0.05f;
    rise = 0.8f;
    noise.allocate(WIDTH, HEIGHT, OF_IMAGE_GRAYSCALE);
    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
    update();
}

void HeatDistort::update(void) {
    process_image();
    
    ofPixelsRef pixels = noise.getPixels();
    
    int tmpIndex = 0;
    for( int y = 0; y < noise.getHeight(); y++ ) {
        for( int x = 0; x < noise.getWidth(); x++ ) {
            float tmpNoise = ofNoise( (x0 + x) / frequency, (y0 + y) / frequency, time/100.0 );
            
            pixels[tmpIndex] = tmpNoise * 255.0f;
            tmpIndex++;
        }
    }
    
    noise.update();
    
    process_image();
}

void HeatDistort::process_image(void) {
    ofTexture tex0 = img1->fbo.getTexture();;
    ofTexture tex1 = noise.getTexture();
    time = ofGetElapsedTimef();
    
    fbo.begin();
    ofClear(0, 0, 0, 1);
    
    shader.begin();
    
    shader.setUniformTexture("tex0", tex0, 0);
    shader.setUniformTexture("tex1", tex1, 1);
    shader.setUniform1i("time", time);
    shader.setUniform1f("distort", distort);
    shader.setUniform1f("rise", rise);
    
    draw_quad();
    
    shader.end();
    
    fbo.end();
}

//========================================================================
//_Utilities

double mean(vector<double> A) {
    double avg, sum;
    sum = 0;
    for(int i = 0; i < A.size(); i++) {
        sum += A[i];
    }
    avg = sum / A.size();
    return avg;
}

double std_dev(vector<double> A) {
    double sd = 0;
    double avg = mean(A);
    for(int i = 0; i < A.size(); i++) {
        sd += abs(A[i] - avg);
    }
    return sd;
}