
#include "stdafx.h"
#include "ClientManager.h"
#include "ClientGame.h"
#include "Multiplayer.h"
#include "NetBlenderVehicle.h"

CNetBlenderVehicle::CNetBlenderVehicle(CClientVehicle* pEntity)
{
	m_pEntity = pEntity;
}

void CNetBlenderVehicle::GetPosition(CVector3D& vecPos)
{
	m_pEntity->GetPosition(vecPos);
}

void CNetBlenderVehicle::SetPosition(const CVector3D& vecPos)
{
	auto pBlender = m_pEntity->m_pBlender;
	m_pEntity->m_pBlender = nullptr;
	m_pEntity->SetPosition(vecPos);
	m_pEntity->m_pBlender = pBlender;
}

void CNetBlenderVehicle::GetRotation(CVector3D& vecRotation)
{
	m_pEntity->GetRotation(vecRotation);
}

void CNetBlenderVehicle::SetRotation(const CVector3D& vecRotation)
{
	auto pBlender = m_pEntity->m_pBlender;
	m_pEntity->m_pBlender = nullptr;
	m_pEntity->SetRotation(vecRotation);
	m_pEntity->m_pBlender = pBlender;
}

void CNetBlenderVehicle::GetMoveSpeed(CVector3D& vecMoveSpeed)
{
	m_pEntity->GetVelocity(vecMoveSpeed);
}

void CNetBlenderVehicle::SetMoveSpeed(const CVector3D& vecMoveSpeed)
{
	m_pEntity->SetVelocity(vecMoveSpeed);
}

void CNetBlenderVehicle::GetTurnSpeed(CVector3D& vecTurnSpeed)
{
	m_pEntity->GetRotationVelocity(vecTurnSpeed);
}

void CNetBlenderVehicle::SetTurnSpeed(const CVector3D& vecTurnSpeed)
{
	m_pEntity->SetRotationVelocity(vecTurnSpeed);
}

float CNetBlenderVehicle::GetEngineRPM()
{
	return m_pEntity->GetEngineRPM();
}

void CNetBlenderVehicle::SetEngineRPM(float engineRPM)
{
	m_pEntity->SetEngineRPM(engineRPM);
}

float CNetBlenderVehicle::GetWheelAngle()
{
	return m_pEntity->GetWheelAngle();
}

void CNetBlenderVehicle::SetWheelAngle(float wheelAngle)
{
	m_pEntity->SetWheelAngle(wheelAngle);
}

void CNetBlenderVehicle::UpdateTargetWheelAngle()
{
	if (m_WheelAngle.HasTarget())
	{
		float vecCurrentWheelAngle;
		vecCurrentWheelAngle = GetWheelAngle();

		float vecNewWheelAngle = vecCurrentWheelAngle;
		//m_WheelAngle.Update(vecNewWheelAngle, m_fWheelAngleMaxError);

		SetWheelAngle(vecNewWheelAngle);
	}
}

void CNetBlenderVehicle::UpdateTargetEngineRPM()
{
	if (m_EngineRPM.HasTarget())
	{
		float vecCurrentEngineRPM;
		vecCurrentEngineRPM = GetEngineRPM();

		float vecNewEngineRPM = vecCurrentEngineRPM;
		///m_EngineRPM.Update(vecNewEngineRPM, m_fEngineRPMMaxError);

		SetWheelAngle(vecNewEngineRPM);
	}
}