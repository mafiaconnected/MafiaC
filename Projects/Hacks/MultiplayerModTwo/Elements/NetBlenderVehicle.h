#pragma once
#include "ClientVehicle.h"
#include <NetBlenderLerp.h>

class CNetBlenderVehicleII : public CNetBlenderLerp
{
public:
	CNetBlenderVehicleII(CClientVehicleII* pEntity);

	CClientVehicleII* m_pEntity;

	CInterpolation m_RotationFront;
	CInterpolation m_RotationUp;
	CInterpolation m_RotationRight;

	CInterpolation m_EngineRPM;
	CInterpolation m_WheelAngle;

	float m_fRotationMaxError;
	float m_fWheelAngleMaxError;
	float m_fEngineRPMMaxError;

	void SetTargetRotation(CVector3D& front, CVector3D& up, CVector3D& right);

	void UpdateTargetEngineRPM();
	void UpdateTargetWheelAngle();
	virtual void UpdateTargetRotation() override;

	virtual void GetPosition(CVector3D& vecPos) override;
	virtual void SetPosition(const CVector3D& vecPos) override;

	virtual void GetRotation(CVector3D& vecRotation) override;
	virtual void SetRotation(const CVector3D& vecRotation) override;

	virtual void GetMoveSpeed(CVector3D& vecMoveSpeed) override;
	virtual void SetMoveSpeed(const CVector3D& vecMoveSpeed) override;

	virtual void GetTurnSpeed(CVector3D& vecTurnSpeed) override;
	virtual void SetTurnSpeed(const CVector3D& vecTurnSpeed) override;

	/*
	virtual float GetTargetEngineRPM();
	virtual void SetTargetEngineRPM(float engineRPM);

	virtual float GetTargetWheelAngle();
	virtual void SetTargetWheelAngle(float wheelAngle);


	virtual float GetEngineRPM();
	virtual void SetEngineRPM(float engineRPM);

	virtual float GetWheelAngle();
	virtual void SetWheelAngle(float wheelAngle);
	*/
};
