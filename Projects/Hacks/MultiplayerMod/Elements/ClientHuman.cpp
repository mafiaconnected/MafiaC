#include "stdafx.h"
#include "ClientManager.h"
#include "ClientGame.h"
#include "Multiplayer.h"
#include "ClientVehicle.h"
#include "ClientHuman.h"
#include "ClientEntity.h"
#include "ClientPlayer.h"
#include <Utils/VectorTools.h>

using namespace Galactic3D;

CClientHuman::CClientHuman(CMafiaClientManager* pClientManager) : CClientEntity(pClientManager)
{
	m_Type = ELEMENT_PED;
	m_nVehicleNetworkIndex = INVALID_NETWORK_ID;
	m_nVehicleSeatIndex = 0;
	m_MafiaHuman = nullptr;

	m_bEnteredVehicleEvent = false;
	m_bExitedVehicleEvent = false;
	m_bExitingVehicleEvent = false;
	m_bEnteringVehicleEvent = false;
}

Galactic3D::ReflectedClass* CClientHuman::GetReflectedClass(void)
{
	return static_cast<CMafiaClientManager*>(m_pClientManager)->m_pClientHumanClass;
}

bool CClientHuman::GetPosition(CVector3D& vecPos)
{
	//auto bResult = CClientEntity::GetPosition(vecPos);
	vecPos = CVecTools::ConvertFromMafiaVec(GetGameHuman()->GetInterface()->entity.position);
	return true;
}

bool CClientHuman::SetPosition(const CVector3D& vecPos)
{
	auto bResult = CClientEntity::SetPosition(vecPos);
	GetGameHuman()->GetInterface()->entity.position = CVecTools::ConvertToMafiaVec(vecPos);
	return bResult;
}

bool CClientHuman::GetRotation(CVector3D& vecRot)
{
	if (GetGameHuman() == nullptr) return false;

	vecRot = CVecTools::DirToEuler(CVecTools::ConvertFromMafiaVec(GetGameHuman()->GetInterface()->entity.rotation));
	return true;
}

bool CClientHuman::SetHeading(float heading)
{
	if (GetGameHuman() == nullptr) 
		return false;

	GetGameHuman()->GetInterface()->entity.rotation = CVecTools::ConvertToMafiaVec(CVecTools::ComputeDirVector(heading));

	return true;
}

float CClientHuman::GetHeading()
{
	if (GetGameHuman() == nullptr) 
		return 0.0f;

	CVector3D rot = CVecTools::ConvertFromMafiaVec(GetGameHuman()->GetInterface()->entity.rotation);

	return CVecTools::DirToRotation180(rot);
}

bool CClientHuman::SetRotation(const CVector3D& vecRot)
{
	if (GetGameHuman() == nullptr) 
		return false;

	GetGameHuman()->GetInterface()->entity.rotation = CVecTools::ConvertToMafiaVec(CVecTools::EulerToDir(vecRot));

	return true;
}

bool CClientHuman::SetHealth(float fHealth)
{
	if (GetGameHuman() == nullptr) 
		return false;

	GetGameHuman()->GetInterface()->health = fHealth;
	MafiaSDK::GetIndicators()->PlayerSetWingmanLives((int)(fHealth / 2));

	return true;
}

float CClientHuman::GetHealth()
{
	if (GetGameHuman() == nullptr)
		return -1;

	return GetGameHuman()->GetInterface()->health;
}

bool CClientHuman::SetVelocity(const CVector3D& vecVel)
{
	if (GetGameHuman() == nullptr) 
		return false;

	relPos = vecVel;

	return true;
}

bool CClientHuman::GetVelocity(CVector3D& vecVel)
{
	if (GetGameHuman() == nullptr) 
		return false;

	vecVel = relPos;

	return true;
}

bool CClientHuman::SetRotationVelocity(const CVector3D& vecRotVel)
{
	if (GetGameHuman() == nullptr) 
		return false;

	relRot = vecRotVel;

	return true;
}

bool CClientHuman::GetRotationVelocity(CVector3D& vecRotVel)
{
	if (GetGameHuman() == nullptr) 
		return false;

	vecRotVel = relRot;

	return true;
}

