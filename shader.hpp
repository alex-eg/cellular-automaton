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
    GLuint FragmentShader, VertexShader;
    bool Ready;
    u32 FileLength(std::ifstream &file);
    int LoadShader(const char* filename, GLchar **ShaderSource, u32 &len);
public:
    GLuint ShaderProgram;
    bool Running;
    Shader()
    {
    FragmentShader = 0;
    VertexShader = 0;
    ShaderProgram = 0;
    Ready = false;
    Running = false;
    }

    void Begin();
    void Switch();
    void End();
    bool LoadVertexShader(const std::string filename);
    bool LoadFragmentShader(const std::string filename);
    bool Compile();
};

class DefaultShader : public Shader {
private:
    u32 width, height;
public:
    DefaultShader(u32 w, u32 h) : width(w), height(h) {}
    DefaultShader() {}
    DefaultShader &operator = (const DefaultShader &right)
    {
    if (this == &right) return *this;
    width = right.width;
    height = right.height;
    return *this;
    }

    void Invoke(void) {
        i32 uniform_WindowSize =
            glGetUniformLocation(ShaderProgram, "WindowSize");
        glUniform2f(uniform_WindowSize, static_cast<GLfloat>(width),
                    static_cast<GLfloat>(height));
    }
};
} // namespace ca
