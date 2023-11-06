#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 vTexCoord;
flat out int vDiscard;

uniform vec4 uParticlePositions[1000];
uniform mat4 uView;
uniform mat4 uProjection;


void main()
{
    float time_to_live = uParticlePositions[gl_InstanceID].w;
    if (time_to_live < 0){
        vDiscard = 1;
    }else {
        vDiscard = 0;
    }
    float size = time_to_live;
    vec3 center = uParticlePositions[gl_InstanceID].xyz;
    vec3 vert_pos = vec3(size*aPos,0);

    vec3 cam_right = vec3(uView[0][0], uView[1][0], uView[2][0]);
    vec3 cam_up = vec3(uView[0][1], uView[1][1], uView[2][1]);

    vec3 pos = center + cam_right*vert_pos.x + cam_up*vert_pos.y;

    gl_Position = uProjection*uView*vec4(pos,1);

    vTexCoord = aTexCoord;
}  