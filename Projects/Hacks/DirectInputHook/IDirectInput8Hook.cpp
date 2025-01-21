
#include "pch.h"
#include "IDirectInput8Hook.h"
#include "IDirectInputDevice8Hook.h"

extern bool g_bRemovingInstances;
extern std::unordered_set<IUnknown*> g_Instances;

IDirectInput8Hook::IDirectInput8Hook(IDirectInput8* pDirectInput)
{
	g_Instances.insert(this);

	m_pDirectInput = pDirectInput;
}

IDirectInput8Hook::~IDirectInput8Hook()
{
	if (!g_bRemovingInstances)
		g_Instances.erase(this);
}

HRESULT IDirectInput8Hook::QueryInterface(REFIID riid, void** ppvObj)
{
	if (riid == IID_IDirectInput8A)
	{
		*ppvObj = this;
		AddRef();
		return S_OK;
	}
	return m_pDirectInput->QueryInterface(riid, ppvObj);
}

ULONG IDirectInput8Hook::AddRef()
{
	m_RefCount++;
	return m_RefCount;
}

ULONG IDirectInput8Hook::Release()
{
	if (m_RefCount <= 1)
	{
		m_pDirectInput->Release();
		delete this;
		return 0;
	}
	m_RefCount--;
	return m_RefCount;
}

HRESULT __stdcall IDirectInput8Hook::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE8* lplpDirectInputDevice, LPUNKNOWN pUnkOuter)
{
	LPDIRECTINPUTDEVICE8 lpDirectInputDevice;
	HRESULT hr = m_pDirectInput->CreateDevice(rguid, &lpDirectInputDevice, pUnkOuter);
	if (FAILED(hr))
		return hr;
	
	*lplpDirectInputDevice = new IDirectInputDevice8Hook(lpDirectInputDevice);
	return hr;
}

HRESULT __stdcall IDirectInput8Hook::EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACK lpCallback, LPVOID pvRef, DWORD dwFlags)
{
	return m_pDirectInput->EnumDevices(dwDevType, lpCallback, pvRef, dwFlags);
}

HRESULT __stdcall IDirectInput8Hook::GetDeviceStatus(REFGUID rguidInstance)
{
	return m_pDirectInput->GetDeviceStatus(rguidInstance);
}

HRESULT __stdcall IDirectInput8Hook::RunControlPanel(HWND hwndOwner, DWORD dwFlags)
{
	return m_pDirectInput->RunControlPanel(hwndOwner, dwFlags);
}

HRESULT __stdcall IDirectInput8Hook::Initialize(HINSTANCE hinst, DWORD dwVersion)
{
	return m_pDirectInput->Initialize(hinst, dwVersion);
}

HRESULT __stdcall IDirectInput8Hook::FindDevice(REFGUID rguidClass, LPCSTR ptszName, LPGUID pguidInstance)
{
	return m_pDirectInput->FindDevice(rguidClass, ptszName, pguidInstance);
}

HRESULT __stdcall IDirectInput8Hook::EnumDevicesBySemantics(LPCSTR ptszUserName, LPDIACTIONFORMAT lpdiActionFormat, LPDIENUMDEVICESBYSEMANTICSCB lpCallback, LPVOID pvRef, DWORD dwFlags)
{
	return m_pDirectInput->EnumDevicesBySemantics(ptszUserName, lpdiActionFormat, lpCallback, pvRef, dwFlags);
}

HRESULT __stdcall IDirectInput8Hook::ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK lpdiCallback, LPDICONFIGUREDEVICESPARAMS lpdiCDParams, DWORD dwFlags, LPVOID pvRefData)
{
	return m_pDirectInput->ConfigureDevices(lpdiCallback, lpdiCDParams, dwFlags, pvRefData);
}
