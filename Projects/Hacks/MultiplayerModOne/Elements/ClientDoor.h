#pragma once

#include "ClientEntity.h"
#include "../ClientManager.h"
#include "../ClientGame.h"

class CClientDoor : public CClientEntity
{
public:
	CClientDoor(CMafiaClientManager* pClientManager);

	float m_DoorOpenAngle = 0.0f;
	bool m_bLocked = false;
	MafiaSDK::C_Door* m_MafiaDoor = nullptr;

	virtual Galactic3D::ReflectedClass* GetReflectedClass() override;

	virtual MafiaSDK::C_Door* GetGameDoor();

	virtual bool ReadCreatePacket(Galactic3D::Stream* pStream) override;
	virtual bool WriteCreatePacket(Galactic3D::Stream* pStream) override;

	virtual bool SetOpenAngle(float angle);
	virtual float GetOpenAngle();

	virtual bool SetLocked(bool locked);
	virtual bool GetLocked();
};
