#include "render.h"
#include <glad/glad.h>
#include <gl_header.h>
#include "renders/objectrender.h"
#include "skyboxrender.h"

Render::Render(Camera *cam, const int size[2])
    : m_size{size[0],size[1]},m_cam{cam}, m_pointrender{new PointRender()}, m_objectrender{new ObjectRender{}}, m_skyboxrender{new SkyboxRender()}
{
    m_texrender = new TexRender();
}
Render::~Render()
{
    delete m_pointrender;
    delete m_objectrender;
    delete m_skyboxrender;
    delete m_texrender;
}
Camera *Render::cam() const
{
    return m_cam;
}

void Render::draw_points(std::vector<glm::vec3> points, glm::vec4 color, float size)
{
    m_pointrender->draw(m_cam, points, color, size, m_size);
}

void Render::draw_objects(std::vector<Object*> objects, glm::mat4 offset)
{
    m_objectrender->draw(m_cam, objects, m_size, offset);
}

void Render::draw_skybox(CubeMap* cubemap){
    m_skyboxrender->draw(m_cam, cubemap, m_size);
}

void Render::draw_skeleton(CSkeleton* skeleton){
    skeleton->draw(this);
}

void Render::draw_particle(ParticleEmitter* emitter){
    emitter->draw(m_cam, m_size);
}

void Render::draw_freeform(FreeForm* ffd){
    ffd->draw(this);
}
void Render::draw_turtle(TurtleDraw* turtle, int pos[2]){
    turtle->draw(m_texrender, pos, m_size);
}

int Render::pick_objects(int pos[2], std::vector<Object*> objects, glm::mat4 offset){
    m_objectrender->start_picking();
    draw_objects(objects, offset);
    return m_objectrender->stop_picking(pos, m_size);
}

int Render::pick_skeleton(int pos[2], CSkeleton* skeleton){
    m_objectrender->start_picking();
    draw_skeleton(skeleton);
    return m_objectrender->stop_picking(pos, m_size);
}