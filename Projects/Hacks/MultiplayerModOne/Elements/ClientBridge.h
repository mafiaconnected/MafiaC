#pragma once

#include "ClientEntity.h"
#include "../ClientManager.h"
#include "../ClientGame.h"
#include "NetBlenderBridge.h"

class CClientBridge : public CClientEntity
{
public:
	CClientBridge(CMafiaClientManager* pClientManager);

	bool m_Enabled = false;
	MafiaSDK::C_Bridge* m_MafiaBridge = nullptr;

	virtual Galactic3D::ReflectedClass* GetReflectedClass() override;

	virtual MafiaSDK::C_Bridge* GetGameBridge();

	virtual void Process() override;

	virtual bool ReadCreatePacket(Galactic3D::Stream* pStream) override;
	virtual bool WriteCreatePacket(Galactic3D::Stream* pStream) override;

	virtual bool ReadSyncPacket(Galactic3D::Stream* pStream) override;
	virtual bool WriteSyncPacket(Galactic3D::Stream* pStream) override;

	virtual bool SetPosition(const CVector3D& vecPos) override;
	virtual bool GetPosition(CVector3D& vecPos) override;

	virtual bool SetRotation(const CVector3D& vecRot) override;
	virtual bool GetRotation(CVector3D& vecRot) override;

	virtual void CreateNetBlender() override;

	virtual void SetFromExistingEntity(MafiaSDK::C_Bridge* bridge);

	virtual bool SetEnabled(bool enabled);
	virtual bool GetEnabled();
};
