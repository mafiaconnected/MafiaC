
#include "stdafx.h"
#include "ClientGame.h"
#include "ClientManager.h"
#include "resource.h"
#include <curl/curl.h>
#include <Multiplayer/ClientDownloadManager.h>
#include "Multiplayer.h"
#include "ScriptingFunctions/ScriptingFunctions.h"
#include <Multiplayer/ChatWindow.h>
#include <Multiplayer/CmdWindow.h>
#include <Audio/BassRenderer.h>
#ifdef GAMEMAFIA2_DLL
#include <Audio/BassRenderer.hpp>
#endif
//#include <Audio/FMODRenderer.h>
#include <LucasFont/LucasFont.h>
#include <sstream>
#include <SDL_scancode.h>
#include <SDL_syswm.h>
#include <openssl/err.h>
#include <algorithm>
#include <cctype>
#include <locale>
#include "Hacks/Hacks.h"
#include "Hacks/Hooks.h"
#include <LoadScreen.h>

uint32_t g_uiSyncedTickCountII = 0;

using namespace Galactic3D;

CMafiaCHtmlContainerII::CMafiaCHtmlContainerII(Context* pContext, CClientGameII* pClientGame) : CHtmlContainer(pContext), m_pClientGame(pClientGame)
{
	m_pInternetRequestMgr = &pClientGame->m_InternetRequestMgr;
}

Galactic3D::Stream* CMafiaCHtmlContainerII::OpenFile(const GChar* pszPath)
{
	if (m_pActiveView->m_pResource != nullptr)
		return m_pActiveView->m_pResource->OpenFile(pszPath, false);
	return CHtmlContainer::OpenFile(pszPath);
}

void CMafiaCHtmlContainerII::on_anchor_click(const litehtml::tchar_t* url, const litehtml::element::ptr& el)
{
	CArguments Args;
	Args.AddObject(m_pActiveView);
	Args.AddObject(GetObjectFromElement(m_pActiveView, (litehtml::element::ptr)el));
	Args.AddString(url);
	bool bPreventDefault = false;
	g_pClientGame->m_pOnGUIAnchorClickEventType->Trigger(Args, bPreventDefault);
	if (!bPreventDefault)
		CHtmlContainer::on_anchor_click(url, el);
}

CTextDrawingII::CTextDrawingII()
{
}

void CTextDrawingII::Initialise(LucasGUI::CFonts& Fonts)
{
	m_fLargeSize = CHAT_FONT_SIZE + 1;
	unsigned int uiLargeSize = (unsigned int)(m_fLargeSize * 64.0f);
	m_LargeFontStack.Initialise(Fonts.GetFont(_gstr("Open Sans"))->GetSize(uiLargeSize));
	m_LargeFontStack.m_FontSizes.push_back(Fonts.GetFont(_gstr("Noto Color Emoji"))->GetSize(uiLargeSize));

	m_fSmallSize = 12.0f;
	unsigned int uiSmallSize = (unsigned int)(m_fSmallSize * 64.0f);
	m_SmallFontStack.Initialise(Fonts.GetFont(_gstr("Open Sans"))->GetSize(uiSmallSize));
	m_SmallFontStack.m_FontSizes.push_back(Fonts.GetFont(_gstr("Noto Color Emoji"))->GetSize(uiSmallSize));
}

void CTextDrawingII::ShutDown()
{
	m_LargeFontStack.ShutDown();
	m_SmallFontStack.ShutDown();
}

void CTextDrawingII::MeasureText(size_t Font, CVector2D& vecSize, const GChar* pszText, float fWidth, float fAlign, float fJustify, bool bWordWrap, bool bColourCodes)
{
	float fSize;
	LucasGUI::CFontStack* pFontStack = nullptr;
	switch (Font)
	{
	case 0:
		fSize = m_fLargeSize;
		pFontStack = &m_LargeFontStack;
		break;
	case 1:
		fSize = m_fSmallSize;
		pFontStack = &m_SmallFontStack;
		break;
	default:
		break;
	}
	CDumbUtil::MeasureText(pFontStack, vecSize, pszText, fWidth, fAlign, fJustify, fSize, bWordWrap, bColourCodes);
}

void CTextDrawingII::RenderText(Galactic3D::I2D* p2D, size_t Font, const GChar* pszText, const CVector2D& vecOffset, float fWidth, float fAlign, float fJustify, Galactic3D::COLOUR Colour, bool bWordWrap, bool bColourCodes, bool bIgnoreColourCodes, bool bShadow)
{
	float fSize;
	LucasGUI::CFontStack* pFontStack = nullptr;
	switch (Font)
	{
	case 0:
		fSize = m_fLargeSize;
		pFontStack = &m_LargeFontStack;
		break;
	case 1:
		fSize = m_fSmallSize;
		pFontStack = &m_SmallFontStack;
		break;
	default:
		break;
	}
	CDumbUtil::RenderText(p2D, pFontStack, pszText, vecOffset, fWidth, fAlign, fJustify, fSize, Colour, bWordWrap, bColourCodes, bIgnoreColourCodes, bShadow);
}

CClientGameII::CClientGameII(Galactic3D::Context* pContext)
	: m_GUISystem(pContext, &m_Fonts, &m_LanguageLocalisation, &m_SlotMgr), m_InternetCache(pContext), m_InternetRequestMgr(pContext, &m_InternetCache)
{
	m_pContext = pContext;
}

void CClientGameII::Initialise()
{
	m_TimeManager.Initialise(60.0, 16.0);
	m_GUISystem.m_Action = [this](const GChar* pszAction, LucasGUI::GUI::CButton* pButton) {
		return OnAction(pszAction, pButton);
	};
	m_bCapured = false;

	ClearError();

	if (!m_InternetCache.Initialise())
	{
		if (HasError())
			_glogprintf(_gstr("%s"), GetError());
	}

	{
		auto pStream = Strong<Stream>::New(m_pContext->GetFileSystem()->Open(_gstr("/GUI/Images/ImageNotLoaded.png"), false));
		if (pStream)
		{
			auto pTexture = Texture::CreateFromPNG(pStream, false);
			if (pTexture)
			{
				pTexture->SetName(_gstr("ImageNotLoaded.png"));
				m_SlotMgr.m_Slots[SLOT_GLOBAL].m_Textures.Add(pTexture);
			}
		}
	}
	{
		auto pStream = Strong<Stream>::New(m_pContext->GetFileSystem()->Open(_gstr("/GUI/Images/SizeGrip.png"), false));
		if (pStream)
		{
			auto pTexture = Texture::CreateFromPNG(pStream, false);
			if (pTexture)
			{
				pTexture->SetName(_gstr("SizeGrip.png"));
				m_SlotMgr.m_Slots[SLOT_GLOBAL].m_Textures.Add(pTexture);
			}
		}
	}
	{
		auto pStream = Strong<Stream>::New(m_pContext->GetFileSystem()->Open(_gstr("/GUI/Images/Grid.bmp"), false));
		if (pStream)
		{
			auto pTexture = Texture::CreateFromBMP(pStream, false);
			if (pTexture)
			{
				pTexture->SetName(_gstr("Grid.bmp"));
				m_SlotMgr.m_Slots[SLOT_GLOBAL].m_Textures.Add(pTexture);
			}
		}
	}

	m_pContext->SetVerboseLogging(m_pContext->GetSettings()->GetBoolean(nullptr, _gstr("Verbose Logging"), false));
	m_pContext->GetSettings()->Read(_gstr("Game"), _gstr("D3D9"), &m_bD3D9);

	//m_pLucasFontFunctions = new CLucasFontFunctions;
	//InitialiseScripting();

	CLoadScreen::LoadResources();

	// Load fonts once
	LoadFonts();

	{
		auto pStream = Strong<Stream>::New(m_pContext->GetFileSystem()->Open(_gstr("/master.css"), false));
		if (pStream)
		{
			CTextReader TextReader(pStream);
			size_t TextLength;
			const GChar* pszText = TextReader.LoadText(TextLength);
			CLiteHtmlString Text(false, pszText, TextLength);
			m_BrowserContext.load_master_stylesheet(Text);
		}
	}

	m_pHtmlContainer = new CMafiaCHtmlContainerII(m_pContext, this);
	//m_pHtmlContainer->UseDPIScaling();

	m_GUISystem.m_CreateHtmlView = [this](LucasGUI::GUI::CSystem* pSystem) {
		auto pHtmlView = new CHtmlView(pSystem);
		pHtmlView->m_pBrowserContext = &m_BrowserContext;
		pHtmlView->m_pHtmlContainer = m_pHtmlContainer;
		return pHtmlView;
	};

	InitialiseCVars();

	InitialiseScripting();
}

void CClientGameII::ShutDown()
{
	assert(m_pMultiplayer == nullptr);
	if (m_pMultiplayer != NULL)
	{
		m_pMultiplayer->Disconnect();
		delete m_pMultiplayer;
		m_pMultiplayer = NULL;
	}
	m_GUISystem.Clear();
	ShutdownScripting();
	delete m_pHtmlContainer;
	//ShutdownScripting();
	//delete m_pLucasFontFunctions;
	//if (m_pFonts != nullptr)
	//	delete m_pFonts;
}

void CClientGameII::InitialiseCVars()
{
	m_CVars.Clear();
}

