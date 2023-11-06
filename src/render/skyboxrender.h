#ifndef SKYBOXRENDER_H
#define SKYBOXRENDER_H

class CubeMap;
class Program;
class Camera;
class Object;

class SkyboxRender{
public:
    SkyboxRender();
    ~SkyboxRender();

    void draw(Camera* cam, CubeMap* cubemap,const int screen_size[2]);
private:
    Program* m_shader;
    Object* m_cube;
};

#endif