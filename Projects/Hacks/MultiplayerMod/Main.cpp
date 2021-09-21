
#include "stdafx.h"

#include "Hacks/Hooks.h"
#include "Hacks/Hacks.h"

#include "Renderer.h"
#include "ClientGame.h"
#include "Multiplayer.h"
#include "resource.h"
#include "ClientManager.h"
#include <curl/curl.h>
#include <thread>

#define REQUIRED_MAFIA_SHA256 "08398317C8DC423DBB3E559F0BA53C207A8EAF5BCEFA915DFF83B7A86416AB27"
#define STEAM_MAFIA_SHA256 "08398317C8DC423DBB3E559F0BA53C207A8EAF5BCEFA915DFF83B7A86416AB27"

tHack* g_pHack;
Context* g_pContext;

CClientGame* g_pClientGame;

extern Direct3D9* g_pD3D9;
extern IDirect3DDevice9* g_pD3DDevice;
C2D* g_p2D;

static bool LoadTARArchive(CFileSystem* pFileSystem, Stream* pStream, const GChar* pszTarget, bool bCaseSensitive, bool bExclusive)
{
	CTARArchive* pArchive = new CTARArchive;
	pArchive->SetCaseSensitive(bCaseSensitive);
	if (pArchive->Read(pStream))
	{
		pFileSystem->Mount(pArchive, pszTarget, bExclusive);
		return true;
	}
	else
		delete pArchive;
	return false;
}

static bool LoadTARArchive(CFileSystem* pFileSystem, const GChar* pszArchive, const GChar* pszTarget, bool bCaseSensitive, bool bExclusive)
{
	auto pStream = Strong<Stream>::New(pFileSystem->Open(pszArchive, false));
	if (pStream != nullptr)
		return LoadTARArchive(pFileSystem, pStream, pszTarget, bCaseSensitive, bExclusive);
	return false;
}

static void Load(tHackEventDataLoad* pData)
{
	g_pHack = pData->m_pHack;
	g_pContext = pData->m_pContext;

	{
#if MAFIAC_ARCHIVE_EXTERNAL
		LoadTARArchive(g_pContext->GetFileSystem(), _gstr("/MafiaC.tar"), _gstr("/"), true, false);
#else
		auto pStream = Strong<Stream>::New(OpenResource(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDR_MAIN_ARCHIVE), RT_RCDATA));
		if (pStream != nullptr)
		{
			LoadTARArchive(g_pContext->GetFileSystem(), pStream, _gstr("/"), true, false);
		}
		else
		{
			LoadTARArchive(g_pContext->GetFileSystem(), _gstr("/MafiaC.tar"), _gstr("/"), true, false);
		}
#endif
	}

	curl_global_init(CURL_GLOBAL_DEFAULT);
	enet_initialize();

	g_pClientGame = new CClientGame(g_pContext);
	g_pClientGame->Initialise();

	CGameHacks::InstallHacks();
	CGameHooks::InstallHooks();
}

static void SetD3D8Device(tHackEventDataD3D8* pData)
{
	//_glogerrorprintf(_gstr("D3D8 SET"));
}

static void SetD3D9Device(tHackEventDataD3D9* pData)
{
	//_glogerrorprintf(_gstr("D3D9 SET"));
	g_pD3DDevice = pData->m_pD3DDevice;
}

HACKEVENTRESULT HackMain(uint32_t Event, tHackEventData* pData)
{
	switch (Event)
	{
		case HACKEVENT_LOAD:
			Load((tHackEventDataLoad*)pData);
			//g_GameVersion = GAMEVERSION_UNKNOWN;
			return HACKEVENTRESULT_NORMAL;
			break;

		case HACKEVENT_OND3D8DEVICESET:
			SetD3D8Device((tHackEventDataD3D8*)pData);
			return HACKEVENTRESULT_NORMAL;
			break;

		case HACKEVENT_OND3D9DEVICESET:
			SetD3D9Device((tHackEventDataD3D9*)pData);
			return HACKEVENTRESULT_NORMAL;
			break;
		case HACKEVENT_WNDPROC:
			{
				auto pEvent = (tHackEventDataWindowProcedure*)pData;
				if (pEvent->m_Msg == WM_ACTIVATEAPP)
					return HACKEVENTRESULT_PREVENTDEFAULT;
				//assert(GetCurrentThreadId() == CHackSupport::m_pInstance->m_dwMainThread);
				if (g_pClientGame->OnWndProc(pEvent->m_hWnd, pEvent->m_Msg, pEvent->m_wParam, pEvent->m_lParam, &pEvent->m_Result))
					return HACKEVENTRESULT_PREVENTDEFAULT;
				return HACKEVENTRESULT_NORMAL;
			}
			break;
		case HACKEVENT_POSTWNDPROC:
			{
				auto pEvent = (tHackEventDataWindowProcedure*)pData;
				//assert(GetCurrentThreadId() == CHackSupport::m_pInstance->m_dwMainThread);
				g_pClientGame->OnPostWndProc(pEvent->m_hWnd, pEvent->m_Msg, pEvent->m_wParam, pEvent->m_lParam, &pEvent->m_Result);
			}
			return HACKEVENTRESULT_NORMAL;
			break;
		case HACKEVENT_FRAME:
			{
				//assert(GetCurrentThreadId() == CHackSupport::m_pInstance->m_dwMainThread);
				if (MafiaSDK::IsWindowFocused())
				{
					g_pClientGame->OnFrame();
				}
			}
			return HACKEVENTRESULT_NORMAL;
			break;
		case HACKEVENT_PROCESS:
			{
				//assert(GetCurrentThreadId() == CHackSupport::m_pInstance->m_dwMainThread);
				//if (MafiaSDK::IsWindowFocused())
				//{
					g_pClientGame->OnProcess();
				//}
			}
			return HACKEVENTRESULT_NORMAL;
			break;
		case HACKEVENT_CRASH:
			{
				if (g_pClientGame->m_pMultiplayer != nullptr)
					g_pClientGame->m_pMultiplayer->Disconnect(Galactic3D::DISCONNECT_CRASH);
			}
			return HACKEVENTRESULT_NORMAL;
			break;
		case HACKEVENT_SDLEVENT:
			{
				//_glogerrorprintf(_gstr("SDL EVENT"));
				//assert(GetCurrentThreadId() == CHackSupport::m_pInstance->m_dwMainThread);
				g_pClientGame->OnEvent(((tHackEventDataSDLEvent*)pData)->m_pEvent);
			}
			return HACKEVENTRESULT_NORMAL;
			break;
		case HACKEVENT_DISCORD_JOINREQUEST:
			g_pClientGame->OnJoinRequest((tHackEventDataDiscordUser*)pData);
			return HACKEVENTRESULT_RESULTSET;
			break;
		case HACKEVENT_DISCORD_JOIN:
			g_pClientGame->Connect(((tHackEventDataDiscordJoin*)pData)->secret);
			return HACKEVENTRESULT_NORMAL;
			break;
		default:
			break;
	}
	return HACKEVENTRESULT_UNREQUESTEVENT;
}



