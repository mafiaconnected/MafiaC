#pragma once
#include "ClientVehicle.h"
#include <NetBlenderLerp.h>

class CNetBlenderVehicle : public CNetBlenderLerp
{
public:
	CNetBlenderVehicle(CClientVehicle* pEntity);

	CClientVehicle* m_pEntity;

	CInterpolation m_EngineRPM;
	CInterpolation m_WheelAngle;

	float m_fWheelAngleMaxError;
	float m_fEngineRPMMaxError;
	float m_fMaxInterpolationAmount;

	void UpdateTargetEngineRPM();
	void UpdateTargetWheelAngle();

	virtual void GetPosition(CVector3D& vecPos) override;
	virtual void SetPosition(const CVector3D& vecPos) override;

	virtual void GetRotation(CVector3D& vecRotation) override;
	virtual void SetRotation(const CVector3D& vecRotation) override;

	virtual void GetMoveSpeed(CVector3D& vecMoveSpeed) override;
	virtual void SetMoveSpeed(const CVector3D& vecMoveSpeed) override;

	virtual void GetTurnSpeed(CVector3D& vecTurnSpeed) override;
	virtual void SetTurnSpeed(const CVector3D& vecTurnSpeed) override;

	virtual float GetTargetEngineRPM();
	virtual void SetTargetEngineRPM(float engineRPM);

	virtual float GetTargetWheelAngle();
	virtual void SetTargetWheelAngle(float wheelAngle);

	virtual float GetEngineRPM();
	virtual void SetEngineRPM(float engineRPM);

	virtual float GetWheelAngle();
	virtual void SetWheelAngle(float wheelAngle);
};
