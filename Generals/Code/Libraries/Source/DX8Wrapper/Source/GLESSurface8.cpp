#include "GLESSurface8.h"
#include <android/log.h>
#include <string.h>

#define LOG_TAG "DX8Wrapper_Surface"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

GLESSurface8::GLESSurface8(IDirect3DDevice8* device, UINT width, UINT height, D3DFORMAT format) 
    : m_device(device), m_refCount(1), m_data(nullptr) 
{
    m_desc.Format = format;
    m_desc.Type = D3DRTYPE_SURFACE;
    m_desc.Usage = 0;
    m_desc.Pool = D3DPOOL_DEFAULT;
    m_desc.Size = 0; // todo
    m_desc.MultiSampleType = D3DMULTISAMPLE_NONE;
    m_desc.Width = width;
    m_desc.Height = height;

    // Allocate simplified buffer for basic locking support (e.g. for texture loading)
    // 4 bytes per pixel approximation for now
    m_data = new unsigned char[width * height * 4]; 
    memset(m_data, 0, width * height * 4);
}

GLESSurface8::~GLESSurface8() {
    delete[] (unsigned char*)m_data;
}

HRESULT STDMETHODCALLTYPE GLESSurface8::QueryInterface(REFIID riid, void** ppvObj) {
    *ppvObj = nullptr;
    return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE GLESSurface8::AddRef() {
    return ++m_refCount;
}

ULONG STDMETHODCALLTYPE GLESSurface8::Release() {
    if (--m_refCount == 0) {
        delete this;
        return 0;
    }
    return m_refCount;
}

HRESULT STDMETHODCALLTYPE GLESSurface8::GetDevice(IDirect3DDevice8** ppDevice) {
    *ppDevice = m_device;
    m_device->AddRef();
    return D3D_OK;
}

HRESULT STDMETHODCALLTYPE GLESSurface8::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE GLESSurface8::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE GLESSurface8::FreePrivateData(REFGUID refguid) { return D3D_OK; }

HRESULT STDMETHODCALLTYPE GLESSurface8::GetContainer(REFIID riid, void** ppContainer) {
    *ppContainer = nullptr;
    return E_NOINTERFACE; // Often textures are containers, but standalone surfaces not always
}

HRESULT STDMETHODCALLTYPE GLESSurface8::GetDesc(D3DSURFACE_DESC *pDesc) {
    *pDesc = m_desc;
    return D3D_OK;
}

HRESULT STDMETHODCALLTYPE GLESSurface8::LockRect(D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags) {
    // Simple locking support
    if (pRect) {
        // Calculate offset
        int bpp = 4; // Assume 32bit
        int stride = m_desc.Width * bpp;
        unsigned char* ptr = (unsigned char*)m_data;
        ptr += pRect->top * stride + pRect->left * bpp;
        pLockedRect->pBits = ptr;
        pLockedRect->Pitch = stride;
    } else {
        pLockedRect->pBits = m_data;
        pLockedRect->Pitch = m_desc.Width * 4;
    }
    return D3D_OK;
}

HRESULT STDMETHODCALLTYPE GLESSurface8::UnlockRect() {
    return D3D_OK;
}
