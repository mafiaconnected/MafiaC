#include "pch.h"
#include "MafiaHackSupport.h"


tHack* g_pHack;
static Context* g_pContext;

static HMODULE hRwData;

typedef DWORD (_stdcall* dtaOpen_proc)(const char* file, DWORD params);

dtaOpen_proc dtaOpen;

std::unordered_map<std::string, std::string> g_umapFileNames;

static void ForceDTARead(bool state)
{
	*(BYTE*)(((DWORD)hRwData + 0x12C98)) = state;
}

static DWORD _stdcall HookDtaOpen(const char* file, DWORD params)
{
	_glogverboseprintf(_gstr("Read file: %s"), CString(false, file).CString());

	if (g_umapFileNames.find(file) != g_umapFileNames.end()) {
		// Custom file is available, use it
        _glogverboseprintf(_gstr("Using custom file for: %s (%s)"), CString(false, file).CString(), CString(false, g_umapFileNames[file].c_str()).CString());
		return dtaOpen(g_umapFileNames[file].c_str(), params);
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
                    ForceDTARead(false);

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
                    // Note (Sevenisko): Force the game to load files ONLY from DTA files
                    ForceDTARead(false);

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
            tHackEventDataAddCustomFile* pAddCustomFile = (tHackEventDataAddCustomFile*)pData;
            CString GameFilePath(false, pAddCustomFile->pszGameFilePath);
            CString FilePath(false, pAddCustomFile->pszFilePath);
            //_glogwarnprintf(_gstr("Adding custom file for: %s (%s)"), GameFilePath.CString(), FilePath.CString());
            if (g_umapFileNames.find(pAddCustomFile->pszGameFilePath) == g_umapFileNames.end()) {
                g_umapFileNames[pAddCustomFile->pszGameFilePath] = pAddCustomFile->pszFilePath;
            }
            return HACKEVENTRESULT_NORMAL;
		}
        case HACKEVENT_REMOVECUSTOMFILE:
        {
            tHackEventDataRemoveCustomFile* pRemoveCustomFile = (tHackEventDataRemoveCustomFile*)pData;
            CString GameFilePath(false, pRemoveCustomFile->pszGameFilePath);
            //_glogwarnprintf(_gstr("Adding custom file for: %s (%s)"), GameFilePath.CString(), FilePath.CString());
            if (g_umapFileNames.find(pRemoveCustomFile->pszGameFilePath) != g_umapFileNames.end()) {
                g_umapFileNames.erase(pRemoveCustomFile->pszGameFilePath);
            }
            return HACKEVENTRESULT_NORMAL;
        }
		return HACKEVENTRESULT_NORMAL;
		break;
		default:
			break;
	}
	return HACKEVENTRESULT_UNREQUESTEVENT;
}