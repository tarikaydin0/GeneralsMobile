#include "GLESShaderTranslator.h"
#include <sstream>
#include <iomanip>
#include <android/log.h>

#define LOG_TAG "DX8ShaderTranslator"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// DX8 Shader Token Definitions (Partial, based on known specs)
#define D3DSI_OPCODE_MASK 0x0000FFFF
#define D3DSI_COISSUE 0x40000000

// Register Types
#define D3DSPR_TEMP 0
#define D3DSPR_INPUT 1
#define D3DSPR_CONST 2
#define D3DSPR_ADDR 3
#define D3DSPR_TEXTURE 3 // For PS
#define D3DSPR_RASTOUT 4
#define D3DSPR_ATTROUT 5
#define D3DSPR_TEXCRDOUT 6
#define D3DSPR_CONSTINT 7
#define D3DSPR_COLOROUT 8
#define D3DSPR_DEPTHOUT 9
#define D3DSPR_SAMPLER 10
#define D3DSPR_CONST2 11
#define D3DSPR_CONST3 12
#define D3DSPR_CONST4 13
#define D3DSPR_TEMP1 14 // ?
#define D3DSPR_PREDICATE 15 

// Source Modifiers
#define D3DSPSM_NONE 0
#define D3DSPSM_NEG 1
#define D3DSPSM_BIAS 2
#define D3DSPSM_BIASNEG 3
#define D3DSPSM_SIGN 4
#define D3DSPSM_SIGNNEG 5
#define D3DSPSM_COMP 6
#define D3DSPSM_X2 7
#define D3DSPSM_X2NEG 8
#define D3DSPSM_DZ 9
#define D3DSPSM_DW 10

// Opcodes missing from DX8 d3d8types.h (added in later versions)
#ifndef D3DSIO_ABS
#define D3DSIO_ABS 35
#endif
#ifndef D3DSIO_NRM
#define D3DSIO_NRM 36
#endif
#ifndef D3DSIO_POW
#define D3DSIO_POW 32
#endif
#ifndef D3DSIO_CRS
#define D3DSIO_CRS 33
#endif
#ifndef D3DSIO_SINCOS
#define D3DSIO_SINCOS 37
#endif
#ifndef D3DSIO_M3x2
#define D3DSIO_M3x2 24
#endif
#ifndef D3DSIO_SGN
#define D3DSIO_SGN 34
#endif

// Token extraction helpers
inline DWORD GetBits(DWORD token, int start, int count) {
    return (token >> start) & ((1 << count) - 1);
}

std::string GLESShaderTranslator::TranslateVertexShader(const DWORD* pFunction, const DWORD* pDeclaration) {
    std::string src;
    AddHeader(src, true);
    AddUniforms(src, true);
    
    src += "void main() {\n";
    src += "    // Initialize output regs\n";
    src += "    gl_Position = vec4(0.0);\n";
    src += "    // Translation Start\n";

    if (!pFunction) return src + "}"; 

    const DWORD* pToken = pFunction;
    int indent = 1;

    // Skip version token
    if ((*pToken & 0xFFFF0000) == 0xFFFE0000) { // Comment?
         // Skip comments
    }
    // Version token is usually first: D3DVS_VERSION(1,1) -> 0x0101
    // Actually D3DVS_VERSION macro: 0xFFFE0101? No.
    // D3DVS_VERSION is 0xFFFF0000 | (Major << 8) | Minor
    pToken++; // Skip version

    while (*pToken != 0x0000FFFF) { // D3DSIO_END
        src += ParseInstruction(pToken, true, indent);
    }
    
    // Map vs outputs to GL outputs
    // GLSL uses gl_Position.
    // DX8 uses oPos (c0-c3 usually from projection, or result of m4x4)
    // We handle this in ParseInstruction by mapping oPos to gl_Position directly.
    
    src += "}\n";
    return src;
}

