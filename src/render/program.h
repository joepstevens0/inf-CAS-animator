#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include <vector>
#include "shader.h"

class Camera;

class Program
{
public:
    void bind_bool(std::string name, bool val);
    void bind_vecf4(std::string name, const float val[4]);
    void bind_mat4(std::string name, const float* val);

    void bind_vec2(std::string name, const float val[2]);
    void bind_vec3(std::string name, const float val[3]);
    void bind_i32(std::string name, int val);
    void bind_f32(std::string name, float val);
    void bind_u32(std::string name, unsigned int val);

    void bind_cam(Camera* cam, const int screen_size[2]);
    
    void set_used();

    static Program* from_shaders(std::vector<Shader> shaders);
    static Program* from_path(std::string vert_path, std::string frag_path);

    ~Program();

private:
    Program(GLuint id);
    GLuint id;
};

#endif