#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect tex1;

uniform int w;
uniform int h;
uniform float xscale;
uniform float yscale;

varying vec2 texCoordVarying;

void main() {
    vec4 color_t1 = texture2DRect(tex1, texCoordVarying);
    float brightness = (color_t1[0] + color_t1[1] + color_t1[2]) / 3;
    
    float dx = brightness * xscale;
    float dy = brightness * yscale;
    
    vec2 p1;
    p1[0] = texCoordVarying.x + dx;
    p1[1] = texCoordVarying.y + dy;
    
    // bounds
    vec4 color;
    if(p1[0] < 0 || p1[0] >= w || p1[1] < 0 || p1[1] >= h) {
        color = color_t1;
    } else {
        color = texture2DRect(tex0, p1);
    }
    gl_FragColor = color;
}