void CClientHuman::Spawn(const CVector3D& pos, float angle, bool isLocal)
{
	if (GetGameHuman() != nullptr) 
		Despawn();

	auto pModel = (MafiaSDK::I3D_Model*)MafiaSDK::I3DGetDriver()->CreateFrame(MafiaSDK::I3D_Driver_Enum::FrameType::MODEL);

	UTF8String model(true, m_szModel);

	MafiaSDK::GetModelCache()->Open(pModel, model.CString(), NULL, NULL, NULL, NULL);

	//pModel->SetName(name.CString());
	pModel->SetScale({ 1, 1, 1 });
	pModel->SetWorldPos(CVecTools::ConvertToMafiaVec(pos));
	pModel->Update();

	m_MafiaHuman = reinterpret_cast<MafiaSDK::C_Human*>(MafiaSDK::GetMission()->CreateActor(isLocal ? MafiaSDK::C_Mission_Enum::ObjectTypes::Player : MafiaSDK::C_Mission_Enum::ObjectTypes::Enemy));

	m_MafiaHuman->Init(pModel);

	if (!isLocal) m_MafiaHuman->SetBehavior(MafiaSDK::C_Human_Enum::BehaviorStates::DoesntReactOnWeapon);

	m_MafiaHuman->SetShooting(1);
	m_MafiaHuman->SetActive(true);

	MafiaSDK::GetMission()->GetGame()->AddTemporaryActor(m_MafiaHuman);

	if (isLocal)
	{
		MafiaSDK::C_Game* pGame = MafiaSDK::GetMission()->GetGame();

		if (pGame)
		{
			pGame->GetCamera()->SetCar(NULL);
			pGame->GetCamera()->SetMode(true, 1);
			pGame->GetCamera()->SetPlayer(m_MafiaHuman);
			pGame->SetLocalPlayer((MafiaSDK::C_Player*)m_MafiaHuman);
		}
	}

	auto rot = CVecTools::ComputeDirVector(angle);

	m_MafiaHuman->GetInterface()->entity.position = CVecTools::ConvertToMafiaVec(pos);
	m_MafiaHuman->GetInterface()->entity.rotation = CVecTools::ConvertToMafiaVec(rot);

	SetPosition(pos);
	SetRotation(rot);

	m_pEntity = m_MafiaHuman;

	//_glogprintf(_gstr("Spawned human for element #%d:\n\tModel: %s\n\tPosition: {%f, %f, %f}\n\tAngle: %f"), GetId(), m_szModel, pos.x, pos.y, pos.z, angle);
}

void CClientHuman::Kill(void)
{
	// Note (Sevenisko): Currently no other way available - missing animations and screams (need some RE to get better result)
	GetGameHuman()->Intern_ForceDeath();
}

bool CClientHuman::Create(void)
{
	return false;
}

void CClientHuman::Despawn(void)
{
	if (GetGameHuman() != nullptr)
	{
		if (IsInVehicle())
		{
			RemoveFromVehicle();
		}

		MafiaSDK::GetMission()->GetGame()->RemoveTemporaryActor(GetGameHuman());
		m_MafiaHuman = nullptr;

		//_glogprintf(_gstr("Despawned human"));
	}
}

void CClientHuman::Delete(void)
{
	Despawn();
}

#define ExpandVec(vec) vec.x, vec.y, vec.z

