#include "objectrender.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/gtc/matrix_transform.hpp>

ObjectRender::ObjectRender()
    : m_shader{Program::from_path("shader/3dshader.vert", "shader/3dshader.frag")},
      m_shader_picking{Program::from_path("shader/3dshader.vert", "shader/3dshaderpicking.frag")}
{
}

void ObjectRender::draw(Camera *cam, std::vector<Object *> objects, int window_size[2], glm::mat4 offset)
{
    if (m_picking)
        render(m_shader_picking, cam, objects, window_size, offset);
    else
        render(m_shader, cam, objects, window_size, offset);
}

void ObjectRender::start_picking()
{
    m_picking = true;
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
int ObjectRender::stop_picking(int pos[2], int window_size[2])
{
    m_picking = false;

    GLubyte pixel[4];
    glReadPixels(pos[0], window_size[1] - pos[1], 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixel);
    return pixel[0];
}

void ObjectRender::render(Program* shader, Camera *cam, std::vector<Object *> objects, int window_size[2], glm::mat4 offset)
{
    shader->set_used();
    float windowsize_float[] = {(float)window_size[0], (float)window_size[1]};
    m_shader->bind_vec2("uScreenSize", windowsize_float);
    shader->bind_cam(cam, window_size);

    for (unsigned int i = 0; i < objects.size(); ++i)
    {
        shader->bind_i32("uObjectID", objects[i]->getId());
        objects[i]->draw(shader, offset);
    }
}