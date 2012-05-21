#include "Shader.hpp"

class ShaderBlur : Shader {
private:
    float kernel[25];
    int count;
