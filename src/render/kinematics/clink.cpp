#include "clink.h"
#include <render/renders/object.h>
#include <render/render.h>

#include "cjoint.h"

#define P_MIN -0.5
#define P_MAX 0.5

CLink::CLink(float length, float twist)
    : m_length{length}, m_twist{twist}
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
}
void CLink::init(int& id)
{
    if (m_next_joint)
    {
        m_next_joint->init(id);
    }

    m_object->set_id(id);
    id += 1;

    update();
}

void CLink::update()
{
    glm::vec3 p = glm::vec4(P_MIN, P_MIN, 0, 1);
    m_object->update_vertex({p, glm::vec2(0, 0)}, 0);
    p = glm::vec4(P_MIN, P_MAX, 0, 1);
    m_object->update_vertex({p, glm::vec2(0, 1)}, 1);

    if (m_next_joint)
    {
        glm::mat4 transform = m_next_joint->get_local_transform();
        p = transform * glm::vec4(P_MAX, P_MAX, 0, 1);
        m_object->update_vertex({p, glm::vec2(1, 1)}, 2);
        p = transform * glm::vec4(P_MAX, P_MIN, 0, 1);
        m_object->update_vertex({p, glm::vec2(1, 0)}, 3);
        m_next_joint->update();
    }
}

void CLink::draw(Render* render, glm::mat4 offset)
{
    render->draw_objects({m_object}, offset);
    if (m_next_joint)
        m_next_joint->draw(render, offset);
}

void CLink::set_next(CJoint *joint)
{
    m_next_joint = joint;
}

SearchType CLink::get_by_id(int id){

    if (m_object->getId() == id)
        return {nullptr, this};
    if (m_next_joint)
        return m_next_joint->get_by_id(id);
    return {nullptr, nullptr};
}