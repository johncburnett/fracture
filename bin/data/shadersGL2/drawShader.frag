#version 120
uniform sampler2DRect tex0;
uniform sampler2DRect tex1;
uniform float mouseX;

varying vec4 texCoordVarying;


void main() {

    vec4 coords = texture2DRect(tex0, texCoordVarying.xy);
    vec4 col = texture2DRect(tex1, coords.xy);
    float b = 1.0-(col.x + col.y + col.z)/3.0;
    gl_FragColor =  vec4(b,b,b, mouseX);
}