bool CClientHuman::ReadCreatePacket(Galactic3D::Stream* pStream)
{
	CVector3D pos, rot;
	float health;
	bool isDucking, isAiming;
	uint8_t animState;

	if (!CNetObject::ReadCreatePacket(pStream))
		return false;

	CBinaryReader Reader(pStream);

	size_t size;

    GChar* pszModel = Reader.ReadString(&size);
    if (pszModel == nullptr)
        return false;

    _gstrcpy_s(m_szModel, ARRAY_COUNT(m_szModel), pszModel);
    GFree(pszModel);

	Reader.ReadVector3D(&pos, 1);
	Reader.ReadVector3D(&rot, 1);

	relPos = pos;
	relRot = rot;

	Reader.ReadSingle(&health, 1);
	Reader.ReadInt32(&m_nVehicleNetworkIndex, 1);
	Reader.ReadUInt32(&m_nVehicleSeatIndex, 1);
	Reader.ReadBoolean(isDucking);
	Reader.ReadBoolean(isAiming);
	Reader.ReadUInt8(&animState, 1);

	//_glogprintf(L"Got create packet for element #%d:\n\tModel: %s\n\tPosition: [%f, %f, %f - %f, %f, %f]\n\tRotation: [%f, %f, %f - %f, %f, %f]\n\tHealth: %f\n\tVehicle index: %d\n\tVehicle seat index: %d\n\tDucking: %s\n\tAiming: %s\n\tAnim state: %d", GetId(), model, vecPos.x, vecPos.y, vecPos.z, vecRelPos.x, vecRelPos.y, vecRelPos.z, vecRot.x, vecRot.y, vecRot.z, vecRelRot.x, vecRelRot.y, vecRelRot.z, health, m_nVehicleNetworkIndex, m_nVehicleSeatIndex, isDucking ? L"Yes" : L"No", isAiming ? L"Yes" : L"No", animState);

	MafiaSDK::C_Human_Interface* IHuman;

	if (GetGameHuman() == nullptr)
	{
		// Note (Sevenisko): Spawn the PED only, the Multiplayer will take care of the local player assignment
		Spawn(pos, CVecTools::DirToRotation180(CVecTools::EulerToDir(rot)), GetSyncer() == g_pClientGame->GetActiveMultiplayer()->m_iLocalIndex);

		IHuman = GetGameHuman()->GetInterface();
	}
	else
	{
		IHuman = GetGameHuman()->GetInterface();
		IHuman->entity.position = CVecTools::ConvertToMafiaVec(pos);
		IHuman->entity.rotation = CVecTools::ConvertToMafiaVec(rot);
	}

	IHuman->animState = animState;
	IHuman->health = health;
	IHuman->isDucking = isDucking;
	IHuman->isAiming = isAiming;

	return true;
}

bool CClientHuman::ReadSyncPacket(Galactic3D::Stream* pStream)
{
	if (!CNetObject::ReadSyncPacket(pStream))
		return false;

	if (GetGameHuman() == nullptr)
		return false;

	auto IHuman = GetGameHuman()->GetInterface();

	Galactic3D::CBinaryReader Reader(pStream);

	Reader.ReadSingle(&IHuman->health, 1);
	Reader.ReadInt32(&m_nVehicleNetworkIndex, 1);
	Reader.ReadBoolean(IHuman->isDucking);
	Reader.ReadBoolean(IHuman->isAiming);
	Reader.ReadUInt8(&IHuman->animState, 1);

	if (m_nVehicleNetworkIndex != INVALID_NETWORK_ID) {
		CClientVehicle* pVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(m_nVehicleNetworkIndex, ELEMENT_VEHICLE));
		if (pVehicle != nullptr && pVehicle->GetGameVehicle() != nullptr) {
			WarpIntoVehicle(pVehicle, 0);
		}
	}
	else
	{
		if (IsInVehicle()) {
			RemoveFromVehicle();
		}
	}

	SetPosition(m_Position);
	SetRotation(m_Rotation);

	//_glogprintf(L"Got sync packet for element #%d:\n\tPosition: [%f, %f, %f]\n\tPos. difference: [%f, %f, %f]\n\tRotation: [%f, %f, %f (%f, %f, %f)]\n\tRot. difference: [%f, %f, %f]\n\tHealth: %f\n\tVehicle index: %d\n\tVehicle seat index: %d\n\tDucking: %s\n\tAiming: %s\n\tAnim state: %d", GetId(), vPos.x, vPos.y, vPos.z, vRelPos.x, vRelPos.y, vRelPos.z, vRot.x, vRot.y, vRot.z, IHuman->entity.rotation.x, IHuman->entity.rotation.y, IHuman->entity.rotation.z, vRelRot.x, vRelRot.y, vRelRot.z, IHuman->health, m_nVehicleNetworkIndex, m_nVehicleSeatIndex, IHuman->isDucking ? L"Yes" : L"No", IHuman->isAiming ? L"Yes" : L"No", IHuman->animState);
	
	//if (m_pClientManager->m_pLocalPlayer.GetPointer() != this || !IsSyncer()) {
	//	auto pBlender = static_cast<CNetBlenderLerp*>(m_pBlender);
	//
	//	if (!IsInVehicle())
	//	{
	//		pBlender->SetTargetPosition(vPos);
	//		pBlender->SetTargetRotation(vRot);
	//		pBlender->SetTargetSpeed(relPos, relRot);
	//	}
	//	else
	//	{
	//		pBlender->ResetInterpolation();
	//	}
	//}

	return true;
}

