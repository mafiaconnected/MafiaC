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
};

class CClientPlayerII;

class CMafiaCHtmlContainerII : public CHtmlContainer
{
public:
	CMafiaCHtmlContainerII(Context* pContext, class CClientGameII* pClientGame);

	CClientGameII* m_pClientGame;

	virtual Galactic3D::Stream* OpenFile(const GChar* pszPath) override;
	virtual void on_anchor_click(const litehtml::tchar_t* url, const litehtml::element::ptr& el) override;
};

#ifdef MAFIA_NAMETAGS
class CNametagsII
{
public:
	CNametagsII(Galactic3D::Context* pContext, LucasGUI::CFonts* pFonts, float fScale);
	~CNametagsII();

	Galactic3D::Context* m_pContext;
	LucasGUI::CFonts* m_pFonts;
	LucasGUI::CFontStack* m_pFontStack;
	float m_fScale;
	float m_fSize;

	void renderNametag(Galactic3D::I2D* p2D, const CVector2D& vecPos, float fHealth, const GChar* pszText, float fAlpha);
	void Draw(Galactic3D::I2D* p2D);
};
#endif

class CTextDrawingII
{
public:
	CTextDrawingII();

	float m_fLargeSize;
	LucasGUI::CFontStack m_LargeFontStack;

	float m_fSmallSize;
	LucasGUI::CFontStack m_SmallFontStack;

	void Initialise(LucasGUI::CFonts& Fonts);
	void ShutDown();

	void MeasureText(size_t Font, CVector2D& vecSize, const GChar* pszText, float fWidth, float fAlign, float fJustify, bool bWordWrap, bool bColourCodes = true);
	void RenderText(Galactic3D::I2D* p2D, size_t Font, const GChar* pszText, const CVector2D& vecOffset, float fWidth, float fAlign, float fJustify, Galactic3D::COLOUR Colour, bool bWordWrap, bool bColourCodes = true, bool bIgnoreColourCodes = false, bool bShadow = false);
};

class CClientGameII
{
public:
	CClientGameII(Galactic3D::Context* pContext);
	~CClientGameII(void);

	Galactic3D::Context* m_pContext;
	Galactic3D::CVarSystem m_CVars;
	bool m_bCEGUIInitialised;
	class CMafiaClientManagerII* m_pClientManager;
	Galactic3D::CInternetCache m_InternetCache;
	Galactic3D::CInternetRequestMgr m_InternetRequestMgr;
	bool m_bStopMultiplayerGame;
	int m_iStopMultiplayerGameReason;
	bool m_bMultiplayerWorld;
	bool m_bForceMultiplayerWorld;
	class CMultiplayerII* m_pMultiplayer;
	class CMultiplayerII* m_pNewMultiplayer;
	class CMafiaClientResourceMgrII* m_pResourceMgr;
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
	CTextDrawingII m_TextDrawing;
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
	CNametagsII* m_pNametags;
#endif

	CHtmlContainer* m_pHtmlContainer;
	litehtml::context m_BrowserContext;
	CHtmlView* m_pHtmlView;

	bool m_bCapured;

	bool m_bSupressNetworkedEntities;

	bool m_b2DReady;
	bool m_bChangeMap;

	GChar m_szMap[32];

	bool m_bUseActorInvokedByGame = true;
	bool m_bDoThrowCocotFromCarInvokedByGame = true;
	bool m_bCreateActorInvokedByGame = true;
	bool m_bHumanSetAimPoseInvokedByGame = true;
	bool m_bHumanSetNormalPoseInvokedByGame = true;

	bool m_bReconnectOnDisconnect;
	bool m_bFullReload;

	inline CMultiplayerII* GetMultiplayer(void) { if (m_pNewMultiplayer != nullptr) return m_pNewMultiplayer; return m_pMultiplayer; }
	inline CMultiplayerII* GetActiveMultiplayer(void) { return m_pMultiplayer; }
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

	void HumanEnteringVehicle(CClientHumanII* pClientHuman, CClientVehicleII* pClientVehicle, uint8_t iDoor, uint32_t iAction, uint32_t iHopSeatsBool);
	void HumanEnteredVehicle(CClientHumanII* pClientHuman, CClientVehicleII* pClientVehicle, uint8_t iSeat, uint32_t iAction, uint32_t iUnknown);
	void HumanExitingVehicle(CClientHumanII* pClientHuman, CClientVehicleII* pClientVehicle, uint8_t iDoor, uint32_t iAction, uint32_t iUnknown);
	void HumanExitedVehicle(CClientHumanII* pClientHuman, CClientVehicleII* pClientVehicle, uint8_t iUnknown1, uint32_t iAction, uint32_t iUnknown2);
	void HumanJackVehicle(CClientHumanII* pClientHuman, CClientVehicleII* pClientVehicle, uint8_t iSeat);
	void HumanHit(CClientHumanII* humanTarget, CVector3D vv1, CVector3D vv2, CVector3D vv3, int hitType, float damage, int bodyPart);

	void DestroyUninitializedGameElements();

	M2::eEntityType ToMafiaEntityType(int entityType);

	bool IsGameComponentEnabled(eGameComponent GameComponent);
};

extern CClientGameII* g_pClientGame;

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
