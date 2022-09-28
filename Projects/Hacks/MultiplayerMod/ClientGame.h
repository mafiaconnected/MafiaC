#pragma once

#include <Engine/KeyScanCodes.h>
#include <Audio/Audio.h>
#include <LucasGUI/GUI/GUI.h>
#include <LucasGUI/LanguageLocalisation.h>
#include <LucasGUI/Fonts.h>
#include <LucasGUI/DumbUtil.h>
#include <LucasGUI/HtmlView.h>
#include <LucasFont/LucasFont.h>
#include <Network/InternetRequestMgr.h>
#include "Elements/Elements.h"
#include "RmlUi.h"
//#include <GameVersion.h>

//eGameVersion g_GameVersion;

#define MAFIA_NAMETAGS

namespace Galactic3D
{
	class CClientUpdater;
	class CChatWindow;
	class CCmdWindow2;
	class CClientDownloadManager;
};

enum eGameComponent
{
	GAMECOMPONENT_TRAFFIC,
	GAMECOMPONENT_CIVILIANS,
	GAMECOMPONENT_TRAINS,
	GAMECOMPONENT_BRIDGES,
	GAMECOMPONENT_SCRIPTS,
	GAMECOMPONENT_TROLLEYS,
	GAMECOMPONENT_DOORS,
	GAMECOMPONENT_DOGS,
	GAMECOMPONENT_FUELSTATIONS, 
	GAMECOMPONENT_PLANES,
};

class CClientPlayer;

class CMafiaCHtmlContainer : public CHtmlContainer
{
public:
	CMafiaCHtmlContainer(Context* pContext, class CClientGame* pClientGame);

	CClientGame* m_pClientGame;

	virtual Galactic3D::Stream* OpenFile(const GChar* pszPath) override;
	virtual void on_anchor_click(const litehtml::tchar_t* url, const litehtml::element::ptr& el) override;
};

#ifdef MAFIA_NAMETAGS
class CNametags
{
public:
	CNametags(Galactic3D::Context* pContext, LucasGUI::CFonts* pFonts, float fScale);
	~CNametags();

	Galactic3D::Context* m_pContext;
	LucasGUI::CFonts* m_pFonts;
	LucasGUI::CFontStack* m_pFontStack;
	float m_fScale;
	float m_fSize;

	void renderNametag(Galactic3D::I2D* p2D, const CVector2D& vecPos, float fHealth, const GChar* pszText, float fAlpha);
	void Draw(Galactic3D::I2D* p2D);
};
#endif

class CTextDrawing
{
public:
	CTextDrawing();

	float m_fLargeSize;
	LucasGUI::CFontStack m_LargeFontStack;

	float m_fSmallSize;
	LucasGUI::CFontStack m_SmallFontStack;

	void Initialise(LucasGUI::CFonts& Fonts);
	void ShutDown();

	void MeasureText(size_t Font, CVector2D& vecSize, const GChar* pszText, float fWidth, float fAlign, float fJustify, bool bWordWrap, bool bColourCodes = true);
	void RenderText(Galactic3D::I2D* p2D, size_t Font, const GChar* pszText, const CVector2D& vecOffset, float fWidth, float fAlign, float fJustify, Galactic3D::COLOUR Colour, bool bWordWrap, bool bColourCodes = true, bool bIgnoreColourCodes = false, bool bShadow = false);
};

class CClientGame
{
public:
	CClientGame(Galactic3D::Context* pContext);
	~CClientGame(void);

