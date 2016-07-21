// these are for the programmable pipeline system
uniform mat4 modelViewProjectionMatrix;
attribute vec4 position;
attribute vec2 texcoord;

// this is something we're creating for this shader
varying vec2 texCoordVarying;

void main() {
    // here we move the texture coordinates
    texCoordVarying = texcoord;
    
    // send the vertices to the fragment shader
	gl_Position = modelViewProjectionMatrix * position;
}
