#pragma once

#include "ClientHuman.h"
#include "../ClientManager.h"
#include "../ClientGame.h"

class CClientPlayer : public CClientHuman
{
private:
	MafiaSDK::C_Player* m_MafiaPlayer;

public:
	CClientPlayer(CMafiaClientManager* pClientManager);

	int32_t m_uiMoney = 0;

	virtual Galactic3D::ReflectedClass* GetReflectedClass() override;

	virtual void SetMoney(int32_t uiMoney);
	virtual int32_t GetMoney();

	virtual MafiaSDK::C_Player* GetGamePlayer();
};
