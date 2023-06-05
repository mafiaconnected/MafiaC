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
}

Galactic3D::ReflectedClass* CClientDummy::GetReflectedClass(void)
{
	return static_cast<CMafiaClientManager*>(m_pClientManager)->m_pClientDummyClass;
}