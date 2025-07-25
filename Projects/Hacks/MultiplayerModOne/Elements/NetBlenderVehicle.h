#pragma once
#include "ClientVehicle.h"
#include <NetBlenderLerp.h>

class CNetBlenderVehicle : public CNetBlenderLerp
{
public:
	CNetBlenderVehicle(CClientVehicle* pEntity);

	CClientVehicle* m_pEntity;

	CInterpolation m_RotationFront;
	CInterpolation m_RotationUp;
	CInterpolation m_RotationRight;

	CInterpolation m_RotationQuat;

	CInterpolation m_EngineRPM;
	CInterpolation m_WheelAngle;

	float m_fRotationMaxError = 1.0f;
	float m_fWheelAngleMaxError = 1.0f;
	float m_fEngineRPMMaxError = 1.0f;

	void SetTargetRotationMat(CVector3D& front, CVector3D& up, CVector3D& right);
	void SetTargetRotationQuat(CQuaternion& quatNewRotation);

	virtual void UpdateTargetRotation() override;
	virtual void UpdateTargetRotationMat();
	virtual void UpdateTargetRotationQuat();

	void UpdateTargetEngineRPM();
	void UpdateTargetWheelAngle();

	virtual void GetPosition(CVector3D& vecPos) override;
	virtual void SetPosition(const CVector3D& vecPos) override;

	virtual void GetRotation(CVector3D& vecRotation) override;
	virtual void SetRotation(const CVector3D& vecRotation) override;

	virtual void GetRotationMat(CVector3D& vecRotationFront, CVector3D& vecRotationUp, CVector3D& vecRotationRight);
	virtual void SetRotationMat(const CVector3D& vecRotationFront, const CVector3D& vecRotationUp, const CVector3D& vecRotationRight);

	virtual void GetRotationQuat(CQuaternion& quatRot);
	virtual void SetRotationQuat(CQuaternion& quatRot);

	virtual void GetMoveSpeed(CVector3D& vecMoveSpeed) override;
	virtual void SetMoveSpeed(const CVector3D& vecMoveSpeed) override;

	virtual void GetTurnSpeed(CVector3D& vecTurnSpeed) override;
	virtual void SetTurnSpeed(const CVector3D& vecTurnSpeed) override;

	virtual float GetEngineRPM();
	virtual void SetEngineRPM(float engineRPM);

	virtual float GetWheelAngle();
	virtual void SetWheelAngle(float wheelAngle);

	virtual void ResetInterpolation() override;
};
