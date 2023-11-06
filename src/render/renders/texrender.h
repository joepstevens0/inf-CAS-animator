#ifndef TEXRENDER_H
#define TEXRENDER_H

#include "../shader.h"
#include "../program.h"
#include "../vertexbuffer.h"

class TexRender
{
public:
    TexRender();
    ~TexRender();


    void draw_rect(const int pos[2], const int size[2], const float color[4], int window_size[2]);
    void draw_rect(const int pos[2], const int size[2], const float color[4], float rotation_angle, int window_size[2]);
private:
    VertexBuffer* create_vbo();

    Program* shader;
    VertexBuffer* vbo;
};

#endif