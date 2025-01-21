#include "stdafx.h"
#include "ClientManager.h"
#include "ClientGame.h"
#include "Multiplayer.h"
#include "ClientObject.h"
#include <Utils/VectorTools.h>

using namespace Galactic3D;

CClientObject::CClientObject(CMafiaClientManager* pClientManager) : CClientEntity(pClientManager)
{
	m_Type = ELEMENT_OBJECT;
}

Galactic3D::ReflectedClass* CClientObject::GetReflectedClass()
{
	return static_cast<CMafiaClientManager*>(m_pClientManager)->m_pClientObjectClass;
}

void CClientObject::Create(const CVector3D& pos, const CVector3D& rot)
{
	if (m_MafiaObject != nullptr)
	{
		printf("Failed to create object. Game object already exists.");
		return;
	}

	auto pModel = (MafiaSDK::I3D_Model*)MafiaSDK::I3DGetDriver()->CreateFrame(MafiaSDK::I3D_Driver_Enum::FrameType::MODEL);

	UTF8String model(true, m_szModel);

	MafiaSDK::GetModelCache()->Open(pModel, model.CString(), NULL, NULL, NULL, NULL);

	pModel->SetName("SomeObject");
	pModel->SetScale({ 1, 1, 1 });

	//S_quat quat;
	//CVecTools::rotate(rot.z, rot.y, -M_PI, quat);
	//pVehModel->SetRot(quat);
	//pVehModel->SetWorldPos(CVecTools::ConvertToMafiaVec(pos));

	pModel->Update();

	g_pClientGame->m_bCreateActorInvokedByGame = false;
	m_MafiaObject = reinterpret_cast<MafiaSDK::C_Actor*>(MafiaSDK::GetMission()->CreateActor(MafiaSDK::C_Mission_Enum::ObjectTypes::Physical));
	g_pClientGame->m_bCreateActorInvokedByGame = true;

	if (m_MafiaObject == nullptr)
	{
		printf("Failed to create object. Game actor creation failed. Model: %s\n", model.CString());
		return;
	}

	SetPosition(pos);
	SetRotation(rot);

	m_MafiaObject->Init(pModel);
	m_MafiaObject->SetActive(true);

	MafiaSDK::GetMission()->GetGame()->AddTemporaryActor(m_MafiaObject);

	m_pEntity = m_MafiaObject;
}

void CClientObject::Remove()
{
	CNetObject::Remove();

	Delete();
}

void CClientObject::Delete()
{
	Despawn();
}

void CClientObject::Despawn()
{
	CClientEntity::Delete();

	MafiaSDK::GetMission()->GetGame()->RemoveTemporaryActor(GetGameObject());
}

bool CClientObject::SetModel(const GChar* modelName)
{
	return true;
}

const GChar* CClientObject::GetModel()
{
	return CClientEntity::GetModel();
}

void CClientObject::UpdateGameMatrix()
{
	if (m_MafiaObject->GetFrame() == nullptr)
		return;

	uint32_t frame = (uint32_t)m_MafiaObject->GetFrame();

	*(uint32_t*)(frame + 172) &= ~0x10;

	__asm
	{
		pushad
		pushfd
		mov eax, 0x60FC30 // update frame
		mov ecx, frame
		call eax
		popfd
		popad
	}

	m_MafiaObject->GetFrame()->Update();
}

MafiaSDK::C_Actor* CClientObject::GetGameObject()
{
	return m_MafiaObject;
}

bool CClientObject::ReadCreatePacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntity::ReadCreatePacket(pStream))
		return false;

	//_glogprintf(_gstr("Got create packet for element #%d:\n\tModel: %s\n\tPosition: [%f, %f, %f - %f, %f, %f]\n\tRotation: [%f, %f, %f - %f, %f, %f]\n", GetId(), m_szModel, m_Position.x, m_Position.y, m_Position.z, m_RelativePosition.x, m_RelativePosition.y, m_RelativePosition.z, m_Rotation.x, m_Rotation.y, m_Rotation.z, m_RelativeRotation.x, m_RelativeRotation.y, m_RelativeRotation.z));

	if (GetGameObject() == nullptr)
	{
		Create(m_Position, m_Rotation);
	}

	return true;
}