	Galactic3D::Context* m_pContext;
	Galactic3D::CVarSystem m_CVars;
	bool m_bCEGUIInitialised;
	class CMafiaClientManager* m_pClientManager;
	Galactic3D::CInternetCache m_InternetCache;
	Galactic3D::CInternetRequestMgr m_InternetRequestMgr;
	bool m_bStopMultiplayerGame;
	int m_iStopMultiplayerGameReason;
	bool m_bMultiplayerWorld;
	bool m_bForceMultiplayerWorld;
	class CMultiplayer* m_pMultiplayer;
	class CMultiplayer* m_pNewMultiplayer;
	class CMafiaClientResourceMgr* m_pResourceMgr;
	Galactic3D::CGalacticFunctions* m_pGalacticFunctions;
	Galactic3D::Audio::CAudioScriptingFunctions* m_pAudioScriptingFunctions;
	Galactic3D::CLucasFontFunctions m_LucasFontFunctions;
	Galactic3D::EventHandlers::CEventType* m_pOnEntityProcessEventType;
	Galactic3D::EventHandlers::CEventType* m_pOnDrawHUDEventType;
	Galactic3D::EventHandlers::CEventType* m_pOnDrawnHUDEventType;
	Galactic3D::EventHandlers::CEventType* m_pOnCameraProcessEventType;
	Galactic3D::EventHandlers::CEventType* m_pOnGUIAnchorClickEventType;
	Galactic3D::EventHandlers::CEventType* m_pOnGUIClickEventType;
	Galactic3D::EventHandlers::CEventType* m_pOnExampleEventType;
	Galactic3D::EventHandlers::CEventType* m_pOnMapLoadedEventType;
	Galactic3D::EventHandlers::CEventType* m_pOnKeyPressedEventType;
	Galactic3D::EventHandlers::CEventType* m_pOnHumanHitEventType;
	Galactic3D::EventHandlers::CEventType* m_pOnHumanSpawnEventType;
	Galactic3D::EventHandlers::CEventType* m_pOnHumanDeathEventType;
	Galactic3D::EventHandlers::CEventType* m_pOnHumanEnteringVehicleEventType;
	Galactic3D::EventHandlers::CEventType* m_pOnHumanEnteredVehicleEventType;
	Galactic3D::EventHandlers::CEventType* m_pOnHumanExitingVehicleEventType;
	Galactic3D::EventHandlers::CEventType* m_pOnHumanExitedVehicleEventType;
	Galactic3D::EventHandlers::CEventType* m_pOnHumanJackVehicleEventType;

	Galactic3D::CChatWindow* m_pChatWindow;
	Galactic3D::CCmdWindow2* m_pCmdWindow;
	LucasGUI::GUI::CPage* m_pCmdWindowPage;
	Galactic3D::CClientDownloadManager* m_pDownloadManager;
	Galactic3D::TimeManager m_TimeManager;
	LucasGUI::CFonts m_Fonts;
	CTextDrawing m_TextDrawing;
	bool m_bTrainsEnabled;

	//CProxy2DRenderer m_Proxy2D;
	C2D m_p2D;

	bool m_bD3D9;
	bool m_bDebugMode;

	POINT m_OldCursorPos;

	int m_iIcon;

	bool m_bMouseClipped;
	SDL_SystemCursor m_Cursor;
	SDL_SystemCursor m_NewCursor;
	SDL_Cursor* m_rgpCursors[SDL_NUM_SYSTEM_CURSORS];

	bool m_bScriptCursorEnabled;
	bool m_bScriptControlsDisabled;
private:
	bool m_bFocused;
public:
	bool m_bFocusedSupressInput;
private:
	bool m_bCursorEnabled;
	bool m_bEnableCmdWindowOnCharPress;
public:
	bool m_bShowGameStatistics;
	bool m_bFPSCounter;

	bool m_bHandledKeyEvent;

	const Galactic3D::FrameTimeInfo* m_pTime;
	Uint32 m_LastFrameTicks;

	bool m_bPreviousServerExists;
	GChar m_szPreviousHost[256];
	unsigned short m_usPreviousPort;
	GChar m_szPreviousPassword[1024];

	LucasGUI::CLanguageLocalisation m_LanguageLocalisation;
	LucasGUI::GUI::CSystem m_GUISystem;
	SlotMgr m_SlotMgr;

	CDiagnosticsCounter m_FPSCounter;

	GString m_DiscordUserId;

#ifdef MAFIA_NAMETAGS
	CNametags* m_pNametags;
#endif

	CHtmlContainer* m_pHtmlContainer;
	litehtml::context m_BrowserContext;
	CHtmlView* m_pHtmlView;

#if MAFIAC_RMLUI
	CRmlUi2* m_pRmlUi;
#endif

	bool m_bCapured;

	bool m_bSupressNetworkedEntities;

	bool m_b2DReady;
	bool m_bChangeMap;

	GChar m_szMap[32];

	bool m_bUseActorInvokedByGame = true;
	bool m_bDoThrowCocotFromCarInvokedByGame = true;
	bool m_bCreateActorInvokedByGame = true;
	bool m_bCreateVehicleInvokedByGame = true;
	bool m_bHumanSetAimPoseInvokedByGame = true;
	bool m_bHumanSetNormalPoseInvokedByGame = true;

	bool m_bChangingMaps = false;

