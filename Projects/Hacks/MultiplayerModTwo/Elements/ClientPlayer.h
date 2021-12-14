#pragma once

#include "ClientHuman.h"
#include "../ClientManager.h"
#include "../ClientGame.h"

class CClientPlayerII : public CClientHumanII
{
private:
	M2::C_Player2* m_MafiaPlayer;

public:
	CClientPlayerII(CMafiaClientManagerII* pClientManager);

	int32_t m_uiMoney;

	virtual Galactic3D::ReflectedClass* GetReflectedClass(void);

	virtual void SetMoney(int32_t uiMoney);
	virtual int32_t GetMoney();

	virtual M2::C_Player2* GetGamePlayer();
};
