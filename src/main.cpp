/*
 * fracture/src/main.cpp
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

#include "ofMain.h"
#include "ofApp.h"
#include "main.h"

//========================================================================
int main( ){
    // OF_GAME_MODE seems to alleviate our fullscreen woes
	ofSetupOpenGL(WIDTH, HEIGHT,OF_WINDOW);
	ofRunApp(new ofApp());
}

//========================================================================
void BaseImage::overwrite_fbo(ofFbo * f){
    fbo.begin();
    ofClear(0,0,0,0);
    ofSetColor(255);
    f->draw(0, 0);
    fbo.end();
}

ofFbo BaseImage::get_fbo(void) {
    return fbo;
}

//========================================================================
Still::Still(const char *fname){
    img.load(fname);
    img.resize(WIDTH, HEIGHT);

    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);

    fbo.begin();
    ofClear(0,0,0,0);
    img.draw(0,0);
    fbo.end();
}

Still::Still(ofFbo * _input){
    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
    
    fbo.begin();
    ofClear(0,0,0,0);
    _input->draw(0,0);
    fbo.end();
}


Still::Still(void){
    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
    
    fbo.begin();
    ofClear(0,0,0,0);
    fbo.end();
}

void Still::update(void){
    ;
}

ofTexture Still::get_texture(void){
    return fbo.getTexture();
}

void Still::display(){
    fbo.draw(0, 0);
}

//========================================================================
Video::Video(const char *fname){
    mov.load(fname);
    sleep(15);
    cout << mov.isLoaded() << endl;

    mov.play();
    mov.setPaused(true);

    fbo.allocate(WIDTH, HEIGHT, GL_RGBA);

    update();
}

void Video::update(void){
    if (mov.getPosition() == 1.0) {
        mov.setPosition(0.0);
    }
    mov.nextFrame();
    mov.update();

    fbo.begin();
    ofClear(0,0,0,0);
    mov.draw(0,0,WIDTH,HEIGHT);
    fbo.end();
}

ofTexture Video::get_texture(){
    return mov.getTexture();
}

void Video::display(){
    fbo.draw(0,0);
}

//========================================================================
void NewTransform::draw(void) {
    fbo->draw(0, 0);
}

void NewTransform::init_fbo(void) {
    fbo = new ofFbo();
    fbo->allocate(WIDTH, HEIGHT, GL_RGBA);
}

ofFbo *NewTransform::get_fbo(void) {
    return fbo;
}

void NewTransform::set_fbo(ofFbo *f) {
    fbo = f;
}

void NewTransform::draw_quad(void) {
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(WIDTH, 0); glVertex3f(WIDTH, 0, 0);
    glTexCoord2f(WIDTH, HEIGHT); glVertex3f(WIDTH, HEIGHT, 0);
    glTexCoord2f(0, HEIGHT);  glVertex3f(0, HEIGHT, 0);
    glEnd();
}

//========================================================================
DisplayImage::DisplayImage(void) {
}

void DisplayImage::update(void) {
    process_image();
}

void DisplayImage::process_image(void) {
    fbo->begin();
    ofClear(0, 0, 0, 0);
    input->display();
    fbo->end();
}


//========================================================================
Mirror::Mirror(){
    shader.load("shadersGL2/mirror");
    mode = 0;
}

void Mirror::set_mode(int _mode){
    mode = _mode;
}

void Mirror::update(void){
    process_image();
}

void Mirror::process_image(void){
    ofTexture tex0 = input->fbo.getTexture();
    
    fbo->begin();
    ofClear(0,0,0,1);
    shader.begin();

        shader.setUniformTexture("tex0", tex0, 0);
        shader.setUniform1i("mode", mode);
        shader.setUniform2f("dim", WIDTH, HEIGHT);

    draw_quad();
    shader.end();
    fbo->end();
}


//========================================================================
Smear::Smear(BaseImage *_fcn, float xi, float yi, float init_dx, float init_dy) {
    shader.load("shadersGL2/smear");
    fcn = _fcn;
    x_scale = xi;
    y_scale = yi;
    dx = init_dx;
    dy = init_dy;
}

void Smear::update_delta(float new_dx, float new_dy){
    dx = new_dx;
    dy = new_dy;
}

void Smear::update(void) {
    x_scale += dx;
    y_scale += dy;

    process_image();
}

void Smear::process_image(void) {
    ofTexture tex0 = input->fbo.getTexture();
    ofTexture tex1 = fcn->fbo.getTexture();

    fbo->begin();
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

    fbo->end();
}

//========================================================================
SmearInner::SmearInner(BaseImage *_fcn) {
    shader.load("shadersGL2/smear_inner");
    fcn = _fcn;
    scale = 10.0;
}

void SmearInner::set_scale(float _scale){
    scale = _scale;
}

void SmearInner::update(void) {
    process_image();
}

void SmearInner::process_image(void) {
    ofTexture tex0 = input->fbo.getTexture();
    ofTexture tex1 = fcn->fbo.getTexture();

    fbo->begin();
	ofClear(0, 0, 0, 1);

        shader.begin();

            shader.setUniformTexture("tex0",   tex0, 0);
            shader.setUniformTexture("tex1",  tex1, 1);
            shader.setUniform1i("w", WIDTH);
            shader.setUniform1i("h", HEIGHT);
            shader.setUniform1f("scale", scale);

            draw_quad();

        shader.end();

    fbo->end();
}


//========================================================================
Invert::Invert(float _scale) {
    shader.load("shadersGL2/invert");
    scale = _scale;
}

void Invert::update(void) {
    process_image();
}

void Invert::process_image(void) {
    ofTexture tex0 = input->fbo.getTexture();
    
    fbo->begin();
    ofClear(0, 0, 0, 1);
    
        shader.begin();

            shader.setUniformTexture("tex0", tex0, 0);
            shader.setUniform1f("scale", scale);

            draw_quad();

        shader.end();

    fbo->end();
}

//========================================================================
Grayscale::Grayscale(float _scale) {
    shader.load("shadersGL2/bw");
    scale = _scale;
}

void Grayscale::update(void) {
    process_image();
}

void Grayscale::process_image(void) {
    ofTexture tex0 = input->fbo.getTexture();
    
    fbo->begin();
    ofClear(0, 0, 0, 1);

        shader.begin();

            shader.setUniformTexture("tex0", tex0, 0);
            shader.setUniform1f("scale", scale);

            draw_quad();

        shader.end();

    fbo->end();
}

//========================================================================
ShadowMask::ShadowMask(float _threshold) {
    shader.load("shadersGL2/shadow_mask");
    threshold = _threshold;
}

void ShadowMask::update(void) {
    process_image();
}

void ShadowMask::process_image(void) {
    ofTexture tex0 = input->fbo.getTexture();
    
    fbo->begin();
    ofClear(0, 0, 0, 1);

        shader.begin();

            shader.setUniformTexture("tex0", tex0, 0);
            shader.setUniform1f("threshold", threshold);

            draw_quad();

        shader.end();

    fbo->end();
}

////========================================================================
//ColorMap::ColorMap(Image *source, Image *target) {
//    shader.load("shadersGL2/color_map");
//    img1 = source;
//    img2 = target;
//    processed.allocate(WIDTH, HEIGHT, OF_IMAGE_COLOR);
//}
//
//void ColorMap::update(void) {
//    process_image();
//}
//
//void ColorMap::process_image(void) {
//    vector<double> sr(WIDTH * HEIGHT);
//    vector<double> sg(WIDTH * HEIGHT);
//    vector<double> sb(WIDTH * HEIGHT);
//    vector<double> tr(WIDTH * HEIGHT);
//    vector<double> tg(WIDTH * HEIGHT);
//    vector<double> tb(WIDTH * HEIGHT);
//
//    for(int i = 0; i < HEIGHT; i++) {
//        for(int j = 0; j < WIDTH; j++) {
//            ofColor s_color = img1->img.getColor(j, i);
//            ofColor t_color = img2->img.getColor(j, i);
//            int index = j + (i*WIDTH);
////            sr[index] = s_color.r;
////            sg[index] = s_color.g;
////            sb[index] = s_color.b;
////            tr[index] = t_color.r;
////            tg[index] = t_color.g;
////            tb[index] = t_color.b;
//
//            sr[index] = s_color.getHue();
//            sg[index] = s_color.getSaturation();
//            sb[index] = s_color.getBrightness();
//            tr[index] = t_color.getHue();
//            tg[index] = t_color.getSaturation();
//            tb[index] = t_color.getBrightness();
//        }
//    }
//
//    double source_mean_r = mean(sr);
//    double source_mean_g = mean(sg);
//    double source_mean_b = mean(sb);
//    double target_mean_r = mean(tr);
//    double target_mean_g = mean(tg);
//    double target_mean_b = mean(tb);
//
//    double source_std_r = std_dev(sr);
//    double source_std_g = std_dev(sg);
//    double source_std_b = std_dev(sb);
//    double target_std_r = std_dev(tr);
//    double target_std_g = std_dev(tg);
//    double target_std_b = std_dev(tb);
//
//    double coef_r = target_std_r / source_std_r;
//    double coef_g = target_std_g / source_std_g;
//    double coef_b = target_std_b / source_std_b;
//
//    for(int i = 0; i < HEIGHT; i++) {
//        for(int j = 0; j < WIDTH; j++) {
//            ofColor color = img2->img.getColor(j, i);
//            double r_star = ((color.r - target_mean_r) * coef_r) + target_mean_r;
//            double g_star = ((color.g - target_mean_g) * coef_g) + target_mean_g;
//            double b_star = ((color.b - target_mean_b) * coef_b) + target_mean_b;
////            ofColor p_color = ofColor(r_star, g_star, b_star, 1);
//            ofColor p_color = ofColor(0);
//            p_color.setHsb(r_star, g_star, b_star);
//            processed.setColor(j, i, p_color);
//        }
//    }
//    processed.update();
//
//    fbo->begin();
//    ofClear(0, 0, 0, 1);
//    processed.draw(0, 0);
//    fbo->end();
//}

//========================================================================
Twirl::Twirl(void) {
    shader.load("shadersGL2/twirl");
    scale = 0.15;
    center = ofVec2f(0,0);
}

void Twirl::set_center(float new_x, float new_y){
    center.x = new_x;
    center.y = new_y;
}

void Twirl::set_scale(float _scale){
    scale = _scale;
}

void Twirl::update(void) {
    set_center(ofGetMouseX(), ofGetMouseY());
    process_image();
}

void Twirl::process_image(void) {
    ofTexture tex0 = input->fbo.getTexture();
    
    fbo->begin();
    ofClear(0, 0, 0, 1);

        shader.begin();

            shader.setUniformTexture("tex0", tex0, 0);
            shader.setUniform1f("scale", scale);
            shader.setUniform1i("width", WIDTH);
            shader.setUniform1i("height", HEIGHT);
            shader.setUniform1i("time", ofGetFrameNum());
            shader.setUniform2f("center", center.x, center.y);
            draw_quad();

        shader.end();

    fbo->end();
}

//========================================================================
NoiseMask::NoiseMask(BaseImage*_mask) {
    shader.load("shadersGL2/noise_mask");
    mask = _mask;
    scale = 1.0;
}

void NoiseMask::update(void) {
    process_image();
}

void NoiseMask::set_scale(float _scale){
    scale = _scale;
}

void NoiseMask::process_image(void) {
    ofTexture tex0 = input->fbo.getTexture();
    ofTexture tex1 = mask->fbo.getTexture();
    
    fbo->begin();
    ofClear(0, 0, 0, 1);

        shader.begin();

            shader.setUniformTexture("tex0", tex0, 0);
            shader.setUniformTexture("tex1", tex1, 1);
            shader.setUniform1f("time", ofGetElapsedTimef());
            shader.setUniform1f("scale", scale);

            draw_quad();

        shader.end();

    fbo->end();
}

//========================================================================
HeatDistort::HeatDistort(BaseImage*i1, BaseImage*i2) {
    shader.load("shadersGL2/heat");
    input = i1;
    mask = i2;
    x0 = y0 = 0;
    frequency = 40.0f;
    time = 0.0f;
    distort = 0.05f;
    rise = 0.8f;

}

void HeatDistort::update(void) {
    process_image();
}

void HeatDistort::process_image(void) {
    ofTexture tex0 = input->fbo.getTexture();
    

    
    //TO DO: optimize to grayscale
    ofTexture tex1 = mask->fbo.getTexture();
    time = ofGetElapsedTimef();

    fbo->begin();
    ofClear(0, 0, 0, 1);

        shader.begin();

            shader.setUniformTexture("tex0", tex0, 0);
            shader.setUniformTexture("tex1", tex1, 1);
            shader.setUniform1f("time", time);
            shader.setUniform2f("mouse", ofGetMouseX(), ofGetMouseY());

            draw_quad();

        shader.end();

    fbo->end();
}

//========================================================================
// UNFINISHED>>>>>>>>>TODO<<<<<<<<<<<UNFINISHED
Aberration::Aberration(void){
    shader.load("shadersGL2/chromatic_aberration");
    
}

void Aberration::update(void){
    float distortionSize(1.12);
    float distortion(0.08);
    float aberrationAmount(0.08);
    float vignetteSharpness(8.0);
    float vignetteSize(0.8);
    float noiseAmount(0.1);
    ofTexture tex0 = input->fbo.getTexture();
    fbo->begin();
    ofClear(0,0,0,1);
    shader.begin();
    shader.setUniformTexture("tex0", tex0, 0);
    shader.setUniform1f("amount", amount);
    draw_quad();
    shader.end();
    fbo->end();
    
}

//========================================================================
NoiseMaker::NoiseMaker(void){
    shader.load("shadersGL2/noise");
    update();
}

void NoiseMaker::update(void){
    process_image();
}

void NoiseMaker::process_image(void){
    fbo->begin();
    ofClear(0,0,0,1);

        shader.begin();

            shader.setUniform1f("time", ofGetElapsedTimef());

            draw_quad();

        shader.end();

    fbo->end();

}

//========================================================================
Swarm::Swarm(void){
    
    opacity = 0.0;

    //initialize the particle texture
    w =650;
    h =650;
    
    createFbo();
    createMesh();

    // shaders
    updateShader.load("shadersGL2/updateShader");
    drawShader.load("shadersGL2/drawShader");

    createPoints();

}

void Swarm::setOpacity(float op_in){
    opacity = op_in;
}

void Swarm::createFbo(){
    ofFbo::Settings s;
    s.internalformat = GL_RGBA32F_ARB;
    s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    s.minFilter = GL_NEAREST;
    s.maxFilter = GL_NEAREST;
    s.wrapModeHorizontal = GL_CLAMP;
    s.wrapModeVertical = GL_CLAMP;
    s.width = w;
    s.height = h;
    particleFbo.allocate(s);
}

void Swarm::createMesh(){
    mesh.clear();
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            mesh.addVertex(ofVec3f(0,0));
            mesh.addTexCoord(ofVec2f(x, y));
        }
    }

    mesh.setMode(OF_PRIMITIVE_POINTS);


    quadMesh.addVertex(ofVec3f(-1.f, -1.f, 0.f));
    quadMesh.addVertex(ofVec3f(1.f, -1.f, 0.f));
    quadMesh.addVertex(ofVec3f(1.f, 1.f, 0.f));
    quadMesh.addVertex(ofVec3f(-1.f, 1.f, 0.f));

    quadMesh.addTexCoord(ofVec2f(0.f, 0.f));
    quadMesh.addTexCoord(ofVec2f(w, 0.f));
    quadMesh.addTexCoord(ofVec2f(w, h));
    quadMesh.addTexCoord(ofVec2f(0.f, h));

    quadMesh.addIndex(0);
    quadMesh.addIndex(1);
    quadMesh.addIndex(2);
    quadMesh.addIndex(0);
    quadMesh.addIndex(2);
    quadMesh.addIndex(3);

    quadMesh.setMode(OF_PRIMITIVE_TRIANGLES);
}

void Swarm::createPoints(){
    float* particlePosns = new float[w * h * 4];
    for (unsigned y = 0; y < h; ++y)
    {
        for (unsigned x = 0; x < w; ++x)
        {
            //ofVec2f location = getLocation(mask);
            unsigned idx = y * w + x;
            particlePosns[idx * 4] = ofRandom(WIDTH); // particle x
            particlePosns[idx * 4 + 1] = ofRandom(HEIGHT); // particle y
            particlePosns[idx * 4 + 2] = ofRandom(0, 0); // particle z
            particlePosns[idx * 4 + 3] = 0.f; // dummy
        }
    }

    particleFbo.getTexture().bind();
    glTexSubImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, w, h, GL_RGBA, GL_FLOAT, particlePosns);
    particleFbo.getTexture().unbind();

    delete[] particlePosns;
}

void Swarm::update(){

    // Update the positions of the particles
    particleFbo.begin();
    glPushAttrib(GL_ENABLE_BIT);
    glViewport(0, 0, w, h);
    glDisable(GL_BLEND);

    updateShader.begin();
    updateShader.setUniform3f("mouse", ofGetMouseX(), ofGetMouseY(), 0.0);
    updateShader.setUniform1f("radiusSquared", 200.0);
    updateShader.setUniform1f("elapsed", ofGetElapsedTimef());
    updateShader.setUniform2f("dim", WIDTH, HEIGHT);
    updateShader.setUniformTexture("tex0", particleFbo.getTexture(), 0);
    updateShader.setUniformTexture("velocities", input->fbo.getTexture(), 1);
    quadMesh.draw();
    updateShader.end();

    glPopAttrib();
    particleFbo.end();

    // Draw the image to ofFbo fbo
    process_image();
}

void Swarm::process_image(){
    fbo->begin();
    ofClear(0,0,0,1);

    ofEnableBlendMode(OF_BLENDMODE_ADD);
    drawShader.begin();
    drawShader.setUniformTexture("tex1", input->fbo.getTexture(), 10);
    drawShader.setUniform1f("mouseX", 1.0);
    mesh.draw();
    drawShader.end();
    //source->display();
    ofDisableBlendMode();

    fbo->end();
}

//========================================================================
Particle::Particle(float x, float y, float _d, ofColor c, ofColor bg){
    original_location.x = x;
    original_location.y = y;
    reset_location();
    color = c;
    bg_color = bg;
    d = _d;
}

void Particle::reset_location(){
    location.x = original_location.x;
    location.y = original_location.y;
}

void Particle::up(){
    location.y -= d;
}

void Particle::draw_original(){
    ofSetColor(bg_color);
    ofDrawRectangle(original_location.x, original_location.y, 1,1);
}

void Particle::draw(){
    ofSetColor(color);
    //ofDrawLine(location.x, location.y, original_location.x, original_location.y);
    ofDrawRectangle(location.x, location.y, 1, 1);
}

//========================================================================
Disintegrate::Disintegrate(BaseImage * _source, BaseImage * _mask, BaseImage * _delta){
    source = _source;
    mask = _mask;
    delta = _delta;

    create_particles();
}

void Disintegrate::create_particles(){
    ofPixels delta_pixels, mask_pixels, source_pixels;
    delta->fbo.readToPixels(delta_pixels);
    mask->fbo.readToPixels(mask_pixels);
    source->fbo.readToPixels(source_pixels);
    for (int j = 0; j < HEIGHT; j++){
        for (int i = 0; i < WIDTH; i++){
            float d = delta_pixels.getColor(i, j).getLightness();
            if (d > 0.0){
                ofColor c = source_pixels.getColor(i, j);
                ofColor o = mask_pixels.getColor(i,j);
                if (d < 250.0){
                    d = max(0.f, ofRandom(-200, d));
                }
                Particle * p = new Particle((float)i, (float)j, d/255.0, c, o);
                particles.push_back(p);
            }
        }
    }
}

bool Disintegrate::in_bounds(Particle * p){
    //return (p->location.x >= 0 and p->location.x < WIDTH and p->location.y >= 0 and p->location.y < HEIGHT);
    return (abs(p->location.y - p->original_location.y) < 100.0);
}

void Disintegrate::update(){
    // Update the positions of the particles
    for (Particle * p : particles){
        if (p->location.y < ofGetMouseY())
            p->up();
        if (not in_bounds(p)){
            p->reset_location();
        }
    }

    // Draw the image to ofFbo fbo
    process_image();
}

void Disintegrate::process_image(){
    fbo->begin();
    ofClear(0,0,0,0);
    source->display();
    for (Particle *p: particles){
        p->draw_original();
    }
    for (Particle *p: particles){
        p->draw();
    }
    fbo->end();
}

//========================================================================
/* 
 * Thanks kylemcdonald for this blur¡¡!!
 */
