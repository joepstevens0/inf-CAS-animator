#ifndef CJOINT_H
#define CJOINT_H

#include <vector>
#include <glm/mat4x4.hpp>
#include "searchtype.h"

class Object;
class CLink;
class Render;

class CJoint{
public:
    CJoint(float angle, float offset, float angle_bounds[2], CLink* parent = nullptr);
    ~CJoint();
    void init(int& id);

    void add_link(CLink* link){m_children.push_back(link);}

    void draw(Render* render, glm::mat4 offset);

    SearchType get_by_id(int id);
    glm::vec3 get_pos() const;
    std::vector<CLink*> get_children() const;
    glm::mat4 get_model() const { return m_model;}
    glm::mat4 get_local_transform() const { return m_local_transform;}

    void update();

    float& get_offset(){ return m_offset;}
    float& get_angle() { return m_angle;}
    float& get_lower_bound() {return m_angle_bounds[0]; }
    float& get_upper_bound() {return m_angle_bounds[1]; }

    std::vector<CJoint*> collect_joints();
private:
    glm::mat4 createLocalTransfrom();

    float m_angle;
    float m_offset;
    float m_angle_bounds[2];
    glm::mat4 m_model;
    glm::mat4 m_local_transform;
    CLink* m_parent;
    std::vector<CLink*> m_children;

    Object* m_object;
};

#endif