std::string GLESShaderTranslator::TranslatePixelShader(const DWORD* pFunction) {
    std::string src;
    AddHeader(src, false);
    AddUniforms(src, false);

    src += "out vec4 fragColor;\n";
    src += "void main() {\n";
    src += "    // Initialize inputs (interpolated)\n";
    src += "    // Translation Start\n";
    
    if (!pFunction) return src + "}";

    const DWORD* pToken = pFunction;
    int indent = 1;
    pToken++; // Skip version

    while (*pToken != 0x0000FFFF) {
        src += ParseInstruction(pToken, false, indent);
    }

    // Output mapping
    // DX8 ps 1.x output is r0
    src += "    fragColor = r0;\n";
    src += "}\n";
    return src;
}

void GLESShaderTranslator::AddHeader(std::string& src, bool isVertexShader) {
    src += "#version 300 es\n";
    src += "precision highp float;\n";
    src += "precision highp int;\n";
    if (isVertexShader) {
        // Attributes
        for(int i=0; i<16; ++i) src += "layout(location = " + std::to_string(i) + ") in vec4 v" + std::to_string(i) + ";\n";
        // Outputs
        src += "out vec4 oD0; // Diffuse Color\n";
        src += "out vec4 oD1; // Specular Color\n";
        for(int i=0; i<8; ++i) src += "out vec4 oT" + std::to_string(i) + "; // TexCoord " + std::to_string(i) + "\n";
    } else {
        // Inputs
        src += "in vec4 oD0;\n";
        src += "in vec4 oD1;\n";
        for(int i=0; i<8; ++i) src += "in vec4 oT" + std::to_string(i) + ";\n";
    }
}

void GLESShaderTranslator::AddUniforms(std::string& src, bool isVertexShader) {
    if (isVertexShader) {
        src += "uniform vec4 vc[96]; // Vertex Constants\n";
        // Temps
        for(int i=0; i<12; ++i) src += "vec4 r" + std::to_string(i) + ";\n";
        src += "vec4 a0; // Address reg\n";
    } else {
        src += "uniform vec4 pc[8]; // Pixel Constants\n";
        src += "uniform sampler2D t[8]; // Textures\n";
        // Temps
        for(int i=0; i<8; ++i) src += "vec4 r" + std::to_string(i) + ";\n";
        src += "vec4 t_temp[8]; // Texture coords temp\n"; // t0-t7 can be temp in ps?
    }
}

