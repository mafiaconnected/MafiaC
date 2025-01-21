#pragma once

#include "../ClientManager.h"
#include "../ClientGame.h"

class CClientEntityII : public CNetObject
{
public:
	CClientEntityII(CMafiaClientManagerII* pClientManager);

	CMafiaClientManagerII* m_pClientManager;

	M2::C_Entity* m_pEntity = nullptr;
	CMatrix4x4 m_mat;
	uint32_t m_Model = 0;
	CClientEntityII* m_pRelativeElement = nullptr;
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

	virtual bool SetModel(uint32_t modelName);
	virtual uint32_t GetModel();

	virtual bool SetPosition(const CVector3D& vecPos) override;
	virtual bool GetPosition(CVector3D& vecPos) override;

	virtual bool SetRotation(const CVector3D& vecRotation) override;
	virtual bool GetRotation(CVector3D& vecRotation) override;

	virtual bool SetHeading(float heading) { return true; };
	virtual float GetHeading() { return 0.0f; };
};
