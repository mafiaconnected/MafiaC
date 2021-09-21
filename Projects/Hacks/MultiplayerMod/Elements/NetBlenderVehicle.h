#pragma once
#include "ClientVehicle.h"
#include <NetBlenderLerp.h>

class CNetBlenderVehicle : public CNetBlenderLerp
{
public:
	CNetBlenderVehicle(CClientVehicle* pEntity);

	CClientVehicle* m_pEntity;

	virtual void GetPosition(CVector3D& vecPos) override;
	virtual void SetPosition(const CVector3D& vecPos) override;

	virtual void GetRotation(CVector3D& vecRotation) override;
	virtual void SetRotation(const CVector3D& vecRotation) override;

	virtual void GetMoveSpeed(CVector3D& vecMoveSpeed) override;
	virtual void SetMoveSpeed(const CVector3D& vecMoveSpeed) override;

	virtual void GetTurnSpeed(CVector3D& vecTurnSpeed) override;
	virtual void SetTurnSpeed(const CVector3D& vecTurnSpeed) override;
};
