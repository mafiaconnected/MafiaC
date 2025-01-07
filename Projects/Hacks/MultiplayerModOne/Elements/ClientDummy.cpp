#include "stdafx.h"
#include "ClientManager.h"
#include "ClientGame.h"
#include "Multiplayer.h"
#include "ClientDummy.h"
#include <Utils/VectorTools.h>

using namespace Galactic3D;

CClientDummy::CClientDummy(CMafiaClientManager* pClientManager) : CClientEntity(pClientManager)
{
	m_Type = ELEMENT_DUMMY;

	m_Flags.m_bFindSyncer = false;
	m_Flags.m_bSendSync = false;
	//m_Flags.m_bNetworked = false;
}

Galactic3D::ReflectedClass* CClientDummy::GetReflectedClass(void)
{
	return static_cast<CMafiaClientManager*>(m_pClientManager)->m_pClientDummyClass;
}