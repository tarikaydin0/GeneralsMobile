#pragma once
#include <d3d8.h>
#include <vector>
#include <map>
#include "GLESShader.h"

#include <EGL/egl.h>
#include <GLES3/gl3.h>

class DX8Wrapper_Direct3DDevice8 : public IDirect3DDevice8 {
public:
    DX8Wrapper_Direct3DDevice8(HWND hWnd);
    virtual ~DX8Wrapper_Direct3DDevice8();
    // IUnknown
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj) override { *ppvObj = nullptr; return E_NOINTERFACE; }
    ULONG STDMETHODCALLTYPE AddRef() override { return 1; }
    ULONG STDMETHODCALLTYPE Release() override { return 1; }

    // IDirect3DDevice8
    HRESULT STDMETHODCALLTYPE TestCooperativeLevel() override;
    UINT STDMETHODCALLTYPE GetAvailableTextureMem() override;
    HRESULT STDMETHODCALLTYPE ResourceManagerDiscardBytes(DWORD Bytes) override;
    HRESULT STDMETHODCALLTYPE GetDirect3D(IDirect3D8** ppD3D8) override;
    HRESULT STDMETHODCALLTYPE GetDeviceCaps(D3DCAPS8* pCaps) override;
    HRESULT STDMETHODCALLTYPE GetDisplayMode(D3DDISPLAYMODE* pMode) override;
    HRESULT STDMETHODCALLTYPE GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters) override;
    HRESULT STDMETHODCALLTYPE SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface8* pCursorBitmap) override;
    void STDMETHODCALLTYPE SetCursorPosition(UINT X, UINT Y, DWORD Flags) override;
    BOOL STDMETHODCALLTYPE ShowCursor(BOOL bShow) override;
    HRESULT STDMETHODCALLTYPE CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain8** pSwapChain) override;
    HRESULT STDMETHODCALLTYPE Reset(D3DPRESENT_PARAMETERS* pPresentationParameters) override;
    HRESULT STDMETHODCALLTYPE Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) override;
    HRESULT STDMETHODCALLTYPE GetBackBuffer(UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8** ppBackBuffer) override;
    HRESULT STDMETHODCALLTYPE GetRasterStatus(D3DRASTER_STATUS* pRasterStatus) override;
    void STDMETHODCALLTYPE SetGammaRamp(DWORD Flags, CONST D3DGAMMARAMP* pRamp) override;
    void STDMETHODCALLTYPE GetGammaRamp(D3DGAMMARAMP* pRamp) override;
    HRESULT STDMETHODCALLTYPE CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8** ppTexture) override;
    HRESULT STDMETHODCALLTYPE CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture8** ppVolumeTexture) override;
    HRESULT STDMETHODCALLTYPE CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture8** ppCubeTexture) override;
    HRESULT STDMETHODCALLTYPE CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer8** ppVertexBuffer) override;
    HRESULT STDMETHODCALLTYPE CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8** ppIndexBuffer) override;
    HRESULT STDMETHODCALLTYPE CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, BOOL Lockable, IDirect3DSurface8** ppSurface) override;
    HRESULT STDMETHODCALLTYPE CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8** ppSurface) override;
    HRESULT STDMETHODCALLTYPE CreateImageSurface(UINT Width, UINT Height, D3DFORMAT Format, IDirect3DSurface8** ppSurface) override;
    HRESULT STDMETHODCALLTYPE CopyRects(IDirect3DSurface8* pSourceSurface, CONST RECT* pSourceRectsArray, UINT cRects, IDirect3DSurface8* pDestinationSurface, CONST POINT* pDestPointsArray) override;
    HRESULT STDMETHODCALLTYPE UpdateTexture(IDirect3DBaseTexture8* pSourceTexture, IDirect3DBaseTexture8* pDestinationTexture) override;
    HRESULT STDMETHODCALLTYPE GetFrontBuffer(IDirect3DSurface8* pDestSurface) override;
    HRESULT STDMETHODCALLTYPE SetRenderTarget(IDirect3DSurface8* pRenderTarget, IDirect3DSurface8* pNewZStencil) override;
    HRESULT STDMETHODCALLTYPE GetRenderTarget(IDirect3DSurface8** ppRenderTarget) override;
    HRESULT STDMETHODCALLTYPE GetDepthStencilSurface(IDirect3DSurface8** ppZStencilSurface) override;
    HRESULT STDMETHODCALLTYPE BeginScene() override;
    HRESULT STDMETHODCALLTYPE EndScene() override;
    HRESULT STDMETHODCALLTYPE Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) override;
    HRESULT STDMETHODCALLTYPE SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) override;
    HRESULT STDMETHODCALLTYPE GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix) override;
    HRESULT STDMETHODCALLTYPE MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) override;
    HRESULT STDMETHODCALLTYPE SetViewport(CONST D3DVIEWPORT8* pViewport) override;
    HRESULT STDMETHODCALLTYPE GetViewport(D3DVIEWPORT8* pViewport) override;
    HRESULT STDMETHODCALLTYPE SetMaterial(CONST D3DMATERIAL8* pMaterial) override;
    HRESULT STDMETHODCALLTYPE GetMaterial(D3DMATERIAL8* pMaterial) override;
    HRESULT STDMETHODCALLTYPE SetLight(DWORD Index, CONST D3DLIGHT8* pLight) override;
    HRESULT STDMETHODCALLTYPE GetLight(DWORD Index, D3DLIGHT8* pLight) override;
    HRESULT STDMETHODCALLTYPE LightEnable(DWORD Index, BOOL Enable) override;
    HRESULT STDMETHODCALLTYPE GetLightEnable(DWORD Index, BOOL* pEnable) override;
    HRESULT STDMETHODCALLTYPE SetClipPlane(DWORD Index, CONST float* pPlane) override;
    HRESULT STDMETHODCALLTYPE GetClipPlane(DWORD Index, float* pPlane) override;
    HRESULT STDMETHODCALLTYPE SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) override;
    HRESULT STDMETHODCALLTYPE GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue) override;
    HRESULT STDMETHODCALLTYPE BeginStateBlock() override;
    HRESULT STDMETHODCALLTYPE EndStateBlock(DWORD* pToken) override;
    HRESULT STDMETHODCALLTYPE ApplyStateBlock(DWORD Token) override;
    HRESULT STDMETHODCALLTYPE CaptureStateBlock(DWORD Token) override;
    HRESULT STDMETHODCALLTYPE DeleteStateBlock(DWORD Token) override;
    HRESULT STDMETHODCALLTYPE CreateStateBlock(D3DSTATEBLOCKTYPE Type, DWORD* pToken) override;
    HRESULT STDMETHODCALLTYPE SetClipStatus(CONST D3DCLIPSTATUS8* pClipStatus) override;
    HRESULT STDMETHODCALLTYPE GetClipStatus(D3DCLIPSTATUS8* pClipStatus) override;
    HRESULT STDMETHODCALLTYPE GetTexture(DWORD Stage, IDirect3DBaseTexture8** ppTexture) override;
    HRESULT STDMETHODCALLTYPE SetTexture(DWORD Stage, IDirect3DBaseTexture8* pTexture) override;
    HRESULT STDMETHODCALLTYPE GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue) override;
    HRESULT STDMETHODCALLTYPE SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) override;
    HRESULT STDMETHODCALLTYPE ValidateDevice(DWORD* pNumPasses) override;
    HRESULT STDMETHODCALLTYPE GetInfo(DWORD DevInfoID, void* pDevInfoStruct, DWORD DevInfoStructSize) override;
    HRESULT STDMETHODCALLTYPE SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries) override;
    HRESULT STDMETHODCALLTYPE GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries) override;
    HRESULT STDMETHODCALLTYPE SetCurrentTexturePalette(UINT PaletteNumber) override;
    HRESULT STDMETHODCALLTYPE GetCurrentTexturePalette(UINT *PaletteNumber) override;
    HRESULT STDMETHODCALLTYPE DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) override;
    HRESULT STDMETHODCALLTYPE DrawIndexedPrimitive(D3DPRIMITIVETYPE, UINT, UINT, UINT, UINT) override;
    HRESULT STDMETHODCALLTYPE DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) override;
    HRESULT STDMETHODCALLTYPE DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) override;
    HRESULT STDMETHODCALLTYPE ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer8* pDestBuffer, DWORD Flags) override;
    HRESULT STDMETHODCALLTYPE CreateVertexShader(CONST DWORD* pDeclaration, CONST DWORD* pFunction, DWORD* pHandle, DWORD Usage) override;
    HRESULT STDMETHODCALLTYPE SetVertexShader(DWORD Handle) override;
    HRESULT STDMETHODCALLTYPE GetVertexShader(DWORD* pHandle) override;
    HRESULT STDMETHODCALLTYPE DeleteVertexShader(DWORD Handle) override;
    HRESULT STDMETHODCALLTYPE SetVertexShaderConstant(DWORD Register, CONST void* pConstantData, DWORD ConstantCount) override;
    HRESULT STDMETHODCALLTYPE GetVertexShaderConstant(DWORD Register, void* pConstantData, DWORD ConstantCount) override;
    HRESULT STDMETHODCALLTYPE GetVertexShaderDeclaration(DWORD Handle, void* pData, DWORD* pSizeOfData) override;
    HRESULT STDMETHODCALLTYPE GetVertexShaderFunction(DWORD Handle, void* pData, DWORD* pSizeOfData) override;
    HRESULT STDMETHODCALLTYPE SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer8* pStreamData, UINT Stride) override;
    HRESULT STDMETHODCALLTYPE GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer8** ppStreamData, UINT* pStride) override;
    HRESULT STDMETHODCALLTYPE SetIndices(IDirect3DIndexBuffer8* pIndexData, UINT BaseVertexIndex) override;
    HRESULT STDMETHODCALLTYPE GetIndices(IDirect3DIndexBuffer8** ppIndexData, UINT* pBaseVertexIndex) override;
    HRESULT STDMETHODCALLTYPE CreatePixelShader(CONST DWORD* pFunction, DWORD* pHandle) override;
    HRESULT STDMETHODCALLTYPE SetPixelShader(DWORD Handle) override;
    HRESULT STDMETHODCALLTYPE GetPixelShader(DWORD* pHandle) override;
    HRESULT STDMETHODCALLTYPE DeletePixelShader(DWORD Handle) override;
    HRESULT STDMETHODCALLTYPE SetPixelShaderConstant(DWORD Register, CONST void* pConstantData, DWORD ConstantCount) override;
    HRESULT STDMETHODCALLTYPE GetPixelShaderConstant(DWORD Register, void* pConstantData, DWORD ConstantCount) override;
    HRESULT STDMETHODCALLTYPE GetPixelShaderFunction(DWORD Handle, void* pData, DWORD* pSizeOfData) override;
    HRESULT STDMETHODCALLTYPE DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo) override;
    HRESULT STDMETHODCALLTYPE DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo) override;
    HRESULT STDMETHODCALLTYPE DeletePatch(UINT Handle) override;

