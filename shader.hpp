#pragma once

#include <iostream>
#include <fstream>
#include <ios>
#include <new>
#include <GL/glew.h>

class Shader {
private:
    GLuint FragmentShader, VertexShader;
    bool Ready;
    unsigned int FileLength(std::ifstream &file);
    int LoadShader(const char* filename, GLchar **ShaderSource, unsigned int &len);
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
    unsigned int width, height;
public:
    DefaultShader(unsigned int w, unsigned int h) : width(w), height(h) {}
    DefaultShader() {}
    DefaultShader &operator = (const DefaultShader &right)
    {
    if (this == &right) return *this;
    width = right.width;
    height = right.height;
    return *this;
    }

    void Invoke(void)
    {
    int uniform_WindowSize = glGetUniformLocation(ShaderProgram,"WindowSize");
        glUniform2f(uniform_WindowSize, static_cast<GLfloat>(width), static_cast<GLfloat>(height));
    }
};