bool CClientHuman::WriteCreatePacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntity::WriteCreatePacket(pStream))
		return false;

	if (GetGameHuman() == nullptr)
		return false;

	auto IHuman = GetGameHuman()->GetInterface();

	int vehicleId = INVALID_NETWORK_ID;

	if (IsInVehicle())
		vehicleId = GetOccupiedVehicle()->GetId();

	CBinaryWriter Writer(pStream);

	Writer.WriteSingle(&IHuman->health, 1);
	Writer.WriteInt32(&vehicleId, 1);
	Writer.WriteUInt32(&m_nVehicleSeatIndex, 1);
	Writer.WriteBoolean(IHuman->isDucking);
	Writer.WriteBoolean(IHuman->isAiming);
	Writer.WriteUInt8(&IHuman->animStateLocal, 1);

	//_glogprintf(L"Sent create packet for element #%d:\n\tModel: %s\n\tPosition: [%f, %f, %f]\n\tRotation: [%f, %f, %f]\n\tHealth: %f\n\tVehicle index: %d\n\tVehicle seat index: %d\n\tDucking: %s\n\tAiming: %s\n\tAnim state: %d", GetId(), m_szModel, pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, IHuman->health, m_nVehicleNetworkIndex, m_nVehicleSeatIndex, IHuman->isDucking ? L"Yes" : L"No", IHuman->isAiming ? L"Yes" : L"No", IHuman->animState);

	return true;
}

bool CClientHuman::WriteSyncPacket(Galactic3D::Stream* pStream)
{
	if (GetGameHuman() == nullptr)
		return false;

	auto IHuman = GetGameHuman()->GetInterface();

	if (!CNetObject::WriteSyncPacket(pStream))
		return false;

	CBinaryWriter Writer(pStream);

	CVector3D pos; GetPosition(pos);
	CVector3D rot; GetRotation(rot);

	CVector3D diffPos = pos - prevPos;
	CVector3D diffRot = rot - prevRot;

	Writer.WriteVector3D(&diffPos, 1);
	Writer.WriteVector3D(&diffRot, 1);

	Writer.WriteSingle(&IHuman->health, 1);

	int vehicleId = INVALID_NETWORK_ID;

	if (IsInVehicle())
		vehicleId = GetOccupiedVehicle()->GetId();

	Writer.WriteInt32(&vehicleId, 1);

	Writer.WriteBoolean(IHuman->isDucking);
	Writer.WriteBoolean(IHuman->isAiming);
	Writer.WriteUInt8(&IHuman->animState, 1);

	//_glogprintf(L"Sent sync packet for element #%d:\n\tPosition: [%f, %f, %f]\n\tPos. difference: [%f, %f, %f]\n\tRotation: [%f, %f, %f]\n\tRot. difference: [%f, %f, %f]\n\tHealth: %f\n\tVehicle index: %d\n\tVehicle seat index: %d\n\tDucking: %s\n\tAiming: %s\n\tAnim state: %d", GetId(), pos.x, pos.y, pos.z, diffPos.x, diffPos.y, diffPos.z, rot.x, rot.y, rot.z, diffRot.x, diffRot.y, diffRot.z, IHuman->health, m_nVehicleNetworkIndex, m_nVehicleSeatIndex, IHuman->isDucking ? L"Yes" : L"No", IHuman->isAiming ? L"Yes" : L"No", IHuman->animState);

	return true;
}

void CClientHuman::OnCreated(void)
{
	// Initial vehicle
	if (m_nVehicleNetworkIndex != INVALID_NETWORK_ID)
	{
		CClientVehicle* pVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(m_nVehicleNetworkIndex, ELEMENT_VEHICLE));
		WarpIntoVehicle(pVehicle, m_nVehicleSeatIndex);
	}

	CArguments Args(1);
	Args.AddObject(this);
	g_pClientGame->m_pOnHumanSpawnEventType->Trigger(Args);
}

