#pragma once

#include "ClientEntity.h"
#include "../ClientManager.h"
#include "../ClientGame.h"

class CClientDoor : public CClientEntity
{
public:
	CClientDoor(CMafiaClientManager* pClientManager);

	float m_DoorOpenAngle = 0.0f;
	uint32_t m_uiState = 0;
	MafiaSDK::C_Door* m_MafiaDoor = nullptr;

	virtual Galactic3D::ReflectedClass* GetReflectedClass() override;

	virtual MafiaSDK::C_Door* GetGameDoor();

	virtual bool ReadCreatePacket(Galactic3D::Stream* pStream) override;
	virtual bool WriteCreatePacket(Galactic3D::Stream* pStream) override;

	virtual bool SetOpenAngle(float angle);
	virtual float GetOpenAngle();

	virtual bool SetState(uint32_t state, MafiaSDK::C_Actor* actor, bool unk1, bool unk2);
	uint32_t GetState();

	virtual void SetFromExistingEntity(MafiaSDK::C_Door* door);
};