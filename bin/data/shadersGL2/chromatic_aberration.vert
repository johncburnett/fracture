#version 120

uniform mat4 modelViewProjectionMatrix;

varying vec2 texCoordVarying;

void main() {
    texCoordVarying = gl_MultiTexCoord0.xy;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;; 
}