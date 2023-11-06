#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>



class Camera{
public:
    Camera(){}
    void set_view(glm::mat4 view){ m_view = view;}
    glm::mat4 get_view() const { return m_view;}

    // glm::vec3 get_rot_euler() const;
    // glm::vec3 get_pos() const;
private:
    glm::mat4 m_view;
};

#endif