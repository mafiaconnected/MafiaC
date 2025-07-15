#include "stdafx.h"
#include "ClientManager.h"
#include "ClientGame.h"
#include "Multiplayer.h"
#include "ClientEntity.h"
#include <Utils/VectorTools.h>

std::vector<CClientEntity*> Entities;

using namespace Galactic3D;

CClientEntity::CClientEntity(CMafiaClientManager* pClientManager) : CNetObject(pClientManager), m_pClientManager(pClientManager)
{
	m_Type = ELEMENT_ENTITY;

	m_Flags.m_bFindSyncer = true;
	m_Flags.m_bSendSync = true;

	m_mat.SetIdentity();
	m_uiLastReceivedSyncTicks = OS::GetTicks();
	m_szModel[0] = '\0';
	m_Position = CVector3D(0, 0, 0);
	m_RelativePosition = CVector3D(0, 0, 0);
	m_Rotation = CVector3D(0, 0, 0);
	m_RelativeRotation = CVector3D(0, 0, 0);
}

Galactic3D::ReflectedClass* CClientEntity::GetReflectedClass()
{
	return static_cast<CMafiaClientManager*>(m_pClientManager)->m_pClientEntityClass;
}

bool CClientEntity::SetPosition(const CVector3D& vecPos)
{
	m_Position = vecPos;
	return true;
}

bool CClientEntity::GetPosition(CVector3D& vecPos)
{
	vecPos = m_Position;
	return true;
}

bool CClientEntity::SetRotation(const CVector3D& vecRotation)
{
	m_Rotation = vecRotation;
	return true;
}

bool CClientEntity::GetRotation(CVector3D& vecRotation)
{
	vecRotation = m_Rotation;
	return true;
}

float CClientEntity::GetHeading()
{
	// Override by human/vehicle/etc heading funcs
	return 0.0;
}

bool CClientEntity::SetHeading(float heading)
{
	// Override by human/vehicle/etc heading funcs
	return true;
}

void CClientEntity::Remove()
{
	CNetObject::Remove();
	Delete();
}

// Note (Sevenisko): The Actor creation in Mafia must be specified type (Enemy, Player, Physical, Vehicle, etc)
bool CClientEntity::Create()
{
	return false;
}

void CClientEntity::Delete()
{
	if (m_pEntity != nullptr)
	{
		MafiaSDK::GetMission()->DelActor(m_pEntity);
		m_pEntity = nullptr;
	}
}

bool CClientEntity::ReadCreatePacket(Stream* pStream)
{
	if (!CNetObject::ReadCreatePacket(pStream))
		return false;

	CBinaryReader Reader(pStream);
	AutoFree<const GChar> mdl = Reader.ReadString(nullptr);
	_gstrcpy_s(m_szModel, ARRAY_COUNT(m_szModel), mdl);

	tEntityCreatePacket Packet;
	if (pStream->Read(&Packet, sizeof(Packet)) != sizeof(Packet))
		return false;

	m_Position = Packet.position;
	m_RelativePosition = Packet.positionRel;
	m_Rotation = Packet.rotation;
	m_RelativeRotation = Packet.rotationRel;

	return true;
}

bool CClientEntity::ReadSyncPacket(Stream* pStream)
{
	if (!CNetObject::ReadSyncPacket(pStream))
		return false;

	tEntitySyncPacket Packet;

	if (pStream->Read(&Packet, sizeof(Packet)) != sizeof(Packet))
		return false;

	m_Position = Packet.position;
	m_RelativePosition = Packet.positionRel;
	m_Rotation = Packet.rotation;
	m_RelativeRotation = Packet.rotationRel;

	return true;
}

bool CClientEntity::WriteCreatePacket(Stream* pStream)
{
	if (!CNetObject::WriteCreatePacket(pStream))
		return false;

	CBinaryWriter Writer(pStream);
	Writer.WriteString(m_szModel);

	tEntityCreatePacket Packet;

	Packet.position = m_Position;
	Packet.positionRel = m_RelativePosition;
	Packet.rotation = m_Rotation;
	Packet.rotationRel = m_RelativeRotation;

	if (pStream->Write(&Packet, sizeof(Packet)) != sizeof(Packet))
		return false;

	return true;
}

bool CClientEntity::WriteSyncPacket(Stream* pStream)
{
	if (!CNetObject::WriteSyncPacket(pStream))
		return false;

	tEntitySyncPacket Packet;

	Packet.position = m_Position;
	Packet.positionRel = m_RelativePosition;
	Packet.rotation = m_Rotation;
	Packet.rotationRel = m_RelativeRotation;

	if (pStream->Write(&Packet, sizeof(Packet)) != sizeof(Packet))
		return false;

	m_uiLastSendSyncTicks = OS::GetTicks();
	return true;
}

void CClientEntity::OnCreated()
{
	CNetObject::OnCreated();

	if (m_pEntity != nullptr)
	{
		CVector3D vecPos;
		GetPosition(vecPos);
	}
}

void CClientEntity::Process()
{
	if (m_pEntity == nullptr && m_Type != ELEMENT_DUMMY)
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

bool CClientEntity::SetModel(const GChar* modelName)
{
	_gstrcpy_s(m_szModel, ARRAY_COUNT(m_szModel), modelName);

	return true;
}

const GChar* CClientEntity::GetModel()
{
	return m_szModel;
}
