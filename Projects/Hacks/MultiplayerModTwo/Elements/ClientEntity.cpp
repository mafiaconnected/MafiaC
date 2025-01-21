#include "stdafx.h"
#include "ClientManager.h"
#include "ClientGame.h"
#include "Multiplayer.h"
#include "ClientEntity.h"
#include <Utils/VectorTools.h>

std::vector<CClientEntityII*> Entities;

using namespace Galactic3D;

CClientEntityII::CClientEntityII(CMafiaClientManagerII* pClientManager) : CNetObject(pClientManager), m_pClientManager(pClientManager)
{
	m_Type = ELEMENT_ENTITY;

	m_Flags.m_bFindSyncer = true;
	m_Flags.m_bSendSync = true;

	m_mat.SetIdentity();
	m_uiLastReceivedSyncTicks = OS::GetTicks();
	m_Position = CVector3D(0, 0, 0);
	m_RelativePosition = CVector3D(0, 0, 0);
	m_Rotation = CVector3D(0, 0, 0);
	m_RelativeRotation = CVector3D(0, 0, 0);
}

Galactic3D::ReflectedClass* CClientEntityII::GetReflectedClass()
{
	return static_cast<CMafiaClientManagerII*>(m_pClientManager)->m_pClientEntityClass;
}

bool CClientEntityII::SetPosition(const CVector3D& vecPos)
{
	m_Position = vecPos;
	return true;
}

bool CClientEntityII::GetPosition(CVector3D& vecPos)
{
	vecPos = m_Position;
	return true;
}

bool CClientEntityII::SetRotation(const CVector3D& vecRotation)
{
	m_Rotation = vecRotation;
	return true;
}

bool CClientEntityII::GetRotation(CVector3D& vecRotation)
{
	vecRotation = m_Rotation;
	return true;
}

void CClientEntityII::Remove()
{
	CNetObject::Remove();
	Delete();
}

// Note (Sevenisko): The Actor creation in Mafia must be specified type (Enemy, Player, Physical, Vehicle, etc)
bool CClientEntityII::Create()
{
	return false;
}

void CClientEntityII::Delete()
{
	if (m_pEntity != nullptr)
	{
		//M2::Wrappers::DestroyEntity(m_pEntity, g_pClientGame->ToMafiaEntityType(m_Type));
		m_pEntity = nullptr;
	}
}

bool CClientEntityII::ReadCreatePacket(Stream* pStream)
{
    if (!CNetObject::ReadCreatePacket(pStream))
        return false;

	tMafia2EntityCreatePacket Packet;

	if (pStream->Read(&Packet, sizeof(Packet)) != sizeof(Packet))
		return false;

	m_Model = Packet.model;
	m_Position = Packet.position;
	m_RelativePosition = Packet.positionRel;
	m_Rotation = Packet.rotation;
	m_RelativeRotation = Packet.rotationRel;

    return true;
}

bool CClientEntityII::ReadSyncPacket(Stream* pStream)
{
	if (!CNetObject::ReadSyncPacket(pStream))
		return false;

	tMafia2EntitySyncPacket Packet;

	if (pStream->Read(&Packet, sizeof(Packet)) != sizeof(Packet))
		return false;

	m_Position = Packet.position;
	m_RelativePosition = Packet.positionRel;
	m_Rotation = Packet.rotation;
	m_RelativeRotation = Packet.rotationRel;

	return true;
}

bool CClientEntityII::WriteCreatePacket(Stream* pStream)
{
	if (!CNetObject::WriteCreatePacket(pStream))
		return false;

	tMafia2EntityCreatePacket Packet;

	Packet.model = m_Model;
	Packet.position = m_Position;
	Packet.positionRel = m_RelativePosition;
	Packet.rotation = m_Rotation;
	Packet.rotationRel = m_RelativeRotation;

	if (pStream->Write(&Packet, sizeof(Packet)) != sizeof(Packet))
		return false;

	return true;
}

bool CClientEntityII::WriteSyncPacket(Stream* pStream)
{
	if (!CNetObject::WriteSyncPacket(pStream))
		return false;

	tMafia2EntitySyncPacket Packet;

	Packet.position = m_Position;
	Packet.positionRel = m_RelativePosition;
	Packet.rotation = m_Rotation;
	Packet.rotationRel = m_RelativeRotation;

	if (pStream->Write(&Packet, sizeof(Packet)) != sizeof(Packet))
		return false;

	m_uiLastSendSyncTicks = OS::GetTicks();
	return true;
}

void CClientEntityII::OnCreated()
{
	CNetObject::OnCreated();

	if (m_pEntity != nullptr)
	{
		CVector3D vecPos;
		GetPosition(vecPos);
	}
}

void CClientEntityII::Process()
{
	if (m_pEntity == nullptr)
		return;

	{
		bool bPreventDefault = false;
		CArguments Arguments(1);
		Arguments.AddObject(this);
		g_pClientGame->m_pOnEntityProcessEventType->Trigger(Arguments, bPreventDefault);
		/*if (!bPreventDefault)
			m_pEntity->ProcessControl();*/
	}

	if (GetChildren().GetCount() > 0)
	{
		CVector3D vecPos;
		GetPosition(vecPos);

		for (size_t i = 0; i<GetChildren().GetSize(); i++)
		{
			if (GetChildren().IsUsedAt(i))
			{
				CNetObject* pAttachedElement = static_cast<CNetObject*>(GetChildren().GetAt(i));

				if (pAttachedElement->IsType(ELEMENT_ENTITY))
				{
					pAttachedElement->SetPosition(vecPos);
				}
			}
		}
	}
}

bool CClientEntityII::SetModel(uint32_t model)
{
	m_Model = model;
	return true;
}

uint32_t CClientEntityII::GetModel()
{
	return m_Model;
}
