#pragma once

#include <HackSupport.h>

enum eMafiaHackEvent
{
	//HACKEVENT_LOADSETTINGS = HACKEVENT_CUSTOM,

	HACKEVENT_OND3D8DEVICESET = HACKEVENT_CUSTOM,
	HACKEVENT_OND3D8DEVICELOST,
	HACKEVENT_OND3D8DEVICERESET,

	HACKEVENT_OND3D9DEVICESET,
	HACKEVENT_OND3D9DEVICELOST,
	HACKEVENT_OND3D9DEVICERESET,

	HACKEVENT_REGISTERFUNCTIONS,
	HACKEVENT_ADDCUSTOMFILE,
};

struct tHackEventDataRegisterFunctions : public tHackEventData
{
	Galactic3D::Interfaces::IScripting* m_pScripting;
};

struct tHackEventDataD3D8 : public tHackEventData
{
	struct IDirect3DDevice8* m_pD3DDevice;
};

struct tHackEventDataD3D9 : public tHackEventData
{
	struct IDirect3DDevice9* m_pD3DDevice;
};

struct tHackEventDataCustomFile : public tHackEventData
{
	const char* pszFilePath;
	const char* pszGameFilePath;
};
