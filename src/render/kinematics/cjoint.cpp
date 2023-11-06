#include "cjoint.h"
#include <render/renders/object.h>
#include <render/render.h>
#include "clink.h"

#include "dh_mat.h"

CJoint::CJoint(float angle, float offset, float angle_bounds[2], CLink *parent)
    : m_angle{angle}, m_offset{offset}, m_angle_bounds{angle_bounds[0], angle_bounds[1]}, m_parent{parent}
{
    m_object = Object::from_cube();
    m_object->set_model(glm::scale(glm::mat4(1.), glm::vec3(1)));
}
void CJoint::init(int &id)
{
    for (unsigned int i = 0; i < m_children.size(); ++i)
    {
        m_children[i]->init(id);
    }
    m_object->set_id(id);
    id += 1;

    update();
}

CJoint::~CJoint()
{
    delete m_object;

    for (unsigned int i = 0; i < m_children.size() ;++i){
        delete m_children[i];
    }
}

glm::mat4 CJoint::createLocalTransfrom()
{
    if (!m_parent)
    {
        return dh_create_transform(m_offset, m_angle, 0, 0);
    }
    float length = m_parent->get_length();
    float twist = m_parent->get_twist();
    return dh_create_transform(m_offset, m_angle, length, twist);
}

void CJoint::draw(Render* render, glm::mat4 offset)
{
    m_model = offset*createLocalTransfrom();
    render->draw_objects({m_object}, m_model);
    for (unsigned int i = 0; i < m_children.size(); ++i)
    {
        m_children[i]->draw(render, m_model);
    }
}

SearchType CJoint::get_by_id(int id)
{
    if (m_object->getId() == id)
        return {this, nullptr};
    for (unsigned int i = 0; i < m_children.size(); ++i)
    {
        SearchType r = m_children[i]->get_by_id(id);
        if (r.joint || r.link)
            return r;
    }
    return {nullptr, nullptr};
}

void CJoint::update(){

    m_local_transform = createLocalTransfrom();
}
#include <glm/gtx/string_cast.hpp>

glm::vec3 CJoint::get_pos() const{
    return glm::vec3(m_model[3]);
}

std::vector<CLink*> CJoint::get_children() const{
    return m_children;
}

std::vector<CJoint*> CJoint::collect_joints(){
    std::vector<CJoint*> joints = {this};
    for (unsigned int i = 0; i < m_children.size();++i){
        CJoint* child = m_children[i]->get_joint();
        if (child){
            std::vector<CJoint*> child_joints = child->collect_joints();
            joints.insert(joints.end(), child_joints.begin(), child_joints.end());
        }
    }
    return joints;
}