#pragma once

#include "ClientEntity.h"
#include "../ClientManager.h"
#include "../ClientGame.h"
#include "NetBlenderHuman.h"

class CClientObject : public CClientEntity
{
private:
	MafiaSDK::C_Actor* m_MafiaObject;

public:
	CClientObject(CMafiaClientManager* pClientManager);

	virtual Galactic3D::ReflectedClass* GetReflectedClass(void);

	virtual MafiaSDK::C_Actor* GetGameObject();

	//virtual void Process(void) override;
	virtual void Create(const CVector3D& pos, const CVector3D& rot);
	virtual void Delete(void);
	virtual void Despawn(void);
	virtual void Remove(void);

	virtual bool SetModel(const GChar* modelName) override;
	virtual const GChar* GetModel() override;

	void UpdateGameMatrix(void);

	//virtual bool SetPosition(const CVector3D& vecPos) override;
	//virtual bool GetPosition(CVector3D& vecPos) override;

	//virtual bool SetRotation(const CVector3D& vecPos) override;
	//virtual bool GetRotation(CVector3D& vecPos) override;

	virtual bool ReadCreatePacket(Galactic3D::Stream* pStream) override;
	//virtual bool ReadSyncPacket(Galactic3D::Stream* pStream) override;

	//virtual bool WriteCreatePacket(Galactic3D::Stream* pStream) override;
	//virtual bool WriteSyncPacket(Galactic3D::Stream* pStream) override;
};
