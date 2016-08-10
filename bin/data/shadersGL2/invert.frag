#version 120

uniform sampler2DRect tex0;

uniform float scale;

varying vec2 texCoordVarying;

void main() {
    vec4 color = texture2DRect(tex0, texCoordVarying);
    vec4 invert = vec4(1.0-color.r, 1.0-color.g, 1.0-color.b, color.a);
    float scale_i = 1.0 - scale;
    vec4 r1 = color*scale_i;
    vec4 r2 = invert*scale;
    vec4 result = r1 + r2;
    gl_FragColor = result;
}