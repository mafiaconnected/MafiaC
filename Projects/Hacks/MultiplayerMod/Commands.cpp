
#include "stdafx.h"
#include "ClientGame.h"
#include "ClientManager.h"
#include "Commands.h"
#include "Multiplayer.h"
#include <Multiplayer/ChatWindow.h>
#include <Multiplayer/CmdWindow.h>

extern bool g_bSkipMenu;

void CMafiaCSayCommandHandler::Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient)
{
	if (g_pClientGame->m_pMultiplayer != NULL)
		g_pClientGame->m_pMultiplayer->SendChat(pszArguments, _gstrlen(pszArguments));
	else
	{
#ifdef _DEBUG
		g_pClientGame->m_pChatWindow->AddMessage(_gstr("%s"), CHAT_TYPE_CHAT, pszArguments);
#else
		m_pCommandHandlers->m_pLogger->LogFormatted(LOGTYPE_WARN, _gstr("Not connected to a server!"));
#endif
	}
}

void CMafiaCUnhandledCommandHandler::Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient)
{
	if (g_pClientGame->m_pMultiplayer != NULL)
		g_pClientGame->m_pMultiplayer->SendCommand(pszCommandName, _gstrlen(pszCommandName), pszArguments, (pszArguments == nullptr) ? 0 : _gstrlen(pszArguments));
	else
		CUnhandledCommandHandler::Execute(pszCommandName, pszArguments, pClient);
}

void CSetNameCommandHandler::Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient)
{
	if (pszArguments == nullptr || _gstrlen(pszArguments) >= NETGAME_MAX_NAME_BUFFER)
	{
		m_pCommandHandlers->m_pLogger->LogFormatted(LOGTYPE_WARN, _gstr("Invalid name!"));
		return;
	}

	g_pClientGame->m_pContext->GetSettings()->Write(NULL, _gstr("Name"), pszArguments);
	g_pClientGame->m_pContext->GetSettings()->Flush();

	if (g_pClientGame->m_pMultiplayer != NULL)
		m_pCommandHandlers->m_pLogger->LogFormatted(LOGTYPE_INFO, _gstr("Reconnect for name to change on this server!"));
	else
		m_pCommandHandlers->m_pLogger->LogFormatted(LOGTYPE_INFO, _gstr("Name changed!"));
}

void CConnectCommandHandler::Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient)
{
	if (g_pClientGame->m_pMultiplayer != nullptr)
	{
		g_pClientGame->StopMultiplayerGame(DISCONNECT_GRACEFUL, true);
	}

	if (pszArguments == nullptr)
		m_pCommandHandlers->m_pLogger->LogFormatted(LOGTYPE_WARN, _gstr("Nothing specified!"));
	else
	{
		if (!g_pClientGame->Connect(pszArguments))
			m_pCommandHandlers->m_pLogger->LogFormatted(LOGTYPE_WARN, _gstr("Unable to connect!"));
	}
}

void CReconnectCommandHandler::Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient)
{
	auto pMultiplayer = g_pClientGame->GetMultiplayer();

	if (pMultiplayer != nullptr)
	{
		g_pClientGame->m_bReconnectOnDisconnect = true;
		g_pClientGame->StopMultiplayerGameWhenSafe(DISCONNECT_GRACEFUL);
	}
	else
	{
		if (g_pClientGame->m_bPreviousServerExists)
		{
			g_pClientGame->OnStartInGame(true);
			g_pClientGame->Connect(g_pClientGame->m_szPreviousHost, g_pClientGame->m_usPreviousPort, g_pClientGame->m_szPreviousPassword);
		}
		else
		{
			m_pCommandHandlers->m_pLogger->LogFormatted(LOGTYPE_WARN, _gstr("Haven't connected to a server!"));
		}
	}
}

void CDisconnectCommandHandler::Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient)
{
	if (g_pClientGame->m_pMultiplayer != nullptr)
		g_pClientGame->StopMultiplayerGameWhenSafe(DISCONNECT_GRACEFUL);
		//g_pClientGame->StopMultiplayerGame(DISCONNECT_GRACEFUL, false);
	else
		m_pCommandHandlers->m_pLogger->LogFormatted(LOGTYPE_WARN, _gstr("Not connected to a server!"));
}

void CMafiaCExitCommandHandler::Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient)
{
	//g_pClientGame->ShutDown();

	g_pClientGame->StopMultiplayerGame(DISCONNECT_GRACEFUL, false);

	if (GHWND != nullptr) {
		SendMessageW(GHWND, WM_CLOSE, 0, 0);
	}
}

