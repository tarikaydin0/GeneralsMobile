#pragma once
#include "d3d8.h"
#include <vector>
#include <string>

#include <GLES3/gl3.h>

// Preliminary shader class to hold tokens
class GLESVertexShader {
public:
    GLESVertexShader(const DWORD* declaration, const DWORD* function);
    ~GLESVertexShader();
    const std::vector<DWORD>& GetFunction() const { return m_function; }
    GLuint GetShaderObject() const { return m_shaderObject; }
private:
    std::vector<DWORD> m_function;
    GLuint m_shaderObject = 0;
    GLuint m_program = 0;
};

class GLESPixelShader {
public:
    GLESPixelShader(const DWORD* function);
    ~GLESPixelShader();
    const std::vector<DWORD>& GetFunction() const { return m_function; }
    GLuint GetShaderObject() const { return m_shaderObject; }
private:
    std::vector<DWORD> m_function;
    GLuint m_shaderObject = 0;
};
