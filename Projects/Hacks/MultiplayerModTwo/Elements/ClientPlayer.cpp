#include "stdafx.h"
#include "ClientManager.h"
#include "ClientGame.h"
#include "ClientPlayer.h"
#include "ClientHuman.h"
#include "ClientEntity.h"
#include "Multiplayer.h"

std::vector<CClientPlayerII*> Players;

using namespace Galactic3D;

CClientPlayerII::CClientPlayerII(CMafiaClientManagerII* pClientManager) : CClientHumanII(pClientManager)
{
	m_Type = ELEMENT_PLAYER;
	m_uiMoney = 0;
}

Galactic3D::ReflectedClass* CClientPlayerII::GetReflectedClass(void)
{
	return static_cast<CMafiaClientManagerII*>(m_pClientManager)->m_pClientPlayerClass;
}

void CClientPlayerII::SetMoney(int32_t uiMoney)
{
	// m_uiMoney =
}

int32_t CClientPlayerII::GetMoney()
{
	return 0;

	// return m_uiMoney;
}

M2::C_Player2* CClientPlayerII::GetGamePlayer()
{
	return (M2::C_Player2*)GetGameHuman();
}