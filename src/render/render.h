#ifndef RENDER_H
#define RENDER_H

#include "camera.h"
#include <vector>
#include "renders/pointrender.h"
#include "renders/object.h"
#include "renders/turtledraw.h"
#include "renders/texrender.h"
#include "kinematics/cskeleton.h"
#include "cubemap.h"
#include "particle/particle_emitter.h"
#include "freeform/freeform.h"

class ObjectRender;
class SkyboxRender;

class Render{
public:
    Render(Camera* cam, const int size[2]);
    ~Render();
    Camera* cam() const;

    void draw_points(std::vector<glm::vec3> points, glm::vec4 color, float size);
    void draw_objects(std::vector<Object*> objects, glm::mat4 offset = glm::mat4(1.f));
    void draw_skybox(CubeMap* cubemap);
    void draw_skeleton(CSkeleton* skeleton);
    void draw_particle(ParticleEmitter* emitter);
    void draw_freeform(FreeForm* ffd);
    void draw_turtle(TurtleDraw* turtle, int pos[2]);

    int pick_objects(int pos[2], std::vector<Object*> objects, glm::mat4 offset = glm::mat4(1.f));
    int pick_skeleton(int pos[2], CSkeleton* skeleton);

    void set_window_size(int size[2]){m_size[0] = size[0];m_size[1] = size[1];}
private:
    int m_size[2];
    
    Camera* m_cam;
    PointRender* m_pointrender;
    ObjectRender* m_objectrender;
    SkyboxRender* m_skyboxrender;
    TexRender* m_texrender;
};

#endif
