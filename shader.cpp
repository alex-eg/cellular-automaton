#include "shader.hpp"

namespace ca {
u32 Shader::file_length(std::ifstream &file) {
    if (!file.good()) {
        return 0;
    }

    file.seekg(0, std::ios::end);
    long len = file.tellg();
    file.seekg(std::ios::beg);

    return static_cast<u32>(len);
}

int Shader::load_shader(const char *filename, GLchar **shader_source, u32 &len) {
    std::ifstream file;

    file.open(filename, std::ios::in);
    if (!file) {
        return -1;
    }
    len = file_length(file);
    if (len == 0) {
        return -2;
    }
    *shader_source = new char[len + 1];
    if (shader_source == nullptr) {
        return -3;
    }

    (*shader_source)[len] = '\0';

    u32 i = 0;

    while (file.good()) {
        (*shader_source)[i] = static_cast<GLchar>(file.get());
        if (!file.eof()) {
            i++;
        }
    }

    (*shader_source)[i] = '\0';

    file.close();

    return 0;
}

bool Shader::load_vertex_shader(const std::string filename) {
    GLchar *ssource;
    u32 slen;

    int ret = load_shader(filename.c_str(), &ssource, slen);
    if (ret != 0) {
        return false;
    }

    vertex_shader = glCreateShaderObjectARB(GL_VERTEX_SHADER);

    const GLchar *c_ssource = ssource;
    const GLint c_len = static_cast<GLint>(slen);
    glShaderSourceARB(vertex_shader, 1, &c_ssource, &c_len);
    glCompileShaderARB(vertex_shader);

    int compiled = 0;
    glGetObjectParameterivARB(vertex_shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        return false;
    }

    return true;
}

bool Shader::load_fragment_shader(const std::string filename) {
    GLchar *ssource;
    u32 slen;

    int ret = load_shader(filename.c_str(), &ssource, slen);
    if (ret != 0) {
        return false;
    }

    fragment_shader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);

    const GLchar *c_ssource = ssource;
    const GLint c_len = static_cast<GLint>(slen);
    glShaderSourceARB(fragment_shader, 1, &c_ssource, &c_len);
    glCompileShaderARB(fragment_shader);

    int compiled = 0;
    glGetObjectParameterivARB(fragment_shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        return false;
    }

    return true;
}

bool Shader::compile() {
    shader_program = glCreateProgram();
    if (vertex_shader) {
        glAttachShader(shader_program, vertex_shader);
    }
    if (fragment_shader) {
        glAttachShader(shader_program, fragment_shader);
    }
    glLinkProgram(shader_program);

    GLint linked;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &linked);
    if (!linked) {
        return false;
    }
    ready = true;
    return true;
}

void Shader::begin() {
    if (ready) {
        glUseProgram(shader_program);
        running = true;
    }
}

void Shader::end() {
    glUseProgram(0);
    running = false;
}

void Shader::Switch() {
    if (running) {
        end();
    } else {
        begin();
    }
}
} // namespace ca
