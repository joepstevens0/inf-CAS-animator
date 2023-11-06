#include "character_animation.h"
#include "imgui.h"
#include "render/kinematics/cjoint.h"
#include "render/kinematics/clink.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "matrix.h"
#include <chrono>

#define JACOBIAN_HEIGHT 6

inline Skeleton::JointTree temp_create(CJoint* joint){
    Skeleton::JointTree t;
    t.angle = joint->get_angle();
    t.offset = joint->get_offset();
    t.length = 0;
    t.twist = 0;
    t.lower_bound = joint->get_lower_bound();
    t.upper_bound = joint->get_upper_bound();

    std::vector<CLink*> children = joint->get_children();
    for (unsigned int i = 0; i < children.size();++i){
        CLink* link = children[i];
        if (link->get_joint()){
            Skeleton::JointTree child_tree = temp_create(link->get_joint());
            child_tree.length = link->get_length();
            child_tree.twist = link->get_twist();
            t.children.push_back(child_tree);
        }
    }
    return t;
}

CharacterAnimation::CharacterAnimation(std::string channel_name)
    : m_channel_name{channel_name}
{
    m_move_cube = Object::from_cube();
    m_move_cube->scale(glm::vec3{0.5, 0.7, 0.5});

    // const float ub = 10, lb = -10;
    // Skeleton::JointTree t = {5, 0, glm::radians(-90.), glm::radians(0.), lb,ub, {
    //     {0, 2, glm::radians(90.), glm::radians(125.), lb,ub,{
    //         {0, 15, glm::radians(0.), glm::radians(-35.), lb,ub,{
    //             {15, 2, glm::radians(90.), glm::radians(0.), lb,ub,{
    //                 {0, 0, glm::radians(-90.), glm::radians(90.), lb,ub,{
    //                     {2, 0, glm::radians(-90.), glm::radians(0.), lb,ub,{}}
    //                 }}
    //             }}
    //         }}
    //     }}
    // ,
    //     {0, -2, glm::radians(90.), glm::radians(125.), lb,ub,{
    //         {0, 15, glm::radians(180.), glm::radians(-35.), lb,ub,{
    //             {15, 2, glm::radians(90.), glm::radians(0.), lb,ub,{
    //                 {0, 0, glm::radians(-90.), glm::radians(90.), lb,ub,{
    //                     {2, 0, glm::radians(-90.), glm::radians(0.), lb,ub,{}}
    //                 }}
    //             }}
    //         }}
    //     }}
    // }};
    CSkeleton* other = new CSkeleton("skeleton.skl");
    CJoint* j =  other->get_root();
    Skeleton::JointTree t = temp_create(j);
    m_skel = new Skeleton(t);
    delete other;
}
CharacterAnimation::~CharacterAnimation()
{
    delete m_skel;
    delete m_move_cube;
}

void CharacterAnimation::draw_ui(int frame, Render *render)
{
    ImGui::Checkbox("IK active", &m_animate);
    ImGui::Separator();
    ImGui::DragFloat3("Target pos", &m_endpos[0], 0.2);
    ImGui::DragFloat3("Target rot", &m_endrot[0], 1.);
    ImGui::Separator();
    ImGui::DragFloat3("Skeleton pos", &m_skel_pos[0], 0.2);
    ImGui::DragFloat("Skeleton scale", &m_skel_scale, 0.01);
    // if (ImGui::DragFloat("Angle step", &m_step, 0.001)){
    //     if (m_step < 0) m_step = 0;
    //     if (m_step > 1) m_step = 1;
    // }
}

void CharacterAnimation::draw(int frame, Render *render)
{
    m_move_cube->set_pos({0,0,0});
    glm::mat4 model = glm::mat4(glm::fquat{glm::radians(m_endrot)});
    model[3] = glm::vec4(m_endpos, 1);

    static auto last_update = std::chrono::system_clock::now();
    auto current_time = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = current_time - last_update;
    double elapsed_update = diff.count();
    if (elapsed_update > 0.005){
        if (m_animate)
            m_skel->ik_move(model, m_step);
        last_update = current_time;
    }
    glm::mat4 skel_model = glm::scale(glm::translate(glm::mat4(1.f),  m_skel_pos), glm::vec3(m_skel_scale));
    render->draw_objects({m_move_cube}, skel_model*model);
    m_skel->draw(render, skel_model);
}

const char *CharacterAnimation::get_name()
{
    return m_channel_name.c_str();
}

void CharacterAnimation::load(FILE *in)
{
    char buff[255];

    fscanf(in, "CharacterAnimationStart: %19[^\n]\n", buff);
    m_channel_name = std::string(buff);

    fscanf(in, "pos(%f %f %f) scale(%f)\n",  
            &m_skel_pos.x, &m_skel_pos.y, &m_skel_pos.z, 
            &m_skel_scale
        );

    if (fgets(buff, 255, in) != NULL)
    {
        if (strcmp(buff, "CharacterAnimationEnd\n") != 0)
        {
            fprintf(stderr, "Failed to load CharacterAnimation channel\n");
            throw;
        }
    }
}
void CharacterAnimation::save(FILE *out)
{
    fprintf(out, "CharacterAnimationStart: %s\n", get_name());
    fprintf(out, "pos(%f %f %f) scale(%f)\n",  
            m_skel_pos.x, m_skel_pos.y, m_skel_pos.z, 
            m_skel_scale
        );
    fprintf(out, "CharacterAnimationEnd\n");
}

void CharacterAnimation::onmousedown(int pos[2], Render *render)
{
}