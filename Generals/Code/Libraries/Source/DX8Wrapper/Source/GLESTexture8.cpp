#include "GLESTexture8.h"
#include "GLESSurface8.h"
#include <android/log.h>

#define LOG_TAG "DX8Wrapper_Texture"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

GLESTexture8::GLESTexture8(IDirect3DDevice8* device, UINT width, UINT height, UINT levels, DWORD usage, D3DFORMAT format, D3DPOOL pool) 
    : m_device(device), m_width(width), m_height(height), m_levels(levels), m_format(format), m_refCount(1), m_textureID(0)
{
    device->AddRef();
    if (m_levels == 0) m_levels = 1; // Basic handling

    // Allocate mip levels
    m_mipLevels.resize(m_levels);
    UINT w = width;
    UINT h = height;
    for (UINT i = 0; i < m_levels; i++) {
        m_mipLevels[i].width = w;
        m_mipLevels[i].height = h;
        m_mipLevels[i].data.resize(w * h * 4); 
        if (w > 1) w /= 2;
        if (h > 1) h /= 2;
    }

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    // Set default params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLESTexture8::~GLESTexture8() {
    if (m_device) m_device->Release();
    if (m_textureID) glDeleteTextures(1, &m_textureID);
}

HRESULT STDMETHODCALLTYPE GLESTexture8::QueryInterface(REFIID riid, void** ppvObj) {
    *ppvObj = nullptr;
    return E_NOINTERFACE;
}
ULONG STDMETHODCALLTYPE GLESTexture8::AddRef() { return ++m_refCount; }
ULONG STDMETHODCALLTYPE GLESTexture8::Release() { 
    if (--m_refCount == 0) { delete this; return 0; }
    return m_refCount;
}

HRESULT STDMETHODCALLTYPE GLESTexture8::GetDevice(IDirect3DDevice8** ppDevice) {
    *ppDevice = m_device;
    m_device->AddRef();
    return D3D_OK;
}
HRESULT STDMETHODCALLTYPE GLESTexture8::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE GLESTexture8::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE GLESTexture8::FreePrivateData(REFGUID refguid) { return D3D_OK; }
DWORD STDMETHODCALLTYPE GLESTexture8::SetPriority(DWORD PriorityNew) { return 0; }
DWORD STDMETHODCALLTYPE GLESTexture8::GetPriority() { return 0; }
void STDMETHODCALLTYPE GLESTexture8::PreLoad() {}
D3DRESOURCETYPE STDMETHODCALLTYPE GLESTexture8::GetType() { return D3DRTYPE_TEXTURE; }

DWORD STDMETHODCALLTYPE GLESTexture8::SetLOD(DWORD LODNew) { return 0; }
DWORD STDMETHODCALLTYPE GLESTexture8::GetLOD() { return 0; }
DWORD STDMETHODCALLTYPE GLESTexture8::GetLevelCount() { return m_levels; }

HRESULT STDMETHODCALLTYPE GLESTexture8::GetLevelDesc(UINT Level, D3DSURFACE_DESC *pDesc) {
    if (Level >= m_levels) return D3DERR_INVALIDCALL;
    pDesc->Format = m_format;
    pDesc->Type = D3DRTYPE_SURFACE;
    pDesc->Usage = 0;
    pDesc->Pool = D3DPOOL_MANAGED;
    pDesc->Size = m_mipLevels[Level].data.size();
    pDesc->MultiSampleType = D3DMULTISAMPLE_NONE;
    pDesc->Width = m_mipLevels[Level].width;
    pDesc->Height = m_mipLevels[Level].height;
    return D3D_OK;
}

HRESULT STDMETHODCALLTYPE GLESTexture8::GetSurfaceLevel(UINT Level, IDirect3DSurface8** ppSurfaceLevel) {
    if (Level >= m_levels) return D3DERR_INVALIDCALL;
    // Create a proxy surface or return a surface that points to this texture data?
    // D3D8 expects a separate ref-counted object. 
    // Implementing this correctly requires a Surface class that wraps the texture level.
    // For now, simpler: Create a new independent surface and hope the game doesn't expect modifications to reflect back immediately 
    // OR (Better) Make GLESSurface8 able to wrap a memory pointer.
    
    // Stub: create a new surface with same dims.
    *ppSurfaceLevel = new GLESSurface8(m_device, m_mipLevels[Level].width, m_mipLevels[Level].height, m_format);
    return D3D_OK;
}

HRESULT STDMETHODCALLTYPE GLESTexture8::LockRect(UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags) {
    if (Level >= m_levels) return D3DERR_INVALIDCALL;
    
    if (pRect) {
        // Offset
        unsigned char* ptr = m_mipLevels[Level].data.data();
        int stride = m_mipLevels[Level].width * 4; 
        ptr += pRect->top * stride + pRect->left * 4;
        pLockedRect->pBits = ptr;
        pLockedRect->Pitch = stride;
    } else {
        pLockedRect->pBits = m_mipLevels[Level].data.data();
        pLockedRect->Pitch = m_mipLevels[Level].width * 4;
    }
    return D3D_OK;
}

HRESULT STDMETHODCALLTYPE GLESTexture8::UnlockRect(UINT Level) {
    if (Level >= m_levels) return D3DERR_INVALIDCALL;

    glBindTexture(GL_TEXTURE_2D, m_textureID);
    // Assume RGBA for now
    // D3D8 uses BGRA usually?
    // Generals uses A8R8G8B8 (BGRA in memory on little endian)
    // GL_BGRA_EXT might handle it.
    // Standard GL ES 3.0 supports GL_RGBA.
    // Check m_format. D3DFMT_A8R8G8B8?
    // For now use GL_RGBA and bytes.
    
    // Upload
    glTexImage2D(GL_TEXTURE_2D, Level, GL_RGBA, m_mipLevels[Level].width, m_mipLevels[Level].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_mipLevels[Level].data.data());
    
    if (m_levels > 1) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    return D3D_OK;
}

HRESULT STDMETHODCALLTYPE GLESTexture8::AddDirtyRect(CONST RECT* pDirtyRect) {
    return D3D_OK;
}
