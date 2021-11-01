#pragma once

#include <Multiplayer/NetBlender.h>

class CNetBlenderLerp : public CNetBlender
{
public:
	CNetBlenderLerp();
	virtual ~CNetBlenderLerp();

	class CInterpolation
	{
	public:
		CInterpolation();

		CVector3D m_vecStart;
		CVector3D m_vecTarget;
		CVector3D m_vecError;
		float m_fLastAlpha;
		uint32_t m_uiStartTime;
		uint32_t m_uiFinishTime;

		inline bool HasTarget() { return m_uiFinishTime != 0; }
		inline void RemoveTarget() { m_uiFinishTime = 0; }

		void Update(CVector3D& vecResult, float fMaxError);
		void SetTarget(const CVector3D& vecTarget, const CVector3D& vecError, uint32_t uiDelay);
	};

	uint32_t m_uiDelay;
	CVector3D m_vecErrorMultiplier;

	float m_fPositionMaxError;
	float m_fMaxInterpolationDistance;

	CInterpolation m_Position;
	CInterpolation m_Rotation;
	CInterpolation m_MoveSpeed;
	CInterpolation m_TurnSpeed;

	void SetDelay(uint32_t uiDelay);

	void UpdateTargetPosition();
	virtual void UpdateTargetRotation() = 0;
	void UpdateTargetSpeed();

	virtual bool IsInterpolating() override;
	virtual void Interpolate() override;

	void SetTargetPosition(const CVector3D& vecPosition);
	void SetTargetSpeed(const CVector3D& vecMoveSpeed, const CVector3D& vecTurnSpeed);

	virtual void ResetInterpolation() override;
	virtual void RemoveInterpolation() override;

	virtual void GetPosition(CVector3D& vecPos) = 0;
	virtual void SetPosition(const CVector3D& vecPos) = 0;

	virtual void GetRotation(CVector3D& vecRotation) = 0;
	virtual void SetRotation(const CVector3D& vecRotation) = 0;

	virtual void GetMoveSpeed(CVector3D& vecMoveSpeed) = 0;
	virtual void SetMoveSpeed(const CVector3D& vecMoveSpeed) = 0;

	virtual void GetTurnSpeed(CVector3D& vecTurnSpeed) = 0;
	virtual void SetTurnSpeed(const CVector3D& vecTurnSpeed) = 0;
};
