#include "stdafx.h"
#include "ClientManager.h"
#include "ClientGame.h"
#include "Multiplayer.h"
#include "ClientActor.h"
#include <Utils/VectorTools.h>

using namespace Galactic3D;

CClientActor::CClientActor(CMafiaClientManager* pClientManager) : CClientEntity(pClientManager)
{
	m_Type = ELEMENT_ACTOR;
}

Galactic3D::ReflectedClass* CClientActor::GetReflectedClass()
{
	return static_cast<CMafiaClientManager*>(m_pClientManager)->m_pClientActorClass;
}

MafiaSDK::C_Actor* CClientActor::GetGameActor()
{
	if (m_pMafiaActor != nullptr)
	{
		if (m_pMafiaActor->GetInterface() != nullptr)
			return m_pMafiaActor;

		// No longer valid (e.g. the scene section holding it was unloaded) - drop the cache and re-resolve
		// by name below, same as when it was never found in the first place.
		m_pMafiaActor = nullptr;
	}

	if (GetName() == nullptr)
		return nullptr;

	UTF8String Name(true, GetName());
	MafiaSDK::C_Actor* pActor = MafiaSDK::GetMission()->FindActorByName(Name);

	if (pActor != nullptr && pActor->GetInterface() != nullptr)
		m_pMafiaActor = pActor;

	return m_pMafiaActor;
}

void CClientActor::Process()
{
	if (!IsSyncer() && m_pBlender != nullptr && GetGameActor() != nullptr)
	{
		m_pBlender->Interpolate();
	}
}

bool CClientActor::SetPosition(const CVector3D& vecPos)
{
	if (GetGameActor() == nullptr)
		return false;

	GetGameActor()->GetInterface()->entity.position = CVecTools::ConvertToMafiaVec(vecPos);

	if (GetGameActor()->GetFrame() != nullptr)
	{
		GetGameActor()->GetFrame()->UpdateWMatrixProc();
		GetGameActor()->GetFrame()->Update();
	}

	return true;
}

bool CClientActor::GetPosition(CVector3D& vecPos)
{
	if (GetGameActor() == nullptr || GetGameActor()->GetFrame() == nullptr)
		return false;

	vecPos = CVecTools::ConvertFromMafiaVec(GetGameActor()->GetFrame()->GetInterface()->position);
	return true;
}

bool CClientActor::SetRotation(const CVector3D& vecRotation)
{
	if (GetGameActor() == nullptr)
		return false;

	GetGameActor()->GetInterface()->entity.rotation = CVecTools::ConvertToMafiaVec(vecRotation);

	if (GetGameActor()->GetFrame() != nullptr)
	{
		GetGameActor()->GetFrame()->UpdateWMatrixProc();
		GetGameActor()->GetFrame()->Update();
	}

	return true;
}

bool CClientActor::GetRotation(CVector3D& vecRotation)
{
	if (GetGameActor() == nullptr || GetGameActor()->GetFrame() == nullptr)
		return false;

	vecRotation = CVecTools::ConvertFromMafiaVec(GetGameActor()->GetFrame()->GetInterface()->rotation);
	return true;
}

bool CClientActor::SetVelocity(const CVector3D& vecVel)
{
	m_vecVelocity = vecVel;
	return true;
}

bool CClientActor::GetVelocity(CVector3D& vecVel)
{
	vecVel = m_vecVelocity;
	return true;
}

bool CClientActor::SetRotationVelocity(const CVector3D& vecRotVel)
{
	m_vecRotationVelocity = vecRotVel;
	return true;
}

bool CClientActor::GetRotationVelocity(CVector3D& vecRotVel)
{
	vecRotVel = m_vecRotationVelocity;
	return true;
}

bool CClientActor::ReadCreatePacket(Stream* pStream)
{
	// Deliberately does not fail (and does not require GetGameActor() to already resolve) - unlike
	// vehicles/peds, this element isn't spawned by this mod, so there's no guarantee our own client has
	// finished loading the map section containing it yet. ClientManager::ReadCreatePacket drops the object
	// permanently on a false return with no retry, so GetGameActor() must be re-attempted lazily instead
	// (see its own comment).
	return CClientEntity::ReadCreatePacket(pStream);
}

bool CClientActor::ReadSyncPacket(Stream* pStream)
{
	if (!CClientEntity::ReadSyncPacket(pStream))
		return false;

	if (!IsSyncer() && m_pBlender != nullptr)
	{
		auto pBlender = static_cast<CNetBlenderActor*>(m_pBlender);

		pBlender->SetTargetPosition(m_Position);
		pBlender->SetTargetRotation(m_Rotation);
		pBlender->SetTargetSpeed(m_RelativePosition, m_RelativeRotation);
	}

	return true;
}

bool CClientActor::WriteCreatePacket(Stream* pStream)
{
	if (GetGameActor() == nullptr)
		return false;

	GetPosition(m_Position);
	GetRotation(m_Rotation);

	return CClientEntity::WriteCreatePacket(pStream);
}

bool CClientActor::WriteSyncPacket(Stream* pStream)
{
	if (GetGameActor() == nullptr)
		return false;

	uint32_t uiPrevTicks = m_uiLastSendSyncTicks;
	uint32_t uiNowTicks = OS::GetTicks();

	CVector3D vecPrevPos = m_vecPrevSyncPosition;
	CVector3D vecPrevRot = m_vecPrevSyncRotation;

	GetPosition(m_Position);
	GetRotation(m_Rotation);

	float fDeltaSeconds = (uiPrevTicks != 0) ? (uiNowTicks - uiPrevTicks) / 1000.0f : 0.0f;

	if (fDeltaSeconds > 0.001f)
	{
		m_vecVelocity = (m_Position - vecPrevPos) / fDeltaSeconds;
		m_vecRotationVelocity = (m_Rotation - vecPrevRot) / fDeltaSeconds;
	}
	else
	{
		m_vecVelocity = CVector3D(0, 0, 0);
		m_vecRotationVelocity = CVector3D(0, 0, 0);
	}

	m_RelativePosition = m_vecVelocity;
	m_RelativeRotation = m_vecRotationVelocity;

	m_vecPrevSyncPosition = m_Position;
	m_vecPrevSyncRotation = m_Rotation;

	return CClientEntity::WriteSyncPacket(pStream);
}

void CClientActor::CreateNetBlender()
{
	auto pBlender = new CNetBlenderActor(this);
	auto pMultiplayer = g_pClientGame->GetMultiplayer();
	if (pMultiplayer != nullptr)
		pBlender->m_uiDelay = pMultiplayer->m_usSyncIntervalInMS + 70;
	m_pBlender = pBlender;
}
