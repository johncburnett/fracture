
precision highp float;

uniform sampler2DRect tex0;
uniform sampler2DRect tex1;

varying vec2 texCoordVarying;

void main() {
    gl_FragColor = texture2D(tex0, texCoordVarying);
}
