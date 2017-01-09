#version 120

uniform sampler2DRect tex0;
uniform vec2 corner;
uniform vec2 dim;

varying vec2 texCoordVarying;

void main() {
    vec2 new_color = texCoordVarying - corner;
    gl_FragColor = texture2DRect(tex0, new_color);
}