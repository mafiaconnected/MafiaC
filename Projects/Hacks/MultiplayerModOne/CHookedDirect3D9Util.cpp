
#include "stdafx.h"
#include "CHookedDirect3D9Util.h"
#include "Renderer.h"

bool CHookedDirect3D9Util::m_bSimulateDeviceLost = false;
bool CHookedDirect3D9Util::m_bSimulateDeviceNotReset = true;

extern Direct3D9* g_pD3D9;

void CHookedDirect3D9Util::OnPreCreateDevice(IDirect3DDevice9* pD3DDevice, IDirect3D9* pHookedDirect3D8, UINT* pAdapter, D3DDEVTYPE* pDeviceType, HWND* phFocusWindow, DWORD* pBehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	OnPreResetDevice(pD3DDevice, pHookedDirect3D8, true, pPresentationParameters);
}

void CHookedDirect3D9Util::OnPostCreateDevice(IDirect3DDevice9* pD3DDevice, IDirect3D9* pHookedDirect3D8, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, const D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	OnPostResetDevice(pD3DDevice, pHookedDirect3D8, true, pPresentationParameters);
}

void CHookedDirect3D9Util::OnPreResetDevice(IDirect3DDevice9* pD3DDevice, IDirect3D9* pHookedDirect3D8, bool bCreate, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
}

void CHookedDirect3D9Util::OnPostResetDevice(IDirect3DDevice9* pD3DDevice, IDirect3D9* pHookedDirect3D8, bool bCreate, const D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	CClientRenderer::OnDeviceReset(pD3DDevice);
	_glogprintf(__gstr(__FUNCTION__));
}

void CHookedDirect3D9Util::OnPreLostDevice(IDirect3DDevice9* pD3DDevice, IDirect3D9* pHookedDirect3D8, bool bDelete)
{

}

void CHookedDirect3D9Util::OnPostLostDevice(IDirect3DDevice9* pD3DDevice, IDirect3D9* pHookedDirect3D8, bool bDelete)
{
	if (g_pD3D9)
		g_pD3D9->OnDeviceLost();
}

void CHookedDirect3D9Util::ForceDeviceReset(void)
{
	m_bSimulateDeviceLost = true;
}
