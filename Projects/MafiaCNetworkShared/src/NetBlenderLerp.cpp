
#include "pch.h"
#include "NetBlenderLerp.h"

using namespace Galactic3D;

static float GetDifferenceBetweenAngles(float a, float b)
{
	float c = (b > a) ? b - a : 0.0f - (a - b);

	if (c > PI)
		c = 0.0f - (TWO_PI - c);
	else if (c <= -PI)
		c = (TWO_PI + c);

	return c;
}

static CVector3D GetDifferenceBetweenAngles(const CVector3D& a, const CVector3D& b)
{
	return CVector3D(GetDifferenceBetweenAngles(a.x, b.x), GetDifferenceBetweenAngles(a.y, b.y), GetDifferenceBetweenAngles(a.z, b.z));
}

template <typename T> static T Lerp(const T& start, float fAlpha, const T& end)
{
	return (end - start) * fAlpha + start;
}

// Find the relative position of fPos between fStart and fEnd
static float Unlerp(float fStart, float fPos, float fEnd)
{
	// Avoid dividing by 0 (results in INF values)
	if (fStart == fEnd) return 1.0f;

	return (fPos - fStart) / (fEnd - fStart);
}

// Find the relative position of fPos between fStart and fEnd
// and clamp it between 0 and 1 avoiding extrapolation
static float UnlerpClamped(float fStart, float fPos, float fEnd)
{
	return Clamp(Unlerp(fStart, fPos, fEnd), 0.0f, 1.0f);
}

CNetBlenderLerp::CInterpolation::CInterpolation() :
	m_uiFinishTime(0)
{
}

void CNetBlenderLerp::CInterpolation::Update(CVector3D& vecResult, float fMaxError)
{
	// Get the factor of time spent from the interpolation start
	// to the current time.
	uint32_t uiCurrentTime = OS::GetTicks();
	float fAlpha = Unlerp((float)m_uiStartTime, (float)uiCurrentTime, (float)m_uiFinishTime);

	// Don't let it overcompensate the error too much
	fAlpha = Clamp(fAlpha, 0.0f, fMaxError);

	// Get the current error portion to compensate
	float fCurrentAlpha = (fAlpha - m_fLastAlpha);
	m_fLastAlpha = fAlpha;

	// Apply the error compensation
	CVector3D vecCompensation = Lerp(CVector3D(0, 0, 0), fCurrentAlpha, m_vecError);

	// If we finished compensating the error, finish it for the next pulse
	if (fAlpha == fMaxError)
		m_uiFinishTime = 0;

	vecResult += vecCompensation;
}

void CNetBlenderLerp::CInterpolation::Update(CQuaternion& quatResult, float fMaxError)
{
	// Get the factor of time spent from the interpolation start
	// to the current time.
	uint32_t uiCurrentTime = OS::GetTicks();
	float fAlpha = Unlerp((float)m_uiStartTime, (float)uiCurrentTime, (float)m_uiFinishTime);

	// Don't let it overcompensate the error too much
	fAlpha = Clamp(fAlpha, 0.0f, fMaxError);

	// Get the current error portion to compensate
	float fCurrentAlpha = (fAlpha - m_fLastAlpha);
	m_fLastAlpha = fAlpha;

	// Apply the error compensation
	CQuaternion quatCompensation(0, 0, 0, 1);
	CQuaternion quat1(0, 0, 0, 1);
	quatResult.SetSlerp(quatResult, m_quatTarget, fCurrentAlpha);

	// If we finished compensating the error, finish it for the next pulse
	if (fAlpha == fMaxError)
		m_uiFinishTime = 0;
}

void CNetBlenderLerp::CInterpolation::SetTarget(const CVector3D& vecTarget, const CVector3D& vecError, uint32_t uiDelay)
{
	uint32_t uiTime = OS::GetTicks();

	m_vecTarget = vecTarget;
	m_vecError = vecError;

	m_uiStartTime = uiTime;
	m_uiFinishTime = (uiTime + uiDelay);

	m_fLastAlpha = 0.0f;
}

void CNetBlenderLerp::CInterpolation::SetTarget(const CQuaternion& quatTarget, const CQuaternion& quatError, uint32_t uiDelay)
{
	uint32_t uiTime = OS::GetTicks();

	m_quatTarget = quatTarget;
	m_quatError = quatError;

	m_uiStartTime = uiTime;
	m_uiFinishTime = (uiTime + uiDelay);

	m_fLastAlpha = 0.0f;
}

CNetBlenderLerp::CNetBlenderLerp()
{
	SetDelay(100);

	m_fPositionMaxError = 1.5f;
	m_fMaxInterpolationDistance = 20.0f;
}

