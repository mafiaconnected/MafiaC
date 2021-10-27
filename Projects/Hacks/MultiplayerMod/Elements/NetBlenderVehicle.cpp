
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
	CVector3D currPos;
	m_pEntity->GetPosition(currPos);
	if (vecPos == currPos)
		return;

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

void CNetBlenderVehicle::GetVehicleRotation(CVector3D& vecRotationFront, CVector3D& vecRotationUp, CVector3D& vecRotationRight)
{
	m_pEntity->GetVehicleRotation(vecRotationFront, vecRotationUp, vecRotationRight);
}

void CNetBlenderVehicle::SetVehicleRotation(const CVector3D& vecRotationFront, const CVector3D& vecRotationUp, const CVector3D& vecRotationRight)
{
	auto pBlender = m_pEntity->m_pBlender;
	m_pEntity->m_pBlender = nullptr;
	m_pEntity->SetVehicleRotation(vecRotationFront, vecRotationUp, vecRotationRight);
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

void CNetBlenderVehicle::SetTargetVehicleRotation(const CVector3D& vecRotationFront, const CVector3D& vecRotationUp, const CVector3D& vecRotationRight)
{
	CVector3D vecLocalRotationFront;
	CVector3D vecLocalRotationUp;
	CVector3D vecLocalRotationRight;
	GetVehicleRotation(vecLocalRotationFront, vecLocalRotationUp, vecLocalRotationRight);

	CVector3D vecRotationFrontError = (vecRotationFront - vecLocalRotationFront);
	vecRotationFrontError *= m_vecErrorMultiplier;

	CVector3D vecRotationUpError = (vecRotationUp - vecLocalRotationUp);
	vecRotationUpError *= m_vecErrorMultiplier;

	CVector3D vecRotationRightError = (vecRotationRight - vecLocalRotationRight);
	vecRotationRightError *= m_vecErrorMultiplier;

	m_RotationFront.SetTarget(vecRotationFront, vecRotationFrontError, m_uiDelay);
	m_RotationUp.SetTarget(vecRotationUp, vecRotationUpError, m_uiDelay);
	m_RotationRight.SetTarget(vecRotationRight, vecRotationRightError, m_uiDelay);
}

void CNetBlenderVehicle::UpdateTargetVehicleRotation()
{
	CVector3D vecCurrentRotationFront;
	CVector3D vecCurrentRotationUp;
	CVector3D vecCurrentRotationRight;

	GetVehicleRotation(vecCurrentRotationFront, vecCurrentRotationUp, vecCurrentRotationRight);

	CVector3D vecNewRotationFront = vecCurrentRotationFront;
	CVector3D vecNewRotationUp = vecCurrentRotationUp;
	CVector3D vecNewRotationRight = vecNewRotationRight;

	if (m_RotationFront.HasTarget())
	{
		m_RotationFront.Update(vecNewRotationFront, 1.0f);
	}

	if (m_RotationUp.HasTarget())
	{
		m_RotationUp.Update(vecNewRotationUp, 1.0f);
	}

	if (m_RotationRight.HasTarget())
	{
		m_RotationUp.Update(vecNewRotationRight, 1.0f);
	}

	SetVehicleRotation(vecNewRotationFront, vecNewRotationUp, vecNewRotationRight);
}