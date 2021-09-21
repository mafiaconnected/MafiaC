
#include "pch.h"
#include "IDirectInputDevice8Hook.h"

extern bool g_bRemovingInstances;
extern std::unordered_set<IUnknown*> g_Instances;

IDirectInputDevice8Hook::IDirectInputDevice8Hook(IDirectInputDevice8* pDirectInputDevice)
{
	g_Instances.insert(this);

	m_pDirectInputDevice = pDirectInputDevice;
	m_RefCount = 1;
}

IDirectInputDevice8Hook::~IDirectInputDevice8Hook()
{
	if (!g_bRemovingInstances)
		g_Instances.erase(this);
}

HRESULT IDirectInputDevice8Hook::QueryInterface(REFIID riid, void** ppvObj)
{
	if (riid == IID_IDirectInputDevice8W)
	{
		*ppvObj = this;
		AddRef();
		return S_OK;
	}
	return m_pDirectInputDevice->QueryInterface(riid, ppvObj);
}

ULONG IDirectInputDevice8Hook::AddRef()
{
	m_RefCount++;
	return m_RefCount;
}

ULONG IDirectInputDevice8Hook::Release()
{
	if (m_RefCount <= 1)
	{
		m_pDirectInputDevice->Release();
		delete this;
		return 0;
	}
	m_RefCount--;
	return m_RefCount;
}

HRESULT __stdcall IDirectInputDevice8Hook::GetCapabilities(LPDIDEVCAPS a1)
{
	return m_pDirectInputDevice->GetCapabilities(a1);
}

HRESULT __stdcall IDirectInputDevice8Hook::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACK a1, LPVOID a2, DWORD a3)
{
	return m_pDirectInputDevice->EnumObjects(a1, a2, a3);
}

HRESULT __stdcall IDirectInputDevice8Hook::GetProperty(REFGUID a1, LPDIPROPHEADER a2)
{
	return m_pDirectInputDevice->GetProperty(a1, a2);
}

HRESULT __stdcall IDirectInputDevice8Hook::SetProperty(REFGUID a1, LPCDIPROPHEADER a2)
{
	return m_pDirectInputDevice->SetProperty(a1, a2);
}

HRESULT __stdcall IDirectInputDevice8Hook::Acquire()
{
	return m_pDirectInputDevice->Acquire();
}

HRESULT __stdcall IDirectInputDevice8Hook::Unacquire()
{
	return m_pDirectInputDevice->Unacquire();
}

HRESULT __stdcall IDirectInputDevice8Hook::GetDeviceState(DWORD a1, LPVOID a2)
{
	return m_pDirectInputDevice->GetDeviceState(a1, a2);
}

HRESULT __stdcall IDirectInputDevice8Hook::GetDeviceData(DWORD a1, LPDIDEVICEOBJECTDATA a2, LPDWORD a3, DWORD a4)
{
	return m_pDirectInputDevice->GetDeviceData(a1, a2, a3, a4);
}

HRESULT __stdcall IDirectInputDevice8Hook::SetDataFormat(LPCDIDATAFORMAT a1)
{
	return m_pDirectInputDevice->SetDataFormat(a1);
}

HRESULT __stdcall IDirectInputDevice8Hook::SetEventNotification(HANDLE a1)
{
	return m_pDirectInputDevice->SetEventNotification(a1);
}

