#include "Direct3DDevice8.h"
#include "GLESTexture8.h"
#include "GLESVertexBuffer8.h"
#include "GLESIndexBuffer8.h"
#include "GLESSurface8.h"

#define LOG_TAG "DX8Wrapper"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

DX8Wrapper_Direct3DDevice8::DX8Wrapper_Direct3DDevice8(HWND hWnd) {
    InitEGL(hWnd);
}

DX8Wrapper_Direct3DDevice8::~DX8Wrapper_Direct3DDevice8() {
    CleanupEGL();
    for (auto* shader : m_vertexShaders) delete shader;
    for (auto* shader : m_pixelShaders) delete shader;
    m_vertexShaders.clear();
    m_pixelShaders.clear();
}

void DX8Wrapper_Direct3DDevice8::InitEGL(HWND hWnd) {
    m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (m_display == EGL_NO_DISPLAY) {
        LOGE("eglGetDisplay failed");
        return;
    }

    if (!eglInitialize(m_display, nullptr, nullptr)) {
        LOGE("eglInitialize failed");
        return;
    }

    const EGLint attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_STENCIL_SIZE, 8,
        EGL_NONE
    };

    EGLConfig config;
    EGLint numConfigs;
    if (!eglChooseConfig(m_display, attribs, &config, 1, &numConfigs) || numConfigs == 0) {
        LOGE("eglChooseConfig failed");
        return;
    }

    const EGLint contextAttribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
    };
    m_context = eglCreateContext(m_display, config, EGL_NO_CONTEXT, contextAttribs);
    if (m_context == EGL_NO_CONTEXT) {
        LOGE("eglCreateContext failed");
        return;
    }

    m_surface = eglCreateWindowSurface(m_display, config, (EGLNativeWindowType)hWnd, nullptr);
    if (m_surface == EGL_NO_SURFACE) {
        LOGE("eglCreateWindowSurface failed");
        return;
    }

    if (!eglMakeCurrent(m_display, m_surface, m_surface, m_context)) {
        LOGE("eglMakeCurrent failed");
        return;
    }

    LOGI("EGL Initialized successfully");
}

void DX8Wrapper_Direct3DDevice8::CleanupEGL() {
    if (m_display != EGL_NO_DISPLAY) {
        eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (m_context != EGL_NO_CONTEXT) eglDestroyContext(m_display, m_context);
        if (m_surface != EGL_NO_SURFACE) eglDestroySurface(m_display, m_surface);
        eglTerminate(m_display);
    }
    m_display = EGL_NO_DISPLAY;
    m_context = EGL_NO_CONTEXT;
    m_surface = EGL_NO_SURFACE;
}

HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::TestCooperativeLevel() { return D3D_OK; }
UINT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetAvailableTextureMem() { return 1024 * 1024 * 512; } // 512 MB
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::ResourceManagerDiscardBytes(DWORD Bytes) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetDirect3D(IDirect3D8** ppD3D8) { *ppD3D8 = nullptr; return D3D_OK; } // TODO: Store parent D3D8
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetDeviceCaps(D3DCAPS8* pCaps) { 
    if(pCaps) {
        // Fill caps
        pCaps->DeviceType = D3DDEVTYPE_HAL;
        pCaps->AdapterOrdinal = 0;
        pCaps->Caps = 0;
        pCaps->Caps2 = 0;
        pCaps->Caps3 = 0;
        pCaps->PresentationIntervals = D3DPRESENT_INTERVAL_IMMEDIATE;
        pCaps->CursorCaps = 0;
        pCaps->DevCaps = D3DDEVCAPS_HWTRANSFORMANDLIGHT;
        pCaps->PrimitiveMiscCaps = 0;
        pCaps->RasterCaps = 0;
        pCaps->ZCmpCaps = D3DPCMPCAPS_LESSEQUAL;
        pCaps->SrcBlendCaps = D3DPBLENDCAPS_SRCALPHA;
        pCaps->DestBlendCaps = D3DPBLENDCAPS_INVSRCALPHA;
        pCaps->AlphaCmpCaps = D3DPCMPCAPS_ALWAYS;
        pCaps->ShadeCaps = D3DPSHADECAPS_COLORGOURAUDRGB;
        pCaps->TextureCaps = 0;
        pCaps->TextureFilterCaps = D3DPTFILTERCAPS_MINFPOINT | D3DPTFILTERCAPS_MAGFPOINT;
        pCaps->CubeTextureFilterCaps = 0;
        pCaps->VolumeTextureFilterCaps = 0;
        pCaps->TextureAddressCaps = D3DPTADDRESSCAPS_WRAP;
        pCaps->VolumeTextureAddressCaps = 0;
        pCaps->LineCaps = 0;
        pCaps->MaxTextureWidth = 4096;
        pCaps->MaxTextureHeight = 4096;
        pCaps->MaxVolumeExtent = 0;
        pCaps->MaxTextureRepeat = 1;
        pCaps->MaxTextureAspectRatio = 1; //?
        pCaps->MaxAnisotropy = 1;
        pCaps->MaxVertexW = 1.0f;
        pCaps->GuardBandLeft = 0.0f;
        pCaps->GuardBandTop = 0.0f;
        pCaps->GuardBandRight = 0.0f;
        pCaps->GuardBandBottom = 0.0f;
        pCaps->ExtentsAdjust = 0.0f;
        pCaps->StencilCaps = 0;
        pCaps->FVFCaps = D3DFVFCAPS_TEXCOORDCOUNTMASK;
        pCaps->TextureOpCaps = 0;
        pCaps->MaxTextureBlendStages = 1;
        pCaps->MaxSimultaneousTextures = 1;
        pCaps->VertexProcessingCaps = D3DVTXPCAPS_DIRECTIONALLIGHTS;
        pCaps->MaxActiveLights = 8;
        pCaps->MaxUserClipPlanes = 0;
        pCaps->MaxVertexBlendMatrices = 0;
        pCaps->MaxVertexBlendMatrixIndex = 0;
        pCaps->MaxPointSize = 1.0f;
        pCaps->MaxPrimitiveCount = 0xFFFF;
        pCaps->MaxVertexIndex = 0xFFFF;
        pCaps->MaxStreams = 1;
        pCaps->MaxStreamStride = 0;
        pCaps->VertexShaderVersion = D3DVS_VERSION(1, 1);
        pCaps->MaxVertexShaderConst = 96;
        pCaps->PixelShaderVersion = D3DPS_VERSION(1, 1);
        pCaps->MaxPixelShaderValue = 1.0f;
    }
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetDisplayMode(D3DDISPLAYMODE* pMode) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface8* pCursorBitmap) { return D3D_OK; }
void STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetCursorPosition(UINT X, UINT Y, DWORD Flags) {}
BOOL STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::ShowCursor(BOOL bShow) { return TRUE; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain8** pSwapChain) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) { 
    if (m_display != EGL_NO_DISPLAY && m_surface != EGL_NO_SURFACE) {
        if (!eglSwapBuffers(m_display, m_surface)) {
            LOGE("eglSwapBuffers failed");
            return D3DERR_DEVICELOST;
        }
    }
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetBackBuffer(UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8** ppBackBuffer) { 
    // Return a dummy surface representing the backbuffer
    *ppBackBuffer = new GLESSurface8(this, 1920, 1080, D3DFMT_X8R8G8B8); // TODO: Use actual resolution
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetRasterStatus(D3DRASTER_STATUS* pRasterStatus) { return D3D_OK; }
void STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetGammaRamp(DWORD Flags, CONST D3DGAMMARAMP* pRamp) {}
void STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetGammaRamp(D3DGAMMARAMP* pRamp) {}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8** ppTexture) { 
    *ppTexture = new GLESTexture8(this, Width, Height, Levels, Usage, Format, Pool);
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture8** ppVolumeTexture) { return D3DERR_NOTAVAILABLE; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture8** ppCubeTexture) { *ppCubeTexture = nullptr; return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer8** ppVertexBuffer) { 
    *ppVertexBuffer = new GLESVertexBuffer8(this, Length, Usage, FVF, Pool);
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8** ppIndexBuffer) { 
    *ppIndexBuffer = new GLESIndexBuffer8(this, Length, Usage, Format, Pool);
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, BOOL Lockable, IDirect3DSurface8** ppSurface) { 
    *ppSurface = new GLESSurface8(this, Width, Height, Format);
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8** ppSurface) { 
    *ppSurface = new GLESSurface8(this, Width, Height, Format);
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::CreateImageSurface(UINT Width, UINT Height, D3DFORMAT Format, IDirect3DSurface8** ppSurface) { 
    *ppSurface = new GLESSurface8(this, Width, Height, Format);
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::CopyRects(IDirect3DSurface8* pSourceSurface, CONST RECT* pSourceRectsArray, UINT cRects, IDirect3DSurface8* pDestinationSurface, CONST POINT* pDestPointsArray) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::UpdateTexture(IDirect3DBaseTexture8* pSourceTexture, IDirect3DBaseTexture8* pDestinationTexture) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetFrontBuffer(IDirect3DSurface8* pDestSurface) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetRenderTarget(IDirect3DSurface8* pRenderTarget, IDirect3DSurface8* pNewZStencil) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetRenderTarget(IDirect3DSurface8** ppRenderTarget) { *ppRenderTarget = nullptr; return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetDepthStencilSurface(IDirect3DSurface8** ppZStencilSurface) { *ppZStencilSurface = nullptr; return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::BeginScene() { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::EndScene() { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) { 
    GLbitfield mask = 0;
    if (Flags & D3DCLEAR_TARGET) {
        mask |= GL_COLOR_BUFFER_BIT;
        float a = ((Color >> 24) & 0xFF) / 255.0f;
        float r = ((Color >> 16) & 0xFF) / 255.0f;
        float g = ((Color >> 8) & 0xFF) / 255.0f;
        float b = (Color & 0xFF) / 255.0f;
        glClearColor(r, g, b, a);
    }
    if (Flags & D3DCLEAR_ZBUFFER) {
        mask |= GL_DEPTH_BUFFER_BIT;
        glClearDepthf(Z);
    }
    if (Flags & D3DCLEAR_STENCIL) {
        mask |= GL_STENCIL_BUFFER_BIT;
        glClearStencil(Stencil);
    }
    
    if (Count > 0 && pRects) {
        // Scissoring for partial clear
        glEnable(GL_SCISSOR_TEST);
        for (DWORD i = 0; i < Count; ++i) {
            // D3DRECT is x1, y1, x2, y2. GLES Scissor is x, y, width, height (y from bottom)
            // TODO: Y-flip handling
            glScissor(pRects[i].x1, pRects[i].y1, pRects[i].x2 - pRects[i].x1, pRects[i].y2 - pRects[i].y1);
            glClear(mask);
        }
        glDisable(GL_SCISSOR_TEST);
    } else {
        glClear(mask);
    }
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetViewport(CONST D3DVIEWPORT8* pViewport) { 
    if (pViewport) {
        glViewport(pViewport->X, pViewport->Y, pViewport->Width, pViewport->Height);
        glDepthRangef(pViewport->MinZ, pViewport->MaxZ);
    }
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetViewport(D3DVIEWPORT8* pViewport) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetMaterial(CONST D3DMATERIAL8* pMaterial) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetMaterial(D3DMATERIAL8* pMaterial) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetLight(DWORD Index, CONST D3DLIGHT8* pLight) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetLight(DWORD Index, D3DLIGHT8* pLight) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::LightEnable(DWORD Index, BOOL Enable) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetLightEnable(DWORD Index, BOOL* pEnable) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetClipPlane(DWORD Index, CONST float* pPlane) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetClipPlane(DWORD Index, float* pPlane) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) {
    switch(State) {
        case D3DRS_ZENABLE:
            if (Value) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
            break;
        case D3DRS_ZWRITEENABLE:
            glDepthMask(Value ? GL_TRUE : GL_FALSE);
            break;
        case D3DRS_ALPHABLENDENABLE:
            if (Value) glEnable(GL_BLEND); else glDisable(GL_BLEND);
            break;
        case D3DRS_SRCBLEND:
        case D3DRS_DESTBLEND:
            // TODO: Map blend factors
            break;
        case D3DRS_CULLMODE:
            if (Value == D3DCULL_NONE) glDisable(GL_CULL_FACE);
            else {
                glEnable(GL_CULL_FACE);
                // CW vs CCW? DX is usually CW? OGL CCW?
                // D3DCULL_CW: Cull CW faces.
                // D3DCULL_CCW: Cull CCW faces.
                // glCullFace(GL_BACK). glFrontFace(GL_CCW default).
                if (Value == D3DCULL_CW) glFrontFace(GL_CCW); // Cull CW (Back)
                else glFrontFace(GL_CW); // Cull CCW (Front) -> Back is CW?
            }
            break;
        default:
            break;
    }
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::BeginStateBlock() { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::EndStateBlock(DWORD* pToken) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::ApplyStateBlock(DWORD Token) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::CaptureStateBlock(DWORD Token) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::DeleteStateBlock(DWORD Token) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::CreateStateBlock(D3DSTATEBLOCKTYPE Type, DWORD* pToken) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetClipStatus(CONST D3DCLIPSTATUS8* pClipStatus) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetClipStatus(D3DCLIPSTATUS8* pClipStatus) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetTexture(DWORD Stage, IDirect3DBaseTexture8** ppTexture) { 
    if (Stage >= 8) return D3DERR_INVALIDCALL;
    *ppTexture = m_currentTextures[Stage];
    if (*ppTexture) (*ppTexture)->AddRef();
    return D3D_OK; 
}

HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetTexture(DWORD Stage, IDirect3DBaseTexture8* pTexture) {
    if (Stage >= 8) return D3DERR_INVALIDCALL;
    
    // Release old
    if (m_currentTextures[Stage]) m_currentTextures[Stage]->Release();
    
    m_currentTextures[Stage] = pTexture;
    if (pTexture) pTexture->AddRef();
    
    glActiveTexture(GL_TEXTURE0 + Stage);
    if (pTexture) {
        // Assume texture is GLESTexture8 (2D)
        GLESTexture8* tex = static_cast<GLESTexture8*>(pTexture);
        glBindTexture(GL_TEXTURE_2D, tex->GetGLTextureID());
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::ValidateDevice(DWORD* pNumPasses) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetInfo(DWORD DevInfoID, void* pDevInfoStruct, DWORD DevInfoStructSize) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetCurrentTexturePalette(UINT PaletteNumber) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetCurrentTexturePalette(UINT *PaletteNumber) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) {
    if (!m_currentVertexBuffer) return D3DERR_INVALIDCALL;

    GLESVertexBuffer8* vb = static_cast<GLESVertexBuffer8*>(m_currentVertexBuffer);
    BYTE* data = (BYTE*)vb->GetData();
    if (!data) return D3DERR_INVALIDCALL;

    UINT stride = m_currentStride;
    if (stride == 0) return D3DERR_INVALIDCALL;

    data += StartVertex * stride;

    GLenum mode = GL_TRIANGLES;
    UINT vertexCount = 0;
    switch (PrimitiveType) {
        case D3DPT_TRIANGLELIST: mode = GL_TRIANGLES; vertexCount = PrimitiveCount * 3; break;
        case D3DPT_TRIANGLESTRIP: mode = GL_TRIANGLE_STRIP; vertexCount = PrimitiveCount + 2; break;
        case D3DPT_POINTLIST: mode = GL_POINTS; vertexCount = PrimitiveCount; break;
        case D3DPT_LINELIST: mode = GL_LINES; vertexCount = PrimitiveCount * 2; break;
        case D3DPT_LINESTRIP: mode = GL_LINE_STRIP; vertexCount = PrimitiveCount + 1; break;
        case D3DPT_TRIANGLEFAN: mode = GL_TRIANGLE_FAN; vertexCount = PrimitiveCount + 2; break;
        default: break;
    }

    DWORD fvf = m_currentVertexShader;
    BOOL isFVF = (fvf < 0x1000); 

    if (!isFVF) {
        // Programmable Pipeline
        GLuint program = GetShaderProgram(fvf, m_currentPixelShader);
        if (program) {
            glUseProgram(program);
            
            // Bind Uniforms
            GLint locVC = glGetUniformLocation(program, "vc");
            if (locVC != -1) glUniform4fv(locVC, 96, (const GLfloat*)m_vsConstants);

            GLint locPC = glGetUniformLocation(program, "pc");
            if (locPC != -1) glUniform4fv(locPC, 8, (const GLfloat*)m_psConstants);
        }
        
        // Fallback Attribute Setup (Assumes standard layout V0=Pos, V1=Norm, V3=Tex)
        // TODO: Use Vertex Declaration
        BYTE* ptr = data;
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, ptr); // v0 = Pos
        
        if (stride >= 24) {
             glEnableVertexAttribArray(1);
             glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, ptr + 12); // v1 = Norm
        } else glDisableVertexAttribArray(1);

        if (stride >= 28) { // Diffuse? 
             // Generals often has Diffuse at offset 24? or Texture?
             // Let's assume Textures for now as they are more critical
             // v3 = Tex0
             // But offset?
             // If stride 32 (Pos+Norm+Tex2), Tex at 24.
             glEnableVertexAttribArray(3); // v3
             glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, ptr + 24);
        } else glDisableVertexAttribArray(3);
        
        glDisableVertexAttribArray(2); // v2
    } else {
        // FVF Pipeline
        BYTE* ptr = data;
    
        // Position (XYZ=0x002, XYZRHW=0x004)
        if ((fvf & D3DFVF_XYZ) || (fvf & D3DFVF_XYZRHW)) {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, ptr);
        ptr += 3 * sizeof(float);
        if (fvf & D3DFVF_XYZRHW) ptr += sizeof(float);
    } else {
        glDisableVertexAttribArray(0);
    }

    // Normal (0x010)
    if (fvf & D3DFVF_NORMAL) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, ptr);
        ptr += 3 * sizeof(float);
    } else {
         glDisableVertexAttribArray(1);
    }

    // Diffuse (0x040)
    if (fvf & D3DFVF_DIFFUSE) {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, ptr);
        ptr += sizeof(DWORD);
    } else {
         glDisableVertexAttribArray(2);
    }

    // TexCoord (0x100 mask, shift 8)
    int texCount = (fvf & D3DFVF_TEXCOUNT_MASK) >> D3DFVF_TEXCOUNT_SHIFT;
    if (texCount > 0) {
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, ptr);
         ptr += 2 * sizeof(float);
    } else {
         glDisableVertexAttribArray(3);
    }

    }

    glDrawArrays(mode, 0, vertexCount);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

    return D3D_OK;
}

HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT PrimitiveCount) {
    if (!m_currentVertexBuffer || !m_currentIndexBuffer) return D3DERR_INVALIDCALL;

    GLESVertexBuffer8* vb = static_cast<GLESVertexBuffer8*>(m_currentVertexBuffer);
    GLESIndexBuffer8* ib = static_cast<GLESIndexBuffer8*>(m_currentIndexBuffer);
    
    BYTE* vData = (BYTE*)vb->GetData();
    BYTE* iData = (BYTE*)ib->GetData();
    if (!vData || !iData) return D3DERR_INVALIDCALL;
    
    UINT stride = m_currentStride;
    
    // For DrawIndexed, Setup Attributes points to base of VertexBuffer?
    // Not necessarily.
    // glVertexAttribPointer should point to vData (base)
    // But offsets?
    // GLES handles index -> lookup.
    
    BYTE* ptr = vData;

    // FVF Logic (Duplicate... better to refactor)
    DWORD fvf = m_currentVertexShader;
    BOOL isFVF = (fvf < 0x1000); 

    if (!isFVF) {
         GLuint program = GetShaderProgram(fvf, m_currentPixelShader);
         if (program) {
            glUseProgram(program);
            GLint locVC = glGetUniformLocation(program, "vc");
            if (locVC != -1) glUniform4fv(locVC, 96, (const GLfloat*)m_vsConstants);
            GLint locPC = glGetUniformLocation(program, "pc");
            if (locPC != -1) glUniform4fv(locPC, 8, (const GLfloat*)m_psConstants);
         }
         
         BYTE* ptr = vData;
         glEnableVertexAttribArray(0);
         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, ptr);
         
         if (stride >= 24) {
             glEnableVertexAttribArray(1);
             glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, ptr + 12);
         } else glDisableVertexAttribArray(1);

         if (stride >= 32) {
             glEnableVertexAttribArray(3);
             glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, ptr + 24);
         } else glDisableVertexAttribArray(3);
         
         glDisableVertexAttribArray(2);
    } else {
        // FVF
        if ((fvf & D3DFVF_XYZ) || (fvf & D3DFVF_XYZRHW)) {
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, ptr);
            ptr += 3 * sizeof(float);
            if (fvf & D3DFVF_XYZRHW) ptr += sizeof(float);
        } else glDisableVertexAttribArray(0);

        if (fvf & D3DFVF_NORMAL) {
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, ptr);
            ptr += 3 * sizeof(float);
        } else glDisableVertexAttribArray(1);

        if (fvf & D3DFVF_DIFFUSE) {
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, ptr);
            ptr += sizeof(DWORD);
        } else glDisableVertexAttribArray(2);
    
        int texCount = (fvf & D3DFVF_TEXCOUNT_MASK) >> D3DFVF_TEXCOUNT_SHIFT;
        if (texCount > 0) {
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, ptr);
             ptr += 2 * sizeof(float);
        } else glDisableVertexAttribArray(3);
    }
    
    // Mode
    GLenum mode = GL_TRIANGLES;
    UINT indexCount = 0;
    switch (PrimitiveType) {
        case D3DPT_TRIANGLELIST: mode = GL_TRIANGLES; indexCount = PrimitiveCount * 3; break;
        case D3DPT_TRIANGLESTRIP: mode = GL_TRIANGLE_STRIP; indexCount = PrimitiveCount + 2; break;
        // ...
        default: mode = GL_TRIANGLES; indexCount = PrimitiveCount * 3; break;
    }

    // Helper for Index Type
    GLenum type = GL_UNSIGNED_SHORT;
    int indexSize = 2;
    if (ib->GetFormat() == D3DFMT_INDEX32) { type = GL_UNSIGNED_INT; indexSize = 4; }
    
    const void* indices = iData + startIndex * indexSize;

    glDrawElements(mode, indexCount, type, indices);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

    return D3D_OK;
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer8* pDestBuffer, DWORD Flags) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::CreateVertexShader(CONST DWORD* pDeclaration, CONST DWORD* pFunction, DWORD* pHandle, DWORD Usage) {
    if (!pHandle) return D3DERR_INVALIDCALL;
    GLESVertexShader* shader = new GLESVertexShader(pDeclaration, pFunction);
    m_vertexShaders.push_back(shader);
    *pHandle = (DWORD)(m_vertexShaders.size() + 0x1000); // Offset handles to avoid 0
    return D3D_OK;
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetVertexShader(DWORD Handle) {
    m_currentVertexShader = Handle;
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetVertexShader(DWORD* pHandle) { *pHandle = 0; return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::DeleteVertexShader(DWORD Handle) {
    // Basic stub - we don't actually delete from vector to keep indices valid simple
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetVertexShaderConstant(DWORD Register, CONST void* pConstantData, DWORD ConstantCount) {
    if (Register + ConstantCount > 96) return D3DERR_INVALIDCALL;
    memcpy(&m_vsConstants[Register][0], pConstantData, ConstantCount * 4 * sizeof(float));
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetVertexShaderConstant(DWORD Register, void* pConstantData, DWORD ConstantCount) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetVertexShaderDeclaration(DWORD Handle, void* pData, DWORD* pSizeOfData) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetVertexShaderFunction(DWORD Handle, void* pData, DWORD* pSizeOfData) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer8* pStreamData, UINT Stride) { 
    if (StreamNumber == 0) {
        m_currentVertexBuffer = pStreamData;
        m_currentStride = Stride;
    }
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer8** ppStreamData, UINT* pStride) { 
    if(StreamNumber == 0) {
        if(ppStreamData) {
            *ppStreamData = m_currentVertexBuffer;
            if(m_currentVertexBuffer) m_currentVertexBuffer->AddRef();
        }
        if(pStride) *pStride = m_currentStride;
    } else {
        if(ppStreamData) *ppStreamData = nullptr;
        if(pStride) *pStride = 0;
    }
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetIndices(IDirect3DIndexBuffer8* pIndexData, UINT BaseVertexIndex) { 
    m_currentIndexBuffer = pIndexData;
    m_currentBaseVertexIndex = BaseVertexIndex;
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetIndices(IDirect3DIndexBuffer8** ppIndexData, UINT* pBaseVertexIndex) { 
    if(ppIndexData) {
        *ppIndexData = m_currentIndexBuffer;
        if(m_currentIndexBuffer) m_currentIndexBuffer->AddRef();
    }
    if(pBaseVertexIndex) *pBaseVertexIndex = m_currentBaseVertexIndex;
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::CreatePixelShader(CONST DWORD* pFunction, DWORD* pHandle) {
    if (!pHandle) return D3DERR_INVALIDCALL;
    GLESPixelShader* shader = new GLESPixelShader(pFunction);
    m_pixelShaders.push_back(shader);
    *pHandle = (DWORD)(m_pixelShaders.size() + 0x2000);
    return D3D_OK;
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetPixelShader(DWORD Handle) {
    m_currentPixelShader = Handle;
    return D3D_OK;
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetPixelShader(DWORD* pHandle) { *pHandle = 0; return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::DeletePixelShader(DWORD Handle) {
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::SetPixelShaderConstant(DWORD Register, CONST void* pConstantData, DWORD ConstantCount) {
    if (Register + ConstantCount > 8) return D3DERR_INVALIDCALL;
    memcpy(&m_psConstants[Register][0], pConstantData, ConstantCount * 4 * sizeof(float));
    return D3D_OK; 
}
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetPixelShaderConstant(DWORD Register, void* pConstantData, DWORD ConstantCount) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::GetPixelShaderFunction(DWORD Handle, void* pData, DWORD* pSizeOfData) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE DX8Wrapper_Direct3DDevice8::DeletePatch(UINT Handle) { return D3D_OK; }

GLuint DX8Wrapper_Direct3DDevice8::GetShaderProgram(DWORD vsHandle, DWORD psHandle) {
    unsigned long long key = ((unsigned long long)vsHandle << 32) | psHandle;
    auto it = m_programCache.find(key);
    if (it != m_programCache.end()) {
        return it->second;
    }

    // Link new program
    GLuint vs = 0;
    GLuint ps = 0;

    // Resolve VS
    if (vsHandle >= 0x1000) {
        size_t index = vsHandle - 0x1000;
        if (index < m_vertexShaders.size() && m_vertexShaders[index]) {
            vs = m_vertexShaders[index]->GetShaderObject();
        }
    } else {
        // FVF / Fixed Function - Not supported in this path yet
        // TODO: Generate FVF shader on the fly?
        return 0; 
    }

    // Resolve PS
    if (psHandle >= 0x2000) {
        size_t index = psHandle - 0x2000;
        if (index < m_pixelShaders.size() && m_pixelShaders[index]) {
            ps = m_pixelShaders[index]->GetShaderObject();
        }
    } else {
        // Fixed function PS? 0 is valid for no PS?
        // If 0, use default white/texture PS?
    }

    if (!vs) return 0; // VS is mandatory for programmable pipeline

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    if (ps) glAttachShader(program, ps);

    glLinkProgram(program);
    
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint infoLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            std::vector<char> infoLog(infoLen);
            glGetProgramInfoLog(program, infoLen, nullptr, infoLog.data());
            LOGE("Error linking program (VS %u, PS %u):\n%s", (unsigned int)vsHandle, (unsigned int)psHandle, infoLog.data());
        }
        glDeleteProgram(program);
        return 0;
    }

    m_programCache[key] = program;
    return program;
}