float Gaussian(float x, float mean, float variance) {
    x -= mean;
    return (1. / sqrt(TWO_PI * variance)) * exp(-(x * x) / (2 * variance));
}

void GaussianRow(int elements, vector<float>& row, float variance = .2) {
    row.resize(elements);
    for(int i = 0; i < elements; i++) {
        float x = ofMap(i, 0, elements - 1, -1, 1);
        row[i] = Gaussian(x, 0, variance);
    }
}

string generateBlurSource(int radius, float shape) {
    int rowSize = 2 * radius + 1;
    
    // generate row
    vector<float> row;
    GaussianRow(rowSize, row, shape);
    
    // normalize row and coefficients
    vector<float> coefficients;
    float sum = 0;
    for(int i = 0; i < row.size(); i++) {
        sum += row[i];
    }
    for(int i = 0; i < row.size(); i++) {
        row[i] /= sum;
    }
    int center = row.size() / 2;
    coefficients.push_back(row[center]);
    for(int i = center + 1; i < row.size(); i += 2) {
        float weightSum = row[i] + row[i + 1];
        coefficients.push_back(weightSum);
    }
    
    // generate offsets
    vector<float> offsets;
    for(int i = center + 1; i < row.size(); i += 2) {
        int left = i - center;
        int right = left + 1;
        float leftVal = row[i];
        float rightVal = row[i + 1];
        float weightSum = leftVal + rightVal;
        float weightedAverage = (left * leftVal + right * rightVal) / weightSum;
        offsets.push_back(weightedAverage);
    }
    
    stringstream src;
    src << "#version 120\n";
    src << "#extension GL_ARB_texture_rectangle : enable\n";
    src << "uniform sampler2DRect source;\n";
    src << "uniform vec2 direction;\n";
    src << "void main(void) {\n";
    src << "\tvec2 tc = gl_TexCoord[0].st;\n";
    src << "\tgl_FragColor = " << coefficients[0] << " * texture2DRect(source, tc);\n";
    for(int i = 1; i < coefficients.size(); i++) {
        int curOffset = i - 1;
        src << "\tgl_FragColor += " << coefficients[i] << " * \n";
        src << "\t\t(texture2DRect(source, tc - (direction * " << offsets[i - 1] << ")) + \n";
        src << "\t\ttexture2DRect(source, tc + (direction * " << offsets[i - 1] << ")));\n";
    }
    src << "}\n";
    
    return src.str();
}

