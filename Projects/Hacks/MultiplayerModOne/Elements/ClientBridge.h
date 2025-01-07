#pragma once

#include "ClientEntity.h"
#include "../ClientManager.h"
#include "../ClientGame.h"

class CClientBridge : public CClientEntity
{
public:
	CClientBridge(CMafiaClientManager* pClientManager);

	CMafiaClientManager* m_pClientManager;

	bool m_Enabled;
	MafiaSDK::C_Bridge* m_MafiaBridge;

	virtual Galactic3D::ReflectedClass* GetReflectedClass(void);

	virtual MafiaSDK::C_Bridge* GetGameBridge();

	virtual bool ReadCreatePacket(Galactic3D::Stream* pStream) override;
	virtual bool WriteCreatePacket(Galactic3D::Stream* pStream) override;

	virtual bool SetEnabled(bool enabled);
	virtual bool GetEnabled();
};
