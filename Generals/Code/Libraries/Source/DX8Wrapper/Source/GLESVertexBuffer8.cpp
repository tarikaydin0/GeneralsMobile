#include "GLESVertexBuffer8.h"

GLESVertexBuffer8::GLESVertexBuffer8(IDirect3DDevice8* device, UINT length, DWORD usage, DWORD fvf, D3DPOOL pool) 
    : m_device(device), m_length(length), m_usage(usage), m_fvf(fvf), m_pool(pool), m_refCount(1)
{
    device->AddRef();
    m_data = new unsigned char[length];
}

GLESVertexBuffer8::~GLESVertexBuffer8() {
    delete[] (unsigned char*)m_data;
    if (m_device) m_device->Release();
}

HRESULT STDMETHODCALLTYPE GLESVertexBuffer8::QueryInterface(REFIID riid, void** ppvObj) { *ppvObj = nullptr; return E_NOINTERFACE; }
ULONG STDMETHODCALLTYPE GLESVertexBuffer8::AddRef() { return ++m_refCount; }
ULONG STDMETHODCALLTYPE GLESVertexBuffer8::Release() { if (--m_refCount == 0) { delete this; return 0; } return m_refCount; }
HRESULT STDMETHODCALLTYPE GLESVertexBuffer8::GetDevice(IDirect3DDevice8** ppDevice) { *ppDevice = m_device; m_device->AddRef(); return D3D_OK; }
HRESULT STDMETHODCALLTYPE GLESVertexBuffer8::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE GLESVertexBuffer8::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData) { return D3D_OK; }
HRESULT STDMETHODCALLTYPE GLESVertexBuffer8::FreePrivateData(REFGUID refguid) { return D3D_OK; }
DWORD STDMETHODCALLTYPE GLESVertexBuffer8::SetPriority(DWORD PriorityNew) { return 0; }
DWORD STDMETHODCALLTYPE GLESVertexBuffer8::GetPriority() { return 0; }
void STDMETHODCALLTYPE GLESVertexBuffer8::PreLoad() {}
D3DRESOURCETYPE STDMETHODCALLTYPE GLESVertexBuffer8::GetType() { return D3DRTYPE_VERTEXBUFFER; }

HRESULT STDMETHODCALLTYPE GLESVertexBuffer8::Lock(UINT OffsetToLock, UINT SizeToLock, BYTE** ppbData, DWORD Flags) {
    if (SizeToLock == 0) SizeToLock = m_length; // Lock all
    *ppbData = (BYTE*)m_data + OffsetToLock;
    return D3D_OK;
}

HRESULT STDMETHODCALLTYPE GLESVertexBuffer8::Unlock() {
    // Upload to VBO
    return D3D_OK;
}

HRESULT STDMETHODCALLTYPE GLESVertexBuffer8::GetDesc(D3DVERTEXBUFFER_DESC *pDesc) {
    pDesc->Format = D3DFMT_VERTEXDATA;
    pDesc->Type = D3DRTYPE_VERTEXBUFFER;
    pDesc->Usage = m_usage;
    pDesc->Pool = m_pool;
    pDesc->Size = m_length;
    pDesc->FVF = m_fvf;
    return D3D_OK;
}
