#pragma once

#include "../ClientManager.h"
#include "../ClientGame.h"

class CClientEntity : public CNetObject
{
public:
	CClientEntity(CMafiaClientManager* pClientManager);

	CMafiaClientManager* m_pClientManager;

	MafiaSDK::C_Actor* m_pEntity = nullptr;
	CMatrix4x4 m_mat;
	GChar m_szModel[64];
	CClientEntity* m_pRelativeElement = nullptr;
	uint32_t m_uiLastReceivedSyncTicks;
	uint32_t m_uiLastSendSyncTicks = 0;
	float m_fPacketArrivalRatio = 0.0f;

	CVector3D m_Position;
	CVector3D m_Rotation;
	CVector3D m_RelativePosition;
	CVector3D m_RelativeRotation;

	virtual Galactic3D::ReflectedClass* GetReflectedClass() override;

	virtual bool Create();

	virtual void Remove();

	virtual void Delete();

	virtual bool ReadCreatePacket(Galactic3D::Stream* pStream) override;
	virtual bool ReadSyncPacket(Galactic3D::Stream* pStream) override;

	virtual bool WriteCreatePacket(Galactic3D::Stream* pStream) override;
	virtual bool WriteSyncPacket(Galactic3D::Stream* pStream) override;

	virtual void OnCreated();
	virtual void Process();

	virtual bool SetModel(const GChar* modelName);
	virtual const GChar* GetModel();

	virtual bool SetPosition(const CVector3D& vecPos) override;
	virtual bool GetPosition(CVector3D& vecPos) override;

	virtual bool SetRotation(const CVector3D& vecRotation) override;
	virtual bool GetRotation(CVector3D& vecRotation) override;

	virtual bool SetHeading(float heading);
	virtual float GetHeading();
};
