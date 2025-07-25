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
}

Galactic3D::ReflectedClass* CClientBridge::GetReflectedClass()
{
	return static_cast<CMafiaClientManager*>(m_pClientManager)->m_pClientBridgeClass;
}

bool CClientBridge::SetEnabled(bool enabled) {
	m_Enabled = enabled;
	m_MafiaBridge->Shutdown(enabled);
	return true;
}

bool CClientBridge::GetEnabled() {
	return m_Enabled;
}

MafiaSDK::C_Bridge* CClientBridge::GetGameBridge()
{
	return m_MafiaBridge;
}

bool CClientBridge::ReadCreatePacket(Galactic3D::Stream* pStream)
{
	abort();
}

bool CClientBridge::WriteCreatePacket(Galactic3D::Stream* pStream)
{
	abort();
}