string generateCombineSource(int passes, float downsample) {
    vector<string> combineNames;
    for(int i = 0; i < passes; i++) {
        combineNames.push_back("s" + ofToString(i));
    }
    stringstream src;
    src << "#version 120\n";
    src << "#extension GL_ARB_texture_rectangle : enable\n";
    src << "uniform sampler2DRect " << ofJoinString(combineNames, ",") << ";\n";
    src << "uniform float brightness;\n";
    if(downsample == 1) {
        src << "const float scaleFactor = 1.;\n";
    } else {
        src << "const float scaleFactor = " << downsample << ";\n";
    }
    src << "void main(void) {\n";
    src << "\tvec2 tc = gl_TexCoord[0].st;\n";
    for(int i = 0; i < passes; i++) {
        src << "\tgl_FragColor " << (i == 0 ? "=" : "+=");
        src << " texture2DRect(" << combineNames[i] << ", tc);";
        src << (i + 1 != passes ? " tc *= scaleFactor;" : "");
        src << "\n";
    }
    src << "\tgl_FragColor *= brightness / " << passes << ".;\n";
    src << "}\n";
    return src.str();
}

ofxBlur::ofxBlur(void){
    this->setup(WIDTH, HEIGHT, 3, .2, 4);
    scale = 1;
    rotation = 1.0;
    brightness = 1.0;
}

