#version 120
varying vec4 texCoordVarying;
void main()
{
    texCoordVarying = gl_MultiTexCoord0;
	gl_Position = gl_Vertex;
}