void CConsoleCommandHandler::Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient)
{
	bool bConsole = true; //!GContext->IsConsoleEnabled();
	if (g_pClientGame->m_pContext->IsConsoleEnabled() != bConsole)
	{
		GContext->SetConsoleEnabled(bConsole);
		if (bConsole)
			SetConsoleTitle(_T("Debug Console"));
		g_pClientGame->m_pContext->GetSettings()->Write(NULL, _gstr("Console"), bConsole);
		g_pClientGame->m_pContext->GetSettings()->Flush();
	}

	if (bConsole)
		m_pCommandHandlers->m_pLogger->LogFormatted(LOGTYPE_INFO, _gstr("Console enabled"));
	else
		m_pCommandHandlers->m_pLogger->LogFormatted(LOGTYPE_INFO, _gstr("Console disabled"));
}

void CGameStatsCommandHandler::Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient)
{
	g_pClientGame->m_bShowGameStatistics = !g_pClientGame->m_bShowGameStatistics;
	if (g_pClientGame->m_bShowGameStatistics)
		m_pCommandHandlers->m_pLogger->LogFormatted(LOGTYPE_INFO, _gstr("Game stats enabled"));
	else
		m_pCommandHandlers->m_pLogger->LogFormatted(LOGTYPE_INFO, _gstr("Game stats disabled"));
}

void CChatScaleCommandHandler::Execute(const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient)
{
	if (pszArguments != nullptr && *pszArguments != '\0')
	{
		float fScale = (float)_gatof(pszArguments);
		g_pClientGame->m_pChatWindow->SetScale(fScale);
		//g_pClientGame->m_pCmdWindow->ReInitialise(&g_pClientGame->m_Fonts, fScale);
		g_pClientGame->m_pCmdWindow->SetScale(fScale);

		g_pClientGame->m_pContext->GetSettings()->Write(_gstr("Chat Window"), _gstr("Scale"), fScale);

		m_pCommandHandlers->m_pLogger->LogFormatted(LOGTYPE_INFO, _gstr("Changed scale"));
	}
	else
	{
		float fHeight = 0.0;
		float fScale = 0.7f / 900.0f * fHeight;
		g_pClientGame->m_pChatWindow->SetScale(fScale);
		g_pClientGame->m_pCmdWindow->SetScale(fScale);

		g_pClientGame->m_pContext->GetSettings()->Delete(_gstr("Chat Window"), _gstr("Scale"));

		m_pCommandHandlers->m_pLogger->LogFormatted(LOGTYPE_WARN, _gstr("Disabled custom chat scale"));
	}
}

void CClientGame::RegisterCommands()
{
	m_pResourceMgr->m_pCommandHandlers->ClearCmds();

	m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new CMafiaCSayCommandHandler, _gstr("say"));
	m_pResourceMgr->m_pCommandHandlers->AddUnhandledCmdProc(new CMafiaCUnhandledCommandHandler);
	m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new CCallbackCommandHandler([this](const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) {
		m_pChatWindow->FlushBuffers();
	}), _gstr("clear"));

	if (m_pMultiplayer == nullptr)
	{
		m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new CStartResourceCommandHandler(m_pResourceMgr), _gstr("start"));
		m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new CStopResourceCommandHandler(m_pResourceMgr), _gstr("stop"));
		m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new CStopAllResourcesCommandHandler(m_pResourceMgr), _gstr("stopall"));
		m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new CRestartResourceCommandHandler(m_pResourceMgr), _gstr("restart"));
		m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new CRefreshResourcesCommandHandler(m_pResourceMgr), _gstr("refresh"));
		m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new CSetNameCommandHandler, _gstr("setname"));
	}

	m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new CConnectCommandHandler, _gstr("connect"));
	m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new CReconnectCommandHandler, _gstr("reconnect"));
	m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new CDisconnectCommandHandler, _gstr("disconnect"));
	m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new CMafiaCExitCommandHandler, _gstr("q"));
	m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new CMafiaCExitCommandHandler, _gstr("quit"));
	m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new CMafiaCExitCommandHandler, _gstr("exit"));
	m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new CConsoleCommandHandler, _gstr("console"));
	m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new CGameStatsCommandHandler, _gstr("gamestats"));
	m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new CCallbackCommandHandler([this](const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) {
		m_bFPSCounter = !m_bFPSCounter;
	}), _gstr("fpscounter"));
	m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new CVarCommandHandler(&m_CVars), _gstr("cvar"));
	m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new Galactic3D::CDumpDocumentationCommandHandler(m_pResourceMgr), _gstr("dumpdoc"));
	m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new CChatScaleCommandHandler, _gstr("chatscale"));
	//m_pResourceMgr->m_pCommandHandlers->AddCommandHandler(new Galactic3D::CHelpCommandHandler, _gstr("help"));

#if MAFIAC_RMLUI
	m_pResourceMgr->m_pCommandHandlers->AddCommandHandler([](const GChar* pszCommandName, const GChar* pszArguments, CBaseObject* pClient) {
		g_pClientGame->m_pRmlUi->Reload();
		}, _gstr("rml"));
#endif
}
