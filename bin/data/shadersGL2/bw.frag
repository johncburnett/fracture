#version 120

uniform sampler2DRect tex0;

uniform float scale;

varying vec2 texCoordVarying;

void main() {
    vec4 color = texture2DRect(tex0, texCoordVarying);
    float avg = (color.r + color.g + color.b) / 3.0;
    gl_FragColor = vec4(avg, avg, avg, color.a);
}