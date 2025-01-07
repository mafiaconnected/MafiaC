#include "stdafx.h"
#include "ClientManager.h"
#include "ClientGame.h"
#include "Multiplayer.h"
#include "ClientEntity.h"
#include <Utils/VectorTools.h>

std::vector<CClientBridge*> Bridges;

using namespace Galactic3D;

CClientBridge::CClientBridge(CMafiaClientManager* pClientManager) : CClientEntity(pClientManager)
{
	m_Type = ELEMENT_BRIDGE;

	m_Flags.m_bFindSyncer = false;
	m_Flags.m_bSendSync = false;

	m_MafiaBridge = NULL;

	m_Enabled = false;
}

Galactic3D::ReflectedClass* CClientBridge::GetReflectedClass(void)
{
	return static_cast<CMafiaClientManager*>(m_pClientManager)->m_pClientBridgeClass;
}

bool CClientBridge::SetEnabled(bool enabled) {
	m_Enabled = enabled;
	m_MafiaBridge->Shutdown(enabled);
}

bool CClientBridge::GetEnabled() {
	return m_Enabled;
}

MafiaSDK::C_Bridge* CClientBridge::GetGameBridge()
{
	return m_MafiaBridge;
}