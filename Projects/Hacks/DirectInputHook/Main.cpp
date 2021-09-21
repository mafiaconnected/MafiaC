
#include "pch.h"
#include "IDirectInput8Hook.h"
#include "IDirectInputDevice8Hook.h"

tHack* g_pHack;

static Context* g_pContext;
bool g_bRemovingInstances = false;
std::unordered_set<IUnknown*> g_Instances;

static decltype(DirectInput8Create)* g_pDirectInput8Create;

static HRESULT WINAPI HookDirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
{
	HRESULT hr;
	void* pvOut;
	if (FAILED(hr = g_pDirectInput8Create(hinst, dwVersion, riidltf, &pvOut, punkOuter)))
		return hr;

	if (riidltf == IID_IDirectInput8A)
		*ppvOut = new IDirectInput8Hook((IDirectInput8*)pvOut);
	else
		*ppvOut = pvOut;
	return hr;
}

static void Load(tHackEventDataLoad* pData)
{
	g_pHack = pData->m_pHack;
	g_pContext = pData->m_pContext;

	HMODULE hLS3DF = GetModuleHandle(_T("LS3DF.dll"));
	if(hLS3DF != nullptr)
	{
		CHacks::GetImportLookups(hLS3DF, "dinput8.dll", [&](uint16_t Ordinal, const char* pszName, void** ppFunction)
		{
			if (pszName != nullptr)
			{
				if (strcmp(pszName, "DirectInput8Create") == 0)
				{
					g_pDirectInput8Create = (decltype(DirectInput8Create)*)*ppFunction;
					new CHackValueHack(g_pHack, ppFunction, sizeof(void*), &HookDirectInput8Create);
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
		case HACKEVENT_UNLOAD:
			{
				g_bRemovingInstances = true;
				for (auto p : g_Instances)
				{
					while (p->Release()) {}
				}
				g_bRemovingInstances = false;
				g_Instances.clear();
			}
			return HACKEVENTRESULT_NORMAL;
		default:
			break;
	}
	return HACKEVENTRESULT_UNREQUESTEVENT;
}
