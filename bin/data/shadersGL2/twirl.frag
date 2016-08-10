#version 120
#extension GL_EXT_gpu_shader4: enable
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect tex0;
uniform int width;
uniform int height;
uniform float scale;

uniform int time = 0;
uniform float radius = 600.0;
uniform float angle = 0.4;
uniform vec2 center = vec2(0, 540);

varying vec2 texCoordVarying;

void main (void) {
    vec2 uv = texCoordVarying;
    vec2 texSize = vec2(width, height);
    vec2 tc = uv;
    vec2 ci = center;
    ci.x = ci.x + (time * 50) % width;
    tc -= ci;
    float dist = length(tc)+sin(time*.01);
    if (dist < radius) {
        float percent = pow(0.98, dist*scale);
        float theta = percent * percent * angle * 8;
        float s = sin(theta);
        float c = cos(theta);
        tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));
    }
    tc += ci;
    vec3 color = texture2DRect(tex0, (tc / texSize)*texSize).rgb;
    gl_FragColor = vec4(color, 1.0);
}