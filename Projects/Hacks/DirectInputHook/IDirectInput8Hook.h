#pragma once
#define DIRECTINPUT_VERSION 0x0800
#undef UNICODE
#include <dinput.h>

class IDirectInput8Hook : public IDirectInput8
{
public:
	IDirectInput8Hook(IDirectInput8* pDirectInput);
	~IDirectInput8Hook();

	size_t m_RefCount;
	IDirectInput8* m_pDirectInput;

	virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObj) override;
	virtual ULONG __stdcall AddRef() override;
	virtual ULONG __stdcall Release() override;

	virtual HRESULT __stdcall CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE8* lplpDirectInputDevice, LPUNKNOWN pUnkOuter) override;
	virtual HRESULT __stdcall EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACK lpCallback, LPVOID pvRef, DWORD dwFlags) override;
	virtual HRESULT __stdcall GetDeviceStatus(REFGUID rguidInstance) override;
	virtual HRESULT __stdcall RunControlPanel(HWND hwndOwner, DWORD dwFlags) override;
	virtual HRESULT __stdcall Initialize(HINSTANCE hinst, DWORD dwVersion) override;
	virtual HRESULT __stdcall FindDevice(REFGUID rguidClass, LPCSTR ptszName, LPGUID pguidInstance) override;
	virtual HRESULT __stdcall EnumDevicesBySemantics(LPCSTR ptszUserName, LPDIACTIONFORMAT lpdiActionFormat, LPDIENUMDEVICESBYSEMANTICSCB lpCallback, LPVOID pvRef, DWORD dwFlags) override;
	virtual HRESULT __stdcall ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK lpdiCallback, LPDICONFIGUREDEVICESPARAMS lpdiCDParams, DWORD dwFlags, LPVOID pvRefData) override;
};
