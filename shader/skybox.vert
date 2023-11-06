#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 vTexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vTexCoords = (uModel*vec4(aPos, 1.0)).xyz;
    mat4 view = mat4(mat3(uView));  // remove translation from view
    vec4 pos = uProjection * view *uModel *vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}  