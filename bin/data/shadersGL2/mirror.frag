#version 120

uniform sampler2DRect tex0;
uniform int mode;
uniform vec2 dim;

varying vec2 texCoordVarying;

void main() {
    vec2 new_color = texCoordVarying;
    
    if ((0==mode) && (texCoordVarying.x > dim.x/2))
            new_color.x = dim.x - texCoordVarying.x;
    else if ((1==mode) && (texCoordVarying.x < dim.x/2))
            new_color.x = dim.x - texCoordVarying.x;
    else if ((2==mode) && (texCoordVarying.y > dim.y/2))
            new_color.y = dim.y - texCoordVarying.y;
    else if ((3==mode) && (texCoordVarying.y < dim.y/2))
            new_color.y = dim.y - texCoordVarying.y;
    
    gl_FragColor = texture2DRect(tex0, new_color);
}