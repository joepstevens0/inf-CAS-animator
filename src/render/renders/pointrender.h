#ifndef POINTRENDER_H
#define POINTRENDER_H

#include "../shader.h"
#include "../program.h"
#include "../camera.h"
#include "../vertexbuffer.h"

class PointRender{
public:
    PointRender();
    ~PointRender();

    void draw(Camera* cam, std::vector<glm::vec3> points, glm::vec4 color, float size, int window_size[2]);

private:


    Program* m_shader;
    VertexBuffer* m_vbo;

};

#endif