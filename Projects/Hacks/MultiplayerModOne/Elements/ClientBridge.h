#pragma once

#include "ClientEntity.h"
#include "../ClientManager.h"
#include "../ClientGame.h"

class CClientBridge : public CClientEntity
{
public:
	CClientBridge(CMafiaClientManager* pClientManager);

	bool m_Enabled = false;
	MafiaSDK::C_Bridge* m_MafiaBridge = nullptr;

	virtual Galactic3D::ReflectedClass* GetReflectedClass() override;

	virtual MafiaSDK::C_Bridge* GetGameBridge();

	virtual bool ReadCreatePacket(Galactic3D::Stream* pStream) override;
	virtual bool WriteCreatePacket(Galactic3D::Stream* pStream) override;

	virtual bool SetEnabled(bool enabled);
	virtual bool GetEnabled();
};
