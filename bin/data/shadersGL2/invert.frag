#version 120

uniform sampler2DRect tex0;

uniform float scale;

varying vec2 texCoordVarying;

void main() {
    vec4 color = texture2DRect(tex0, texCoordVarying);
    vec4 invert = vec4(1.0-color.r, 1.0-color.g, 1.0-color.b, color.a);
    color.r = (color.r * (1.0-scale)) + (invert.r * scale);
    color.g = (color.g * (1.0-scale)) + (invert.g * scale);
    color.b = (color.b * (1.0-scale)) + (invert.b * scale);
    gl_FragColor = color;
}