	inline CMultiplayer* GetMultiplayer(void) { if (m_pNewMultiplayer != nullptr) return m_pNewMultiplayer; return m_pMultiplayer; }
	inline CMultiplayer* GetActiveMultiplayer(void) { return m_pMultiplayer; }
	inline bool IsMultiplayer() { return m_bMultiplayerWorld || m_pMultiplayer != nullptr; }

	void Initialise();
	void ShutDown();

	void InitialiseCVars();

	void InitialiseScripting(void);
	void ShutdownScripting(void);

	void LoadFonts();

	bool OnWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	void OnPostWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	void OnPreStartInGame(bool bRestarted);
	void OnStartInGame(bool bRestarted);
	void OnEndInGame(void);

	void RegisterCommands();
	void ResetWorld(void);
	void LoadLobbyResource(void);

	void ShowDisconnectReason();

	void OnProcess(void);
	void OnFrame(void);

	bool OnKeyUp(const SDL_Event& Event);
	bool OnKeyDown(const SDL_Event& Event);
	void OnCharacter(wchar_t c);

	void OnRender2DStuff(void);

	bool OnCameraProcess(void);

	void NoName(void);

	bool Connect(const GString& str);
	bool Connect(const GString& str, const GChar* pszPassword);
	bool Connect(const GChar* pszHost, unsigned short usPort, const GChar* pszPassword);
	void StopMultiplayerGame(int iReason = Galactic3D::DISCONNECT_GRACEFUL, bool bPreventRestart = false);
	void StopMultiplayerGameWhenSafe(int iReason = Galactic3D::DISCONNECT_GRACEFUL);

	bool CanRotateCamera(void);
	bool IsInputDisabled(void);
	bool IsCursorEnabled(void);
	bool IsCursorEnabled2(void);
	bool IsDebugMode(void);
	bool UsePlayerInfo();

	bool DontClipCursor();
	void SetCursorClipped(bool bClipped, bool bForce);
	void SetCursorEnabled(bool bEnabled);
	void UpdateCursorEnabled(bool bForce = false);
	void UpdateCursor(SDL_SystemCursor Cursor);
	void UpdateCursor(void);
	void SetMouseCapture(bool bEnabled);
	void EnableInput(bool bEnabled);

	void OnEvent(const SDL_Event *event);
	bool OnAction(const GChar* pszAction, LucasGUI::GUI::CButton* pButton);
	void OnJoinRequest(tHackEventDataDiscordUser* pUser);

	void OnDeviceLost(void);
	void OnDeviceReset(struct IDirect3DDevice8* pD3DDevice);

	void LockControls(bool state);

	void HumanEnteringVehicle(CClientHuman* pClientHuman, CClientVehicle* pClientVehicle, uint8_t iDoor, uint32_t iAction, uint32_t iHopSeatsBool);
	void HumanEnteredVehicle(CClientHuman* pClientHuman, CClientVehicle* pClientVehicle, uint8_t iSeat, uint32_t iAction, uint32_t iUnknown);
	void HumanExitingVehicle(CClientHuman* pClientHuman, CClientVehicle* pClientVehicle, uint8_t iDoor, uint32_t iAction, uint32_t iUnknown);
	void HumanExitedVehicle(CClientHuman* pClientHuman, CClientVehicle* pClientVehicle, uint8_t iUnknown1, uint32_t iAction, uint32_t iUnknown2);
	void HumanJackVehicle(CClientHuman* pClientHuman, CClientVehicle* pClientVehicle, uint8_t iSeat);
	void HumanHit(CClientHuman* humanTarget, CVector3D vv1, CVector3D vv2, CVector3D vv3, int hitType, float damage, int bodyPart);

	void DestroyUninitializedGameElements();

	bool OnTrafficCarCreate(MafiaSDK::C_Car* pCar);
	//bool OnTrafficCarReset(MafiaSDK::C_Car* pCar);
	bool OnTrafficCarRespawn(CClientVehicle *pClientVehicle, MafiaSDK::C_Car* pCar);

	bool IsGameComponentEnabled(eGameComponent GameComponent);
};

extern CClientGame* g_pClientGame;

class CSuppressNetworkedEntities
{
public:
	inline CSuppressNetworkedEntities(bool bNetworked = false)
	{
		m_bOldValue = g_pClientGame->m_bSupressNetworkedEntities;
		g_pClientGame->m_bSupressNetworkedEntities = !bNetworked;
	}

	inline ~CSuppressNetworkedEntities()
	{
		g_pClientGame->m_bSupressNetworkedEntities = m_bOldValue;
	}

private:
	bool m_bOldValue;
};
