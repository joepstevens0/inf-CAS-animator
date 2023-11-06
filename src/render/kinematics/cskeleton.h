#ifndef CSKELETON_H
#define CSKELETON_H

#include <fstream>
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include "searchtype.h"

class CJoint;
class CLink;
class Render;
class Camera;

class CSkeleton{
public:
    CSkeleton(std::string filename);
    void draw(Render* render, glm::mat4 offset = glm::mat4(1.0f));

    void inverse_kinematic(int end_effector_id, glm::vec3 endpos);
    SearchType get_by_id(int id);

    void update();

    std::vector<CJoint*> collect_joints();

    CJoint* get_root() const { return root;}
private:
    void init();

    static std::vector<CLink*> find_links(int amount, std::ifstream& in);
    static CJoint* find_joint(std::ifstream& in, CLink* parent = nullptr);

    CJoint* root;
};

#endif