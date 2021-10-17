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
	m_pEntity = NULL;
	m_mat.SetIdentity();
	m_pRelativeElement = nullptr;
	m_uiLastReceivedSyncTicks = OS::GetTicks();
	m_uiLastSendSyncTicks = 0;
	m_fPacketArrivalRatio = 0.0f;
}

Galactic3D::ReflectedClass* CClientEntity::GetReflectedClass(void)
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

void CClientEntity::Remove(void)
{
	CNetObject::Remove();
	Delete();
}

// Note (Sevenisko): The Actor creation in Mafia must be specified type (Enemy, Player, Physical, Vehicle, etc)
bool CClientEntity::Create(void)
{
	return false;
}

void CClientEntity::Delete(void)
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

    size_t size;

    GChar* pszModel = Reader.ReadString(&size);
    if (pszModel == nullptr)
        return false;

    _gstrcpy_s(m_szModel, ARRAY_COUNT(m_szModel), pszModel);
    GFree(pszModel);

	Reader.ReadVector3D(&m_Position, 1);
	Reader.ReadVector3D(&m_RelativePosition, 1);
	Reader.ReadVector3D(&m_Rotation, 1);
	Reader.ReadVector3D(&m_RelativeRotation, 1);

	//SetPosition(m_Position);
	//SetRotation(m_Rotation);

    return true;
}

bool CClientEntity::ReadSyncPacket(Stream* pStream)
{
	if (!CNetObject::ReadSyncPacket(pStream))
		return false;

	CBinaryReader Reader(pStream);

	Reader.ReadVector3D(&m_Position, 1);
	Reader.ReadVector3D(&m_RelativePosition, 1);
	Reader.ReadVector3D(&m_Rotation, 1);
	Reader.ReadVector3D(&m_RelativeRotation, 1);

	//SetPosition(m_Position);
	//SetRotation(m_Rotation);

	return true;
}

bool CClientEntity::WriteCreatePacket(Stream* pStream)
{
	if (!CNetObject::WriteCreatePacket(pStream))
		return false;

	CBinaryWriter Writer(pStream);

	Writer.WriteString(m_szModel);
	Writer.WriteVector3D(m_Position);
	Writer.WriteVector3D(m_RelativePosition);
	Writer.WriteVector3D(m_Rotation);
	Writer.WriteVector3D(m_RelativeRotation);

	return true;
}

bool CClientEntity::WriteSyncPacket(Stream* pStream)
{
	if (!CNetObject::WriteSyncPacket(pStream))
		return false;

	CBinaryWriter Writer(pStream);

	Writer.WriteVector3D(m_Position);
	Writer.WriteVector3D(m_RelativePosition);
	Writer.WriteVector3D(m_Rotation);
	Writer.WriteVector3D(m_RelativeRotation);

	m_uiLastSendSyncTicks = OS::GetTicks();
	return true;
}

void CClientEntity::OnCreated(void)
{
	CNetObject::OnCreated();

	if (m_pEntity != nullptr)
	{
		CVector3D vecPos;
		GetPosition(vecPos);
	}
}

void CClientEntity::Process(void)
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

bool CClientEntity::SetModel(const GChar* modelName)
{
	_gstrcpy_s(m_szModel, ARRAY_COUNT(m_szModel), modelName);

	return true;
}

const GChar* CClientEntity::GetModel()
{
	return m_szModel;
}