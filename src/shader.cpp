#include "shader.hpp"

unsigned int Shader::FileLength(std::ifstream &file)
{
    if(!file.good()) return 0;
	
    file.seekg(0,std::ios::end);
    unsigned int len = file.tellg();
    file.seekg(std::ios::beg);
    
    return len;
}

int Shader::LoadShader(const char* filename, GLchar **ShaderSource, unsigned int &len)
{
    std::ifstream file;

    file.open(filename, std::ios::in);
    if(!file) return -1;
    len = FileLength(file);
    if (len == 0) return -2;
    *ShaderSource = new char[len+1];
    if (ShaderSource == 0) return -3;

    (*ShaderSource)[len] = '\0'; 

    unsigned int i=0;

    while (file.good()) {
	(*ShaderSource)[i] = file.get();
	if (!file.eof()) i++;
    }
    
    (*ShaderSource)[i] = '\0';
    
    file.close();
      
    return 0;
}

bool Shader::LoadVertexShader(const std::string filename)
{
    GLchar *ssource;
    unsigned int slen;

    int ret = LoadShader(filename.c_str(), &ssource, slen);
    if (ret!=0) return false;

    VertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER);

    const GLchar *c_ssource = ssource;
    const GLint c_len = slen; 
    glShaderSourceARB(VertexShader, 1, &c_ssource, &c_len);
    glCompileShaderARB(VertexShader);
    
    int compiled = 0;
    glGetObjectParameterivARB(VertexShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) return false;

    return true;
}

bool Shader::LoadFragmentShader(const std::string filename)
{
    GLchar *ssource;
    unsigned int slen;

    int ret = LoadShader(filename.c_str(), &ssource, slen);
    if (ret!=0) return false;

    FragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);

    const GLchar *c_ssource = ssource;
    const GLint c_len = slen; 
    glShaderSourceARB(FragmentShader, 1, &c_ssource, &c_len);
    glCompileShaderARB(FragmentShader);
    
    int compiled = 0;
    glGetObjectParameterivARB(FragmentShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) return false;

    return true;
}

bool Shader::Compile()
{
    ShaderProgram = glCreateProgram();
    if (VertexShader) glAttachShader(ShaderProgram, VertexShader);
    if (FragmentShader) glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

    GLint linked;
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &linked);
    if (!linked) return false;
    Ready = true;
    return true;
}

void Shader::Begin()
{
    if (Ready) {
	glUseProgram(ShaderProgram);
	Running = true;
    }
}

void Shader::End()
{
    glUseProgram(0);
    Running = false;
}

void Shader::Switch()
{
    if (Running) End();
    else Begin();
}
