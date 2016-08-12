#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect tex1;

uniform int time;
uniform vec2 mouse;

varying vec2 texCoordVarying;

void main() {
    vec4 color = texture2DRect(tex0, texCoordVarying);
    vec2 coord = texCoordVarying.st;
    
    vec4 heat = texture2DRect(tex1, coord);
    
    vec2 distort_coord = vec2(0,0);
    
    if (1.0 != heat.r){
        distort_coord = texCoordVarying.st + (vec2(0, 100)/(-30.0*heat.r));
        gl_FragColor = texture2DRect(tex0, distort_coord)*(1.0-heat.r) +
            (color*heat.r);
    } else {
        gl_FragColor = color;
    }
}