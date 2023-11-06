
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <gl_header.h>
#include <string>
#include <stdio.h>

class Shader{
public:
    ~Shader(){
        glDeleteShader(id);
    }
    static Shader from_frag_src(std::string src){
        return from_source(src, GL_FRAGMENT_SHADER);
    }
    static Shader from_vert_src(std::string src){
        return from_source(src, GL_VERTEX_SHADER);
    }

    GLuint get_id() const{return id;}
private:
    Shader(GLuint shader_id): id{shader_id}{
    }
    static Shader from_source(std::string src, GLuint shader_type);

    GLuint id;
};

#endif