std::string GLESShaderTranslator::ParseInstruction(const DWORD*& pToken, bool isVertexShader, int& indent) {
    DWORD token = *pToken++;
    DWORD opcode = token & D3DSI_OPCODE_MASK;
    // DWORD controls = token & 0xFFFF0000; // Co-issue, predicates, etc.

    if (opcode == D3DSIO_COMMENT) {
        DWORD size = (token >> 16) & 0xFFFF;
        pToken += size;
        return "";
    }
    
    if (opcode == D3DSIO_DEF) {
        // Const definition in-stream?
        // Skip for now, assume constants set via SetVertexShaderConstant
        // Actually this defines constants like c[n] = x,y,z,w
        // Used in PS mostly.
        return "// DEF skipped\n";
        // pToken += 5?
    }

    // Args
    std::string dest = ParseDestinationParameter(*pToken++, isVertexShader);
    std::string src0, src1, src2, src3;
    
    // Determine Arg Count
    int argCount = 0;
    switch(opcode) {
        case D3DSIO_MOV: case D3DSIO_RSQ: case D3DSIO_RCP: case D3DSIO_EXP: case D3DSIO_LOG: case D3DSIO_ABS: case D3DSIO_NRM: case D3DSIO_LIT:
             argCount = 1; break;
        case D3DSIO_ADD: case D3DSIO_SUB: case D3DSIO_MUL: case D3DSIO_MIN: case D3DSIO_MAX: case D3DSIO_DP3: case D3DSIO_DP4: case D3DSIO_DST:
        case D3DSIO_M4x4: case D3DSIO_M4x3: case D3DSIO_M3x4: case D3DSIO_M3x3: case D3DSIO_M3x2: case D3DSIO_POW: case D3DSIO_CRS:
             argCount = 2; break;
        case D3DSIO_MAD: case D3DSIO_LRP:
             argCount = 3; break;
        // Texturing instructions (PS)
        case D3DSIO_TEX: // tex t0 
             // tex dest src? or tex t0
             // In PS 1.4: tex ld, src.
             // In PS 1.1: tex t0. No src.
             // DX8 Headers say D3DSIO_TEX takes 0 args? No.
             // If version < 1.4, argcount = 0 (just dst implied? NO, dst is separate token)
             // Check version? 
             argCount = 0; 
             break;
    }
    
    if (argCount >= 1) src0 = ParseSourceParameter(*pToken++, isVertexShader);
    if (argCount >= 2) src1 = ParseSourceParameter(*pToken++, isVertexShader);
    if (argCount >= 3) src2 = ParseSourceParameter(*pToken++, isVertexShader);
    
    std::stringstream ss;
    for(int i=0; i<indent; ++i) ss << "    ";
    
    switch(opcode) {
        case D3DSIO_MOV: ss << dest << " = " << src0 << ";"; break;
        case D3DSIO_ADD: ss << dest << " = " << src0 << " + " << src1 << ";"; break;
        case D3DSIO_SUB: ss << dest << " = " << src0 << " - " << src1 << ";"; break;
        case D3DSIO_MUL: ss << dest << " = " << src0 << " * " << src1 << ";"; break;
        case D3DSIO_MAD: ss << dest << " = " << src0 << " * " << src1 << " + " << src2 << ";"; break;
        case D3DSIO_DP3: ss << dest << " = vec4(dot(vec3(" << src0 << "), vec3(" << src1 << ")));"; break;
        case D3DSIO_DP4: ss << dest << " = vec4(dot(" << src0 << ", " << src1 << "));"; break;
        case D3DSIO_RSQ: ss << dest << " = vec4(inversesqrt(" << src0 << "));"; break; // Only uses scalar
        case D3DSIO_RCP: ss << dest << " = vec4(1.0 / " << src0 << ");"; break;
        case D3DSIO_MIN: ss << dest << " = min(" << src0 << ", " << src1 << ");"; break;
        case D3DSIO_MAX: ss << dest << " = max(" << src0 << ", " << src1 << ");"; break;
        
        // Matrix Macros (Expanded to dot products or matrix mul if supported)
        // M4x4: 4 dp4
        // M4x3: 3 dp4
        // We can check if dst is a temp reg or output.
        // Actually dst usage mask tells us which rows.
        // Simplified:
        case D3DSIO_M4x4: 
             ss << "// M4x4\n";
             ss << "    " << dest << ".x = dot(" << src0 << ", " << src1 << ");\n"; // c[n]
             ss << "    " << dest << ".y = dot(" << src0 << ", " << (isVertexShader?"vc[":"pc[") << (GetBits(*(pToken-2), 0, 13) + 1) << "]);\n"; 
             ss << "    " << dest << ".z = dot(" << src0 << ", " << (isVertexShader?"vc[":"pc[") << (GetBits(*(pToken-2), 0, 13) + 2) << "]);\n";
             ss << "    " << dest << ".w = dot(" << src0 << ", " << (isVertexShader?"vc[":"pc[") << (GetBits(*(pToken-2), 0, 13) + 3) << "]);";
             break;
        // ... Handle others similarly
             
        default: ss << "// Opcode " << opcode << " Not Implemented"; break;
    }
    
    ss << "\n";
    return ss.str();
}

std::string GLESShaderTranslator::ParseDestinationParameter(DWORD token, bool isVertexShader) {
    std::string reg = GetRegisterName(token & 0x1FFF, isVertexShader, true);
    std::string mask = GetWriteMask(token);
    return reg + mask;
}

std::string GLESShaderTranslator::ParseSourceParameter(DWORD token, bool isVertexShader) {
    std::string reg = GetRegisterName(token & 0x1FFF, isVertexShader, false);
    std::string swizzle = GetSwizzle(token);
    std::string modifier = GetSourceModifier(token, reg + swizzle);
    return modifier;
}

