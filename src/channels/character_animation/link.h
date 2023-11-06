#ifndef LINK_H
#define LINK_H

#include <glm/mat4x4.hpp>

class Render;
class Object;

class Link{
public:
    Link(glm::mat4 m1, glm::mat4 m2);
    void update(glm::mat4 m1, glm::mat4 m2);

    void draw(Render* render, glm::mat4 offset);
private:
    Object* m_object;
};

#endif