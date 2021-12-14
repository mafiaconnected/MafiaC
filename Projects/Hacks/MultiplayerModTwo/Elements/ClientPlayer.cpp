#include "stdafx.h"
#include "ClientManager.h"
#include "ClientGame.h"
#include "ClientPlayer.h"
#include "ClientHuman.h"
#include "ClientEntity.h"
#include "Multiplayer.h"

std::vector<CClientPlayer*> Players;

using namespace Galactic3D;

CClientPlayer::CClientPlayer(CMafiaClientManager* pClientManager) : CClientHuman(pClientManager)
{
	m_Type = ELEMENT_PLAYER;
	m_uiMoney = 0;
}

Galactic3D::ReflectedClass* CClientPlayer::GetReflectedClass(void)
{
	return static_cast<CMafiaClientManager*>(m_pClientManager)->m_pClientPlayerClass;
}

void CClientPlayer::SetMoney(int32_t uiMoney)
{
	// m_uiMoney =
}

int32_t CClientPlayer::GetMoney()
{
	return 0;

	// return m_uiMoney;
}

M2::C_Player2* CClientPlayer::GetGamePlayer()
{
	return (M2::C_Player2*)GetGameHuman();
}