#pragma once
#include "d3d8.h"

class GLESSurface8 : public IDirect3DSurface8 {
public:
    GLESSurface8(IDirect3DDevice8* device, UINT width, UINT height, D3DFORMAT format);
    virtual ~GLESSurface8();

    // IUnknown
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj) override;
    ULONG STDMETHODCALLTYPE AddRef() override;
    ULONG STDMETHODCALLTYPE Release() override;

    // IDirect3DSurface8
    HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice8** ppDevice) override;
    HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags) override;
    HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData) override;
    HRESULT STDMETHODCALLTYPE FreePrivateData(REFGUID refguid) override;
    HRESULT STDMETHODCALLTYPE GetContainer(REFIID riid, void** ppContainer) override;
    HRESULT STDMETHODCALLTYPE GetDesc(D3DSURFACE_DESC *pDesc) override;
    HRESULT STDMETHODCALLTYPE LockRect(D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags) override;
    HRESULT STDMETHODCALLTYPE UnlockRect() override;

private:
    IDirect3DDevice8* m_device;
    D3DSURFACE_DESC m_desc;
    ULONG m_refCount;
    void* m_data; // Software buffer for locking
};
