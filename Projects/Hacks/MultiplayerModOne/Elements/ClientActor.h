#pragma once

#include "ClientEntity.h"
#include "../ClientManager.h"
#include "../ClientGame.h"
#include "NetBlenderActor.h"

// Wraps a scene-loaded scripted actor (e.g. a trolley) that is looked up in the local game by name,
// rather than spawned by this mod - so it must never be deleted through the generic CClientEntity::Delete()
// path (hence m_pMafiaActor is kept separate from the base CNetObject::m_pEntity field, which IS deleted
// there). Deliberately does not use CClientEntity::m_pEntity for the same reason.
class CClientActor : public CClientEntity
{
private:
	MafiaSDK::C_Actor* m_pMafiaActor = nullptr;

	CVector3D m_vecVelocity{ 0, 0, 0 };
	CVector3D m_vecRotationVelocity{ 0, 0, 0 };

	CVector3D m_vecPrevSyncPosition{ 0, 0, 0 };
	CVector3D m_vecPrevSyncRotation{ 0, 0, 0 };

public:
	CClientActor(CMafiaClientManager* pClientManager);

	virtual Galactic3D::ReflectedClass* GetReflectedClass() override;

	// Re-resolves by name (GetName()) until it succeeds - the actor may not be loaded in this client's
	// scene yet at the moment the create packet arrives, and there is no retry on a failed create.
	virtual MafiaSDK::C_Actor* GetGameActor();

	virtual void Process() override;

	virtual bool SetPosition(const CVector3D& vecPos) override;
	virtual bool GetPosition(CVector3D& vecPos) override;

	virtual bool SetRotation(const CVector3D& vecRotation) override;
	virtual bool GetRotation(CVector3D& vecRotation) override;

	virtual bool SetVelocity(const CVector3D& vecVel);
	virtual bool GetVelocity(CVector3D& vecVel);

	virtual bool SetRotationVelocity(const CVector3D& vecRotVel);
	virtual bool GetRotationVelocity(CVector3D& vecRotVel);

	virtual bool ReadCreatePacket(Galactic3D::Stream* pStream) override;
	virtual bool ReadSyncPacket(Galactic3D::Stream* pStream) override;

	virtual bool WriteCreatePacket(Galactic3D::Stream* pStream) override;
	virtual bool WriteSyncPacket(Galactic3D::Stream* pStream) override;

	virtual void CreateNetBlender() override;
};
