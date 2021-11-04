
#include "stdafx.h"
#include "ClientManager.h"
#include "ClientGame.h"
#include "Multiplayer.h"
#include "NetBlenderVehicle.h"

// TODO - Duplicate utility functions.
static float GetDifferenceBetweenAngles2(float a, float b)
{
	float c = (b > a) ? b - a : 0.0f - (a - b);

	if (c > PI)
		c = 0.0f - (TWO_PI - c);
	else if (c <= -PI)
		c = (TWO_PI + c);

	return c;
}

static CVector3D GetDifferenceBetweenAngles2(const CVector3D& a, const CVector3D& b)
{
	return CVector3D(GetDifferenceBetweenAngles2(a.x, b.x), GetDifferenceBetweenAngles2(a.y, b.y), GetDifferenceBetweenAngles2(a.z, b.z));
}




CNetBlenderVehicle::CNetBlenderVehicle(CClientVehicle* pEntity) :
	m_pEntity(pEntity),
	m_fRotationMaxError(1.0f),
	m_fWheelAngleMaxError(1.0f),
	m_fEngineRPMMaxError(1.0f)
{
}




void CNetBlenderVehicle::SetTargetRotation(CVector3D& frontNew, CVector3D& upNew, CVector3D& rightNew)
{
	UpdateTargetRotation();

	CVector3D frontLocal, upLocal, rightLocal;
	GetVehicleRotation(frontLocal, upLocal, rightLocal);

	CVector3D vecErrorFront = GetDifferenceBetweenAngles2(frontLocal, frontNew);
	CVector3D vecErrorUp = GetDifferenceBetweenAngles2(upLocal, upNew);
	CVector3D vecErrorRight = GetDifferenceBetweenAngles2(rightLocal, rightNew);
	m_RotationFront.SetTarget(frontNew, vecErrorFront, m_uiDelay);
	m_RotationUp.SetTarget(upNew, vecErrorUp, m_uiDelay);
	m_RotationRight.SetTarget(rightNew, vecErrorRight, m_uiDelay);
}

void CNetBlenderVehicle::UpdateTargetRotation()
{
	CVector3D vecCurrentRotationFront;
	CVector3D vecCurrentRotationUp;
	CVector3D vecCurrentRotationRight;

	GetVehicleRotation(vecCurrentRotationFront, vecCurrentRotationUp, vecCurrentRotationRight);

	CVector3D vecNewRotationFront = vecCurrentRotationFront;
	CVector3D vecNewRotationUp = vecCurrentRotationUp;
	CVector3D vecNewRotationRight = vecCurrentRotationRight;

	if (m_RotationFront.HasTarget())
	{
		m_RotationFront.Update(vecNewRotationFront, m_fRotationMaxError);
	}

	if (m_RotationUp.HasTarget())
	{
		m_RotationUp.Update(vecNewRotationUp, m_fRotationMaxError);
	}

	if (m_RotationRight.HasTarget())
	{
		m_RotationRight.Update(vecNewRotationRight, m_fRotationMaxError);
	}

	SetVehicleRotation(vecNewRotationFront, vecNewRotationUp, vecNewRotationRight);
}


void CNetBlenderVehicle::GetPosition(CVector3D& vecPos)
{
	m_pEntity->GetPosition(vecPos);
}

void CNetBlenderVehicle::SetPosition(const CVector3D& vecPos)
{
	CVector3D currPos;
	if (m_pEntity->GetPosition(currPos))
	{
		if (vecPos == currPos)
			return;
	}

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