void ofxBlur::setup(int width, int height, int radius, float shape, int passes, float downsample) {
    string blurSource = generateBlurSource(radius, shape);
    if(ofGetLogLevel() == OF_LOG_VERBOSE) {
        cout << "ofxBlur is loading blur shader:" << endl << blurSource << endl;
    }
    blurShader.setupShaderFromSource(GL_FRAGMENT_SHADER, blurSource);
    blurShader.linkProgram();
    
    if(passes > 1) {
        string combineSource = generateCombineSource(passes, downsample);
        if(ofGetLogLevel() == OF_LOG_VERBOSE) {
            cout << "ofxBlur is loading combine shader:" << endl << combineSource << endl;
        }
        combineShader.setupShaderFromSource(GL_FRAGMENT_SHADER, combineSource);
        combineShader.linkProgram();
    }
    
    base.allocate(width, height);
    base.begin(); ofClear(0); base.end();
    
    ofFbo::Settings settings;
    settings.useDepth = false;
    settings.useStencil = false;
    settings.numSamples = 0;
    ping.resize(passes);
    pong.resize(passes);
    for(int i = 0; i < passes; i++) {
        ofLogVerbose() << "building ping/pong " << width << "x" << height;
        settings.width = width;
        settings.height = height;
        ping[i].allocate(settings);
        ping[i].begin(); ofClear(0); ping[i].end();
        pong[i].allocate(settings);
        pong[i].begin(); ofClear(0); pong[i].end();
        //        ping[i].setDefaultTextureIndex(i);
        //        pong[i].setDefaultTextureIndex(i);
        width *= downsample;
        height *= downsample;
    }
}