void CClientGameII::InitialiseScripting()
{
	m_bScriptCursorEnabled = false;
	m_bScriptControlsDisabled = false;
	m_bCursorEnabled = false;

#if MAFIAC_RMLUI
	m_pRmlUi = new CRmlUi2(m_pContext);
	m_pRmlUi->Initialise();
#endif

	m_pResourceMgr = new CMafiaClientResourceMgrII(m_pContext);

	m_pOnEntityProcessEventType = m_pResourceMgr->m_pEventHandlers->CreateEventType(_gstr("OnEntityProcess"), _gstr("Called whenever an entity is processed"), 1);
	m_pOnEntityProcessEventType->m_bCanPreventDefault = true;
	m_pOnDrawHUDEventType = m_pResourceMgr->m_pEventHandlers->CreateEventType(_gstr("OnDrawHUD"), _gstr("Called right before the hud is drawn, you should draw your HUD here if you want it under the hud"));
	m_pOnDrawHUDEventType->m_bCanPreventDefault = true;
	m_pOnDrawnHUDEventType = m_pResourceMgr->m_pEventHandlers->CreateEventType(_gstr("OnDrawnHUD"), _gstr("Called after all the hud is drawn, you should draw your HUD here if you want it over the hud"));
	m_pOnCameraProcessEventType = m_pResourceMgr->m_pEventHandlers->CreateEventType(_gstr("OnCameraProcess"), _gstr("Called when the camera is about to be processed"));
	m_pOnCameraProcessEventType->m_bCanPreventDefault = true;
	m_pOnGUIAnchorClickEventType = m_pResourceMgr->m_pEventHandlers->CreateEventType(_gstr("OnGUIAnchorClick"), _gstr("Called when anchor tags are clicked in a GUIHtmlView"), 3);
	m_pOnGUIAnchorClickEventType->m_iSource = 0;
	m_pOnGUIAnchorClickEventType->m_bCanPreventDefault = true;
	m_pOnGUIClickEventType = m_pResourceMgr->m_pEventHandlers->CreateEventType(_gstr("OnGUIClick"), _gstr("Called when a GUIElement is clicked on"), 1);

	// Register MafiaC events here
	//m_pOnExampleEventType = m_pResourceMgr->m_pEventHandlers->CreateEventType(_gstr("OnExampleEvent"), _gstr("Called whenever something happens"), 1);
	//m_pOnExampleEventType->m_bCanPreventDefault = true;
	//m_pOnExampleEventType->m_iSource = 0;
	m_pOnMapLoadedEventType = m_pResourceMgr->m_pEventHandlers->CreateEventType(_gstr("OnMapLoaded"), _gstr("Called whenever the map/mission is fully loaded and ready to play"), 1, false);
	//m_pOnKeyPressedEventType = m_pResourceMgr->m_pEventHandlers->CreateEventType(_gstr("OnKeyPressed"), _gstr("Called whenever the key is pressed"));
	m_pOnHumanHitEventType = m_pResourceMgr->m_pEventHandlers->CreateEventType(_gstr("OnPedInflictDamage"), _gstr("Called whenever a ped has been hit"), 7, true);
	m_pOnHumanDeathEventType = m_pResourceMgr->m_pEventHandlers->CreateEventType(_gstr("OnPedDeath"), _gstr("Called whenever a ped dies"), 1, false);
	m_pOnHumanSpawnEventType = m_pResourceMgr->m_pEventHandlers->CreateEventType(_gstr("OnPedSpawn"), _gstr("Called whenever a ped spawns"), 1, false);

	m_pOnHumanEnteringVehicleEventType = m_pResourceMgr->m_pEventHandlers->CreateEventType(_gstr("OnPedEnteringVehicle"), _gstr("Called whenever a ped starts entering a vehicle"), 3, false);
	m_pOnHumanEnteredVehicleEventType = m_pResourceMgr->m_pEventHandlers->CreateEventType(_gstr("OnPedEnteredVehicle"), _gstr("Called whenever a ped finishes entering a vehicle"), 3, false);
	m_pOnHumanExitingVehicleEventType = m_pResourceMgr->m_pEventHandlers->CreateEventType(_gstr("OnPedExitingVehicle"), _gstr("Called whenever a ped starts exiting a vehicle"), 3, false);
	m_pOnHumanExitedVehicleEventType = m_pResourceMgr->m_pEventHandlers->CreateEventType(_gstr("OnPedExitedVehicle"), _gstr("Called whenever a ped finishes exited a vehicle"), 3, false);
	m_pOnHumanJackVehicleEventType = m_pResourceMgr->m_pEventHandlers->CreateEventType(_gstr("OnPedJackVehicle"), _gstr("Called whenever a ped jacks a vehicle"), 3, false);

	m_pGalacticFunctions = new CGalacticFunctions(m_pResourceMgr, false, false, false, false, false);
	m_pGalacticFunctions->m_p2D = &m_p2D;
	m_pAudioScriptingFunctions = new Galactic3D::Audio::CAudioScriptingFunctions(m_pResourceMgr);
	//m_pAudioScriptingFunctions->m_pSoundMgr = new Galactic3D::Audio::CSoundMgr;
	m_pAudioScriptingFunctions->m_pSoundMgr = new CBassSoundMgr;
	//m_pAudioScriptingFunctions->m_pSoundMgr = new CFMODSoundMgr;
	if (!m_pAudioScriptingFunctions->m_pSoundMgr->Initialise())
	{
		delete m_pAudioScriptingFunctions->m_pSoundMgr;
		m_pAudioScriptingFunctions->m_pSoundMgr = NULL;
	}
	m_LucasFontFunctions.RegisterFunctions(m_pResourceMgr->m_pScripting);
	m_LucasFontFunctions.m_CreateFreeTypeTextureFontPageTextureData.m_p2D = &m_p2D;
	m_GUISystem.RegisterFunctions(m_pResourceMgr->m_pScripting);
	m_pHtmlContainer->RegisterFunctions(m_pResourceMgr->m_pScripting, &m_GUISystem);
	if (m_pDownloadManager == nullptr)
		m_pDownloadManager = new CClientDownloadManager(m_pResourceMgr);
	else
		m_pDownloadManager->m_pResourceMgr = m_pResourceMgr;
	m_pResourceMgr->m_pDownloadManager = m_pDownloadManager;

	m_pClientManager = new CMafiaClientManagerII(m_pContext, m_pResourceMgr);
	m_pClientManager->m_pChatWindow = m_pChatWindow;
	CScriptingFunctionsII::RegisterDefines(m_pResourceMgr->m_pDefineHandlers);
	CScriptingFunctionsII::RegisterFunctions(m_pResourceMgr->m_pScripting, this);

	m_pResourceMgr->DefineAllClasses();

	if (m_pMultiplayer != nullptr)
	{
		m_pClientManager->m_pNetGame = m_pMultiplayer;
		m_pClientManager->m_pNetMachines = &m_pMultiplayer->m_NetMachines;
		m_pMultiplayer->m_pClientManager = m_pClientManager;
		m_pMultiplayer->SetNetObjectMgr(m_pClientManager);
	}
}

void CClientGameII::ShutdownScripting()
{
	if (m_pCmdWindow != nullptr)
	{
		delete m_pCmdWindow;
		m_pCmdWindow = nullptr;
	}
	if (m_pChatWindow != nullptr)
	{
		delete m_pChatWindow;
		m_pChatWindow = nullptr;
	}
	//m_pClientManager->Forget();
	m_pClientManager->Clear();
	m_pResourceMgr->ClearAllResources();
	delete m_pClientManager;
	m_pClientManager = nullptr;
	delete m_pGalacticFunctions;
	m_pGalacticFunctions = nullptr;
	if (m_pAudioScriptingFunctions->m_pSoundMgr != NULL)
	{
		delete m_pAudioScriptingFunctions->m_pSoundMgr;
		m_pAudioScriptingFunctions->m_pSoundMgr = nullptr;
	}
	delete m_pAudioScriptingFunctions;
	m_pAudioScriptingFunctions = nullptr;
	if (m_pMultiplayer != nullptr || m_pNewMultiplayer != nullptr)
		m_pResourceMgr->m_pDownloadManager = nullptr;
	else
		m_pDownloadManager = nullptr;
	delete m_pResourceMgr;
	m_pResourceMgr = nullptr;
	m_Fonts.DeleteHWResources();
	m_SlotMgr.DeleteHWResources();
#if MAFIAC_RMLUI
	delete m_pRmlUi;
#endif
}

static bool LoadSystemFontCB(const TCHAR* pszValueName, const TCHAR* pszValue)
{
	if (_tcsstr(pszValueName, _T("Arial")) ||
		_tcsstr(pszValueName, _T("Tahoma")) ||
		_tcsstr(pszValueName, _T("Segoe UI Emoji")) ||
		_tcsstr(pszValueName, _T("Comic Sans MS")) ||
		_tcsstr(pszValueName, _T("Times New Roman")) ||
		_tcsstr(pszValueName, _T("Courier New")) ||
		_tcsstr(pszValueName, _T("Impact")))
		return true;
	return false;
}

void CClientGameII::LoadFonts()
{
	// Clear the existing fonts first
	m_LucasFontFunctions.m_Fonts.Clear();
	m_Fonts.Clear();

	_glogprintf(_gstr("Loading System Fonts\n"));
	m_LucasFontFunctions.m_Fonts.LoadSystemFonts(LoadSystemFontCB);
	m_Fonts.LoadSystemFonts(LoadSystemFontCB);

	{
		m_pContext->GetFileSystem()->Enumerate(_gstr("/Fonts"), [](const Galactic3D::CFileMgr::tDirectoryEntry& Entry, void* pUser) {
			auto pClientGame = ((CClientGameII*)pUser);
			GString Path = Entry.pszPath;
			PathUtil::AppendPath(Path, Entry.pszFileName);
			auto pStream = Strong<Stream>::New(pClientGame->m_pContext->GetFileSystem()->Open(Path.c_str(), false));
			if (pStream != nullptr)
			{
				//_glogprintf(_gstr("Loading Font - %s\n"), Path.c_str());
				pClientGame->m_LucasFontFunctions.m_Fonts.LoadFont(pStream);
				pClientGame->m_Fonts.LoadFont(pStream);
			}
			return true;
			}, this);
	}

	m_TextDrawing.Initialise(m_Fonts);
}

