#include "freeform.h"

#include <render/render.h>
#include <glm/gtc/matrix_transform.hpp>

#define POINTS_S 1
#define POINTS_T 1
#define POINTS_U 1

#define LINK_MIN -0.02
#define LINK_MAX 0.02

const glm::vec3 p0 = glm::vec3(0, 0, 0.5);
const glm::vec3 s_dir = {1,0,0};
const glm::vec3 t_dir = {0,1,0};
const glm::vec3 u_dir = {0,0,-1};

inline int binomialCoeff(int n, int k) {
   if (k == 0 || k == n)
   return 1;
   return binomialCoeff(n - 1, k - 1) + binomialCoeff(n - 1, k);
}

// parameters to starting control position
glm::vec3 FreeForm::calc_control_pos(float i, float j, float k){
    float s_val = (i / (POINTS_S)) ;
    float t_val = (j / (POINTS_T)) ;
    float u_val =  (k / (POINTS_U)) ;
    return p0 + s_val*s_dir + t_val*t_dir + u_val*u_dir;
}

FreeForm::FreeForm(Object* object)
:m_object{object}
{
    map_vertices();

    // create control points
    int id = 1;
    for (float i = 0; i <= POINTS_S; ++i)
    {
        for (float j = 0; j <= POINTS_T; ++j)
        {
            for (float k = 0; k <= POINTS_U; ++k)
            {
                glm::vec3 pos = calc_control_pos(i,j,k);
                Object* point = Object::from_cube();
                point->set_id(id);
                point->scale(glm::vec3{0.2, 0.2, 0.2});
                point->move(pos);
                m_points.push_back(point);
                id += 1;
            }
        }
    }

    // create links
    for (unsigned int i = 0; i < m_points.size() - 1;++i){
        std::vector<Vertex> points = {
        {glm::vec3(LINK_MIN, LINK_MIN, 0), glm::vec2(0, 0)},    // 0 0
        {glm::vec3(LINK_MIN, LINK_MAX, 0), glm::vec2(0, 1)},     // 0 1
        {glm::vec3(LINK_MAX, LINK_MAX, 0), glm::vec2(1, 1)},      // 1 1
        {glm::vec3(LINK_MAX, LINK_MIN, 0), glm::vec2(1, 0)}};    // 1 0

        std::vector<Face> faces = {
            {0, 1, 2},
            {0, 2, 3}};
        Object* link = Object::from_points(points, faces);
    
        m_links.push_back(link);
    }
    update_links();
}

void FreeForm::update_links(){
    for (unsigned int i = 0; i < m_points.size() - 1;++i){
        Object* ob1 = m_points[i];
        Object* ob2 = m_points[i+1];
        Object* link = m_links[i];

        glm::vec3 p = ob1->get_model() * glm::vec4(LINK_MIN, LINK_MIN, 0, 1);
        link->update_vertex({p, glm::vec2(0, 0)}, 0);
        p = ob1->get_model() * glm::vec4(LINK_MIN, LINK_MAX, 0, 1);
        link->update_vertex({p, glm::vec2(0, 1)}, 1);

        p = ob2->get_model() * glm::vec4(LINK_MAX, LINK_MAX, 0, 1);
        link->update_vertex({p, glm::vec2(1, 1)}, 2);
        p = ob2->get_model() * glm::vec4(LINK_MAX, LINK_MIN, 0, 1);
        link->update_vertex({p, glm::vec2(1, 0)}, 3);
    
        m_links.push_back(link);
    }
}

FreeForm::~FreeForm()
{
    for (unsigned int i = 0; i < m_points.size(); ++i)
    {
        delete m_points[i];
    }
}

