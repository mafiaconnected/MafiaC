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

	m_Flags.m_bFindSyncer = true;
	m_Flags.m_bSendSync = true;
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

void CClientBridge::CreateNetBlender()
{
	if (m_pBlender == nullptr)
	{
		m_pBlender = new CNetBlenderBridge(this);
	}
}

void CClientBridge::SetFromExistingEntity(MafiaSDK::C_Bridge* bridge) {
	m_MafiaBridge = bridge;
	SetEnabled(m_Enabled);
}

bool CClientBridge::SetPosition(const CVector3D& vecPos)
{
	if (m_MafiaBridge == nullptr)
		return false;

	m_MafiaBridge->GetFrame()->GetInterface()->position = (CVecTools::ConvertToMafiaVec(vecPos));
	return true;
}

bool CClientBridge::GetPosition(CVector3D& vecPos)
{
	if (m_MafiaBridge == nullptr)
		return false;
	vecPos = CVecTools::ConvertFromMafiaVec(m_MafiaBridge->GetFrame()->GetInterface()->position);
	return true;
}

bool CClientBridge::SetRotation(const CVector3D& vecRot)
{
	if (m_MafiaBridge == nullptr)
		return false;
	m_MafiaBridge->GetFrame()->GetInterface()->rotation = CVecTools::ConvertToMafiaVec(vecRot);
	return true;
}

bool CClientBridge::GetRotation(CVector3D& vecRot)
{
	if (m_MafiaBridge == nullptr)
		return false;
	vecRot = CVecTools::ConvertFromMafiaVec(m_MafiaBridge->GetFrame()->GetInterface()->rotation);
	return true;
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

bool CClientBridge::ReadSyncPacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntity::ReadSyncPacket(pStream))
		return false;

	tBridgeSyncPacket Packet;
	if (pStream->Read(&Packet, sizeof(Packet)) != sizeof(Packet))
		return false;

	m_Enabled = Packet.enabled;
}

bool CClientBridge::WriteSyncPacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntity::WriteSyncPacket(pStream))
		return false;
}

void CClientBridge::Process()
{
	if (!IsSyncer() && m_pBlender != nullptr && GetGameBridge() != nullptr)
	{
		m_pBlender->Interpolate();
	}
}