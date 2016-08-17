#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect tex1;

varying vec4 texCoordVarying;

void main() {
    texCoordVarying = gl_MultiTexCoord0;
    gl_Position = gl_ModelViewProjectionMatrix * vec4(texture2DRect(tex0, gl_MultiTexCoord0.st).xyz, 1.0);
}