
#include "stdafx.h"
#include "ClientManager.h"
#include "ClientGame.h"
#include "Multiplayer.h"
#include "NetBlenderActor.h"

// TODO - Duplicate utility functions.
static float GetDifferenceBetweenAngles3(float a, float b)
{
	float c = (b > a) ? b - a : 0.0f - (a - b);

	if (c > PI)
		c = 0.0f - (TWO_PI - c);
	else if (c <= -PI)
		c = (TWO_PI + c);

	return c;
}

static CVector3D GetDifferenceBetweenAngles3(const CVector3D& a, const CVector3D& b)
{
	return CVector3D(GetDifferenceBetweenAngles3(a.x, b.x), GetDifferenceBetweenAngles3(a.y, b.y), GetDifferenceBetweenAngles3(a.z, b.z));
}

CNetBlenderActor::CNetBlenderActor(CClientActor* pEntity) : m_pEntity(pEntity)
{
}

void CNetBlenderActor::SetTargetRotation(CVector3D& vecRotation)
{
	UpdateTargetRotation();

	CVector3D vecLocalRotation;
	GetRotation(vecLocalRotation);

	CVector3D vecError = GetDifferenceBetweenAngles3(vecLocalRotation, vecRotation);
	m_Rotation.SetTarget(vecRotation, vecError, m_uiDelay);
}

void CNetBlenderActor::GetPosition(CVector3D& vecPos)
{
	m_pEntity->GetPosition(vecPos);
}

void CNetBlenderActor::SetPosition(const CVector3D& vecPos)
{
	auto pBlender = m_pEntity->m_pBlender;
	m_pEntity->m_pBlender = nullptr;
	m_pEntity->SetPosition(vecPos);
	m_pEntity->m_pBlender = pBlender;
}

void CNetBlenderActor::GetRotation(CVector3D& vecRotation)
{
	m_pEntity->GetRotation(vecRotation);
}

void CNetBlenderActor::SetRotation(const CVector3D& vecRotation)
{
	auto pBlender = m_pEntity->m_pBlender;
	m_pEntity->m_pBlender = nullptr;
	m_pEntity->SetRotation(vecRotation);
	m_pEntity->m_pBlender = pBlender;
}

void CNetBlenderActor::GetMoveSpeed(CVector3D& vecMoveSpeed)
{
	m_pEntity->GetVelocity(vecMoveSpeed);
}

void CNetBlenderActor::SetMoveSpeed(const CVector3D& vecMoveSpeed)
{
	m_pEntity->SetVelocity(vecMoveSpeed);
}

void CNetBlenderActor::GetTurnSpeed(CVector3D& vecTurnSpeed)
{
	m_pEntity->GetRotationVelocity(vecTurnSpeed);
}

void CNetBlenderActor::SetTurnSpeed(const CVector3D& vecTurnSpeed)
{
	m_pEntity->SetRotationVelocity(vecTurnSpeed);
}

void CNetBlenderActor::UpdateTargetRotation()
{
	if (m_Rotation.HasTarget())
	{
		CVector3D vecCurrentRotation;
		GetRotation(vecCurrentRotation);

		CVector3D vecNewRotation = vecCurrentRotation;
		m_Rotation.Update(vecNewRotation, m_fRotationMaxError);

		SetRotation(vecNewRotation);
	}
}