bool CClientGameII::OnWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	CArguments args;

	switch (message)
	{
	case WM_CREATE:
	{
		m_Cursor = SDL_SYSTEM_CURSOR_ARROW;
		m_bMouseClipped = false;
		UpdateCursor();
	}
	break;
	case WM_DESTROY:
		for (size_t i = 0; i < ARRAY_COUNT(m_rgpCursors); i++)
			SDL_FreeCursor(m_rgpCursors[i]);
		break;
	case WM_CHAR:
	{
		wchar_t c = (wchar_t)wParam;
		if (M2::C_MafiaFramework::GetActive())
		{
			OnCharacter(c);
		}
	}
	break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
		//args.AddNumber(wParam);
		//args.AddBoolean(GetKeyState(wParam) & 0x8000);
		//m_pOnKeyPressedEventType->Trigger(args);

		//if (wParam == VK_F5 && GetKeyState(VK_CONTROL) & 0x8000) { // Exit the game on Ctrl+Escape
		//	ExitProcess(0);
		//}
		//
		//if(wParam == VK_RETURN) {
		//	_glogprintf(_gstr("Chatbox submit"));
		//	m_pResourceMgr->m_pCommandHandlers->PushLogger(m_pChatWindow);
		//	m_pCmdWindow->ProcessInput(m_pResourceMgr->m_pCommandHandlers);
		//	m_pResourceMgr->m_pCommandHandlers->PopLogger();
		//} else if(wParam == 0x54) { // T for chatbox
		//	_glogprintf(_gstr("Chatbox opening"));
		//	m_bEnableCmdWindowOnCharPress = true;
		//}
	case WM_SYSKEYUP:
	{
		if (M2::C_MafiaFramework::GetActive())
		{
			if (m_bHandledKeyEvent)
			{
				m_bHandledKeyEvent = false;
				*pResult = 0;
				return true;
			}
		}
	}
	break;
	case WM_MOUSELEAVE:
	{
		if (M2::C_MafiaFramework::GetActive())
		{
			if (m_pGalacticFunctions != nullptr)
				m_pGalacticFunctions->OnMouseLeave();
			m_GUISystem.OnMouseLeave();
		}
	}
	break;
	case WM_SETFOCUS:
	{
		if (DontClipCursor() || IsCursorEnabled2())
		{
			m_bFocused = true;
		}
		UpdateCursorEnabled(true);

		if (M2::C_MafiaFramework::GetActive())
		{
			if (m_pGalacticFunctions != nullptr)
				m_pGalacticFunctions->OnFocus();
		}

		m_GUISystem.m_bFocused = true;
	}
	break;
	case WM_SIZE:
	{
		if (m_bMouseClipped)
		{
			RECT Rect;
			GetClientRect(hWnd, &Rect);
			MapWindowRect(hWnd, NULL, &Rect);
			::ClipCursor(&Rect);
			m_bMouseClipped = true;
		}
	}
	break;
	case WM_KILLFOCUS:
	{
		m_bFocused = false;
		m_bFocusedSupressInput = false;
		UpdateCursorEnabled();

		if (M2::C_MafiaFramework::GetActive())
		{
			//SetCursorClipped(false, false);
			if (m_pGalacticFunctions != nullptr)
				m_pGalacticFunctions->OnLostFocus();
			m_GUISystem.OnLostFocus();
		}
	}
	break;
	case WM_SETCURSOR:
	{
		if (LOWORD(lParam) == HTCLIENT)
		{
			UpdateCursor();
			*pResult = TRUE;
			return true;
		}
		else
		{
			*pResult = DefWindowProcW(hWnd, message, wParam, lParam);
			return true;
		}
	}
	break;
	case WM_LBUTTONDOWN:
	{
		if (M2::C_MafiaFramework::GetActive() && IsInputDisabled())
		{
			*pResult = 0;
			return true;
		}
	}
	break;
	case WM_LBUTTONUP:
	{
	}
	break;
	case WM_RBUTTONDOWN:
	{
		if (M2::C_MafiaFramework::GetActive() && IsInputDisabled())
		{
			*pResult = 0;
			return true;
		}
	}
	break;
	case WM_RBUTTONUP:
	{
	}
	break;
	case WM_MOUSEMOVE:
	{
		TRACKMOUSEEVENT EventTrack = {};
		EventTrack.cbSize = sizeof(EventTrack);
		EventTrack.dwFlags = TME_LEAVE;
		EventTrack.hwndTrack = hWnd;

		TrackMouseEvent(&EventTrack);

		if (M2::C_MafiaFramework::GetActive())
		{
			POINT Point;
			if (GetCursorPos(&Point) == FALSE)
				break;
			MapWindowPoints(NULL, hWnd, &Point, 1);

			CVector2D vecPos;
			vecPos.x = (float)GET_X_LPARAM(lParam);
			vecPos.y = (float)GET_Y_LPARAM(lParam);
		}
	}
	break;
	default:
		break;
	}
	return false;
}

void CClientGameII::OnPostWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	switch (message)
	{
	case WM_SIZE:
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	{
		if (!m_bFocused)
		{
			m_bFocused = true;
			m_bFocusedSupressInput = true;
			UpdateCursorEnabled(true);
			//SetCursorClipped(true);
		}
	}
	break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	{
		if (m_bFocusedSupressInput)
			m_bFocusedSupressInput = false;
		UpdateCursorEnabled(true);
	}
	break;
	default:
		break;
	}
}

void CClientGameII::OnPreStartInGame(bool bRestarted)
{
	//_glogverboseprintf(__gstr(__FUNCTION__));

	m_LastFrameTicks = OS::GetTicks();

	m_pResourceMgr->m_pDownloadManager->ResetProgress();

	m_bTrainsEnabled = true;

	//CGameHacks::UpdateCVars();

	UpdateCursorEnabled(true);
}

void CClientGameII::OnStartInGame(bool bRestarted)
{
	//_glogverboseprintf(__gstr(__FUNCTION__));

	bool bMultiplayerInProgress = m_pMultiplayer != nullptr;
	bool bMultiplayerRestartingGame = false;
	if (m_pMultiplayer != nullptr && m_pMultiplayer->m_bRestartingGame)
	{
		_glogprintf(_gstr("ClientGame OnStartInGame - Multiplayer not null, restarting game true"));
		bMultiplayerRestartingGame = true;
		m_pMultiplayer->m_bRestartingGame = false;
	}

	if (bMultiplayerInProgress || m_bForceMultiplayerWorld)
		m_bMultiplayerWorld = true;

	m_bForceMultiplayerWorld = false;

	if (!bMultiplayerInProgress)
	{
		tHackEventDataDiscordState Data;
		Data.m_Size = sizeof(Data);
		Data.m_pszState = _gstr("Offline");
		TriggerHackEvent(HACKEVENT_SETDISCORDDETAILS, &Data);
	}

	if (bMultiplayerRestartingGame)
	{
		tHackEventDataDiscordState Data;
		Data.m_Size = sizeof(Data);
		Data.m_pszState = m_pMultiplayer->m_LocalPlayerName.c_str();
		TriggerHackEvent(HACKEVENT_SETDISCORDSTATE, &Data);
	}

	m_GUISystem.LoadPage(_gstr("/GUI/Main.xml"));

	int width = 0;
	int height = 0;

	if (M2::C_MafiaFramework::GetActive())
	{
		HWND pWindow = GetActiveWindow();
		RECT rect;

		if (GetWindowRect(pWindow, &rect))
		{
			width = rect.right - rect.left;
			height = rect.bottom - rect.top;
		}
	}

	float fScale = 0.7f / 900.0f* (float)height;

	//fScale *= *m_CVars.m_pfChatScale;
	float fScale2;
	if (g_pClientGame->m_pContext->GetSettings()->Read(_gstr("Chat Window"), _gstr("Scale"), &fScale2))
		fScale = fScale2;
	if (m_pChatWindow == nullptr)
	{
		m_pChatWindow = new CChatWindow(m_pContext, &m_Fonts, fScale);
		m_pClientManager->m_pChatWindow = m_pChatWindow;
		m_pChatWindow->m_dwChatInfoColor = Galactic3D::COLOUR(255, 205, 60, 60);

		m_pChatWindow->AddInfoMessage(_gstr("%s %s"), CHackSupport::m_pInstance->m_InjectedData.m_InjectData.m_LauncherData.m_Launcher.m_szTitle, __gstr(LAUNCHER_DISPLAY_VERSION));
	}
	else
	{
		m_pChatWindow->SetEnabled(true);
		m_pChatWindow->SetScale(fScale);
	}

	if (m_iStopMultiplayerGameReason >= 0)
	{
		ShowDisconnectReason();
		m_iStopMultiplayerGameReason = -1;
	}

	{
		auto pPage = static_cast<LucasGUI::GUI::CPage*>(m_GUISystem.m_pPage->GetElementByName(_gstr("CmdWindow")));

		if (m_pCmdWindow != nullptr)
		{
			m_pCmdWindow->ReInitialise(pPage, m_pClientManager->m_pResourceMgr->m_pCommandHandlers, m_pChatWindow);
		}
		else
		{
			m_pCmdWindow = new CCmdWindow2(pPage, m_pClientManager->m_pResourceMgr->m_pCommandHandlers, m_pChatWindow);
		}

		m_bEnableCmdWindowOnCharPress = false;
		m_pCmdWindow->SetScale(fScale);
	}

	if (!bMultiplayerInProgress)
	{
		GString Name;
		if (m_pContext->GetSettings()->GetString(NULL, _gstr("Name"), Name))
		{
			//m_pChatWindow->AddInfoMessage("%s joined the game.",szName);
		}
		else
			NoName();
	}

	ResetWorld();
	if (!bMultiplayerInProgress)
	{
		LoadLobbyResource();
	}

	if (bMultiplayerRestartingGame)
	{
		m_pChatWindow->AddInfoMessage(_gstr("Connected!"));
	}

	if (!bMultiplayerInProgress)
	{
		CHackSupport* pHackSupport = CHackSupport::m_pInstance;
		if (!pHackSupport->m_InjectedData.m_InjectData.m_LauncherData.m_Host.empty())
		{
			const GChar* pszPassword = nullptr;
			if (!pHackSupport->m_InjectedData.m_InjectData.m_LauncherData.m_Password.empty())
				pszPassword = pHackSupport->m_InjectedData.m_InjectData.m_LauncherData.m_Password.c_str();
			Connect(pHackSupport->m_InjectedData.m_InjectData.m_LauncherData.m_Host, pszPassword);
			pHackSupport->m_InjectedData.m_InjectData.m_LauncherData.m_Password.clear();
			pHackSupport->m_InjectedData.m_InjectData.m_LauncherData.m_Host.clear();
		}
	}
}

