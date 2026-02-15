#pragma once
#include "d3d8.h"
#include <GLES3/gl3.h>
#include <vector>

class GLESTexture8 : public IDirect3DTexture8 {
public:
    GLESTexture8(IDirect3DDevice8* device, UINT width, UINT height, UINT levels, DWORD usage, D3DFORMAT format, D3DPOOL pool);
    virtual ~GLESTexture8();

    // IUnknown
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj) override;
    ULONG STDMETHODCALLTYPE AddRef() override;
    ULONG STDMETHODCALLTYPE Release() override;

    // IDirect3DResource8
    HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice8** ppDevice) override;
    HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags) override;
    HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData) override;
    HRESULT STDMETHODCALLTYPE FreePrivateData(REFGUID refguid) override;
    DWORD STDMETHODCALLTYPE SetPriority(DWORD PriorityNew) override;
    DWORD STDMETHODCALLTYPE GetPriority() override;
    void STDMETHODCALLTYPE PreLoad() override;
    D3DRESOURCETYPE STDMETHODCALLTYPE GetType() override;

    // IDirect3DBaseTexture8
    DWORD STDMETHODCALLTYPE SetLOD(DWORD LODNew) override;
    DWORD STDMETHODCALLTYPE GetLOD() override;
    DWORD STDMETHODCALLTYPE GetLevelCount() override;

    // IDirect3DTexture8
    HRESULT STDMETHODCALLTYPE GetLevelDesc(UINT Level, D3DSURFACE_DESC *pDesc) override;
    HRESULT STDMETHODCALLTYPE GetSurfaceLevel(UINT Level, IDirect3DSurface8** ppSurfaceLevel) override;
    HRESULT STDMETHODCALLTYPE LockRect(UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags) override;
    HRESULT STDMETHODCALLTYPE UnlockRect(UINT Level) override;
    HRESULT STDMETHODCALLTYPE AddDirtyRect(CONST RECT* pDirtyRect) override;

    // Internal
    GLuint GetGLTextureID() const { return m_textureID; }

private:
    IDirect3DDevice8* m_device;
    ULONG m_refCount;
    UINT m_width;
    UINT m_height;
    UINT m_levels;
    D3DFORMAT m_format;
    GLuint m_textureID;
    
    struct MipLevel {
        std::vector<unsigned char> data;
        UINT width;
        UINT height;
    };
    std::vector<MipLevel> m_mipLevels;
};
