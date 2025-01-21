#pragma once

#include <d3d8.h>

class IDirect3D8Hook : public IDirect3D8
{
private:
	IDirect3D8* m_pD3D;
	int iRefs = 1;
public:
	IDirect3D8Hook(IDirect3D8 * pDevice);
	~IDirect3D8Hook();

    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObj);
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();

    virtual HRESULT __stdcall RegisterSoftwareDevice(void* pInitializeFunction);
    virtual UINT __stdcall GetAdapterCount();
    virtual HRESULT __stdcall GetAdapterIdentifier(UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER8* pIdentifier);
    virtual UINT __stdcall GetAdapterModeCount(UINT Adapter);
    virtual HRESULT __stdcall EnumAdapterModes(UINT Adapter,UINT Mode,D3DDISPLAYMODE* pMode);
    virtual HRESULT __stdcall GetAdapterDisplayMode(UINT Adapter,D3DDISPLAYMODE* pMode);
    virtual HRESULT __stdcall CheckDeviceType(UINT Adapter,D3DDEVTYPE CheckType,D3DFORMAT DisplayFormat,D3DFORMAT BackBufferFormat,BOOL Windowed);
    virtual HRESULT __stdcall CheckDeviceFormat(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat);
    virtual HRESULT __stdcall CheckDeviceMultiSampleType(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType);
    virtual HRESULT __stdcall CheckDepthStencilMatch(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat);
    virtual HRESULT __stdcall GetDeviceCaps(UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS8* pCaps);
    virtual HMONITOR __stdcall GetAdapterMonitor(UINT Adapter);
    virtual HRESULT __stdcall CreateDevice(UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice8** ppReturnedDeviceInterface);
};