void CClientHuman::Process(void)
{
	//if (!IsSyncer() && m_pBlender != nullptr && GetGameHuman() != nullptr)
	//{
	//	auto pBlender = static_cast<CNetBlenderLerp*>(m_pBlender);
	//	pBlender->Interpolate();
	//}

	//auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
	//if (pMultiplayer != nullptr)
	//{
	//	if (this->m_Type != ELEMENT_PLAYER) {
	//		_glogprintf(_gstr("Human %i - CanSendSync: %i, IsLocal: %i, IsSyncer: %i"),
	//			GetId(),
	//			CanSendSync(g_pClientGame->GetMultiplayer()->m_iLocalIndex),
	//			IsLocal(),
	//			IsSyncer());
	//	}
	//}

	CClientEntity::Process();
}

bool CClientHuman::IsInVehicle(void)
{
	return GetOccupiedVehicle() != nullptr;
}

bool CClientHuman::IsInVehicle(CClientVehicle* pClientVehicle)
{
	return GetOccupiedVehicle() == pClientVehicle;
}

CClientVehicle* CClientHuman::GetOccupiedVehicle(void)
{
	MafiaSDK::C_Car* pVehicle = GetGameHuman()->GetInterface()->playersCar;
	if (pVehicle == nullptr)
		return nullptr;

	CClientVehicle* pClientVehicle = m_pClientManager->FindVehicle(pVehicle);
	return pClientVehicle;
}

CClientVehicle* CClientHuman::GetEnteringExitingVehicle(void)
{
	MafiaSDK::C_Car* pVehicle = GetGameHuman()->GetInterface()->carLeavingOrEntering;
	if (pVehicle == nullptr)
		return nullptr;

	CClientVehicle* pClientVehicle = m_pClientManager->FindVehicle(pVehicle);
	return pClientVehicle;
}

void CClientHuman::EnterVehicle(CClientVehicle* pVehicle, uint8_t iSeat)
{
	_glogverboseprintf(__gstr(__FUNCTION__));

	GetGameHuman()->Use_Actor((MafiaSDK::C_Actor*)pVehicle->GetGameVehicle(), iSeat, 0, 0);

	m_nVehicleNetworkIndex = pVehicle->GetId();
}

void CClientHuman::RemoveFromVehicle(void)
{
	_glogverboseprintf(__gstr(__FUNCTION__));

	CClientVehicle* pVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(m_nVehicleNetworkIndex, ELEMENT_VEHICLE));

	m_nVehicleNetworkIndex = INVALID_NETWORK_ID;

	pVehicle->FreeSeat(m_nVehicleSeatIndex);

	GetGameHuman()->Intern_FromCar();
}

void CClientHuman::ExitVehicle(void)
{
	_glogverboseprintf(__gstr(__FUNCTION__));
	GetOccupiedVehicle()->FreeSeat(m_nVehicleSeatIndex);
	GetGameHuman()->Use_Actor(GetOccupiedVehicle()->GetGameVehicle(), m_nVehicleSeatIndex, 2, 0);

	m_nVehicleSeatIndex = -1;
	m_nVehicleNetworkIndex = INVALID_NETWORK_ID;
}

bool CClientHuman::WarpIntoVehicle(CClientVehicle* pClientVehicle, uint8_t ucSeat)
{
	_glogverboseprintf(__gstr(__FUNCTION__));

	if (GetGameHuman() == nullptr || pClientVehicle->GetGameVehicle() == nullptr) return false;

	if (!pClientVehicle->IsSeatOccupied(ucSeat))
	{
		if (IsInVehicle())
		{
			RemoveFromVehicle();
		}

		pClientVehicle->AssignSeat(this, ucSeat);

		GetGameHuman()->Intern_UseCar(pClientVehicle->GetGameVehicle(), ucSeat);
	}
	return true;
}

bool CClientHuman::SetModel(const GChar* modelName)
{
	CClientEntity::SetModel(modelName);

	if (GetGameHuman() != nullptr)
	{
		UTF8String model(true, modelName);

		GetGameHuman()->Intern_ChangeModel(model);
	}

	return true;
}

const GChar* CClientHuman::GetModel()
{
	return CClientEntity::GetModel();
}

void CClientHuman::PlayAnim(const char* animName)
{
	if (m_MafiaHuman == nullptr) return;

	m_MafiaHuman->Do_PlayAnim(animName);
}

