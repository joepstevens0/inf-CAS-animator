#include "pointrender.h"
#include <fstream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>

PointRender::PointRender()
{
    m_vbo = VertexBuffer::from_rect();
    m_shader = Program::from_path("shader/particle.vert", "shader/particle.frag");
}

PointRender::~PointRender()
{
    delete m_shader;
    delete m_vbo;
}


void PointRender::draw(Camera *cam, std::vector<glm::vec3> points, glm::vec4 color, float size, int window_size[2])
{
    m_shader->set_used();
    m_shader->bind_cam(cam, window_size);
    for (unsigned int i = 0; i < points.size(); ++i)
    {
        glm::vec3 p = points[i];
        float pos[4] = {p[0], p[1], p[2], size};
        m_shader->bind_vecf4("uParticlePositions[" + std::to_string(i) + "]", pos);
        m_shader->bind_vecf4("uColor", &color[0]);
    }
    m_vbo->draw_instanced(points.size());
    
}