
#include "pch.h"
#include <DynamicTLSFixup.h>

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
			// Before TriggerHackEvent - anything it runs needs working TLS (this module's own, and anything
			// statically linked into it, e.g. LibreSSL's per-thread error buffer) already set up.
			FixupDynamicTLS(hModule, ul_reason_for_call);
			TriggerHackEvent(HACKEVENT_THREAD_ATTACH, nullptr);
			break;
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			// After TriggerHackEvent - its handlers may still touch this thread's TLS-backed state, so the
			// fixup's own cleanup (freeing that state) has to run last.
			TriggerHackEvent(HACKEVENT_THREAD_DETACH, nullptr);
			FixupDynamicTLS(hModule, ul_reason_for_call);
			break;
	}
	return TRUE;
}
