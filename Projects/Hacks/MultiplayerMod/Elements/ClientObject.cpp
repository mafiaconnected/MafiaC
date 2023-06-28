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

	m_MafiaObject = nullptr;
}

Galactic3D::ReflectedClass* CClientObject::GetReflectedClass(void)
{
	return static_cast<CMafiaClientManager*>(m_pClientManager)->m_pClientObjectClass;
}

void CClientObject::Create(const GChar* model, const CVector3D& pos, const CVector3D& rot)
{
	if (m_MafiaObject != nullptr)
	{
		printf("Failed to create object. Game object already exists.");
		return;
	}

	auto pModel = (MafiaSDK::I3D_Model*)MafiaSDK::I3DGetDriver()->CreateFrame(MafiaSDK::I3D_Driver_Enum::FrameType::MODEL);

	if (pModel == nullptr)
	{
		printf("Failed to create object. Frame creation failed.\n");
		return;
	}

	UTF8String mdl(true, model);

	if (!MafiaSDK::GetModelCache()->Open(pModel, mdl.CString(), NULL, NULL, NULL, NULL))
	{
		//printf("Failed to create object. Open model cache failed. Model: %s\n", mdl.CString());
		//return;
	}

	pModel->SetName("SomeObject");
	pModel->SetScale({ 1, 1, 1 });

	//S_quat quat;
	//CVecTools::rotate(rot.z, rot.y, -M_PI, quat);
	//pVehModel->SetRot(quat);
	//pVehModel->SetWorldPos(CVecTools::ConvertToMafiaVec(pos));

	pModel->Update();

	g_pClientGame->m_bCreateActorInvokedByGame = false;
	m_MafiaObject = reinterpret_cast<MafiaSDK::C_Actor*>(MafiaSDK::GetMission()->CreateActor(MafiaSDK::C_Mission_Enum::ObjectTypes::Model));
	g_pClientGame->m_bCreateActorInvokedByGame = true;

	if (m_MafiaObject == nullptr)
	{
		printf("Failed to create object. Game actor creation failed. Model: %s\n", mdl.CString());
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

void CClientObject::UpdateGameMatrix(void)
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