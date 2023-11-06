#ifndef SKELETON_H
#define SKELETON_H

#include <glm/mat4x4.hpp>
#include <render/renders/object.h>
#include <render/render.h>
#include "link.h"

class Skeleton{
public:
    struct JointTree{
        float offset;
        float length;
        float twist;
        float angle;
        float lower_bound;
        float upper_bound;
        std::vector<JointTree> children;
    };

    Skeleton(JointTree tree);
    ~Skeleton();

    void ik_move(glm::mat4 target, float step_perc);
    void draw(Render *render, glm::mat4 offset);
private:
    struct Joint{
        float offset;
        float length;
        float twist;
        float angle;
        float lower_bound;
        float upper_bound;
        Object* cube = nullptr;
        glm::mat4 model = glm::mat4(1.f);
        std::vector<Joint*> children;
        std::vector<Link*> links;
    };

    Joint* create_joint(float offset, float length, float twist, float angle, float lower_bound, float upper_bound);
    Joint* tree_to_joint(JointTree tree);
    void add_joint(Joint* parent, Joint* j);

    std::vector<std::vector<float>> create_transposed_jacobian(glm::vec3 end_eff_pos, std::vector<Joint*> chain);
    static void update_joints(Joint* j, glm::mat4 offset = glm::mat4(1.0f));

    Joint* m_first_joint = nullptr;
    Joint* m_last_joint = nullptr;

    std::vector<Joint*> m_joints;
};

#endif