HRESULT __stdcall IDirectInputDevice8Hook::SetCooperativeLevel(HWND a1, DWORD a2)
{
	return m_pDirectInputDevice->SetCooperativeLevel(a1, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}

HRESULT __stdcall IDirectInputDevice8Hook::GetObjectInfo(LPDIDEVICEOBJECTINSTANCE a1, DWORD a2, DWORD a3)
{
	return m_pDirectInputDevice->GetObjectInfo(a1, a2, a3);
}

HRESULT __stdcall IDirectInputDevice8Hook::GetDeviceInfo(LPDIDEVICEINSTANCE a1)
{
	return m_pDirectInputDevice->GetDeviceInfo(a1);
}

HRESULT __stdcall IDirectInputDevice8Hook::RunControlPanel(HWND a1, DWORD a2)
{
	return m_pDirectInputDevice->RunControlPanel(a1, a2);
}

HRESULT __stdcall IDirectInputDevice8Hook::Initialize(HINSTANCE a1, DWORD a2, REFGUID a3)
{
	return m_pDirectInputDevice->Initialize(a1, a2, a3);
}

HRESULT __stdcall IDirectInputDevice8Hook::CreateEffect(REFGUID a1, LPCDIEFFECT a2, LPDIRECTINPUTEFFECT* a3, LPUNKNOWN a4)
{
	return m_pDirectInputDevice->CreateEffect(a1, a2, a3, a4);
}

HRESULT __stdcall IDirectInputDevice8Hook::EnumEffects(LPDIENUMEFFECTSCALLBACK a1, LPVOID a2, DWORD a3)
{
	return m_pDirectInputDevice->EnumEffects(a1, a2, a3);
}

HRESULT __stdcall IDirectInputDevice8Hook::GetEffectInfo(LPDIEFFECTINFO a1, REFGUID a2)
{
	return m_pDirectInputDevice->GetEffectInfo(a1, a2);
}

HRESULT __stdcall IDirectInputDevice8Hook::GetForceFeedbackState(LPDWORD a1)
{
	return m_pDirectInputDevice->GetForceFeedbackState(a1);
}

HRESULT __stdcall IDirectInputDevice8Hook::SendForceFeedbackCommand(DWORD a1)
{
	return m_pDirectInputDevice->SendForceFeedbackCommand(a1);
}

HRESULT __stdcall IDirectInputDevice8Hook::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK a1, LPVOID a2, DWORD a3)
{
	return m_pDirectInputDevice->EnumCreatedEffectObjects(a1, a2, a3);
}

HRESULT __stdcall IDirectInputDevice8Hook::Escape(LPDIEFFESCAPE a1)
{
	return m_pDirectInputDevice->Escape(a1);
}

HRESULT __stdcall IDirectInputDevice8Hook::Poll()
{
	return m_pDirectInputDevice->Poll();
}

HRESULT __stdcall IDirectInputDevice8Hook::SendDeviceData(DWORD a1, LPCDIDEVICEOBJECTDATA a2, LPDWORD a3, DWORD a4)
{
	return m_pDirectInputDevice->SendDeviceData(a1, a2, a3, a4);
}

HRESULT __stdcall IDirectInputDevice8Hook::EnumEffectsInFile(LPCSTR a1, LPDIENUMEFFECTSINFILECALLBACK a2, LPVOID a3, DWORD a4)
{
	return m_pDirectInputDevice->EnumEffectsInFile(a1, a2, a3, a4);
}

HRESULT __stdcall IDirectInputDevice8Hook::WriteEffectToFile(LPCSTR a1, DWORD a2, LPDIFILEEFFECT a3, DWORD a4)
{
	return m_pDirectInputDevice->WriteEffectToFile(a1, a2, a3, a4);
}

HRESULT __stdcall IDirectInputDevice8Hook::BuildActionMap(LPDIACTIONFORMAT a1, LPCSTR a2, DWORD a3)
{
	return m_pDirectInputDevice->BuildActionMap(a1, a2, a3);
}

HRESULT __stdcall IDirectInputDevice8Hook::SetActionMap(LPDIACTIONFORMAT a1, LPCSTR a2, DWORD a3)
{
	return m_pDirectInputDevice->SetActionMap(a1, a2, a3);
}

HRESULT __stdcall IDirectInputDevice8Hook::GetImageInfo(LPDIDEVICEIMAGEINFOHEADER a1)
{
	return m_pDirectInputDevice->GetImageInfo(a1);
}