void ofxBlur::setScale(float scale) {
    this->scale = scale;
}

void ofxBlur::setRotation(float rotation) {
    this->rotation = rotation;
}

void ofxBlur::setBrightness(float brightness) {
    this->brightness = brightness;
}

void ofxBlur::begin() {
    base.begin();
}

void ofxBlur::end() {
    base.end();
    
    ofPushStyle();
    ofSetColor(255);
    
    ofVec2f xDirection = ofVec2f(scale, 0).getRotatedRad(rotation);
    ofVec2f yDirection = ofVec2f(0, scale).getRotatedRad(rotation);
    for(int i = 0; i < ping.size(); i++) {
        ofFbo& curPing = ping[i];
        ofFbo& curPong = pong[i];
        
        // resample previous result into ping
        curPing.begin();
        int w = curPing.getWidth();
        int h = curPing.getHeight();
        if(i > 0) {
            ping[i - 1].draw(0, 0, w, h);
        } else {
            base.draw(0, 0, w, h);
        }
        curPing.end();
        
        // horizontal blur ping into pong
        curPong.begin();
        blurShader.begin();
        blurShader.setUniformTexture("source", curPing.getTexture(), 0);
        blurShader.setUniform2f("direction", xDirection.x, xDirection.y);
        curPing.draw(0, 0);
        blurShader.end();
        curPong.end();
        
        // vertical blur pong into ping
        curPing.begin();
        blurShader.begin();
        blurShader.setUniformTexture("source", curPong.getTexture(), 0);
        blurShader.setUniform2f("direction", yDirection.x, yDirection.y);
        curPong.draw(0, 0);
        blurShader.end();
        curPing.end();
    }
    
    // render ping back into base
    if(ping.size() > 1) {
        int w = base.getWidth();
        int h = base.getHeight();
        
        ofPlanePrimitive plane;
        plane.set(w, h);
        plane.mapTexCoordsFromTexture(ping[0].getTexture());
        
        base.begin();
        combineShader.begin();
        for(int i = 0; i < ping.size(); i++) {
            string name = "s" + ofToString(i);
            combineShader.setUniformTexture(name.c_str(),
                                            ping[i].getTexture(),
                                            1 + i);
        }
        combineShader.setUniform1f("brightness", brightness);
        ofPushMatrix();
        ofTranslate(w / 2, h / 2);
        plane.draw();
        ofPopMatrix();
        combineShader.end();
        base.end();
    } else {
        base.begin();
        ping[0].draw(0, 0);
        base.end();
    }
    
    ofPopStyle();
}

ofTexture& ofxBlur::getTextureReference() {
    return base.getTexture();
}

void ofxBlur::draw() {
    base.draw(0, 0);
}

void ofxBlur::draw(ofRectangle rect) {
    base.draw(rect);
}

void ofxBlur::update(void){
    process_image();
}

void ofxBlur::process_image(void){
    this->begin();
    //ofClear(0,0,0,1);
    this->input->fbo.draw(0,0);
    this->end();
    
    fbo->begin();
    //ofClear(0,0,0,1);
    this->draw();
    fbo->end();
}
// kylemcdonald end


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

void run_supercollider(void) {
    pid_t pid = fork();

    if(pid == 0) {
        system("../../../../sc ../../../../src/sc/audio.scd > ../../../../log/sc_log.txt");
    }
}