#version 330 core

layout (location = 0) in vec2 Position;
layout (location = 1) in vec2 TexCoord;

uniform vec2 uScreenSize;

uniform vec2 uOffset;
uniform vec2 uSize;
uniform float uRot;

out vec2 vTexCoord;

void main(){
    float rad = radians(uRot);
    mat2 rot = mat2(
        cos(rad),-sin(rad),
        sin(rad),cos(rad)
    );

    vec2 size = vec2(uSize)/uScreenSize;
    vec2 offset = 2*vec2(uOffset)/uScreenSize;
    offset.y = 2. - offset.y  - 2*size.y;

    vec2 p = Position + vec2(1,1);
    p = p*size;
    p = rot*p;
    p -= vec2(1,1);
    p += offset;

    gl_Position = vec4(p, 0, 1.0);
    vTexCoord = TexCoord;
}