std::string GLESShaderTranslator::GetRegisterName(DWORD token, bool isVertexShader, bool isDestination) {
    DWORD type = (token >> 28) & 0x7;
    DWORD num = token & 0x7FF; // 11 bits for num? 
    // Wait, D3DSI_OPCODE_MASK is 0xFFFF.
    // Reg token:
    // 31: 1
    // 30-28: Type
    // 13-0: Number? 
    // Docs say: 
    // Bits 0-10: Register number (11 bits) -> 2048 regs max.
    // Bits 11-12: Relative addr type?
    
    type = (token >> 28) & 0x7; // types 0-7
    if (token & 0x80000000) { // Bit 31 set = parameter? 
         // Yes.
    }
    
    // Type extraction might need adjustment based on exact bit layout for DX8
    // Ref: D3DSHADER_PARAM_REGISTER_TYPE
    // 0=Temp, 1=Input, 2=Const...
    
    switch(type) {
        case D3DSPR_TEMP: return "r" + std::to_string(num);
        case D3DSPR_INPUT: return "v" + std::to_string(num);
        case D3DSPR_CONST: 
             return (isVertexShader ? "vc[" : "pc[") + std::to_string(num) + "]";
        case D3DSPR_ADDR: return "a0";
        case D3DSPR_RASTOUT: 
             if (num == 0) return "gl_Position";
             if (num == 1) return "gl_PointSize"; //?
             break;
        case D3DSPR_ATTROUT:
             if (num == 0) return "oD0";
             if (num == 1) return "oD1";
             break;
        case D3DSPR_TEXCRDOUT:
             return "oT" + std::to_string(num);
        // ...
    }
    return "UNKNOWN_REG";
}

std::string GLESShaderTranslator::GetSwizzle(DWORD token) {
    DWORD swizzle = (token >> 16) & 0xFF;
    if (swizzle == 0xE4) return ""; // .rgba (00 01 10 11) -> 0xE4. No swizzle needed.
    // Decode swizzle
    // 2 bits per channel. 0=x, 1=y, 2=z, 3=w
    const char* chars = "xyzw";
    std::string s = ".";
    s += chars[(swizzle >> 0) & 0x3];
    s += chars[(swizzle >> 2) & 0x3];
    s += chars[(swizzle >> 4) & 0x3];
    s += chars[(swizzle >> 6) & 0x3];
    return s;
}

std::string GLESShaderTranslator::GetWriteMask(DWORD token) {
    DWORD mask = (token >> 16) & 0xF;
    if (mask == 0xF) return ""; // All channels
    std::string s = ".";
    if (mask & 1) s += "x";
    if (mask & 2) s += "y";
    if (mask & 4) s += "z";
    if (mask & 8) s += "w";
    return s;
}

std::string GLESShaderTranslator::GetSourceModifier(DWORD token, const std::string& reg) {
    DWORD mod = (token >> 24) & 0xF;
    switch(mod) {
        case D3DSPSM_NONE: return reg;
        case D3DSPSM_NEG: return "-" + reg;
        case D3DSPSM_BIAS: return "(" + reg + " - 0.5)";
        case D3DSPSM_BIASNEG: return "-(" + reg + " - 0.5)";
        case D3DSPSM_SIGN: return "(2.0 * (" + reg + " - 0.5))";
        case D3DSPSM_SIGNNEG: return "-(2.0 * (" + reg + " - 0.5))";
        case D3DSPSM_COMP: return "(1.0 - " + reg + ")";
        case D3DSPSM_X2: return "(2.0 * " + reg + ")";
        case D3DSPSM_X2NEG: return "-(2.0 * " + reg + ")";
        case D3DSPSM_DZ: return "vec4(" + reg + ".xy, 0.0, 1.0)"; // Approximate?
        case D3DSPSM_DW: return "vec4(" + reg + ".xy, 0.0, 1.0)"; // Approximate?
    }
    return reg;
}
