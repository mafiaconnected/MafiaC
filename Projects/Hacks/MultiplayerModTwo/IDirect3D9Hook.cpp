
#include "stdafx.h"
#include "IDirect3D9Hook.h"
#include "CHookedDirect3D9Util.h"

decltype(Direct3DCreate9)* g_pDirect3DCreate9;

IDirect3D9* WINAPI HookDirect3DCreate9(UINT SDKVersion)
{
	return new IDirect3D9Hook(g_pDirect3DCreate9(SDKVersion));
}

IDirect3D9Hook::IDirect3D9Hook(IDirect3D9* pDirect3D9)
{
	m_pDirect3D9 = pDirect3D9;
}

HRESULT IDirect3D9Hook::QueryInterface(REFIID riid, void** ppvObj)
{
	return m_pDirect3D9->QueryInterface(riid,ppvObj);
}

ULONG IDirect3D9Hook::AddRef()
{
	//m_pDirect3D9->AddRef();
	m_RefCount++;
	return m_RefCount;
}

ULONG IDirect3D9Hook::Release()
{
	if (m_RefCount <= 1)
	{
		m_pDirect3D9->Release();
		delete this;
		return 0;
	}
	m_RefCount--;
	return m_RefCount;
}

HRESULT IDirect3D9Hook::RegisterSoftwareDevice(void* pInitializeFunction)
{
	return m_pDirect3D9->RegisterSoftwareDevice(pInitializeFunction);
}

UINT IDirect3D9Hook::GetAdapterCount()
{
	return m_pDirect3D9->GetAdapterCount();
}

HRESULT IDirect3D9Hook::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier)
{
	return m_pDirect3D9->GetAdapterIdentifier(Adapter,Flags,pIdentifier);
}

UINT IDirect3D9Hook::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format)
{
	//if (Adapter >= GetAdapterCount())
	//	return 0;
	return m_pDirect3D9->GetAdapterModeCount(Adapter,Format);
}

HRESULT IDirect3D9Hook::EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode)
{
	//if (Adapter >= GetAdapterCount())
	//	return D3DERR_INVALIDCALL;
	//if (Mode >= GetAdapterModeCount(Adapter,Format))
	//	return D3DERR_INVALIDCALL;
	return m_pDirect3D9->EnumAdapterModes(Adapter,Format,Mode,pMode);
}

HRESULT IDirect3D9Hook::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode)
{
	return m_pDirect3D9->GetAdapterDisplayMode(Adapter,pMode);
}

HRESULT IDirect3D9Hook::CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed)
{
	return m_pDirect3D9->CheckDeviceType(Adapter,DevType,AdapterFormat,BackBufferFormat,bWindowed);
}

HRESULT IDirect3D9Hook::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
{
	return m_pDirect3D9->CheckDeviceFormat(Adapter,DeviceType,AdapterFormat,Usage,RType,CheckFormat);
}

HRESULT IDirect3D9Hook::CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels)
{
	return m_pDirect3D9->CheckDeviceMultiSampleType(Adapter,DeviceType,SurfaceFormat,Windowed,MultiSampleType,pQualityLevels);
}

HRESULT IDirect3D9Hook::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
{
	return m_pDirect3D9->CheckDepthStencilMatch(Adapter,DeviceType,AdapterFormat,RenderTargetFormat,DepthStencilFormat);
}

HRESULT IDirect3D9Hook::CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat)
{
	return m_pDirect3D9->CheckDeviceFormatConversion(Adapter,DeviceType,SourceFormat,TargetFormat);
}

HRESULT IDirect3D9Hook::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps)
{
	return m_pDirect3D9->GetDeviceCaps(Adapter,DeviceType,pCaps);
}

HMONITOR IDirect3D9Hook::GetAdapterMonitor(UINT Adapter)
{
	return m_pDirect3D9->GetAdapterMonitor(Adapter);
}

HRESULT IDirect3D9Hook::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
{
	CHookedDirect3D9Util::OnPreResetDevice(nullptr,this,true,pPresentationParameters);
	IDirect3DDevice9* pD3DDevice;
	HRESULT hrResult = m_pDirect3D9->CreateDevice(Adapter,DeviceType,hFocusWindow,BehaviorFlags,pPresentationParameters,&pD3DDevice);
	if (SUCCEEDED(hrResult))
	{
		CHookedDirect3D9Util::OnPostResetDevice(pD3DDevice,this,true,pPresentationParameters);
		*ppReturnedDeviceInterface = new IDirect3DDevice9Hook(this,pD3DDevice);
		//*ppReturnedDeviceInterface = pD3DDevice;
	}
	return hrResult;
}
