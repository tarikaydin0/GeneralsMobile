#pragma once
#include <string>
#include <vector>
#include "d3d8.h"

class GLESShaderTranslator {
public:
    static std::string TranslateVertexShader(const DWORD* pFunction, const DWORD* pDeclaration = nullptr);
    static std::string TranslatePixelShader(const DWORD* pFunction);

private:
    static std::string ParseInstruction(const DWORD*& pToken, bool isVertexShader, int& indent);
    static std::string ParseDestinationParameter(DWORD token, bool isVertexShader);
    static std::string ParseSourceParameter(DWORD token, bool isVertexShader);
    static std::string GetRegisterName(DWORD token, bool isVertexShader, bool isDestination);
    static std::string GetSwizzle(DWORD token);
    static std::string GetWriteMask(DWORD token);
    static std::string GetSourceModifier(DWORD token, const std::string& reg);

    static void AddHeader(std::string& src, bool isVertexShader);
    static void AddUniforms(std::string& src, bool isVertexShader);
};
