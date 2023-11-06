#ifndef FREEFORM_H
#define FREEFORM_H

#include <glm/glm.hpp>
#include <vector>

#include <render/vertexbuffer.h>

class Camera;
class Render;
class Object;

#define NO_CONTROL_POINT -1

class FreeForm{
public:
    FreeForm(Object* object);
    ~FreeForm();
    void draw(Render* render);

    void mousedown(int pos[2],Render* render);
    void mouseup(int pos[2],Render* render);

    std::vector<glm::vec3> save_points();
    void load_points(std::vector<glm::vec3> points);

    void updateObject();
private:
    glm::vec3 calc_control_pos(float i, float j, float k);
    void map_vertices();
    glm::vec3 update_point(float s, float t, float u);
    void update_links();

    std::vector<Object*> m_points;
    std::vector<Object*> m_links;
    Object* m_object;

    int m_controlPoint = NO_CONTROL_POINT;
    int m_last_pos[2] = {0, 0};
    int m_ismouseup = true;

    std::vector<glm::vec3> m_vertexMapping;
};

#endif