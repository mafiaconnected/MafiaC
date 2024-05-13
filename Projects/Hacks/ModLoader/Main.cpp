#include "pch.h"
#include "MafiaHackSupport.h"


tHack* g_pHack;
static Context* g_pContext;

static HMODULE hRwData;

typedef DWORD (_stdcall* dtaOpen_proc)(const char* file, DWORD params);

dtaOpen_proc dtaOpen;

std::unordered_map<const char*, const char*> g_umapFileNames;

static void ForceDTARead(bool state)
{
	*(BYTE*)(((DWORD)hRwData + 0x12C98)) = state;
}

static DWORD _stdcall HookDtaOpen(const char* file, DWORD params)
{
	_glogprintf(_gstr("Read file: %hs"), file);

	if (g_umapFileNames.count(file) > 0) {
		// Custom file is available, use it
		return dtaOpen(g_umapFileNames[file], params);
	} else {
		// Custom file is not available, use the original file
		return dtaOpen(file, params);
	}

	//return dtaOpen(file, params);
}

static void Load(tHackEventDataLoad* pData)
{
	g_pHack = pData->m_pHack;
	g_pContext = pData->m_pContext;

    hRwData = GetModuleHandle(_T("rw_data.dll"));
	HMODULE hGame = GetModuleHandle(_T("Game.exe"));
	HMODULE hLS3DF = GetModuleHandle(_T("LS3DF.dll"));

	assert(hGame);
	assert(hLS3DF);
	assert(hRwData);

    if (hGame != nullptr)
    {
        CHacks::GetImportLookups(hGame, "rw_data.dll", [&](uint16_t Ordinal, const char* pszName, void** ppFunction) {
            if (pszName != nullptr)
            {
                if (strcmp(pszName, "_dtaOpen@8") == 0)
                {
                    // Note (Sevenisko): Force the game to load files ONLY from DTA files
                    ForceDTARead(true);

                    dtaOpen = (dtaOpen_proc)*ppFunction;

                    new CHackValueHack(g_pHack, ppFunction, sizeof(void*), &HookDtaOpen);
                    return false;
                }
            }
            return true;
        });

        // Note (Sevenisko): Both LS3DF and Game calls for dtaOpen need to be hooked in order to make all the modded files loadable

        CHacks::GetImportLookups(hLS3DF, "rw_data.dll", [&](uint16_t Ordinal, const char* pszName, void** ppFunction) {
            if (pszName != nullptr)
            {
                if (strcmp(pszName, "_dtaOpen@8") == 0)
                {
                    dtaOpen = (dtaOpen_proc)*ppFunction;

                    new CHackValueHack(g_pHack, ppFunction, sizeof(void*), &HookDtaOpen);
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
		{
			Load((tHackEventDataLoad*)pData);
			return HACKEVENTRESULT_NORMAL;
			break;
		}
		case HACKEVENT_ADDCUSTOMFILE:
		{
			tHackEventDataCustomFile* pAddCustomFile = (tHackEventDataCustomFile*)pData;
			g_umapFileNames[pAddCustomFile->pszGameFilePath] = pAddCustomFile->pszFilePath;
			return HACKEVENTRESULT_NORMAL;
			break;
		}
		return HACKEVENTRESULT_NORMAL;
		break;
		default:
			break;
	}
	return HACKEVENTRESULT_UNREQUESTEVENT;
}