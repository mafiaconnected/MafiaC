
#include "stdafx.h"
#include "ClientManager.h"
#include "ClientGame.h"
#include "Multiplayer.h"
#include "Elements/Elements.h"
#include <Multiplayer/ChatWindow.h>
#include <d3dx9.h>

using namespace Galactic3D;

CMafiaClientManager::CMafiaClientManager(Galactic3D::Context* pContext, CClientResourceMgr* pResourceMgr) :
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

CNetObject* CMafiaClientManager::Create(int Type)
{
	CClientVehicle* veh;
	CClientHuman* ped;
	CClientPlayer* player;

	switch (Type)
	{
	case ELEMENT_VEHICLE:
		veh = new CClientVehicle(this);
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
		player = new CClientPlayer(this);
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
		ped = new CClientHuman(this);
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

bool CMafiaClientManager::IsConnecting(void)
{
	if (IsConnected())
		return false;
	return g_pClientGame->m_pMultiplayer != nullptr;
}

bool CMafiaClientManager::Connect(const GChar* pszHost, unsigned short usPort, const GChar* pszPassword)
{
	return g_pClientGame->Connect(pszHost, usPort, pszPassword);
}

bool CMafiaClientManager::Disconnect(void)
{
	if (g_pClientGame->m_pMultiplayer != nullptr)
		g_pClientGame->StopMultiplayerGame();
	return true;
}

extern bool RegisterLuaVM(CScripting* pScripting);
extern bool RegisterJSVM(CScripting* pScripting);
extern bool RegisterSqVM(CScripting* pScripting);

CMafiaClientResourceMgr::CMafiaClientResourceMgr(Galactic3D::Context* pContext) : CClientResourceMgr(pContext)
{
	RegisterLuaVM(m_pScripting);
	RegisterJSVM(m_pScripting);
	RegisterSqVM(m_pScripting);
}

void CMafiaClientResourceMgr::RemoveThingsAssociatedWithResource(CResource* pResource)
{
	if (g_pClientGame->m_GUISystem.m_pPage != nullptr)
		g_pClientGame->m_GUISystem.m_pPage->m_Elements.Remove(pResource);
	if (g_pClientGame->m_GUISystem.m_pScriptPage != nullptr)
		g_pClientGame->m_GUISystem.m_pScriptPage->m_Elements.Remove(pResource);
	Galactic3D::LinkedList<Galactic3D::Audio::ISound*>::Node* pNode = g_pClientGame->m_pAudioScriptingFunctions->m_Sounds.GetFirstNode();
	while (pNode != NULL)
	{
		Galactic3D::LinkedList<Galactic3D::Audio::ISound*>::Node* pThisNode = pNode;
		pNode = pNode->GetNextNode();
		if (pThisNode->m_Value->m_pResource != NULL && pThisNode->m_Value->m_pResource == pResource)
		{
			pThisNode->m_Value->Stop();
		}
	}
	CClientResourceMgr::RemoveThingsAssociatedWithResource(pResource);
}

CClientVehicle* CMafiaClientManager::FindVehicle(M2::C_Car* pVehicle)
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

CClientHuman* CMafiaClientManager::FindHuman(M2::C_Human2* pHuman)
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

CClientPlayer* CMafiaClientManager::FindPlayer(M2::C_Player2* pPlayer)
{
	if (pPlayer == nullptr)
		return nullptr;

	for (auto pElement : m_rgpPlayers)
	{
		if (pElement != NULL && pElement.GetPointer() != nullptr && pElement.GetPointer()->GetGameHuman() == pPlayer)
		{
			return pElement.GetPointer();
		}
	}
	return nullptr;
}

bool CMafiaClientManager::OnProcess(void) {
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