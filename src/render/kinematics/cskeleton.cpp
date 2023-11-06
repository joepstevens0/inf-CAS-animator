#include "cskeleton.h"

#include <stdio.h>
#include <sstream>

#include "cjoint.h"
#include "clink.h"
#include <render/renders/object.h>
#include <render/render.h>
#include <glm/gtx/string_cast.hpp>

#define RESIZE 50

CSkeleton::CSkeleton(std::string filename)
{

    std::ifstream in{filename};
    if (!in.is_open())
    {
        printf("Error, failed to open file: %s\n", filename.c_str());
        throw;
    }

    // find root
    root = find_joint(in);

    in.close();

    init();
}

std::vector<CLink *> CSkeleton::find_links(int amount, std::ifstream &in)
{
    if (amount <= 0)
        return {};

    std::vector<CLink *> links;
    std::string line;
    while (getline(in, line))
    {
        if (line.length() <= 0)
            continue;
        if (line[0] == '#')
            continue;

        if (line.rfind("LINK  ", 0) == 0)
        {
            std::stringstream s{line.substr(6)};
            float length, twist;
            bool has_next;
            s >> length >> twist >> has_next;

            CLink *link = new CLink(length*RESIZE, twist);
            if (has_next)
                link->set_next(find_joint(in, link));

            links.push_back(link);
            if ((int)links.size() >= amount)
            {
                return links;
            }
        }
    }
    return links;
}
CJoint *CSkeleton::find_joint(std::ifstream &in, CLink *parent)
{
    std::string line;
    while (getline(in, line))
    {
        if (line.length() <= 0)
            continue;
        if (line[0] == '#')
            continue;

        if (line.rfind("JOINT ", 0) == 0)
        {
            std::stringstream s{line.substr(6)};
            float angle, angle_bounds[2], offset, n_children;
            s >> angle >> angle_bounds[0] >> angle_bounds[1] >> offset >> n_children;

            CJoint *j = new CJoint(angle, offset*RESIZE, angle_bounds, parent);
            std::vector<CLink *> links = find_links(n_children, in);
            for (unsigned int i = 0; i < links.size(); ++i)
            {
                j->add_link(links[i]);
            }

            return j;
        }
    }
    return nullptr;
}

void CSkeleton::draw(Render* render, glm::mat4 offset)
{
    root->draw(render, offset);
}

void CSkeleton::init()
{
    int id = 5;
    root->init(id);
    printf("Last id: %d\n", id - 1);
}

void CSkeleton::update(){
    root->update();
}

SearchType CSkeleton::get_by_id(int id){
    return root->get_by_id(id);
}


// inline Matrix getJacobianTransposed(glm::vec3 effector_pos, std::vector<CJoint *> joints)
// {
//     glm::vec3 rot_axis(0, 0, 1);
//     Matrix J{6,0};
//     // for (int i = 0; i < joints.size(); ++i)
//     // {
//     //     glm::vec3 joint_pos = joints[i]->model() * glm::vec4(0, 0, 0, 1);
//     //     glm::vec3 p = cross(rot_axis, effector_pos - joint_pos);
//     //     glm::vec3 a = rot_axis;
//     //     J.addColumn({p.x, p.y, p.z, a.x, a.y, a.z});
//     // }
//     return J.transpose();
// }

void CSkeleton::inverse_kinematic(int end_effector_id, glm::vec3 endpos)
{
    // CJoint* effector_ob = root->get_by_id(end_effector_id);
    // glm::vec3 effector_pos = effector_ob->model()*glm::vec4(0,0,0,1);
    // printf("Effector pos: %s\n", glm::to_string(effector_pos).c_str());

    // Matrix O;  // all orientations
    // float h = 0.1;  // step size

    // while (F > 0.001){
    //     Matrix jt = getJacobianTransposed(effector_pos);
    //     Matrix F = {endpos - effector_pos, endRot - effector_rot};

    //     Matrix dO = jt.mul(F);

    //     O.add(dO.mul(h));

    // }
}

std::vector<CJoint*> CSkeleton::collect_joints() { return root->collect_joints();}