#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect tex1;

uniform int time;
uniform float distort;
uniform float rise;

varying vec2 texCoordVarying;

void main() {
    vec4 color = texture2DRect(tex0, texCoordVarying);
    vec2 coord = texCoordVarying.st;
    coord.t -= time * rise * 100;
    
    vec4 value = texture2DRect(tex1, coord);
    vec2 offset = value.xy;
    
    // [0, 1] => [-1, 1]
    offset -= vec2(0.5f, 0.5f);
    offset *= 2.0f;
    offset *= distort;
    
    offset *= (1.0f - texCoordVarying.y); // taper
    vec2 distort_coord = texCoordVarying.st + offset;
    gl_FragColor = color * texture2DRect(tex0, distort_coord);
}