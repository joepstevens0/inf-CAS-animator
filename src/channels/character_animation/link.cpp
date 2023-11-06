#include "link.h"
#include <render/renders/object.h>
#include <render/render.h>


#define P_MIN -0.5
#define P_MAX 0.5

Link::Link(glm::mat4 m1, glm::mat4 m2)
{
    std::vector<Vertex> points = {
        {glm::vec3(P_MIN, P_MIN, -3), glm::vec2(0, 0)},    // 0 0
        {glm::vec3(P_MIN, P_MAX, -3), glm::vec2(0, 1)},     // 0 1
        {glm::vec3(P_MAX, P_MAX, -3), glm::vec2(1, 1)},      // 1 1
        {glm::vec3(P_MAX, P_MIN, -3), glm::vec2(1, 0)}}; // 1 0

    std::vector<Face> faces = {
        {0, 1, 2},
        {0, 2, 3}};
    m_object = Object::from_points(points, faces);
    update(m1, m2);
}
void Link::update(glm::mat4 m1, glm::mat4 m2){
    glm::vec3 p = m1*glm::vec4(P_MIN, P_MIN, 0, 1);
    m_object->update_vertex({p, glm::vec2(0, 0)}, 0);
    p = m1*glm::vec4(P_MIN, P_MAX, 0, 1);
    m_object->update_vertex({p, glm::vec2(0, 1)}, 1);
    p = m2 * glm::vec4(P_MAX, P_MAX, 0, 1);
    m_object->update_vertex({p, glm::vec2(1, 1)}, 2);
    p = m2 * glm::vec4(P_MAX, P_MIN, 0, 1);
    m_object->update_vertex({p, glm::vec2(1, 0)}, 3);
}
void Link::draw(Render* render, glm::mat4 offset)
{
    render->draw_objects({m_object}, offset);
}
