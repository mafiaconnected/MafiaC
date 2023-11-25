#include "pch.h"

tHack* g_pHack;
static Context* g_pContext;

static HMODULE hRwData;

typedef DWORD (_stdcall* dtaOpen_proc)(const char* file, DWORD params);

dtaOpen_proc dtaOpen;

//std::unordered_map <const char*, Galactic3D::Stream> g_umapFileNames;

static void ForceDTARead(bool state)
{
	*(BYTE*)(((DWORD)hRwData + 0x12C98)) = state;
}

static DWORD _stdcall HookDtaOpen(const char* file, DWORD params)
{
	//if (g_umapFileNames.count(file) > 0) {	
		// Custom file is available, use it
		//return dtaOpen(file, params);
	//}

	//_glogprintf(_gstr("Read file: %hs"), file);

	//CString File(false, file);

	//std::wstring strFile = CHackSupport::m_pInstance->m_GamePath;
	//strFile += File;

	//UTF8String String(false, strFile.c_str());

	//return dtaOpen(String, params);
	
	return dtaOpen(file, params);
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
		case HACKEVENT_REGISTERFUNCTIONS:
		{
			/*
			Interfaces::IScripting* pScripting = ((tHackEventDataRegisterFunctions*)pData)->m_pScripting;

			Strong<Interfaces::IReflectedNamespace> pGlobal;
			if (!Failed(pScripting->GetGlobal(&pGlobal)))
			{
				Strong<Interfaces::IReflectedNamespace> pModLoader;
				if (!Failed(pGlobal->AddNamespace("modloader", &pModLoader)))
				{
					pModLoader->RegisterFunction("addFile", "xs", FunctionAddGameFile, nullptr);
				}
			}
			*/
		}
		return HACKEVENTRESULT_NORMAL;
		break;
		default:
			break;
	}
	return HACKEVENTRESULT_UNREQUESTEVENT;
}
