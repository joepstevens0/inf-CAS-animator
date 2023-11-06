#include "texrender.h"

#include <fstream>
#include <iostream>
#include <sstream>


TexRender::TexRender()
    : shader{Program::from_path("shader/2dshader.vert", "shader/2dshader.frag")}, vbo{create_vbo()}
{
}

TexRender::~TexRender(){
    delete vbo;
}

void TexRender::draw_rect(const int pos[2], const int size[2], const float color[4], int window_size[2])
{
    draw_rect(pos, size, color, 0., window_size);
}

void TexRender::draw_rect(const int pos[2], const int size[2], const float color[4], float rotation_angle, int window_size[2]){
    float size_float[] = {(float)size[0], (float)size[1]};
    float pos_float[] = {(float)pos[0], (float)pos[1]};
    float windowsize_float[] = {(float)window_size[0], (float)window_size[1]};
    shader->set_used();
    shader->bind_vec2("uScreenSize", windowsize_float);
    shader->bind_vec2("uOffset", pos_float);
    shader->bind_vec2("uSize", size_float);
    shader->bind_bool("uHasTex", false);
    shader->bind_vecf4("uColor", color);
    shader->bind_f32("uRot", rotation_angle);
    vbo->draw();
}


VertexBuffer* TexRender::create_vbo()
{
    return VertexBuffer::from_rect();
}