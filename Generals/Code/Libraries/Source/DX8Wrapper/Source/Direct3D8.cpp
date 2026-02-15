#include "Direct3DDevice8.h"
#include <android/log.h>

#define LOG_TAG "DX8Wrapper"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Helper macros for IUnknown implementation
#define IMPLEMENT_IUNKNOWN \
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObj) override { \
        *ppvObj = nullptr; \
        return E_NOINTERFACE; \
    } \
    ULONG STDMETHODCALLTYPE AddRef() override { return 1; } \
    ULONG STDMETHODCALLTYPE Release() override { return 1; }

class DX8Wrapper_Direct3D8 : public IDirect3D8 {
public:
    IMPLEMENT_IUNKNOWN

    HRESULT STDMETHODCALLTYPE RegisterSoftwareDevice(void* pInitializeFunction) override {
        return D3D_OK;
    }

    UINT STDMETHODCALLTYPE GetAdapterCount() override {
        return 1;
    }

    HRESULT STDMETHODCALLTYPE GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER8* pIdentifier) override {
        // if (pIdentifier) ...
        return D3D_OK;
    }

    UINT STDMETHODCALLTYPE GetAdapterModeCount(UINT Adapter) override {
        return 1;
    }

    HRESULT STDMETHODCALLTYPE EnumAdapterModes(UINT Adapter, UINT Mode, D3DDISPLAYMODE* pMode) override {
        if (pMode) {
            pMode->Width = 1920;
            pMode->Height = 1080;
            pMode->RefreshRate = 60;
            pMode->Format = D3DFMT_X8R8G8B8;
        }
        return D3D_OK;
    }

    HRESULT STDMETHODCALLTYPE GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode) override {
        return EnumAdapterModes(Adapter, 0, pMode);
    }

    HRESULT STDMETHODCALLTYPE CheckDeviceType(UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed) override {
        return D3D_OK;
    }

    HRESULT STDMETHODCALLTYPE CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat) override {
        return D3D_OK;
    }

    HRESULT STDMETHODCALLTYPE CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType) override {
        return D3D_OK; 
    }

    HRESULT STDMETHODCALLTYPE CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) override {
        return D3D_OK;
    }

    HRESULT STDMETHODCALLTYPE GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS8* pCaps) override {
        // We could delegate to device if we had a persistent one, or just return basic stuff
        return D3D_OK;
    }

    HMONITOR STDMETHODCALLTYPE GetAdapterMonitor(UINT Adapter) override {
        return (HMONITOR)0x1234; // Dummy
    }

    HRESULT STDMETHODCALLTYPE CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice8** ppReturnedDeviceInterface) override {
        LOGI("CreateDevice called");
        *ppReturnedDeviceInterface = new DX8Wrapper_Direct3DDevice8(hFocusWindow); 
        return D3D_OK; 
    }
};

extern "C" IDirect3D8* WINAPI Direct3DCreate8(UINT SDKVersion) {
    LOGI("Direct3DCreate8 called with version %u", SDKVersion);
    return new DX8Wrapper_Direct3D8();
}
