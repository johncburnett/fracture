#version 150

uniform sampler2D tex0;
uniform sampler2D tex1;

out vec4 outputColor;

void main() {
    gl_FragColor = texture(tex0, texCoordVarying);
}