private:
    std::vector<GLESVertexShader*> m_vertexShaders;
    std::vector<GLESPixelShader*> m_pixelShaders;

    EGLDisplay m_display = EGL_NO_DISPLAY;
    EGLContext m_context = EGL_NO_CONTEXT;
    EGLSurface m_surface = EGL_NO_SURFACE;

    void InitEGL(HWND hWnd);
    void CleanupEGL();

    // Current State
    IDirect3DVertexBuffer8* m_currentVertexBuffer = nullptr;
    UINT m_currentStride = 0;
    IDirect3DIndexBuffer8* m_currentIndexBuffer = nullptr;
    UINT m_currentBaseVertexIndex = 0;
    DWORD m_currentVertexShader = 0;
    DWORD m_currentPixelShader = 0;

    // Shader Constants
    float m_vsConstants[96][4]; // c0-c95
    float m_psConstants[8][4];  // c0-c7
    
    // Textures
    IDirect3DBaseTexture8* m_currentTextures[8] = {nullptr};

    // Shader Program Cache
    // Map key: (VertexShaderHandle << 16) | PixelShaderHandle
    // Note: Handles are indices + offset.
    std::map<unsigned long long, GLuint> m_programCache;
    GLuint GetShaderProgram(DWORD vsHandle, DWORD psHandle);
};
