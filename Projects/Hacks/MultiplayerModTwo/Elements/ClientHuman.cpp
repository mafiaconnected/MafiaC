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

	m_vecCamera = CVector3D(0.0f, 0.0f, 0.0f);
}

Galactic3D::ReflectedClass* CClientHuman::GetReflectedClass(void)
{
	return static_cast<CMafiaClientManager*>(m_pClientManager)->m_pClientHumanClass;
}

bool CClientHuman::GetPosition(CVector3D& vecPos)
{
	//auto bResult = CClientEntity::GetPosition(vecPos);
	vecPos = CVecTools::ConvertFromMafiaVec(GetGameHuman()->GetPos());
	return true;
}

bool CClientHuman::SetPosition(const CVector3D& vecPos)
{
	auto bResult = CClientEntity::SetPosition(vecPos);
	GetGameHuman()->SetPos(CVecTools::ConvertToMafiaVec(vecPos));

	// Disable interpolation
	if (m_pBlender != nullptr)
		m_pBlender->ResetInterpolation();

	return bResult;
}

bool CClientHuman::GetRotation(CVector3D& vecRot)
{
	if (GetGameHuman() == nullptr) return false;
	vecRot = CVecTools::ConvertFromMafiaVec(GetGameHuman()->GetRot().toEularAngles());
	return true;
}

bool CClientHuman::SetHeading(float heading)
{
	if (GetGameHuman() == nullptr) 
		return false;

	GetGameHuman()->SetRot(CVecTools::ConvertVec3ToMafiaQuat(CVecTools::ComputeDirVector(heading)));

	// Disable interpolation
	if (m_pBlender != nullptr)
		m_pBlender->ResetInterpolation();

	return true;
}

float CClientHuman::GetHeading()
{
	if (GetGameHuman() == nullptr) 
		return 0.0f;

	return GetGameHuman()->GetRot().z;
}

bool CClientHuman::SetRotation(const CVector3D& vecRot)
{
	if (GetGameHuman() == nullptr) 
		return false;

	GetGameHuman()->SetRot(CVecTools::ConvertVec3ToMafiaQuat(vecRot));

	// Disable interpolation
	if (m_pBlender != nullptr)
		m_pBlender->ResetInterpolation();

	return true;
}

bool CClientHuman::SetHealth(float fHealth)
{
	if (GetGameHuman() == nullptr) 
		return false;

	GetGameHuman()->GetScript()->SetHealth(fHealth);

	return true;
}

float CClientHuman::GetHealth()
{
	if (GetGameHuman() == nullptr)
		return -1;

	return GetGameHuman()->GetScript()->m_fHealth;
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
	
	M2::C_Entity* pHuman;

	if (isLocal) 
	{
		pHuman = M2::Wrappers::CreateEntity(M2::eEntityType::MOD_ENTITY_PED, m_Model);
		pHuman->SetPosition(CVecTools::ConvertToMafiaVec(pos));
		pHuman->SetRotation(CVecTools::ConvertVec3ToMafiaQuat(CVecTools::ComputeDirVector(angle)));
		pHuman->Setup();
		pHuman->Activate();
	} 
	else 
	{
		pHuman = M2::Wrappers::CreateEntity(M2::eEntityType::MOD_ENTITY_PLAYER, m_Model);
		pHuman->SetPosition(CVecTools::ConvertToMafiaVec(pos));
		pHuman->SetRotation(CVecTools::ConvertVec3ToMafiaQuat(CVecTools::ComputeDirVector(angle)));
		pHuman->Setup();
		pHuman->Activate();
	}

	m_MafiaHuman = (M2::C_Human2*)pHuman;
	return;
}

void CClientHuman::Kill(void)
{
	GetGameHuman()->GetScript()->SetHealth(0);
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

		M2::Wrappers::DestroyEntity((M2::C_Entity*)m_MafiaHuman, (M2::eEntityType)M2::EntityTypes::Entity_Human);
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
	if (!CClientEntity::ReadCreatePacket(pStream))
		return false;

	return true;
}

bool CClientHuman::ReadSyncPacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntity::ReadSyncPacket(pStream))
		return false;

	return true;
}

bool CClientHuman::WriteCreatePacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntity::WriteCreatePacket(pStream))
		return false;

	if (GetGameHuman() == nullptr)
		return false;

	return true;
}