void CClientGameII::OnEndInGame()
{
	//_glogverboseprintf(__gstr(__FUNCTION__));

	m_pHtmlContainer->m_Images.clear();

	{
		tHackEventDataDiscordState Data;
		Data.m_Size = sizeof(Data);
		Data.m_pszState = _gstr("");
		TriggerHackEvent(HACKEVENT_SETDISCORDSTATE, &Data);
	}

	if (m_pMultiplayer != NULL)
	{
		m_pMultiplayer->Reset();

		if (!m_pMultiplayer->m_bRestartingGame)
		{
			m_pMultiplayer->Disconnect(Galactic3D::DISCONNECT_GRACEFUL);
			delete m_pMultiplayer;
			m_pMultiplayer = nullptr;

			// Reset the cvars when disconnecting
			InitialiseCVars();
		}
	}

	if (m_pNewMultiplayer != nullptr)
	{
		m_pNewMultiplayer->Reset();

		// If the game is restarting and we haven't connected, delete this new multiplayer
		if (!m_pNewMultiplayer->IsConnected())
		{
			delete m_pNewMultiplayer;
			m_pNewMultiplayer = nullptr;
		}
	}

	m_pResourceMgr->ClearAllResources();

	//if (m_pFonts != nullptr)
	//	m_pFonts->UninitialiseRendering();

	m_pCmdWindow->ReInitialise();

	// ANYTHING USING FONTS NEEDS TO BE GONE HERE FOR NOW!!!!!
	if (m_pCmdWindow != nullptr)
	{
		delete m_pCmdWindow;
		m_pCmdWindow = nullptr;
	}

	if (m_pChatWindow != nullptr && GetActiveMultiplayer() == nullptr) //&& !g_bStartNewGame && m_pMultiplayer == nullptr)
	{
		delete m_pChatWindow;
		m_pChatWindow = nullptr;
	}

	if (m_pChatWindow != nullptr) // Incase server disabled it
		m_pChatWindow->SetEnabled(true);

	m_GUISystem.Clear();

	m_Fonts.DeleteHWResources();
	m_SlotMgr.DeleteHWResources();

	m_bMultiplayerWorld = m_bForceMultiplayerWorld;

	if (m_pNewMultiplayer != nullptr)
	{
		m_pMultiplayer = m_pNewMultiplayer;
		m_pNewMultiplayer = nullptr;
		m_bMultiplayerWorld = true;
	}

	UpdateCursorEnabled();
}

void CClientGameII::ResetWorld()
{
	m_pResourceMgr->ClearAllResources();
	//m_pCmdWindow->ReInitialise();

	RegisterCommands();
}

void CClientGameII::LoadLobbyResource()
{
	m_pResourceMgr->RefreshResources(false);

	// Mark all resources as privileged
	for (auto it : m_pResourceMgr->m_Resources)
		it.second->m_bPrivileged = true;

	Stream* pStream = m_pContext->GetFileSystem()->Open(_gstr("/resources.xml"), false);
	if (pStream != nullptr)
	{
		CElementChunk* pRootElement = CElementChunk::Load(pStream);
		pStream->Release();
		if (pRootElement != nullptr)
		{
			const CElementChunk* pModulesElement = pRootElement->FirstChildElement(_gstr("modules"));
			if (pModulesElement != nullptr)
			{
				int iTotal = 0;
				int iSuccess = 0;
				int iErrors = 0;
				_glogprintf(_gstr("Loading modules..."));
				for (const CElementChunk* pResourceElement = pModulesElement->FirstChildElement(_gstr("module")); pResourceElement != nullptr; pResourceElement = pResourceElement->NextSiblingElement())
				{
					iTotal++;
					const GChar* pszName = pResourceElement->Attribute(_gstr("src"));
					if (pszName == NULL)
					{
						_glogerrorprintf(_gstr("Error loading module: attribute src not defined!"));
						iErrors++;
					}
					else
					{
						CString LauncherPath(false, CHackSupport::m_pInstance->m_LauncherPath.c_str(), CHackSupport::m_pInstance->m_LauncherPath.length());
						GString ModulePath = LauncherPath;
						CString Name(false, pszName);
						PathUtil::AppendPath(ModulePath, Name.CString());
						if (m_pResourceMgr->LoadModule(ModulePath.c_str()))
						{
							_glogprintf(_gstr("Loaded module %s"), pszName);
							iSuccess++;
						}
						else
						{
							_glogerrorprintf(_gstr("Error loading module %s!"), pszName);
							iErrors++;
						}
					}
				}
				_glogmessageprintf((iErrors > 0) ? LOGTYPE_WARN : LOGTYPE_INFO, _gstr("%i/%i Modules loaded, %i failed"), iSuccess, iTotal, iErrors);
			}

			// Modules may adjust classes, redefine those now!
			m_pResourceMgr->DefineAllClasses();

			const CElementChunk* pResourcesElement = pRootElement->FirstChildElement(_gstr("resources"));
			if (pResourcesElement != NULL)
			{
				int iTotal = 0;
				int iSuccess = 0;
				int iErrors = 0;
				_glogprintf(_gstr("Starting resources..."));
				for (const CElementChunk* pResourceElement = pResourcesElement->FirstChildElement(_gstr("resource")); pResourceElement != nullptr; pResourceElement = pResourceElement->NextSiblingElement())
				{
					iTotal++;
					const GChar* pszName = pResourceElement->Attribute(_gstr("src"));
					if (pszName == NULL)
					{
						_glogerrorprintf(_gstr("Error loading resource: attribute src not defined!"));
						iErrors++;
					}
					else
					{
						CClientResource* pResource;
						if ((pResource = static_cast<CClientResource*>(m_pResourceMgr->FindResourceByName(pszName))) != NULL)
						{
							if (pResource->Start())
								iSuccess++;
							else
							{
								_glogerrorprintf(_gstr("Error loading resource %s!"), pszName);
								iErrors++;
							}
						}
						else
						{
							_glogerrorprintf(_gstr("Error loading resource %s: resource not found!"), pszName);
							iErrors++;
						}
					}
				}
				_glogmessageprintf((iErrors > 0) ? LOGTYPE_WARN : LOGTYPE_INFO, _gstr("%i/%i Resources loaded, %i failed"), iSuccess, iTotal, iErrors);
			}

			delete pRootElement;
		}
	}
}

void CClientGameII::OnProcess()
{
	if (!IsCursorEnabled())
	{
		float fMouseXRel = 0.0f;
		float fMouseYRel = 0.0f;
		if (fMouseXRel != 0 && fMouseYRel != 0)
			m_pGalacticFunctions->OnMouseMove(0, false, CVector2D(fMouseXRel, fMouseYRel));
	}

	const Galactic3D::FrameTimeInfo* pTime = m_TimeManager.Process();
	m_pTime = pTime;

	uint32_t Ticks = OS::GetTicks();

	m_LastFrameTicks = Ticks;

	int width = 0;
	int height = 0;

	if (M2::C_MafiaFramework::GetActive())
	{
		HWND pWindow = GetActiveWindow();
		RECT rect;

		if (GetWindowRect(pWindow, &rect))
		{
			width = rect.right - rect.left;
			height = rect.bottom - rect.top;
		}
	}

	{
		m_GUISystem.m_fLeft = 0.0f;
		m_GUISystem.m_fTop = 0.0f;
		m_GUISystem.m_fWidth = (float)width;
		m_GUISystem.m_fHeight = (float)height;
		m_GUISystem.m_fClipLeft = 0.0f;
		m_GUISystem.m_fClipTop = 0.0f;
		m_GUISystem.m_fClipRight = m_GUISystem.m_fWidth;
		m_GUISystem.m_fClipBottom = m_GUISystem.m_fHeight;
		m_GUISystem.m_fScale = 1.0f;/* * ((float)GGlobal.iDPIX / 96.0f)*/;
		m_GUISystem.UpdateSite();
		m_GUISystem.UpdateClip();
	}

	if (m_pChatWindow != nullptr) {
		m_pChatWindow->Process((float)pTime->m_fDeltaTime);
	}

	//_glogprintf(_gstr("OnProcess delta time (%f)"), (float)pTime->m_fDeltaTime);
	m_GUISystem.Process((float)pTime->m_fDeltaTime);

#if MAFIAC_RMLUI
	m_pRmlUi->m_pRmlContext->SetDimensions(Rml::Vector2i(MafiaSDK::GetIGraph()->Scrn_sx(), MafiaSDK::GetIGraph()->Scrn_sy()));
	m_pRmlUi->Process();
#endif

	m_InternetRequestMgr.Process();

	if (m_pAudioScriptingFunctions != nullptr && m_pAudioScriptingFunctions->m_pSoundMgr != NULL)
	{
		m_pAudioScriptingFunctions->m_pSoundMgr->OnProcess(pTime); // TODO: Consider time?
	}

	if (m_bStopMultiplayerGame)
	{
		StopMultiplayerGame(m_iStopMultiplayerGameReason);
		m_bStopMultiplayerGame = false;
	}

	if (m_pResourceMgr != NULL)
	{
		m_pResourceMgr->Process(pTime->m_fDeltaTime);
	}

	UpdateCursorEnabled();

	if (m_pMultiplayer != NULL)
	{
		m_pMultiplayer->ProcessNewPeerElements();
		m_pMultiplayer->ProcessEachFrame();
	}

	if (m_pNewMultiplayer != nullptr)
	{
		m_pNewMultiplayer->ProcessEachFrame();
	}

	if (m_pGalacticFunctions != nullptr)
	{
		m_pGalacticFunctions->OnProcess(pTime);
	}

	m_pClientManager->OnProcess();
}

