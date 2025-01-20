#include "stdafx.h"
#include "ClientManager.h"
#include "ClientGame.h"
#include "Multiplayer.h"
#include "ClientEntity.h"
#include <Utils/VectorTools.h>

std::vector<CClientDoor*> Doors;

using namespace Galactic3D;

CClientDoor::CClientDoor(CMafiaClientManager* pClientManager) : CClientEntity(pClientManager)
{
	m_Type = ELEMENT_DOOR;

	m_Flags.m_bFindSyncer = false;
	m_Flags.m_bSendSync = false;

	m_pEntity = NULL;

	m_bLocked = false;
	m_DoorOpenAngle = 0.0f;
}

Galactic3D::ReflectedClass* CClientDoor::GetReflectedClass(void)
{
	return static_cast<CMafiaClientManager*>(m_pClientManager)->m_pClientDoorClass;
}

MafiaSDK::C_Door* CClientDoor::GetGameDoor()
{
	return m_MafiaDoor;
}

bool CClientDoor::ReadCreatePacket(Galactic3D::Stream* pStream)
{
	abort();
}

bool CClientDoor::WriteCreatePacket(Galactic3D::Stream* pStream)
{
	abort();
}

bool CClientDoor::SetOpenAngle(float angle)
{
	abort();
}

float CClientDoor::GetOpenAngle()
{
	abort();
}

bool CClientDoor::SetLocked(bool locked)
{
	abort();
}

bool CClientDoor::GetLocked()
{
	abort();
}
