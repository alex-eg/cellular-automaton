#pragma once

#include "base.hpp"

#include <iostream>
#include <fstream>
#include <ios>
#include <new>
#include <GL/glew.h>

namespace ca {
class Shader {
private:
    GLuint fragment_shader, vertex_shader;
    bool ready;
    u32 file_length(std::ifstream &file);
    int load_shader(const char* filename, GLchar **shader_source, u32 &len);
public:
    GLuint shader_program;
    bool running;
    Shader()
    {
    fragment_shader = 0;
    vertex_shader = 0;
    shader_program = 0;
    ready = false;
    running = false;
    }

    void begin();
    void Switch();
    void end();
    bool load_vertex_shader(const std::string filename);
    bool load_fragment_shader(const std::string filename);
    bool compile();
};

class DefaultShader : public Shader {
private:
    u32 width, height;
public:
    DefaultShader(u32 w, u32 h) : width(w), height(h) {}
    DefaultShader() {}
    DefaultShader &operator = (const DefaultShader &right)
    {
    if (this == &right) { return *this;
}
    width = right.width;
    height = right.height;
    return *this;
    }

    void invoke(void) {
        i32 uniform_window_size =
            glGetUniformLocation(shader_program, "WindowSize");
        glUniform2f(uniform_window_size, static_cast<GLfloat>(width),
                    static_cast<GLfloat>(height));
    }
};
} // namespace ca
