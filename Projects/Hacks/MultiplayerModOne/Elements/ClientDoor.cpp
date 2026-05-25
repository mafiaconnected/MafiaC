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
	m_Flags.m_bDistanceStreaming = false;
}

Galactic3D::ReflectedClass* CClientDoor::GetReflectedClass()
{
	return static_cast<CMafiaClientManager*>(m_pClientManager)->m_pClientDoorClass;
}

MafiaSDK::C_Door* CClientDoor::GetGameDoor()
{
	return m_MafiaDoor;
}

bool CClientDoor::ReadCreatePacket(Galactic3D::Stream* pStream)
{
	if (GetGameDoor() == nullptr)
		return false;
}

bool CClientDoor::WriteCreatePacket(Galactic3D::Stream* pStream)
{
	if (GetGameDoor() == nullptr)
		return false;
}

bool CClientDoor::SetOpenAngle(float angle)
{
	if (GetGameDoor() == nullptr)
		return false;

	GetGameDoor()->SetOpenAngle(angle);
	return true;
}

float CClientDoor::GetOpenAngle()
{
	if (GetGameDoor() == nullptr)
		return false;

	return GetGameDoor()->GetInterface()->open_angle;
}

bool CClientDoor::SetState(uint32_t state, MafiaSDK::C_Actor* actor, bool unk1, bool unk2) {
	if (GetGameDoor() == nullptr)
		return false;

	GetGameDoor()->SetState((MafiaSDK::C_Door_Enum::States)state, actor, unk1, unk2);
}

uint32_t CClientDoor::GetState() {
	if (GetGameDoor() == nullptr)
		return false;

	return (uint32_t)GetGameDoor()->GetInterface()->current_state;
}

void CClientDoor::SetFromExistingEntity(MafiaSDK::C_Door* door)
{
	m_pEntity = door;
	m_MafiaDoor = door;

	/*
	if (g_pClientGame->GetMultiplayer() != nullptr)
	{
		if (IsSyncer())
		{
			Packet Packet(MAFIAPACKET_DOOR_UPDATE);
			Packet.Write<int32_t>(GetId());
			Packet.Write<uint32_t>(GetState());
			Packet.Write<float>(m_DoorOpenAngle);
			g_pClientGame->GetMultiplayer()->SendHostPacket(&Packet);
		}
	}
	*/
}