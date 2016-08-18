#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect tex1;

uniform int w;
uniform int h;
uniform float scale;

varying vec2 texCoordVarying;

void main() {
    vec4 color_t1 = texture2DRect(tex1, texCoordVarying);
    float brightness = (color_t1[0] + color_t1[1] + color_t1[2]) / 3;
    
    vec2 p1 = vec2(w/2, h/2) - texCoordVarying.xy;
    p1 = normalize(p1);
    
    p1 *= scale*brightness;
    p1.y = 0;
    
    p1 += texCoordVarying;
    // bounds
    vec4 color;
    if(p1[0] < 0 || p1[0] >= w || p1[1] < 0 || p1[1] >= h) {
        color = color_t1;
    } else {
        color = vec4(texture2DRect(tex0, p1).rgb, 1.0);
    }
    
    gl_FragColor = color;
}