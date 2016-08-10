#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect tex1;
uniform sampler2DRect noise;

varying vec2 texCoordVarying;

void main(void) {
    vec4 c1 = texture2DRect(tex0, texCoordVarying);
    vec4 c2 = texture2DRect(tex1, texCoordVarying);
    vec4 val = texture2DRect(noise, texCoordVarying);
    float s = val[0];
    float si = 1.0 - s;
    vec4 r1 = c1*s;
    vec4 r2 = c2*si;
    vec4 color = r1 + r2;
    gl_FragColor = color;
}