void CClientGameII::OnFrame()
{
	m_FPSCounter.Frame();

	if (m_pGalacticFunctions)
		m_pGalacticFunctions->m_bCursorEnabled = m_bCursorEnabled;
}

bool CClientGameII::OnKeyUp(const SDL_Event& Event)
{
	//_glogprintf(_gstr("Key up (%i)"), Event.key.keysym);

	if (!M2::C_MafiaFramework::GetActive())
		return false;

	if (m_pCmdWindow != nullptr)
	{
		if (m_pCmdWindow->IsEnabled())
		{
			if (m_pCmdWindow->OnKeyUp(Event.key.keysym))
				return true;
			return true;
		}
	}

	m_pGalacticFunctions->OnEvent(Event);
	m_pClientManager->TriggerKeyBind(Event.key.keysym, Event.key.repeat, false);

	return false;
}



bool CClientGameII::OnKeyDown(const SDL_Event& Event)
{

	if (!M2::C_MafiaFramework::GetActive())
		return false;

	//if (Event.key.keysym.sym == SDLK_ESCAPE && IsInputDisabled())
	//	m_bWaitingForEscapeKeyRelease = true;

	//if (m_pMultiplayer != nullptr)
	//{
	//	if (!m_pMultiplayer->IsConnected())
	//	{
	//		if (Event.key.keysym.sym == SDLK_ESCAPE)
	//		{
	//			StopMultiplayerGame();
	//			return true;
	//		}
	//	}
	//}

	if (m_pCmdWindow != NULL)
	{
		if (m_pCmdWindow->IsEnabled())
		{
			if (Event.key.keysym.sym == SDLK_BACKQUOTE)
			{
				m_pCmdWindow->Disable();
				UpdateCursorEnabled();
				return true;
			}
			else if (Event.key.keysym.sym == SDLK_RETURN || Event.key.keysym.sym == SDLK_RETURN2 || Event.key.keysym.sym == SDLK_KP_ENTER)
			{
				SetCursorEnabled(false);
				LockControls(false);

				//_glogprintf(_gstr("CmdWindow ProcessInput"));
				m_pResourceMgr->m_pCommandHandlers->PushLogger(m_pChatWindow);
				m_pCmdWindow->ProcessInput(m_pResourceMgr->m_pCommandHandlers);
				m_pResourceMgr->m_pCommandHandlers->PopLogger();
			}
			else if (Event.key.keysym.sym == SDLK_ESCAPE)
			{
				SetCursorEnabled(false);
				LockControls(false);
			}

			if (m_pCmdWindow->OnKeyDown(Event.key.keysym))
			{
				UpdateCursorEnabled();
				return true;
			}
			return true;
		}
		else
		{
			if (Event.key.keysym.sym == SDLK_t || Event.key.keysym.sym == SDLK_BACKQUOTE)
			{
				LockControls(true);
				m_bEnableCmdWindowOnCharPress = true;
				SetCursorEnabled(true);
				return true;
			}
		}
	}

	m_pGalacticFunctions->OnEvent(Event);
	m_pClientManager->TriggerKeyBind(Event.key.keysym, Event.key.repeat, true);

	return false;
}

void CClientGameII::OnCharacter(wchar_t c)
{
#if MAFIAC_RMLUI
	if (c >= 32)
		m_pRmlUi->m_pRmlContext->ProcessTextInput(Rml::Character(c));
#endif

	if (!M2::C_MafiaFramework::GetActive())
		return;

	if (GetMultiplayer() != nullptr)
	{
		if (!GetMultiplayer()->IsConnected())
		{
			return;
		}
	}

	if (m_pCmdWindow != NULL)
	{
		if (!m_pCmdWindow->IsEnabled() && m_pChatWindow->IsEnabled())
		{
			if (m_bEnableCmdWindowOnCharPress)
			{
				m_pCmdWindow->Enable();
				UpdateCursorEnabled();
				m_bEnableCmdWindowOnCharPress = false;

				LockControls(true);
			}
		}
	}
}

void CClientGameII::OnRender2DStuff()
{
	CVector2D vecSize;

	if (!M2::C_MafiaFramework::GetActive())
	{
		if (m_pCmdWindow != nullptr && m_pCmdWindow->IsEnabled())
		{
			m_pCmdWindow->Disable();
			UpdateCursorEnabled();
		}
		return;
	}

	//m_pOnDrawnHUDEventType->Trigger();

	int width = 0;
	int height = 0;

	if (M2::C_MafiaFramework::GetActive())
	{
		HWND pWindow = GetActiveWindow();
		RECT rect;
		m_pOnDrawnHUDEventType->Trigger();

		if (GetWindowRect(pWindow, &rect))
		{
			width = rect.right - rect.left;
			height = rect.bottom - rect.top;
		}
	}

	CMultiplayerII* pMultiplayer = GetMultiplayer();
	if (pMultiplayer != nullptr)
	{
		if (!pMultiplayer->IsConnected())
		{
			m_TextDrawing.RenderText(m_pGalacticFunctions->m_p2D, 0, _gstr("Connecting ..."), CVector2D(0.0f, height - (m_TextDrawing.m_fLargeSize * 2.0f)), width, 0.975f, 0.0f, Galactic3D::COLOUR::Lime, false);
		}
	}

	if (m_bShowGameStatistics)
	{
		GString Machines = _gstr("Machines: \n");
		{
			auto pMultiplayer = GetActiveMultiplayer();
			if (pMultiplayer != nullptr)
			{
				size_t Count = 0;
				for (size_t i = 0; i < ARRAY_COUNT(pMultiplayer->m_NetMachines.m_rgpMachines); i++)
				{
					auto pNetMachine = pMultiplayer->m_NetMachines.GetMachine(i);
					if (pNetMachine != nullptr)
					{
						GChar szIP[MAX_IPSTRING];
						pNetMachine->m_IPAddress.ToString(szIP, ARRAY_COUNT(szIP));
						GChar szBuffer[256];
						_gsnprintf(szBuffer, ARRAY_COUNT(szBuffer), _gstr("Name: %s\nIP: %s\nIndex: %d\nJoined: %s\nConsole: %s\nAdministrator: %s\n\n"), pNetMachine->GetName(), szIP, pNetMachine->m_nIndex, pNetMachine->m_bJoined ? _gstr("true") : _gstr("false"), pNetMachine->m_bConsole ? _gstr("true") : _gstr("false"), pNetMachine->m_bAdministrator ? _gstr("true") : _gstr("false"));
						Machines += szBuffer;
					}
				}
			}
		}

		float fPadding = 10.0f / 800.0f * width;

		m_TextDrawing.MeasureText(1, vecSize, Machines.c_str(), width - (fPadding * 2.0f), 0.0f, 0.0f, false);
		m_TextDrawing.RenderText(m_pGalacticFunctions->m_p2D, 1, Machines.c_str(), CVector2D(width - vecSize.x - fPadding, 200.0f), width - vecSize.x - (fPadding * 2.0f), 0.0f, 0.0f, Galactic3D::COLOUR::Lime, false, false, false, true);
	}

	if (m_bFPSCounter)
	{
		float fPadding = 10.0f / 800.0f * width;

		CVector2D vecSize;
		GString FPS = _gtostring(m_FPSCounter.GetFPS());

		m_TextDrawing.MeasureText(0, vecSize, FPS.c_str(), width - fPadding, 0.0f, 0.0f, false);
		m_TextDrawing.RenderText(m_pGalacticFunctions->m_p2D, 0, FPS.c_str(), CVector2D(fPadding, 0.0f), width - (fPadding * 2.0f), 1.0f, 0.0f, Galactic3D::COLOUR::Aqua, false, false, false, true);
	}

	if (m_pResourceMgr->m_pDownloadManager != NULL) // all codebase should support the download manager being NULL!
	{
		if (m_pResourceMgr->m_pDownloadManager->ShouldShowProgress())
		{
			float fProgress = m_pResourceMgr->m_pDownloadManager->GetProgress();
			CBufferedRectangle Rect(CVector2D(15.0f - 2.5f, width - 50.0f - 2.5f), CVector2D(width - 30.0f + 5.0f, 15.0f), Galactic3D::COLOUR::Black, Galactic3D::COLOUR::Black, Galactic3D::COLOUR::Black, Galactic3D::COLOUR::Black, 0.0f, CVector2D(0.0f, 0.0f));
			m_pGalacticFunctions->m_p2D->DrawRectangle(nullptr, Rect);
			Rect = CBufferedRectangle(CVector2D(15.0f, height - 50.0f), CVector2D(width - 30.0f, 10.0f), Galactic3D::COLOUR(0, 127, 0), Galactic3D::COLOUR(0, 127, 0), Galactic3D::COLOUR(0, 127, 0), Galactic3D::COLOUR(0, 127, 0), 0.0f, CVector2D(0.0f, 0.0f));
			m_pGalacticFunctions->m_p2D->DrawRectangle(nullptr, Rect);
			Rect = CBufferedRectangle(CVector2D(15.0f, height - 50.0f), CVector2D((width - 30.0f) * fProgress, 10.0f), Galactic3D::COLOUR(0, 255, 0), Galactic3D::COLOUR(0, 255, 0), Galactic3D::COLOUR(0, 255, 0), Galactic3D::COLOUR(0, 255, 0), 0.0f, CVector2D(0.0f, 0.0f));
			m_pGalacticFunctions->m_p2D->DrawRectangle(nullptr, Rect);
		}
	}

	if (m_pChatWindow != NULL) {
		m_pChatWindow->Draw(m_pGalacticFunctions->m_p2D);
	}

	m_GUISystem.m_bOverrideFocus = true;
	m_GUISystem.m_bFocused = true;

	m_GUISystem.Render(m_pGalacticFunctions->m_p2D);

#if MAFIAC_RMLUI
	m_pRmlUi->Render();
#endif
}

