#pragma once
#include "ClientBridge.h"
#include <NetBlenderLerp.h>

class CNetBlenderBridge : public CNetBlenderLerp
{
public:
	CNetBlenderBridge(CClientBridge* pEntity);

	CClientBridge* m_pEntity;
	float m_fRotationMaxError = 1.0f;

	void SetTargetRotation(CVector3D& vecRotation);

	virtual void GetPosition(CVector3D& vecPos) override;
	virtual void SetPosition(const CVector3D& vecPos) override;

	virtual void GetRotation(CVector3D& vecRotation) override;
	virtual void SetRotation(const CVector3D& vecRotation) override;

	virtual void GetMoveSpeed(CVector3D& vecMoveSpeed) override;
	virtual void SetMoveSpeed(const CVector3D& vecMoveSpeed) override;

	virtual void GetTurnSpeed(CVector3D& vecTurnSpeed) override;
	virtual void SetTurnSpeed(const CVector3D& vecTurnSpeed) override;

	virtual void UpdateTargetRotation() override;
};
