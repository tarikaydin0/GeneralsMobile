#pragma once
#include "d3d8.h"

class GLESVertexBuffer8 : public IDirect3DVertexBuffer8 {
public:
    GLESVertexBuffer8(IDirect3DDevice8* device, UINT length, DWORD usage, DWORD fvf, D3DPOOL pool);
    virtual ~GLESVertexBuffer8();

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
    
    void* GetData() const { return m_data; }

    // IDirect3DVertexBuffer8
    HRESULT STDMETHODCALLTYPE Lock(UINT OffsetToLock, UINT SizeToLock, BYTE** ppbData, DWORD Flags) override;
    HRESULT STDMETHODCALLTYPE Unlock() override;
    HRESULT STDMETHODCALLTYPE GetDesc(D3DVERTEXBUFFER_DESC *pDesc) override;

private:
    IDirect3DDevice8* m_device;
    ULONG m_refCount;
    UINT m_length;
    DWORD m_usage;
    DWORD m_fvf;
    D3DPOOL m_pool;
    void* m_data; // Software buffer
    // GLuint m_vbo; // TODO: Implement VBO
};
