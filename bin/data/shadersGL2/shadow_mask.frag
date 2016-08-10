#version 120

uniform sampler2DRect tex0;

uniform float threshold;

varying vec2 texCoordVarying;

void main(void) {
    vec4 color = texture2DRect(tex0, texCoordVarying);
    float brightness = (color[0] + color[1] + color[2]) / 3.0;
    if(brightness > threshold) {
        color = vec4(0.0, 0.0, 0.0, 0.0);
    }
    gl_FragColor = color;
}