
#include "stdafx.h"
#include <d3d8.h>
#include "IDirect3D8Hook.h"

tHack* g_pHack;
static Context* g_pContext;

extern struct ID3DXEffect* g_pEffect;

static IDirect3D8* WINAPI HookDirect3DCreate8(UINT SDKVersion)
{
	auto pDevice = Direct3DCreate8(SDKVersion);
	return new IDirect3D8Hook(pDevice);
}

static void Load(tHackEventDataLoad* pData)
{
	g_pHack = pData->m_pHack;
	g_pContext = pData->m_pContext;

    HMODULE hD3D8 = GetModuleHandle(_T("d3d8.dll"));
	HMODULE hLS3DF = GetModuleHandle(_T("LS3DF.dll"));

    if (hD3D8 != nullptr && hLS3DF != nullptr)
    {
        CHacks::GetImportLookups(hLS3DF, "d3d8.dll", [&](uint16_t Ordinal, const char* pszName, void** ppFunction) {
            if (pszName != nullptr)
            {
                if (strcmp(pszName, "Direct3DCreate8") == 0)
                {
                    new CHackValueHack(g_pHack, ppFunction, sizeof(void*), &HookDirect3DCreate8);
                    return false;
                }
            }
            return true;
        });
    }
}

HACKEVENTRESULT HackMain(uint32_t Event, tHackEventData* pData)
{
	switch (Event)
	{
		case HACKEVENT_LOAD:
			Load((tHackEventDataLoad*)pData);
			return HACKEVENTRESULT_NORMAL;
			break;
		default:
			break;
	}
	return HACKEVENTRESULT_UNREQUESTEVENT;
}
