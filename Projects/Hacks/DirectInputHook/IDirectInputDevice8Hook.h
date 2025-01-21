#pragma once

#include "IDirectInput8Hook.h"

enum eDIDeviceType
{
	DIDEVICE_TYPE_UNKNOWN,
	DIDEVICE_TYPE_MOUSE,
	DIDEVICE_TYPE_KEYBOARD,
	DIDEVICE_TYPE_JOYSTICK
};

class IDirectInputDevice8Hook : public IDirectInputDevice8
{
public:
	IDirectInputDevice8Hook(IDirectInputDevice8* pDirectInputDevice);
	~IDirectInputDevice8Hook();

	eDIDeviceType m_DeviceType;

	size_t m_RefCount = 1;
	IDirectInputDevice8* m_pDirectInputDevice;

	virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObj) override;
	virtual ULONG __stdcall AddRef() override;
	virtual ULONG __stdcall Release() override;

	virtual HRESULT __stdcall GetCapabilities(LPDIDEVCAPS) override;
	virtual HRESULT __stdcall EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACK, LPVOID, DWORD) override;
	virtual HRESULT __stdcall GetProperty(REFGUID, LPDIPROPHEADER) override;
	virtual HRESULT __stdcall SetProperty(REFGUID, LPCDIPROPHEADER) override;
	virtual HRESULT __stdcall Acquire() override;
	virtual HRESULT __stdcall Unacquire() override;
	virtual HRESULT __stdcall GetDeviceState(DWORD, LPVOID) override;
	virtual HRESULT __stdcall GetDeviceData(DWORD, LPDIDEVICEOBJECTDATA, LPDWORD, DWORD) override;
	virtual HRESULT __stdcall SetDataFormat(LPCDIDATAFORMAT) override;
	virtual HRESULT __stdcall SetEventNotification(HANDLE) override;
	virtual HRESULT __stdcall SetCooperativeLevel(HWND, DWORD) override;
	virtual HRESULT __stdcall GetObjectInfo(LPDIDEVICEOBJECTINSTANCE, DWORD, DWORD) override;
	virtual HRESULT __stdcall GetDeviceInfo(LPDIDEVICEINSTANCE) override;
	virtual HRESULT __stdcall RunControlPanel(HWND, DWORD) override;
	virtual HRESULT __stdcall Initialize(HINSTANCE, DWORD, REFGUID) override;
	virtual HRESULT __stdcall CreateEffect(REFGUID, LPCDIEFFECT, LPDIRECTINPUTEFFECT*, LPUNKNOWN) override;
	virtual HRESULT __stdcall EnumEffects(LPDIENUMEFFECTSCALLBACK, LPVOID, DWORD) override;
	virtual HRESULT __stdcall GetEffectInfo(LPDIEFFECTINFO, REFGUID) override;
	virtual HRESULT __stdcall GetForceFeedbackState(LPDWORD) override;
	virtual HRESULT __stdcall SendForceFeedbackCommand(DWORD) override;
	virtual HRESULT __stdcall EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK, LPVOID, DWORD) override;
	virtual HRESULT __stdcall Escape(LPDIEFFESCAPE) override;
	virtual HRESULT __stdcall Poll() override;
	virtual HRESULT __stdcall SendDeviceData(DWORD, LPCDIDEVICEOBJECTDATA, LPDWORD, DWORD) override;
	virtual HRESULT __stdcall EnumEffectsInFile(LPCSTR, LPDIENUMEFFECTSINFILECALLBACK, LPVOID, DWORD) override;
	virtual HRESULT __stdcall WriteEffectToFile(LPCSTR, DWORD, LPDIFILEEFFECT, DWORD) override;
	virtual HRESULT __stdcall BuildActionMap(LPDIACTIONFORMAT, LPCSTR, DWORD) override;
	virtual HRESULT __stdcall SetActionMap(LPDIACTIONFORMAT, LPCSTR, DWORD) override;
	virtual HRESULT __stdcall GetImageInfo(LPDIDEVICEIMAGEINFOHEADER) override;
};
