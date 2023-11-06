
#include "shader.h"

Shader Shader::from_source(std::string src, GLuint shader_type){
    GLuint id = glCreateShader(shader_type);

    auto shader_src = src.c_str();
    glShaderSource(id, 1, &shader_src, nullptr);
    glCompileShader(id);

    GLint success = 1;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (success == 0) {
        GLint len = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        GLchar* error = new GLchar[len];

        glGetShaderInfoLog(
            id,
            len,
            nullptr,
            error
        );
        printf("Failed to create shader:\n%s", error);
        delete[] error;
        throw "Failed to create shader";
    }

    return Shader{id};
}