#version 120

uniform sampler2DRect tex0;

varying vec2 texCoordVarying;

void main() {
    vec4 color = texture2DRect(tex0, texCoordVarying);
    gl_FragColor = color;
}