
#ifndef __CHOOKEDDIRECT3D9UTIL__H__
#define __CHOOKEDDIRECT3D9UTIL__H__

#include <d3d9.h>

class CHookedDirect3D9Util
{
public:
	static bool m_bSimulateDeviceLost;
	static bool m_bSimulateDeviceNotReset;

	static void OnPreCreateDevice(IDirect3DDevice9* pD3DDevice, IDirect3D9* pHookedDirect3D8, UINT* pAdapter, D3DDEVTYPE* pDeviceType, HWND* phFocusWindow, DWORD* pBehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters);
	static void OnPostCreateDevice(IDirect3DDevice9* pD3DDevice, IDirect3D9* pHookedDirect3D8, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, const D3DPRESENT_PARAMETERS* pPresentationParameters);
	static void OnPreResetDevice(IDirect3DDevice9* pD3DDevice, IDirect3D9* pHookedDirect3D8, bool bCreate, D3DPRESENT_PARAMETERS* pPresentationParameters);
	static void OnPostResetDevice(IDirect3DDevice9* pD3DDevice, IDirect3D9* pHookedDirect3D8, bool bCreate, const D3DPRESENT_PARAMETERS* pPresentationParameters);
	static void OnPreLostDevice(IDirect3DDevice9* pD3DDevice, IDirect3D9* pHookedDirect3D8, bool bDelete);
	static void OnPostLostDevice(IDirect3DDevice9* pD3DDevice, IDirect3D9* pHookedDirect3D8, bool bDelete);

	static void ForceDeviceReset(void);
};

#endif
