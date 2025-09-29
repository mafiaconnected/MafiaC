
#include "stdafx.h"
#include "ClientManager.h"
#include "ClientGame.h"
#include "Multiplayer.h"
#include "NetBlenderBridge.h"

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

CNetBlenderBridge::CNetBlenderBridge(CClientHuman* pEntity) : m_pEntity(pEntity)
{
}

void CNetBlenderBridge::SetTargetRotation(CVector3D& vecRotation)
{
	UpdateTargetRotation();

	CVector3D vecLocalRotation;
	GetRotation(vecLocalRotation);

	CVector3D vecError = GetDifferenceBetweenAngles2(vecLocalRotation, vecRotation);
	m_Rotation.SetTarget(vecRotation, vecError, m_uiDelay);
}

void CNetBlenderBridge::GetPosition(CVector3D& vecPos)
{
	m_pEntity->GetPosition(vecPos);
}

void CNetBlenderBridge::SetPosition(const CVector3D& vecPos)
{
	auto pBlender = m_pEntity->m_pBlender;
	m_pEntity->m_pBlender = nullptr;
	m_pEntity->SetPosition(vecPos);
	m_pEntity->m_pBlender = pBlender;
}

void CNetBlenderBridge::GetRotation(CVector3D& vecRotation)
{
	m_pEntity->GetRotation(vecRotation);
}

void CNetBlenderBridge::SetRotation(const CVector3D& vecRotation)
{
	auto pBlender = m_pEntity->m_pBlender;
	m_pEntity->m_pBlender = nullptr;
	m_pEntity->SetRotation(vecRotation);
	m_pEntity->m_pBlender = pBlender;
}

void CNetBlenderBridge::GetMoveSpeed(CVector3D& vecMoveSpeed)
{
	m_pEntity->GetVelocity(vecMoveSpeed);
}

void CNetBlenderBridge::SetMoveSpeed(const CVector3D& vecMoveSpeed)
{
	m_pEntity->SetVelocity(vecMoveSpeed);
}

void CNetBlenderBridge::GetTurnSpeed(CVector3D& vecTurnSpeed)
{
	m_pEntity->GetRotationVelocity(vecTurnSpeed);
}

void CNetBlenderBridge::SetTurnSpeed(const CVector3D& vecTurnSpeed)
{
	m_pEntity->SetRotationVelocity(vecTurnSpeed);
}

void CNetBlenderBridge::UpdateTargetRotation()
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