bool CClientGameII::OnCameraProcess()
{
	{
		bool bPreventDefault = false;
		m_pOnCameraProcessEventType->Trigger(bPreventDefault);

		if (bPreventDefault)
			return false;
	}

	return true;
}

void CClientGameII::NoName()
{
	m_pChatWindow->AddMessage(_gstr("Please set your name by using /setname <name>"), Galactic3D::COLOUR::Red);
}

bool CClientGameII::Connect(const GString& str)
{
	GString strHost;
	unsigned short usPort = 22000;
	bool bIPv6;
	size_t Index = str.find(' ');
	if (Index != GString::npos)
	{
		if (!StringManager::ParseHostAndPort(str.substr(0, Index), &strHost, &usPort, &bIPv6))
			return false;
		GString strPassword = str.substr(Index + 1);
		return Connect(strHost.c_str(), usPort, strPassword.c_str());
	}
	if (!StringManager::ParseHostAndPort(str, &strHost, &usPort, &bIPv6))
		return false;
	return Connect(strHost.c_str(), usPort, nullptr);
}

bool CClientGameII::Connect(const GString& str, const GChar* pszPassword)
{
	GString strHost;
	unsigned short usPort = 22000;
	bool bIPv6;
	if (!StringManager::ParseHostAndPort(str, &strHost, &usPort, &bIPv6))
		return false;
	return Connect(strHost.c_str(), usPort, pszPassword);
}

bool CClientGameII::Connect(const GChar* pszHost, unsigned short usPort, const GChar* pszPassword)
{
	if (GetMultiplayer() != NULL)
	{
		StopMultiplayerGame();
		m_pChatWindow->AddMessage(_gstr("Disconnected!"), Galactic3D::COLOUR::Red);
		return false;
	}
	{
		GString Name;
		if (!m_pContext->GetSettings()->GetString(NULL, _gstr("Name"), Name))
		{
			NoName();
			return false;
		}
	}
	//m_pResourceMgr->ClearAllResources();
	m_pNewMultiplayer = new CMultiplayerII(m_pClientManager, &m_CVars);

	m_pCmdWindow->FlushBuffers();
	m_pCmdWindow->Disable();

	if (m_pNewMultiplayer->Connect(pszHost, usPort, pszPassword))
	{
		if (m_szPreviousHost != pszHost)
		{
			m_bPreviousServerExists = true;
			_gstrlcpy(m_szPreviousHost, pszHost, ARRAY_COUNT(m_szPreviousHost));
			m_usPreviousPort = usPort;
			if (pszPassword == nullptr)
				m_szPreviousPassword[0] = '\0';
			else
				_gstrlcpy(m_szPreviousPassword, pszPassword, ARRAY_COUNT(m_szPreviousPassword));
		}

		m_pChatWindow->AddInfoMessage(_gstr("Connecting to %s:%d..."), pszHost, (unsigned int)usPort);

		m_CVars.Clear();

		return true;
	}
	else
	{
		m_pChatWindow->AddMessage(_gstr("Failed to connect!"), Galactic3D::COLOUR::Red);
		delete m_pNewMultiplayer;
		m_pNewMultiplayer = NULL;
	}

	return false;
}

void CClientGameII::StopMultiplayerGame(int iReason, bool bPreventRestart)
{
	CMultiplayerII* pMultiplayer = GetMultiplayer();
	if (pMultiplayer != nullptr)
	{
		tHackEventDataDiscordUpdate Data = {};
		Data.m_Size = sizeof(Data);
		Data.m_pszJoinSecret = _gstr("");
		TriggerHackEvent(HACKEVENT_DISCORD_UPDATE, &Data);

		m_pClientManager->OnDisconnect(iReason);
		bool bWasConnected = m_pMultiplayer != nullptr;
		if (m_pMultiplayer != nullptr)
			m_pMultiplayer->Disconnect(Galactic3D::DISCONNECT_GRACEFUL);
		m_pResourceMgr->m_pDownloadManager->ResetProgress();
		if (m_pMultiplayer != nullptr)
		{
			delete m_pMultiplayer;
			m_pMultiplayer = nullptr;
		}
		if (m_pNewMultiplayer != nullptr)
		{
			delete m_pNewMultiplayer;
			m_pNewMultiplayer = nullptr;
		}
		InitialiseCVars();

		if (!bPreventRestart && bWasConnected)
		{
			// Clear out CClientManager here
			m_pResourceMgr->ClearAllResources();
			m_pClientManager->Clear();
		}

		OnEndInGame();

		//MafiaSDK::GetMission()->MapLoad("FREERIDE");

		OnStartInGame(true);

		if (m_bReconnectOnDisconnect)
		{
			m_bReconnectOnDisconnect = false;

			if (m_bPreviousServerExists)
			{
				Connect(m_szPreviousHost, m_usPreviousPort, m_szPreviousPassword);
			}
		}
	}
}

void CClientGameII::StopMultiplayerGameWhenSafe(int iReason)
{
	if (m_pMultiplayer != nullptr || m_pNewMultiplayer != nullptr)
	{
		m_bStopMultiplayerGame = true;
		m_iStopMultiplayerGameReason = iReason;
	}
}

bool CClientGameII::CanRotateCamera()
{
	if (m_pCmdWindow != nullptr && m_pCmdWindow->IsEnabled())
		return false;
	if (IsCursorEnabled())
		return false;
	if (m_bScriptControlsDisabled)
		return false;
	return true;
}

bool CClientGameII::IsInputDisabled()
{
	if (!m_bFocused)
		return true;
	if (m_bFocusedSupressInput)
		return true;
	if (m_pCmdWindow != nullptr && m_pCmdWindow->IsEnabled())
		return true;
	if (m_GUISystem.m_pPage)
	{
		auto pPage = static_cast<LucasGUI::GUI::CPage*>(m_GUISystem.m_pPage->GetElementByName(_gstr("JoinRequest")));
		if (pPage != nullptr && pPage->m_bVisible)
			return true;
	}
	if (m_bScriptControlsDisabled)
		return true;
	return false;
}

bool CClientGameII::IsCursorEnabled()
{
	if (!m_bFocused)
		return true;
	return IsCursorEnabled2();
}

bool CClientGameII::IsCursorEnabled2()
{
	if (!M2::C_MafiaFramework::GetActive())
		return false;
	if (m_pCmdWindow != nullptr && m_pCmdWindow->IsEnabled())
		return true;
	if (m_GUISystem.m_pPage)
	{
		auto pPage = static_cast<LucasGUI::GUI::CPage*>(m_GUISystem.m_pPage->GetElementByName(_gstr("JoinRequest")));
		if (pPage != nullptr && pPage->m_bVisible)
			return true;
	}
	if (m_bScriptCursorEnabled)
		return true;
	if (m_pResourceMgr != nullptr && m_pResourceMgr->IsCursorEnabled())
		return true;
	return false;
}

bool CClientGameII::DontClipCursor()
{
	return M2::C_MafiaFramework::GetActive();
}

void CClientGameII::SetCursorClipped(bool bClipped, bool bForce)
{
	if (bForce || m_bMouseClipped)
	{
		::ClipCursor(nullptr);
		if (m_bMouseClipped && !DontClipCursor())
			SetCursorPos(m_OldCursorPos.x, m_OldCursorPos.y);
		m_bMouseClipped = false;
	}
}

void CClientGameII::SetCursorEnabled(bool bEnabled)
{
	m_bScriptCursorEnabled = bEnabled;

	UpdateCursorEnabled();
}

void CClientGameII::UpdateCursorEnabled(bool bForce)
{
	bool bCursorEnabled = IsCursorEnabled();
	bool bCursorClipped = !bCursorEnabled;
	SDL_ShowCursor(bCursorEnabled ? 1 : 0);
}

void CClientGameII::UpdateCursor(SDL_SystemCursor Cursor)
{
	if (m_bCapured) // HACK?
	{
		m_NewCursor = Cursor;
		return;
	}

	if (m_Cursor != Cursor)
	{
		if (!m_rgpCursors[Cursor])
			m_rgpCursors[Cursor] = SDL_CreateSystemCursor(Cursor);
		SDL_SetCursor(m_rgpCursors[Cursor]);
		m_Cursor = Cursor;
	}
}

void CClientGameII::UpdateCursor()
{
}