CNetBlenderLerp::~CNetBlenderLerp()
{
}

void CNetBlenderLerp::SetDelay(uint32_t uiDelay)
{
	m_uiDelay = uiDelay;

	// Apply the error over 400ms (i.e. 1/4 per 100ms)
	float fMultiplier = Lerp<float>(0.25f, UnlerpClamped(100.0f, (float)uiDelay, 400.0f), 1.0f);

	m_vecErrorMultiplier = CVector3D(fMultiplier, fMultiplier, fMultiplier);
}

void CNetBlenderLerp::UpdateTargetPosition()
{
	if (m_Position.HasTarget())
	{
		CVector3D vecCurrentPosition;
		GetPosition(vecCurrentPosition);

		CVector3D vecNewPosition = vecCurrentPosition;
		m_Position.Update(vecNewPosition, m_fPositionMaxError);

		// Check if the distance to interpolate is too far
		if ((vecCurrentPosition - m_Position.m_vecTarget).GetLength() > m_fMaxInterpolationDistance)
		{
			ResetInterpolation();
		}
		else
		{
			SetPosition(vecNewPosition);
		}
	}
}

void CNetBlenderLerp::UpdateTargetSpeed()
{
	if (m_MoveSpeed.HasTarget())
	{
		CVector3D vecCurrentSpeed;
		GetMoveSpeed(vecCurrentSpeed);

		CVector3D vecNewSpeed = vecCurrentSpeed;
		m_MoveSpeed.Update(vecNewSpeed, 1.0f);

		SetMoveSpeed(vecNewSpeed);
	}

	if (m_TurnSpeed.HasTarget())
	{
		CVector3D vecCurrentSpeed;
		GetTurnSpeed(vecCurrentSpeed);

		CVector3D vecNewSpeed = vecCurrentSpeed;
		m_TurnSpeed.Update(vecNewSpeed, 1.0f);

		SetTurnSpeed(vecNewSpeed);
	}
}

bool CNetBlenderLerp::IsInterpolating()
{
	if (m_Position.HasTarget())
		return true;

	if (m_Rotation.HasTarget())
		return true;

	if (m_MoveSpeed.HasTarget())
		return true;

	if (m_TurnSpeed.HasTarget())
		return true;

	return false;
}

void CNetBlenderLerp::Interpolate()
{
	UpdateTargetPosition();
	UpdateTargetRotation();
	UpdateTargetSpeed();
}

void CNetBlenderLerp::SetTargetPosition(const CVector3D& vecPosition)
{
	UpdateTargetPosition();

	CVector3D vecLocalPosition;
	GetPosition(vecLocalPosition);

	CVector3D vecError = (vecPosition - vecLocalPosition);
	vecError *= m_vecErrorMultiplier;

	m_Position.SetTarget(vecPosition, vecError, m_uiDelay);
}

void CNetBlenderLerp::SetTargetSpeed(const CVector3D& vecMoveSpeed, const CVector3D& vecTurnSpeed)
{
	UpdateTargetSpeed();

	CVector3D vecLocalMoveSpeed;
	GetMoveSpeed(vecLocalMoveSpeed);

	CVector3D vecLocalTurnSpeed;
	GetTurnSpeed(vecLocalTurnSpeed);

	CVector3D vecError = (vecMoveSpeed - vecLocalMoveSpeed);
	vecError *= m_vecErrorMultiplier;

	m_MoveSpeed.SetTarget(vecMoveSpeed, vecError, m_uiDelay);

	vecError = (vecTurnSpeed - vecLocalTurnSpeed);
	vecError *= m_vecErrorMultiplier;

	m_TurnSpeed.SetTarget(vecTurnSpeed, vecError, m_uiDelay);
}

void CNetBlenderLerp::ResetInterpolation()
{
	if (m_Position.HasTarget())
		SetPosition(m_Position.m_vecTarget);

	if (m_Rotation.HasTarget())
		SetRotation(m_Rotation.m_vecTarget);

	if (m_MoveSpeed.HasTarget())
		SetMoveSpeed(m_MoveSpeed.m_vecTarget);

	if (m_TurnSpeed.HasTarget())
		SetTurnSpeed(m_TurnSpeed.m_vecTarget);

	m_Position.RemoveTarget();
	m_Rotation.RemoveTarget();
	m_MoveSpeed.RemoveTarget();
	m_TurnSpeed.RemoveTarget();
}

void CNetBlenderLerp::RemoveInterpolation()
{
	m_Position.RemoveTarget();
	m_Rotation.RemoveTarget();
	m_MoveSpeed.RemoveTarget();
	m_TurnSpeed.RemoveTarget();
}
