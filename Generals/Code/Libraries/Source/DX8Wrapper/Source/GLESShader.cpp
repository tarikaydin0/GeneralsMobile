#include "GLESShader.h"
#include "GLESShaderTranslator.h"
#include <GLES3/gl3.h>
#include <android/log.h>
#include <vector>

#define LOG_TAG "DX8Shader"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

GLuint CompileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            std::vector<char> infoLog(infoLen);
            glGetShaderInfoLog(shader, infoLen, nullptr, infoLog.data());
            LOGE("Error compiling shader:\n%s\nLog:\n%s", src, infoLog.data());
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLESVertexShader::GLESVertexShader(const DWORD* declaration, const DWORD* function) {
    if (function) {
        const DWORD* p = function;
        while (*p != 0x0000FFFF) { // D3DVS_END
             m_function.push_back(*p++);
        }
        m_function.push_back(0x0000FFFF);
    }
    
    std::string glsl = GLESShaderTranslator::TranslateVertexShader(function, declaration);
    LOGI("Generated VS:\n%s", glsl.c_str());
    
    GLuint shader = CompileShader(GL_VERTEX_SHADER, glsl);
    if (shader) {
        m_program = glCreateProgram();
        glAttachShader(m_program, shader);
        // Link later? Or link with a dummy PS?
        // In GLES, we need both VS and FS linked in a program.
        // We can't fully link here without the FS.
        // Option: Store the shader object and link in SetVertexShader/SetPixelShader when both are known.
        // Or create a program pipeline (separate shader objects).
        // Since we need to support DrawPrimitive which effectively switches shaders...
        // Let's store the compiled shader object for now.
        m_shaderObject = shader;
    }
}

GLESVertexShader::~GLESVertexShader() {
    if (m_program) glDeleteProgram(m_program);
    if (m_shaderObject) glDeleteShader(m_shaderObject);
}

GLESPixelShader::GLESPixelShader(const DWORD* function) {
    if (function) {
        const DWORD* p = function;
        while (*p != 0x0000FFFF) {
             m_function.push_back(*p++);
        }
        m_function.push_back(0x0000FFFF);
    }

    std::string glsl = GLESShaderTranslator::TranslatePixelShader(function);
    LOGI("Generated PS:\n%s", glsl.c_str());

    GLuint shader = CompileShader(GL_FRAGMENT_SHADER, glsl);
    if (shader) {
        m_shaderObject = shader;
    }
}

GLESPixelShader::~GLESPixelShader() {
    if (m_shaderObject) glDeleteShader(m_shaderObject);
}
