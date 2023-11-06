#ifndef DH_MAT_H
#define DH_MAT_H

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

inline glm::mat4 dh_create_transform(float offset, float angle, float length, float twist){
    glm::mat4 mat{
        cos(angle), -sin(angle), 0, length,
        cos(twist)*sin(angle), cos(twist)*cos(angle), -sin(twist), -offset*sin(twist),
        sin(twist)*sin(angle), sin(twist)*cos(angle), cos(twist), offset*cos(twist),
        0,0,0,1
    };

    return glm::transpose(mat);
}

#endif