bool CClientHuman::WriteSyncPacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntity::WriteCreatePacket(pStream))
		return false;

	if (GetGameHuman() == nullptr)
		return false;

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
	if(!IsSyncer() && m_pBlender != nullptr && GetGameHuman() != nullptr)
	{
		m_pBlender->Interpolate();
	}

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
	M2::C_Car* pVehicle = GetGameHuman()->m_pCurrentCar;

	CClientVehicle* pClientVehicle = m_pClientManager->FindVehicle(pVehicle);
	return pClientVehicle;
}

CClientVehicle* CClientHuman::GetEnteringExitingVehicle(void)
{
	return nullptr;
}

int8_t CClientHuman::GetVehicleSeat(void)
{
	CClientVehicle* pClientVehicle = GetEnteringExitingVehicle();
	if (pClientVehicle == nullptr)
	{
		pClientVehicle = GetOccupiedVehicle();

		if (pClientVehicle == nullptr)
		{
			return -1;
		}
	}

	for (int8_t i = 0; i < 4; i++)
	{
		CClientHuman *pClientHuman = pClientVehicle->GetHumanInSeat(i);
		if(pClientHuman != nullptr && this == pClientHuman)
		{
			return i;
		}
	}

	return -1;
}

void CClientHuman::EnterVehicle(CClientVehicle* pVehicle, uint8_t iSeat)
{
	_glogverboseprintf(__gstr(__FUNCTION__));

	m_nVehicleNetworkIndex = pVehicle->GetId();
}

void CClientHuman::RemoveFromVehicle(void)
{
	_glogverboseprintf(__gstr(__FUNCTION__));

	CClientVehicle* pVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(m_nVehicleNetworkIndex, ELEMENT_VEHICLE));

	m_nVehicleNetworkIndex = INVALID_NETWORK_ID;

	if (pVehicle != nullptr)
	{
		pVehicle->FreeSeat(m_nVehicleSeatIndex);
	}
}

void CClientHuman::ExitVehicle(void)
{
	_glogverboseprintf(__gstr(__FUNCTION__));
	GetOccupiedVehicle()->FreeSeat(m_nVehicleSeatIndex);

	m_nVehicleSeatIndex = -1;
	m_nVehicleNetworkIndex = INVALID_NETWORK_ID;
}

bool CClientHuman::WarpIntoVehicle(CClientVehicle* pClientVehicle, uint8_t ucSeat)
{
	_glogverboseprintf(__gstr(__FUNCTION__));

	if (GetGameHuman()->GetInterface()->carLeavingOrEntering != nullptr)
		return false;

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

bool CClientHuman::SetModel(uint32_t model)
{
	CClientEntity::SetModel(model);

	Despawn();
	Spawn(m_Position, m_Rotation.z, (m_Type == ELEMENT_PLAYER) ? true : false);

	return true;
}

const GChar* CClientHuman::GetModel()
{
	return CClientEntity::GetModel();
}

void CClientHuman::PlayAnim(const char* animName)
{
	if (m_MafiaHuman == nullptr) return;
}

void CClientHuman::Shoot(bool state, const CVector3D& dstPos)
{
	if (m_MafiaHuman == nullptr) return;
}

void CClientHuman::Jump()
{
	if (m_MafiaHuman == nullptr) return;
}

void CClientHuman::ThrowGrenade(const CVector3D& dstPos)
{
	if (m_MafiaHuman == nullptr) return;
}

void CClientHuman::Reload()
{
	if (m_MafiaHuman == nullptr) return;
}

void CClientHuman::HolsterWeapon()
{
	if (m_MafiaHuman == nullptr) return;
}

void CClientHuman::Crouch(bool state)
{
	if (m_MafiaHuman == nullptr) return;
}

M2::C_Human2* CClientHuman::GetGameHuman()
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

void CClientHuman::SetActiveWeapon(unsigned short usWeapon)
{

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

}

void CClientHuman::DropWeapon()
{
	if (m_MafiaHuman == nullptr) return;
}

int CClientHuman::GetAnimationState()
{

}

int CClientHuman::GetAnimationStateLocal()
{
}

void CClientHuman::SetFromExistingEntity(M2::C_Human2* human) {
	m_MafiaHuman = human;
}

void CClientHuman::SetBehavior(uint32_t iBehavior)
{
	
}

void CClientHuman::CreateNetBlender()
{
	auto pBlender = new CNetBlenderHuman(this);
	auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
	if (pMultiplayer != nullptr)
		pBlender->m_uiDelay = pMultiplayer->m_usSyncIntervalInMS + 0;
	m_pBlender = pBlender;
}