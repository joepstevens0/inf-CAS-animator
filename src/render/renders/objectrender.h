#ifndef OBJECTRENDER_H
#define OBJECTRENDER_H

#include "../shader.h"
#include "../program.h"
#include "../camera.h"
#include "object.h"

class ObjectRender{
public:
    ObjectRender();

    void draw(Camera* cam, std::vector<Object*> objects, int window_size[2], glm::mat4 offset = glm::mat4(1.f));

    void start_picking();
    int stop_picking(int pos[2], int window_size[2]);

private:

    void render(Program* shader, Camera* cam, std::vector<Object*> objects, int window_size[2], glm::mat4 offset);

    bool m_picking = false;
    Program* m_shader;
    Program* m_shader_picking;

};

#endif