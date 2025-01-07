#pragma once

#include "ClientEntity.h"
#include "../ClientManager.h"
#include "../ClientGame.h"

class CClientDoor : public CClientEntity
{
public:
	CClientDoor(CMafiaClientManager* pClientManager);

	CMafiaClientManager* m_pClientManager;

	float m_DoorOpenAngle;
	bool m_bLocked;
	MafiaSDK::C_Door* m_MafiaDoor;

	virtual Galactic3D::ReflectedClass* GetReflectedClass(void);

	virtual MafiaSDK::C_Door* GetGameDoor();

	virtual bool ReadCreatePacket(Galactic3D::Stream* pStream) override;
	virtual bool WriteCreatePacket(Galactic3D::Stream* pStream) override;

	virtual bool SetOpenAngle(float angle);
	virtual float GetOpenAngle();

	virtual bool SetLocked(bool locked);
	virtual bool GetLocked();
};