// create parameters for vertices
void FreeForm::map_vertices()
{
    m_vertexMapping.clear();

    glm::vec3 t_x_u = glm::cross(t_dir, u_dir);
    glm::vec3 u_x_s = glm::cross(u_dir, s_dir);
    glm::vec3 s_x_t = glm::cross(s_dir, t_dir);
    for (int i = 0; i < m_object->total_verts(); ++i)
    {
        Vertex vert = m_object->get_vert(i);
        glm::vec3 p = vert.pos;
        float s_val = glm::dot(t_x_u,p - p0)/glm::dot(t_x_u,s_dir);
        float t_val = glm::dot(u_x_s,p - p0)/glm::dot(u_x_s,t_dir);
        float u_val = glm::dot(s_x_t,p - p0)/glm::dot(s_x_t,u_dir);
        m_vertexMapping.push_back({s_val, t_val, u_val});
    }
}

// update object to new control points
void FreeForm::updateObject()
{
    for (unsigned int i = 0; i < m_vertexMapping.size(); ++i)
    {
        Vertex vert = m_object->get_vert(i);
        glm::vec3 mapping = m_vertexMapping[i];
        vert.pos = update_point(mapping[0], mapping[1], mapping[2]);
        m_object->update_vertex(vert, i);
    }
    update_links();
}

void FreeForm::draw(Render *render)
{
    render->draw_objects(m_links, m_object->get_model());
    render->draw_objects(m_points, m_object->get_model());
}

void FreeForm::mousedown(int pos[2],Render* render)
{
    if (m_ismouseup){
        m_ismouseup = false;

        int id = render->pick_objects(pos, m_points, m_object->get_model());
        m_controlPoint = id-1;
        m_last_pos[0] = pos[0];
        m_last_pos[1] = pos[1];
    }

    if (m_controlPoint == NO_CONTROL_POINT)
        return;

    int xDiff = m_last_pos[0] - pos[0];
    int yDiff = m_last_pos[1] - pos[1];
    m_points[m_controlPoint]->move({-xDiff / 100., yDiff / 100., 0});
    m_last_pos[0] = pos[0];
    m_last_pos[1] = pos[1];
}

void FreeForm::mouseup(int pos[2],Render* render)
{
    if (!m_ismouseup){
        m_ismouseup = true;

        updateObject();
    }
}

// parameters to world position
glm::vec3 FreeForm::update_point(float s, float t, float u){
    glm::vec3 result{0,0,0};

    float l = POINTS_S;
    float m = POINTS_T;
    float n = POINTS_U;

    for (float i = 0; i <= POINTS_S; ++i)
    {
        glm::vec3 t_sum{0,0,0};
        for (float j = 0; j <= POINTS_T; ++j)
        {
            glm::vec3 u_sum{0,0,0};
            for (float k = 0; k <= POINTS_U; ++k)
            {
                glm::vec3 p_ijk = m_points[i*(POINTS_T+1)*(POINTS_U+1) + j*(POINTS_U+1) + k]->getPos();
                // printf("point [%f,%f,%f] %.2f:  {%f ,%f , %f}\n", i, j, k, i*(POINTS_T+1)*(POINTS_U+1) + j*(POINTS_U+1) + k, p_ijk[0], p_ijk[1], p_ijk[2]);
                float v = binomialCoeff(n, k)*pow(1-u, n-k)*pow(u, k);
                u_sum += v*p_ijk;
            }
            float v = binomialCoeff(m, j)*pow(1-t, m-j)*pow(t, j);
            t_sum += v*u_sum;
        }
        float v = binomialCoeff(l, i)*pow(1-s, l-i)*pow(s, i);
        result += v*t_sum;
    }
    return result;
}

std::vector<glm::vec3> FreeForm::save_points(){
    std::vector<glm::vec3> points;
    for (unsigned int i = 0; i < m_points.size();++i)
        points.push_back(m_points[i]->getPos());
    return points;
}
void FreeForm::load_points(std::vector<glm::vec3> points){
    for (unsigned int i = 0; i < m_points.size();++i)
        m_points[i]->set_pos(points[i]);
    updateObject();
}