#include "skyboxrender.h"
#include <glad/glad.h>
#include "program.h"
#include <fstream>
#include <sstream>
#include "camera.h"
#include "renders/object.h"
#include "cubemap.h"

SkyboxRender::SkyboxRender():
m_shader{Program::from_path("shader/skybox.vert", "shader/skybox.frag")}{
    m_cube = Object::from_cube();
    m_cube->scale(glm::vec3(2));
}
SkyboxRender::~SkyboxRender(){
    delete m_shader;
    delete m_cube;
}

void SkyboxRender::draw(Camera* cam, CubeMap* cubemap,const int screen_size[2]){
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    m_shader->set_used();


    m_shader->bind_cam(cam, screen_size);

    cubemap->bind(0);

    m_cube->draw(m_shader);
    glDepthFunc(GL_LESS); // set depth function back to default
}