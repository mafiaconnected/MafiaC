#pragma once
#include "ClientHuman.h"
#include <NetBlenderLerp.h>

class CNetBlenderHuman : public CNetBlenderLerp
{
public:
	CNetBlenderHuman(CClientHuman* pEntity);

	CClientHuman* m_pEntity;
	float m_fRotationMaxError = 1.0f;
	float m_fVehicleAimMaxError = 1.0f;

	CInterpolation m_VehicleAim;

	void SetTargetRotation(CVector3D& vecRotation);
	void SetTargetVehicleAim(float aim);

	virtual void GetPosition(CVector3D& vecPos) override;
	virtual void SetPosition(const CVector3D& vecPos) override;

	virtual void GetRotation(CVector3D& vecRotation) override;
	virtual void SetRotation(const CVector3D& vecRotation) override;

	virtual void GetMoveSpeed(CVector3D& vecMoveSpeed) override;
	virtual void SetMoveSpeed(const CVector3D& vecMoveSpeed) override;

	virtual void GetTurnSpeed(CVector3D& vecTurnSpeed) override;
	virtual void SetTurnSpeed(const CVector3D& vecTurnSpeed) override;

	bool IsInVehicle();

	float GetVehicleAim();
	void SetVehicleAim(float aim);

	virtual void UpdateTargetRotation() override;
	void UpdateTargetVehicleAim();

	virtual void ResetInterpolation() override;

	virtual void Interpolate() override;
};
