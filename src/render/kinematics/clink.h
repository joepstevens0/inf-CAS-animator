#ifndef CLINK_H
#define CLINK_H

#include <glm/mat4x4.hpp>
#include "searchtype.h"

class CJoint;
class Render;
class Object;

class CLink{
public:
    CLink(float length, float twist);
    void init(int& id);
    void update();

    void set_next(CJoint* joint);
    void draw(Render* render, glm::mat4 offset);

    CJoint* get_joint() const{return m_next_joint;}

    float& get_length() {return m_length;}
    float& get_twist() {return m_twist;}

    SearchType get_by_id(int id);
private:
    float m_length;
    float m_twist;
    CJoint* m_next_joint = nullptr;

    Object* m_object;
};

#endif