void CClientGameII::SetMouseCapture(bool bEnabled)
{
#ifdef _WIN32
	if (bEnabled)
	{
		SetCapture(GHWND);
		m_NewCursor = m_Cursor;
		m_bCapured = true;
	}
	else
	{
		if (m_bCapured)
		{
			ReleaseCapture();
			m_bCapured = false;
		}
	}
#else
	if (m_bCapured != bEnabled)
	{
		SDL_CaptureMouse(bEnabled ? SDL_TRUE : SDL_FALSE);
		m_bCapured = true;
	}
#endif
}

void CClientGameII::EnableInput(bool bEnabled)
{

}

void CClientGameII::OnEvent(const SDL_Event* event)
{
	if (!M2::C_MafiaFramework::GetActive())
		return;

	if (m_pResourceMgr == nullptr)
		return;

	m_GUISystem.OnEvent(*event);

	if (event->type == SDL_MOUSEMOTION && !IsCursorEnabled())
		return;

	switch (event->type)
	{
	case SDL_MOUSEMOTION:
	{
		SDL_SystemCursor Cursor;
		if (m_GUISystem.GetCursor(CVector2D((float)event->motion.x, (float)event->motion.y), Cursor))
			UpdateCursor(Cursor);
		else
			UpdateCursor(SDL_SYSTEM_CURSOR_ARROW);

#if MAFIAC_RMLUI
		m_pRmlUi->m_pRmlContext->ProcessMouseMove(event->motion.x, event->motion.y, m_pRmlUi->m_SystemInterface.GetKeyModifiers());
#endif
	}
	break;
	case SDL_MOUSEBUTTONDOWN:
	{
		if (event->button.which != SDL_TOUCH_MOUSEID)
		{
			// To allow drags to go outside the window
			if (event->button.button == SDL_BUTTON_LEFT)
			{
				SetMouseCapture(true);
			}
		}
#if MAFIAC_RMLUI
		m_pRmlUi->m_pRmlContext->ProcessMouseButtonDown(m_pRmlUi->m_SystemInterface.TranslateMouseButton(event->button.button), m_pRmlUi->m_SystemInterface.GetKeyModifiers());
#endif
	}
	break;
	case SDL_MOUSEBUTTONUP:
	{
		if (event->button.which != SDL_TOUCH_MOUSEID)
		{
			if (event->button.button == SDL_BUTTON_LEFT)
			{
				SetMouseCapture(false);
				UpdateCursor(m_NewCursor);

				{
					SDL_SystemCursor Cursor;
					int x, y;
					SDL_GetMouseState(&x, &y);
					m_GUISystem.OnMouseMove(CVector2D((float)x, (float)y));
					if (m_GUISystem.GetCursor(CVector2D((float)x, (float)y), Cursor))
						UpdateCursor(Cursor);
				}
			}
		}
#if MAFIAC_RMLUI
		m_pRmlUi->m_pRmlContext->ProcessMouseButtonUp(m_pRmlUi->m_SystemInterface.TranslateMouseButton(event->button.button), m_pRmlUi->m_SystemInterface.GetKeyModifiers());
#endif
	}
	break;
	case SDL_MOUSEWHEEL:
	{
#if MAFIAC_RMLUI
		m_pRmlUi->m_pRmlContext->ProcessMouseWheel(float(-event->wheel.y), m_pRmlUi->m_SystemInterface.GetKeyModifiers());
#endif
		return;
	}
	break;
	case SDL_KEYUP:
	{
		if (OnKeyUp(*event))
			m_bHandledKeyEvent = true;
#if MAFIAC_RMLUI
		m_pRmlUi->m_pRmlContext->ProcessKeyUp(m_pRmlUi->m_SystemInterface.TranslateKey(event->key.keysym.sym), m_pRmlUi->m_SystemInterface.GetKeyModifiers());
#endif

		return;
	}
	break;
	case SDL_KEYDOWN:
	{
		if (OnKeyDown(*event))
			m_bHandledKeyEvent = true;

#if MAFIAC_RMLUI
		m_pRmlUi->m_pRmlContext->ProcessKeyDown(m_pRmlUi->m_SystemInterface.TranslateKey(event->key.keysym.sym), m_pRmlUi->m_SystemInterface.GetKeyModifiers());
#endif
		return;
	}
	break;
	case SDL_WINDOWEVENT_FOCUS_LOST:
	case SDL_WINDOWEVENT_LEAVE:
		return;
	default:
		break;
	}

	m_pGalacticFunctions->OnEvent(*event);
}

bool CClientGameII::OnAction(const GChar* pszAction, LucasGUI::GUI::CButton* pButton)
{
	if (_gstrcmp(pszAction, _gstr("DiscordRespondYes")) == 0)
	{
		if (!m_DiscordUserId.empty())
		{
			tHackEventDataDiscordRespond Event;
			Event.m_Size = sizeof(Event);
			Event.m_pszUserId = m_DiscordUserId.c_str();
			Event.m_Reply = 1;
			TriggerHackEvent(HACKEVENT_DISCORD_RESPOND, &Event);
		}

		auto pPage = static_cast<LucasGUI::GUI::CPage*>(m_GUISystem.m_pPage->GetElementByName(_gstr("JoinRequest")));

		pPage->m_bVisible = false;
		m_DiscordUserId.clear();

		return true;
	}
	if (_gstrcmp(pszAction, _gstr("DiscordRespondNo")) == 0)
	{
		if (!m_DiscordUserId.empty())
		{
			tHackEventDataDiscordRespond Event;
			Event.m_Size = sizeof(Event);
			Event.m_pszUserId = m_DiscordUserId.c_str();
			Event.m_Reply = 0;
			TriggerHackEvent(HACKEVENT_DISCORD_RESPOND, &Event);
		}

		auto pPage = static_cast<LucasGUI::GUI::CPage*>(m_GUISystem.m_pPage->GetElementByName(_gstr("JoinRequest")));

		pPage->m_bVisible = false;
		m_DiscordUserId.clear();

		return true;
	}

	return false;
}

void CClientGameII::OnJoinRequest(tHackEventDataDiscordUser* pUser)
{
	if (!m_DiscordUserId.empty())
	{
		tHackEventDataDiscordRespond Event;
		Event.m_Size = sizeof(Event);
		Event.m_pszUserId = m_DiscordUserId.c_str();
		Event.m_Reply = 2;
		TriggerHackEvent(HACKEVENT_DISCORD_RESPOND, &Event);
		m_DiscordUserId.clear();
	}

	auto pPage = static_cast<LucasGUI::GUI::CPage*>(m_GUISystem.m_pPage->GetElementByName(_gstr("JoinRequest")));

	pPage->m_bVisible = true;

	m_DiscordUserId = pUser->userId;

	auto pMessage = static_cast<LucasGUI::GUI::CText*>(pPage->GetElementByName(_gstr("Message")));

	auto pAvatar = static_cast<LucasGUI::GUI::CRectangle*>(pPage->GetElementByName(_gstr("Avatar")));

	GString URL = _gstr("https://cdn.discordapp.com/avatars/");
	URL += pUser->userId;
	URL += _gstr("/");
	URL += pUser->avatar;
	URL += _gstr(".png");

	GString Message = pUser->username;
	Message += _gstr(" would like to join your game.");
	pMessage->SetText(Message.c_str());

	auto pRequest = new CInternetTextureRequest(&m_InternetRequestMgr);
	m_InternetRequestMgr.Push(pRequest, 0, URL.c_str());

	pAvatar->m_Material.SetTexture(pRequest->m_pTexture);
}

void CClientGameII::OnDeviceLost()
{
}

void CClientGameII::OnDeviceReset(struct IDirect3DDevice8* pD3DDevice)
{
}

void CClientGameII::LockControls(bool state)
{
	if (m_pClientManager == nullptr)
		return;
	if (m_pClientManager->m_pLocalPlayer == nullptr)
		return;
	if (m_pClientManager->m_pLocalPlayer.StaticCast<CClientPlayerII>()->GetGamePlayer() == nullptr)
		return;
	m_pClientManager->m_pLocalPlayer.StaticCast<CClientPlayerII>()->GetGamePlayer()->LockControls(state);
}

void CClientGameII::HumanEnteringVehicle(CClientHumanII* pClientHuman, CClientVehicleII* pClientVehicle, uint8_t iDoor, uint32_t iAction, uint32_t iHopSeatsBool)
{
	int8_t iSeat = iHopSeatsBool == 0 ? iDoor : (iDoor - 1);

	_glogprintf(_gstr("Human entering vehicle"));
	CArguments Args;
	Args.AddObject(pClientHuman);
	Args.AddObject(pClientVehicle);
	Args.AddNumber(iSeat);
	m_pOnHumanEnteringVehicleEventType->Trigger(Args);

	if (pClientHuman->IsSyncer()) {
		Packet Packet(MAFIAPACKET_HUMAN_ENTERINGVEHICLE);
		Packet.Write<int32_t>(pClientHuman->GetId());
		Packet.Write<int32_t>(pClientVehicle->GetId());
		Packet.Write<int8_t>(iDoor);
		Packet.Write<int32_t>(iAction);
		Packet.Write<int32_t>(iHopSeatsBool);
		m_pMultiplayer->SendHostPacket(&Packet);
	}
	else
	{
		g_pClientGame->m_bUseActorInvokedByGame = false;
		//pClientHuman->GetGameHuman()->Use_Actor(pClientVehicle->GetGameVehicle(), iAction, iDoor, iHopSeatsBool);
		g_pClientGame->m_bUseActorInvokedByGame = true;
	}

	pClientVehicle->AssignSeat(pClientHuman, iSeat);
}