void CClientHuman::Shoot(bool state, const CVector3D& dstPos)
{
	if (m_MafiaHuman == nullptr) return;

	m_MafiaHuman->Do_Shoot(state, CVecTools::ConvertToMafiaVec(dstPos));
}

void CClientHuman::Jump()
{
	if (m_MafiaHuman == nullptr) return;

	m_MafiaHuman->Do_Jump();
}

void CClientHuman::ThrowGrenade(const CVector3D& dstPos)
{
	if (m_MafiaHuman == nullptr) return;

	m_MafiaHuman->Do_ThrowGranade(CVecTools::ConvertToMafiaVec(dstPos));
}

void CClientHuman::Reload()
{
	if (m_MafiaHuman == nullptr) return;

	m_MafiaHuman->Do_Reload();
}

void CClientHuman::HolsterWeapon()
{
	if (m_MafiaHuman == nullptr) return;

	m_MafiaHuman->Do_Holster();
}

void CClientHuman::Crouch(bool state)
{
	if (m_MafiaHuman == nullptr) return;

	m_MafiaHuman->Do_Crouched(state);
}

MafiaSDK::C_Human* CClientHuman::GetGameHuman()
{
	return m_MafiaHuman;
}

void CClientHuman::ClearInventory()
{
	for (int i = 0; i < 8; i++)
	{
		auto item = items[i];

		if (item.weapId != 0) GetGameHuman()->G_Inventory_RemoveWeapon(item.weapId);

		item.weapId = item.ammo1 = item.ammo2 = 0;
	}
}

void CClientHuman::GiveWeapon(unsigned short ucWeapon, unsigned short ucAmmo1, unsigned short ucAmmo2)
{
	if (m_MafiaHuman == nullptr) return;

	if (!HasWeapon(ucWeapon))
	{
		auto index = GetFirstEmptyWeaponIndex();

		if (index >= 8)
		{
			if(this == m_pClientManager->m_pLocalPlayer.GetPointer()) MafiaSDK::GetIndicators()->ConsoleAddText("The inventory is full!", 0xFFFFFFFF);
		}
		else if (index >= 0)
		{
			InventoryItem item = items[index];

			item.weapId = ucWeapon;
			item.ammo1 = ucAmmo1;
			item.ammo2 = ucAmmo2;

			m_MafiaHuman->G_Inventory_AddItem({ ucWeapon, ucAmmo1, ucAmmo2 });
		}
	}
}

bool CClientHuman::HasWeapon(unsigned short ucWeapon)
{
	if (m_MafiaHuman == nullptr) return false;

	for (int i = 0; i < 8; i++)
	{
		InventoryItem item = items[i];

		if (item.weapId == ucWeapon) return true;
	}

	return false;
}

int CClientHuman::GetFirstEmptyWeaponIndex()
{
	for (int i = 0; i < 8; i++)
	{
		InventoryItem item = items[i];

		if (item.weapId == 0) return i;
	}

	return -1;
}

int CClientHuman::GetIndexOfWeapon(unsigned short ucWeapon)
{
	for (int i = 0; i < 8; i++)
	{
		InventoryItem item = items[i];

		if (item.weapId == ucWeapon) return i;
	}

	return -1;
}

void CClientHuman::TakeWeapon(unsigned short ucWeapon)
{
	if (m_MafiaHuman == nullptr) return;

	if (HasWeapon(ucWeapon))
	{
		auto index = GetIndexOfWeapon(ucWeapon);

		InventoryItem item = items[index];

		item.weapId = 0;
		item.ammo1 = 0;
		item.ammo2 = 0;

		m_MafiaHuman->G_Inventory_RemoveWeapon(ucWeapon);
	}
}

void CClientHuman::DropWeapon()
{
	if (m_MafiaHuman == nullptr) return;

	m_MafiaHuman->Do_WeaponDrop();
}

int CClientHuman::GetAnimationState()
{
	return GetGameHuman()->GetInterface()->animState;
}

int CClientHuman::GetAnimationStateLocal()
{
	return GetGameHuman()->GetInterface()->animStateLocal;
}

void CClientHuman::SetFromExistingEntity(MafiaSDK::C_Human* human) {
	m_MafiaHuman = human;
}

void CClientHuman::SetBehavior(uint32_t iBehavior)
{
	
}