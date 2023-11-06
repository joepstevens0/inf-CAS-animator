#include "program.h"

#include <fstream>
#include <sstream>
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

void Program::bind_bool(std::string name, bool val)
{
    auto loc = glGetUniformLocation(id, name.c_str());
    glUniform1i(loc, val);
}
void Program::bind_vecf4(std::string name, const float val[4])
{
    auto loc = glGetUniformLocation(id, name.c_str());
    glUniform4f(loc, val[0], val[1], val[2], val[3]);
}

void Program::bind_vec2(std::string name, const float val[2])
{
    auto loc = glGetUniformLocation(id, name.c_str());
    glUniform2f(loc, val[0], val[1]);
}
void Program::bind_vec3(std::string name, const float val[3]){
    auto loc = glGetUniformLocation(id, name.c_str());
    glUniform3f(loc, val[0], val[1], val[3]);
}
void Program::bind_mat4(std::string name, const float *val)
{
    auto loc = glGetUniformLocation(id, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, val);
}
void Program::bind_i32(std::string name, int val)
{
    auto loc = glGetUniformLocation(id, name.c_str());
    glUniform1i(loc, val);
}
void Program::bind_u32(std::string name, unsigned int val)
{
    auto loc = glGetUniformLocation(id, name.c_str());
    glUniform1ui(loc, val);
}
void Program::bind_f32(std::string name, float val)
{
    auto loc = glGetUniformLocation(id, name.c_str());
    glUniform1f(loc, val);
}
void Program::set_used()
{
    glUseProgram(id);
}

Program* Program::from_shaders(std::vector<Shader> shaders)
{
    auto id = glCreateProgram();

    for (unsigned int i = 0; i < shaders.size(); ++i)
    {
        Shader *shader = &shaders[i];
        {
            glAttachShader(id, shader->get_id());
        }
    }
    glLinkProgram(id);
    for (unsigned int i = 0; i < shaders.size(); ++i)
    {
        Shader *shader = &shaders[i];
        glDetachShader(id, shader->get_id());
    }

    GLint success = 1;
    glGetProgramiv(id, GL_LINK_STATUS, &success);

    if (success == 0)
    {
        GLint len = 0;

        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);

        GLchar *error = new GLchar[len];

        glGetProgramInfoLog(
            id,
            len,
            nullptr,
            error);

        printf("Error creating shader program:\n%s", error);
        delete[] error;
        throw;
    }

    return new Program{id};
}
Program::~Program()
{
    glDeleteProgram(id);
}

Program::Program(GLuint id) : id{id}
{
}

Program* Program::from_path(std::string vert_path, std::string frag_path){
    // read shaders
    std::ifstream in;
    in.open(vert_path); // open the input file

    std::stringstream vert_stream;
    vert_stream << in.rdbuf(); // read the file
    std::string src_vert = vert_stream.str();
    in.close();

    in.open(frag_path);
    std::stringstream frag_stream;
    frag_stream << in.rdbuf();
    std::string src_frag = frag_stream.str();
    in.close();

    // create program
    Shader vert_shader = Shader::from_vert_src(src_vert);
    Shader frag_shader = Shader::from_frag_src(src_frag);
    return Program::from_shaders(
        std::vector<Shader>{vert_shader, frag_shader});

}

void Program::bind_cam(Camera* cam, const int screen_size[2]){
    glm::mat4 view = cam->get_view();

    glm::mat4 projection = glm::mat4(1.0f);
    const float fov = 100.0f;
    projection = glm::perspective(glm::radians(fov), (float)screen_size[0] / (float)screen_size[1], 0.1f, 100.0f);

    bind_mat4("uView", &view[0][0]);
    bind_mat4("uProjection", &projection[0][0]);
}