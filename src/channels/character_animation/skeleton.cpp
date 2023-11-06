#include "skeleton.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

inline float radians(float angle)
{
    return (angle * glm::pi<float>() * 2. / 360.);
}

inline glm::mat4 dh_create_transform(float offset, float angle, float length, float twist)
{
    float cos_angle = cos(angle);
    float sin_angle = sin(angle);

    float cos_twist = cos(twist);
    float sin_twist = sin(twist);

    glm::mat4 mat{
        cos_angle, -sin_angle, 0, length,
        cos_twist * sin_angle, cos_twist * cos_angle, -sin_twist, -offset * sin_twist,
        sin_twist * sin_angle, sin_twist * cos_angle, cos_twist, offset * cos_twist,
        0, 0, 0, 1};

    return mat;
}
inline std::vector<std::vector<float>> mat_dot(std::vector<std::vector<float>> mat1, std::vector<std::vector<float>> mat2)
{
    std::vector<std::vector<float>> result;
    unsigned int rows = mat1.size();
    unsigned int columns = mat2[0].size();
    unsigned int max_k = mat1[0].size();
    for (unsigned int y = 0; y < rows; ++y)
    {
        std::vector<float> row;
        for (unsigned int x = 0; x < columns; ++x)
        {
            row.push_back(0);
        }
        result.push_back(row);
    }

    for (unsigned int i = 0; i < rows; i++)
    {
        for (unsigned int j = 0; j < columns; j++)
        {
            for (unsigned int k = 0; k < max_k; k++)
            {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }

    return result;
}
inline glm::vec3 rot_diff(glm::vec3 a1, glm::vec3 a2) {
	glm::vec3 diff = a1 - a2;
	if (diff.x < -glm::pi<float>()) diff.x += 2*glm::pi<float>();
    if (diff.y < -glm::pi<float>()) diff.y += 2*glm::pi<float>();
    if (diff.z < -glm::pi<float>()) diff.z += 2*glm::pi<float>();
	if (diff.x > glm::pi<float>()) diff.x -= 2*glm::pi<float>();
    if (diff.y > glm::pi<float>()) diff.y -= 2*glm::pi<float>();
    if (diff.z > glm::pi<float>()) diff.z -= 2*glm::pi<float>();
	return glm::degrees(diff);
}

Skeleton::Skeleton(JointTree tree)
{
    add_joint(nullptr, tree_to_joint(tree));

    Joint* j = m_first_joint;
    while (j){
        m_last_joint = j;
        if (j->children.size() > 0){
            j = j->children[0];
        } else {
            j = nullptr;
        }
    }

    update_joints(m_first_joint);
}

Skeleton::Joint* Skeleton::tree_to_joint(Skeleton::JointTree tree){
    Joint* j = create_joint(tree.offset, tree.length, tree.twist, tree.angle, tree.lower_bound, tree.upper_bound);
    for (unsigned int i = 0; i < tree.children.size();++i){
        Joint* child = tree_to_joint(tree.children[i]);
        add_joint(j, child);
    }
    return j;
}

Skeleton::~Skeleton()
{
    for (unsigned int i = 0; i < m_joints.size(); ++i)
    {
        delete m_joints[i]->cube;
        for (unsigned int j = 0; j < m_joints[i]->links.size(); ++j)
            delete m_joints[i]->links[j];
        delete m_joints[i];
    }
}

void Skeleton::draw(Render *render, glm::mat4 offset)
{
    std::vector<Object *> objects;
    for (unsigned int i = 0; i < m_joints.size(); ++i)
    {
        objects.push_back(m_joints[i]->cube);
        for (unsigned int j = 0; j < m_joints[i]->links.size(); ++j)
        {
            m_joints[i]->links[j]->draw(render, offset);
        }
    }

    render->draw_objects(objects, offset);
}

Skeleton::Joint* Skeleton::create_joint(float offset, float length, float twist, float angle, float lower_bound, float upper_bound)
{
    Joint* j = new Joint();
    j->offset = offset;
    j->length = length;
    j->twist = twist;
    j->angle = angle;
    j->lower_bound = lower_bound;
    j->upper_bound = upper_bound;
    j->cube = Object::from_cube();
    return j;
}
void Skeleton::add_joint(Joint* parent, Joint* j)
{
    m_joints.push_back(j);
    Joint *ptr = m_joints[m_joints.size() - 1];

    ptr->model = dh_create_transform(ptr->offset, ptr->angle, ptr->length, ptr->twist);
    ptr->cube->set_model(ptr->model);
    if (parent){

        parent->children.push_back(ptr);

        // add link
        glm::mat4 m1 = parent->model;
        glm::mat4 m2 = ptr->model;
        parent->links.push_back(new Link(m1, m2));
    } else {
        m_first_joint = ptr;
    }
}

void Skeleton::ik_move(glm::mat4 target, float step_perc)
{
    // calc chain
    std::vector<Joint*> chain;
    Joint* j = m_first_joint;
    while (j){
        chain.push_back(j);
        if (j->children.size() > 0){
            j = j->children[0];
        } else {
            j = nullptr;
        }
    }

    Joint* end_eff = m_last_joint;
    glm::vec3 end_eff_pos = end_eff->model[3];
    glm::vec3 end_eff_rot = glm::eulerAngles(glm::fquat(end_eff->model));

    glm::vec3 target_pos = target[3];
    glm::vec3 target_rot = glm::eulerAngles(glm::fquat(target));

    // calc required force
    glm::vec3 Fp = target_pos - end_eff_pos;  // position difference
    glm::vec3 Fa = rot_diff(target_rot, end_eff_rot);     // rotation diffrence

    std::vector<float> F = {Fp[0], Fp[1], Fp[2], Fa[0], Fa[1], Fa[2]};

    // calc jacobian
    std::vector<std::vector<float>> jacobian_t = create_transposed_jacobian(end_eff_pos, chain);

    // calc required angle changes
    std::vector<std::vector<float>> angles_mat = mat_dot(jacobian_t, {{F[0]}, {F[1]}, {F[2]}, {F[3]}, {F[4]}, {F[5]}});

    // angles to 1D array
    std::vector<float> angles;
    for (unsigned int i = 0; i < angles_mat.size();++i){
        angles.push_back(angles_mat[i][0]);
    }

    for (unsigned int i = 0; i < chain.size();++i){
        Joint* j = chain[i];
        j->angle = glm::clamp(j->angle + (step_perc*glm::radians(angles[i])), j->lower_bound, j->upper_bound);
    }

    if (m_first_joint){
        update_joints(m_first_joint);
    }
}

std::vector<std::vector<float>> Skeleton::create_transposed_jacobian(glm::vec3 end_eff_pos, std::vector<Joint*> chain)
{
    // create jacobian transposed
    std::vector<std::vector<float>> jacobian_t;
    for (unsigned int i = 0; i < chain.size(); ++i){
        glm::mat4 j_mat = chain[i]->model;
        glm::vec3 j_pos = j_mat[3];
        glm::vec3 z_axis = j_mat[2];


        glm::vec3 jt = glm::cross(z_axis, end_eff_pos - j_pos);
        jacobian_t.push_back({
            jt.x, jt.y, jt.z, z_axis.x, z_axis.y, z_axis.z
        });
    }

    return jacobian_t;
}
void Skeleton::update_joints(Joint* j, glm::mat4 offset){
    // calc new model
    j->model = offset * dh_create_transform(j->offset, j->angle, j->length, j->twist);
    j->cube->set_model(j->model);

    // update children
    for (unsigned int i = 0; i < j->children.size();++i){
        update_joints(j->children[i], j->model);
    }

    // update links
    for (unsigned int i = 0; i < j->children.size();++i){
        glm::mat4 m2 = j->children[i]->model;
        j->links[i]->update(j->model, m2);
    }
}