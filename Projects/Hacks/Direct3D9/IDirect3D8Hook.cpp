
#include "stdafx.h"
#include "IDirect3D8Hook.h"
#include "IDirect3DDevice8Hook.h"

IDirect3D8Hook::IDirect3D8Hook(IDirect3D8* pD3D)
{
	m_pD3D = pD3D;
	//pD3D->AddRef();
	//iRefs = pD3D->Release();
}

IDirect3D8Hook::~IDirect3D8Hook()
{
}

HRESULT IDirect3D8Hook::QueryInterface(REFIID iid, void** ppvObject)
{
	return m_pD3D->QueryInterface(iid, ppvObject);
}

ULONG IDirect3D8Hook::AddRef()
{
	iRefs++;
	return m_pD3D->AddRef();
}

ULONG IDirect3D8Hook::Release()
{
	iRefs--;
	ULONG ulResult = m_pD3D->Release();
	if (iRefs <= 0)
		delete this;
	return ulResult;
}

HRESULT IDirect3D8Hook::RegisterSoftwareDevice(void*pInitializeFunction)
{
	return m_pD3D->RegisterSoftwareDevice(pInitializeFunction);
}

UINT IDirect3D8Hook::GetAdapterCount()
{
	return m_pD3D->GetAdapterCount();
}

HRESULT IDirect3D8Hook::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER8* pIdentifier)
{
	return m_pD3D->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
}

UINT IDirect3D8Hook::GetAdapterModeCount(UINT Adapter)
{
	return m_pD3D->GetAdapterModeCount(Adapter);
}

HRESULT IDirect3D8Hook::EnumAdapterModes(UINT Adapter, UINT Mode, D3DDISPLAYMODE* pMode)
{
	return m_pD3D->EnumAdapterModes(Adapter, Mode, pMode);
}

HRESULT IDirect3D8Hook::GetAdapterDisplayMode(UINT Adapter,D3DDISPLAYMODE*pMode)
{
	return m_pD3D->GetAdapterDisplayMode(Adapter, pMode);
}

HRESULT IDirect3D8Hook::CheckDeviceType(UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed)
{
	return m_pD3D->CheckDeviceType(Adapter, CheckType, DisplayFormat, BackBufferFormat, Windowed);
}

HRESULT IDirect3D8Hook::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
{
	return m_pD3D->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

HRESULT IDirect3D8Hook::CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType)
{
	return m_pD3D->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType);
}

HRESULT IDirect3D8Hook::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
{
	return m_pD3D->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

HRESULT IDirect3D8Hook::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS8* pCaps)
{
	return m_pD3D->GetDeviceCaps(Adapter, DeviceType, pCaps);
}

HMONITOR IDirect3D8Hook::GetAdapterMonitor(UINT Adapter)
{
	return m_pD3D->GetAdapterMonitor(Adapter);
}

HRESULT IDirect3D8Hook::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice8** ppReturnedDeviceInterface)
{
	IDirect3DDevice8* pDevice;
	HRESULT hr = m_pD3D->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, &pDevice);
	if(SUCCEEDED(hr))
		*ppReturnedDeviceInterface = new IDirect3DDevice8Hook(this,pDevice);
	return hr;
}