void CClientGameII::HumanEnteredVehicle(CClientHumanII* pClientHuman, CClientVehicleII* pClientVehicle, uint8_t iSeat, uint32_t iAction, uint32_t iUnknown)
{
	_glogprintf(_gstr("Human entered vehicle"));
	CArguments Args;
	Args.AddObject(pClientHuman);
	Args.AddObject(pClientVehicle);
	Args.AddNumber(iSeat);
	m_pOnHumanEnteredVehicleEventType->Trigger(Args);

	if (pClientHuman->IsSyncer()) {
		Packet Packet(MAFIAPACKET_HUMAN_ENTEREDVEHICLE);
		Packet.Write<int32_t>(pClientHuman->GetId());
		Packet.Write<int32_t>(pClientVehicle->GetId());
		Packet.Write<int8_t>(iSeat);
		Packet.Write<int32_t>(iAction);
		Packet.Write<int32_t>(iUnknown);
		m_pMultiplayer->SendHostPacket(&Packet);
	}
	else
	{
		//g_pClientGame->m_bUseActorInvokedByGame = false;
		//pClientHuman->GetGameHuman()->Use_Actor(pClientVehicle->GetGameVehicle(), iAction, iSeat, iUnknown);
		//g_pClientGame->m_bUseActorInvokedByGame = true;
	}

	//pClientVehicle->AssignSeat(pClientHuman, iSeat);
}

void CClientGameII::HumanExitingVehicle(CClientHumanII* pClientHuman, CClientVehicleII* pClientVehicle, uint8_t iUnknown1, uint32_t iAction, uint32_t iUnknown2)
{
	int8_t iSeat = pClientHuman->GetVehicleSeat();

	_glogprintf(_gstr("Human exiting vehicle"));
	CArguments Args;
	Args.AddObject(pClientHuman);
	Args.AddObject(pClientVehicle);
	Args.AddNumber(iSeat);
	m_pOnHumanExitingVehicleEventType->Trigger(Args);

	if (pClientHuman->IsSyncer()) {
		Packet Packet(MAFIAPACKET_HUMAN_EXITINGVEHICLE);
		Packet.Write<int32_t>(pClientHuman->GetId());
		Packet.Write<int32_t>(pClientVehicle->GetId());
		Packet.Write<int8_t>(iUnknown1);
		Packet.Write<int32_t>(iAction);
		Packet.Write<int32_t>(iUnknown2);
		m_pMultiplayer->SendHostPacket(&Packet);
	}
	else
	{
		g_pClientGame->m_bUseActorInvokedByGame = false;
		// Use game SDK to enter vehicle
		// pClientHuman->GetGameHuman()->Use_Actor(pClientVehicle->GetGameVehicle(), iAction, iUnknown1, iUnknown2);
		g_pClientGame->m_bUseActorInvokedByGame = true;
	}

	pClientVehicle->FreeSeat(iSeat);
}

void CClientGameII::HumanExitedVehicle(CClientHumanII* pClientHuman, CClientVehicleII* pClientVehicle, uint8_t iSeat, uint32_t iAction, uint32_t iUnknown)
{
	_glogprintf(_gstr("Human exited vehicle"));
	CArguments Args;
	Args.AddObject(pClientHuman);
	Args.AddObject(pClientVehicle);
	Args.AddNumber(iSeat);
	m_pOnHumanExitedVehicleEventType->Trigger(Args);

	if (pClientHuman->IsSyncer()) {
		Packet Packet(MAFIAPACKET_HUMAN_EXITEDVEHICLE);
		Packet.Write<int32_t>(pClientHuman->GetId());
		Packet.Write<int32_t>(pClientVehicle->GetId());
		Packet.Write<int8_t>(iSeat);
		Packet.Write<int32_t>(iAction);
		Packet.Write<int32_t>(iUnknown);
		m_pMultiplayer->SendHostPacket(&Packet);
	}
	else
	{
		//g_pClientGame->m_bUseActorInvokedByGame = false;
		//pClientHuman->GetGameHuman()->Use_Actor(pClientVehicle->GetGameVehicle(), iAction, iSeat, iUnknown);
		//g_pClientGame->m_bUseActorInvokedByGame = true;
	}

	//pClientVehicle->FreeSeat(iSeat);
}

void CClientGameII::HumanJackVehicle(CClientHumanII* pClientHuman, CClientVehicleII* pClientVehicle, uint8_t iSeat)
{
	CArguments Args;
	Args.AddObject(pClientHuman);
	Args.AddObject(pClientVehicle);
	Args.AddNumber(iSeat);
	m_pOnHumanJackVehicleEventType->Trigger(Args);

	if (pClientHuman->IsSyncer())
	{
		Packet Packet(MAFIAPACKET_HUMAN_JACKVEHICLE);
		Packet.Write<int32_t>(pClientHuman->GetId());
		Packet.Write<int32_t>(pClientVehicle->GetId());
		Packet.Write<int8_t>(iSeat);
		m_pMultiplayer->SendHostPacket(&Packet);
	}
	else
	{
		g_pClientGame->m_bDoThrowCocotFromCarInvokedByGame = false;
		//pClientHuman->GetGameHuman()->Do_ThrowCocotFromCar(pClientVehicle->GetGameVehicle(), iSeat);
		g_pClientGame->m_bDoThrowCocotFromCarInvokedByGame = true;
	}
}

void CClientGameII::HumanHit(CClientHumanII* pClientHumanTarget, CVector3D vec1, CVector3D vec2, CVector3D vec3, int uiHitType, float fDamage, int uiBodyPart)
{
	bool bPreventDefault = false;
	CArguments args;
	args.AddObject(pClientHumanTarget);
	//args.AddObject(pClientHumanAttacker);
	args.AddVector3D(vec1);
	args.AddVector3D(vec2);
	args.AddVector3D(vec3);
	args.AddNumber(uiHitType);
	args.AddNumber(fDamage);
	args.AddNumber(uiBodyPart);
	g_pClientGame->m_pOnHumanHitEventType->Trigger(args, bPreventDefault);

	if (bPreventDefault)
		return;

	auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
	if (pMultiplayer != nullptr)
	{
		pMultiplayer->SendHumanHit(pClientHumanTarget, vec1, vec2, vec3, uiHitType, fDamage, uiBodyPart);
	}

	float fOldHealth = pClientHumanTarget->GetHealth();
	float fNewHealth = fOldHealth - fDamage;

	if (fNewHealth <= 0.0f)
	{
		fNewHealth = 0.0f;
	}

	_glogprintf(_gstr("Old Health: %f\nNew Health: %f\nDamage: %f"), fOldHealth, fNewHealth, fDamage);

	pClientHumanTarget->SetHealth(fNewHealth);

	if (fNewHealth <= 0.0f)
	{
		pClientHumanTarget->Kill();

		CArguments args;
		args.AddObject(pClientHumanTarget);
		//args.AddObject(entityAttacker);
		g_pClientGame->m_pOnHumanDeathEventType->Trigger(args);

		auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
		if (pMultiplayer != nullptr)
		{
			//pMultiplayer->SendHumanDeath(pClientHumanTarget, entityAttacker);
			pMultiplayer->SendHumanDeath(pClientHumanTarget, nullptr);
		}
	}
}

void CClientGameII::DestroyUninitializedGameElements()
{

}

/*
bool CClientGameII::OnTrafficCarRespawn(CClientVehicleII* pClientVehicle, MafiaSDK::C_Car* pCar)
{
	pClientVehicle->SetFromExistingEntity(pCar);

	return true;
}
*/

bool CClientGameII::IsGameComponentEnabled(eGameComponent GameComponent)
{
	switch (GameComponent)
	{
	case GAMECOMPONENT_TRAFFIC:
		return m_CVars.GetBoolean(_gstr("Traffic"), true);
	case GAMECOMPONENT_CIVILIANS:
		return m_CVars.GetBoolean(_gstr("Civilians"), true);
	//case GAMECOMPONENT_SCRIPTS:
	//	return m_CVars.GetBoolean(_gstr("Scripts"), true);
	//case GAMECOMPONENT_BRIDGES:
	//	return m_CVars.GetBoolean(_gstr("Bridges"), true);
	//case GAMECOMPONENT_TROLLEYS:
	//	return m_CVars.GetBoolean(_gstr("Trains"), true);
	//case GAMECOMPONENT_TRAINS:
	//	return m_CVars.GetBoolean(_gstr("Trains"), true);
	//case GAMECOMPONENT_DOGS:
	//	return m_CVars.GetBoolean(_gstr("Dogs"), true);
	//case GAMECOMPONENT_FUELSTATIONS:
	//	return m_CVars.GetBoolean(_gstr("FuelStations"), true);
	//case GAMECOMPONENT_DOORS:
	//	return m_CVars.GetBoolean(_gstr("Doors"), true);
	//case GAMECOMPONENT_PLANES:
	//	return m_CVars.GetBoolean(_gstr("Planes"), true);
	//case GAMECOMPONENT_BIGMAP:
	//	return m_CVars.GetBoolean(_gstr("BigMap"), true);
	default:
		break;
	}
	return true;
}

void CClientGameII::ShowDisconnectReason()
{
	const GChar* rgpszReasons[] = {
		_gstr("TIMEOUT"),
		_gstr("FULL"),
		_gstr("UNSUPPORTED CLIENT"),
		_gstr("UNSUPPORTED ENGINE"),
		_gstr("WRONG PASSWORD"),
		_gstr("UNSUPPORTED EXECUTABLE"),
		_gstr("GRACEFUL"),
		_gstr("BANNED"),
		_gstr("FAILED"),
		_gstr("INVALID NAME"),
		_gstr("CRASH"),
		_gstr("PUBLIC KEY MISMATCH"),
		_gstr("NAME IN USE"),
		_gstr("KICKED")
	};
	m_pChatWindow->AddMessage(_gstr("Disconnected [%s]"), Galactic3D::COLOUR::Red, rgpszReasons[m_iStopMultiplayerGameReason]);
}