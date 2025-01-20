
#include "stdafx.h"
#include "ClientManager.h"
#include "ClientGame.h"
#include "Multiplayer.h"
#include "Elements/Elements.h"
#include <Multiplayer/ChatWindow.h>
#include <d3dx9.h>

using namespace Galactic3D;

CMafiaClientManagerII::CMafiaClientManagerII(Galactic3D::Context* pContext, CClientResourceMgr* pResourceMgr) :
	CClientManager(pContext,pResourceMgr)
{
	m_Games.Register(_gstr("mafia:one"), GAME_MAFIA_ONE);
	m_Games.Register(_gstr("mafia:two"), GAME_MAFIA_TWO);
	m_Games.Register(_gstr("mafia:three"), GAME_MAFIA_THREE);
	m_Games.Register(_gstr("mafia:one_de"), GAME_MAFIA_ONE_DE);

    auto pElements = pResourceMgr->m_pScripting->m_Global.AddNamespace(_gstr("elements"));
    m_pClientEntityClass = pElements->NewClass(_gstr("Entity"), m_pNetObjectClass);
    m_pClientHumanClass = pElements->NewClass(_gstr("Ped"), m_pClientEntityClass);
    m_pClientPlayerClass = pElements->NewClass(_gstr("Player"), m_pClientHumanClass);
    m_pClientVehicleClass = pElements->NewClass(_gstr("Vehicle"), m_pClientEntityClass);

	m_pSurfaceClass = pResourceMgr->m_pScripting->m_Global.NewClass(_gstr("Surface"));
	m_pTextureClass = pResourceMgr->m_pScripting->m_Global.NewClass(_gstr("Texture"), m_pSurfaceClass);

	//RegisterFunctions(pResourceMgr->m_pScripting);
}

CNetObject* CMafiaClientManagerII::Create(int Type)
{
	CClientVehicleII* veh;
	CClientHumanII* ped;
	CClientPlayerII* player;

	switch (Type)
	{
	case ELEMENT_VEHICLE:
		veh = new CClientVehicleII(this);
		for (int i = 0; i < MAX_VEHICLES; i++)
		{
			if (m_rgpVehicles[i].IsNull())
			{
				m_rgpVehicles[i] = veh;
				return veh;
			}
		}

		break;
	case ELEMENT_PLAYER:
		player = new CClientPlayerII(this);
		for (int i = 0; i < MAX_PEDS; i++)
		{
			if (m_rgpPlayers[i].IsNull()) 
			{
				m_rgpPlayers[i] = player;
				return player;
			}
		}

		break;
	case ELEMENT_PED:
		ped = new CClientHumanII(this);
		for (int i = 0; i < MAX_PEDS; i++)
		{
			if (m_rgpPeds[i].IsNull())
			{
				m_rgpPeds[i] = ped;
				return ped;
			}
		}
		break;
		
	default:
		break;
	}
	return nullptr;
}

bool CMafiaClientManagerII::IsConnecting(void)
{
	if (IsConnected())
		return false;
	return g_pClientGame->m_pMultiplayer != nullptr;
}

bool CMafiaClientManagerII::Connect(const GChar* pszHost, unsigned short usPort, const GChar* pszPassword)
{
	return g_pClientGame->Connect(pszHost, usPort, pszPassword);
}

bool CMafiaClientManagerII::Disconnect(void)
{
	if (g_pClientGame->m_pMultiplayer != nullptr)
		g_pClientGame->StopMultiplayerGame();
	return true;
}

extern bool RegisterLuaVM(CScripting* pScripting);
extern bool RegisterJSVM(CScripting* pScripting);
extern bool RegisterSqVM(CScripting* pScripting);

CMafiaClientResourceMgrII::CMafiaClientResourceMgrII(Galactic3D::Context* pContext) : CClientResourceMgr(pContext)
{
	RegisterLuaVM(m_pScripting);
	RegisterJSVM(m_pScripting);
	RegisterSqVM(m_pScripting);
}

void CMafiaClientResourceMgrII::RemoveThingsAssociatedWithResource(CResource* pResource)
{
	if (g_pClientGame->m_GUISystem.m_pPage != nullptr)
		g_pClientGame->m_GUISystem.m_pPage->m_Elements.Remove(pResource);
	if (g_pClientGame->m_GUISystem.m_pScriptPage != nullptr)
		g_pClientGame->m_GUISystem.m_pScriptPage->m_Elements.Remove(pResource);
	g_pClientGame->m_pAudioScriptingFunctions->OnResourceStop(pResource);
	CClientResourceMgr::RemoveThingsAssociatedWithResource(pResource);
}

CClientVehicleII* CMafiaClientManagerII::FindVehicle(M2::C_Car* pVehicle)
{
	if (pVehicle == nullptr)
		return nullptr;

	for (auto pElement : m_rgpVehicles)
	{
		if (pElement != NULL && pElement.GetPointer() != nullptr && pElement.GetPointer()->GetGameVehicle() == pVehicle)
		{
			return pElement.GetPointer();
		}
	}
	return nullptr;
}

CClientHumanII* CMafiaClientManagerII::FindHuman(M2::C_Human2* pHuman)
{
	if (pHuman == nullptr)
		return nullptr;

	for (auto pElement : m_rgpPeds)
	{
		if (pElement != NULL && pElement.GetPointer() != nullptr && pElement.GetPointer()->GetGameHuman() == pHuman)
		{
			return pElement.GetPointer();
		}
	}

	for (auto pElement : m_rgpPlayers)
	{
		if (pElement != NULL && pElement.GetPointer() != nullptr && pElement.GetPointer()->GetGameHuman() == pHuman)
		{
			return pElement.GetPointer();
		}
	}
	return nullptr;
}

CClientPlayerII* CMafiaClientManagerII::FindPlayer(M2::C_Player2* pPlayer)
{
	if (pPlayer == nullptr)
		return nullptr;

	for (auto pElement : m_rgpPlayers)
	{
		if (pElement != NULL && pElement.GetPointer() != nullptr && pElement.GetPointer()->GetGamePlayer() == pPlayer)
		{
			return pElement.GetPointer();
		}
	}
	return nullptr;
}

bool CMafiaClientManagerII::OnProcess(void) {
	for (auto pPed : m_rgpPeds)
	{
		if (pPed != NULL && pPed.GetPointer() != nullptr && pPed.GetPointer()->GetGameHuman() != nullptr) {
			pPed.GetPointer()->Process();
		}
	}

	for (auto pPlayer : m_rgpPlayers)
	{
		if (pPlayer != NULL && pPlayer.GetPointer() != nullptr && pPlayer.GetPointer()->GetGameHuman() != nullptr) {
			pPlayer.GetPointer()->Process();
		}
	}

	for (auto pVehicle : m_rgpVehicles)
	{
		if (pVehicle != NULL && pVehicle.GetPointer() != nullptr && pVehicle.GetPointer()->GetGameVehicle() != nullptr) {
			pVehicle.GetPointer()->Process();
		}
	}

	return true;
}