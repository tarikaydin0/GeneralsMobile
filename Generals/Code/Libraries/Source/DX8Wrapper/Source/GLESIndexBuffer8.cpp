#include "GLESIndexBuffer8.h"

GLESIndexBuffer8::GLESIndexBuffer8(IDirect3DDevice8* device, UINT length, DWORD usage, D3DFORMAT format, D3DPOOL pool)
    : m_device(device), m_length(length), m_usage(usage), m_format(format), m_pool(pool), m_refCount(1)
{
    device->AddRef();
    m_data = new unsigned char[length];
}

GLESIndexBuffer8::~GLESIndexBuffer8() {
    delete[] (unsigned char*)m_data;
    if (m_device) m_device->Release();
}

HRESULT STDMETHODCALLTYPE GLESIndexBuffer8::QueryInterface(REFIID riid, void** ppvObj) { *ppvObj = nullptr; return E_NOINTERFACE; }
ULONG STDMETHODCALLTYPE GLESIndexBuffer8::AddRef() { return ++m_refCount; }
ULONG STDMETHODCALLTYPE GLESIndexBuffer8::Release() { if (--m_refCount == 0) { delete this; return 0; } return m_refCount; }
HRESULT STDMETHODCALLTYPE GLESIndexBuffer8::GetDevice(IDirect3DDevice8** ppDevice) { *ppDevice = m_device; m_device->AddRef(); return D3D_OK; }
HRESULT STDMETHODCALLTYPE GLESIndexBuffer8::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE GLESIndexBuffer8::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE GLESIndexBuffer8::FreePrivateData(REFGUID refguid) { return D3D_OK; }
DWORD STDMETHODCALLTYPE GLESIndexBuffer8::SetPriority(DWORD PriorityNew) { return 0; }
DWORD STDMETHODCALLTYPE GLESIndexBuffer8::GetPriority() { return 0; }
void STDMETHODCALLTYPE GLESIndexBuffer8::PreLoad() {}
D3DRESOURCETYPE STDMETHODCALLTYPE GLESIndexBuffer8::GetType() { return D3DRTYPE_INDEXBUFFER; }

HRESULT STDMETHODCALLTYPE GLESIndexBuffer8::Lock(UINT OffsetToLock, UINT SizeToLock, BYTE** ppbData, DWORD Flags) {
    if (SizeToLock == 0) SizeToLock = m_length;
    *ppbData = (BYTE*)m_data + OffsetToLock;
    return D3D_OK;
}

HRESULT STDMETHODCALLTYPE GLESIndexBuffer8::Unlock() {
    // Upload to IBO
    return D3D_OK;
}

HRESULT STDMETHODCALLTYPE GLESIndexBuffer8::GetDesc(D3DINDEXBUFFER_DESC *pDesc) {
    pDesc->Format = m_format;
    pDesc->Type = D3DRTYPE_INDEXBUFFER;
    pDesc->Usage = m_usage;
    pDesc->Pool = m_pool;
    pDesc->Size = m_length;
    return D3D_OK;
}
