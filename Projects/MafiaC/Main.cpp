
#include "pch.h"

// Ensure the lua copyright stays intact
#pragma comment(linker, "/include:_lua_ident")

HACKEVENTRESULT HackMain(uint32_t Event, tHackEventData* pData)
{
	switch (Event)
	{
		case HACKEVENT_LOAD:
			{
				return HACKEVENTRESULT_NORMAL;
			}
			break;
		default:
			break;
	}
	return HACKEVENTRESULT_UNREQUESTEVENT;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
			TriggerHackEvent(HACKEVENT_THREAD_ATTACH, nullptr);
			break;
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			TriggerHackEvent(HACKEVENT_THREAD_DETACH, nullptr);
			break;
	}
	return TRUE;
}
