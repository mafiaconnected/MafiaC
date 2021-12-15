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

CClientHumanII::CClientHumanII(CMafiaClientManagerII* pClientManager) : CClientEntityII(pClientManager)
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

Galactic3D::ReflectedClass* CClientHumanII::GetReflectedClass(void)
{
	return static_cast<CMafiaClientManagerII*>(m_pClientManager)->m_pClientHumanClass;
}

bool CClientHumanII::GetPosition(CVector3D& vecPos)
{
	//auto bResult = CClientEntityII::GetPosition(vecPos);
	vecPos = CVecToolsII::ConvertFromMafiaVec(GetGameHuman()->GetPos());
	return true;
}

bool CClientHumanII::SetPosition(const CVector3D& vecPos)
{
	auto bResult = CClientEntityII::SetPosition(vecPos);
	GetGameHuman()->SetPos(CVecToolsII::ConvertToMafiaVec(vecPos));

	// Disable interpolation
	if (m_pBlender != nullptr)
		m_pBlender->ResetInterpolation();

	return bResult;
}

bool CClientHumanII::GetRotation(CVector3D& vecRot)
{
	if (GetGameHuman() == nullptr) return false;
	vecRot = CVecToolsII::ConvertFromMafiaVec(GetGameHuman()->GetRot().toEularAngles());
	return true;
}

bool CClientHumanII::SetHeading(float heading)
{
	if (GetGameHuman() == nullptr) 
		return false;

	GetGameHuman()->SetRot(CVecToolsII::ConvertVec3ToMafiaQuat(CVecToolsII::ComputeDirVector(heading)));

	// Disable interpolation
	if (m_pBlender != nullptr)
		m_pBlender->ResetInterpolation();

	return true;
}

float CClientHumanII::GetHeading()
{
	if (GetGameHuman() == nullptr) 
		return 0.0f;

	return GetGameHuman()->GetRot().z;
}

bool CClientHumanII::SetRotation(const CVector3D& vecRot)
{
	if (GetGameHuman() == nullptr) 
		return false;

	GetGameHuman()->SetRot(CVecToolsII::ConvertVec3ToMafiaQuat(vecRot));

	// Disable interpolation
	if (m_pBlender != nullptr)
		m_pBlender->ResetInterpolation();

	return true;
}

bool CClientHumanII::SetHealth(float fHealth)
{
	if (GetGameHuman() == nullptr) 
		return false;

	GetGameHuman()->GetScript()->SetHealth(fHealth);

	return true;
}

float CClientHumanII::GetHealth()
{
	if (GetGameHuman() == nullptr)
		return -1;

	return GetGameHuman()->GetScript()->m_fHealth;
}

bool CClientHumanII::SetVelocity(const CVector3D& vecVel)
{
	if (GetGameHuman() == nullptr) 
		return false;

	relPos = vecVel;

	return true;
}

bool CClientHumanII::GetVelocity(CVector3D& vecVel)
{
	if (GetGameHuman() == nullptr) 
		return false;

	vecVel = relPos;

	return true;
}

bool CClientHumanII::SetRotationVelocity(const CVector3D& vecRotVel)
{
	if (GetGameHuman() == nullptr) 
		return false;

	relRot = vecRotVel;

	return true;
}

bool CClientHumanII::GetRotationVelocity(CVector3D& vecRotVel)
{
	if (GetGameHuman() == nullptr) 
		return false;

	vecRotVel = relRot;

	return true;
}

void CClientHumanII::Spawn(const CVector3D& pos, float angle, bool isLocal)
{
	if (GetGameHuman() != nullptr) 
		Despawn();
	
	M2::C_Entity* pHuman;

	if (isLocal) 
	{
		pHuman = M2::Wrappers::CreateEntity(M2::eEntityType::MOD_ENTITY_PED, m_Model);
		pHuman->SetPosition(CVecToolsII::ConvertToMafiaVec(pos));
		pHuman->SetRotation(CVecToolsII::ConvertVec3ToMafiaQuat(CVecToolsII::ComputeDirVector(angle)));
		pHuman->Setup();
		pHuman->Activate();
	} 
	else 
	{
		pHuman = M2::Wrappers::CreateEntity(M2::eEntityType::MOD_ENTITY_PLAYER, m_Model);
		pHuman->SetPosition(CVecToolsII::ConvertToMafiaVec(pos));
		pHuman->SetRotation(CVecToolsII::ConvertVec3ToMafiaQuat(CVecToolsII::ComputeDirVector(angle)));
		pHuman->Setup();
		pHuman->Activate();
	}

	m_MafiaHuman = (M2::C_Human2*)pHuman;
	return;
}

void CClientHumanII::Kill(void)
{
	GetGameHuman()->GetScript()->SetHealth(0);
}

bool CClientHumanII::Create(void)
{
	return false;
}

void CClientHumanII::Despawn(void)
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

void CClientHumanII::Delete(void)
{
	Despawn();
}

#define ExpandVec(vec) vec.x, vec.y, vec.z

bool CClientHumanII::ReadCreatePacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntityII::ReadCreatePacket(pStream))
		return false;

	return true;
}

bool CClientHumanII::ReadSyncPacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntityII::ReadSyncPacket(pStream))
		return false;

	return true;
}

bool CClientHumanII::WriteCreatePacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntityII::WriteCreatePacket(pStream))
		return false;

	if (GetGameHuman() == nullptr)
		return false;

	return true;
}

bool CClientHumanII::WriteSyncPacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntityII::WriteCreatePacket(pStream))
		return false;

	if (GetGameHuman() == nullptr)
		return false;

	return true;
}

void CClientHumanII::OnCreated(void)
{
	// Initial vehicle
	if (m_nVehicleNetworkIndex != INVALID_NETWORK_ID)
	{
		CClientVehicleII* pVehicle = static_cast<CClientVehicleII*>(m_pClientManager->FromId(m_nVehicleNetworkIndex, ELEMENT_VEHICLE));
		WarpIntoVehicle(pVehicle, m_nVehicleSeatIndex);
	}

	CArguments Args(1);
	Args.AddObject(this);
	g_pClientGame->m_pOnHumanSpawnEventType->Trigger(Args);
}

void CClientHumanII::Process(void)
{
	if(!IsSyncer() && m_pBlender != nullptr && GetGameHuman() != nullptr)
	{
		m_pBlender->Interpolate();
	}

	CClientEntityII::Process();
}

bool CClientHumanII::IsInVehicle(void)
{
	return GetOccupiedVehicle() != nullptr;
}

bool CClientHumanII::IsInVehicle(CClientVehicleII* pClientVehicle)
{
	return GetOccupiedVehicle() == pClientVehicle;
}

CClientVehicleII* CClientHumanII::GetOccupiedVehicle(void)
{
	M2::C_Car* pVehicle = GetGameHuman()->m_pCurrentCar;

	CClientVehicleII* pClientVehicle = m_pClientManager->FindVehicle(pVehicle);
	return pClientVehicle;
}

CClientVehicleII* CClientHumanII::GetEnteringExitingVehicle(void)
{
	return nullptr;
}

int8_t CClientHumanII::GetVehicleSeat(void)
{
	CClientVehicleII* pClientVehicle = GetEnteringExitingVehicle();
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
		CClientHumanII *pClientHuman = pClientVehicle->GetHumanInSeat(i);
		if(pClientHuman != nullptr && this == pClientHuman)
		{
			return i;
		}
	}

	return -1;
}

void CClientHumanII::EnterVehicle(CClientVehicleII* pVehicle, uint8_t iSeat)
{
	_glogverboseprintf(__gstr(__FUNCTION__));

	m_nVehicleNetworkIndex = pVehicle->GetId();
}

void CClientHumanII::RemoveFromVehicle(void)
{
	_glogverboseprintf(__gstr(__FUNCTION__));

	CClientVehicleII* pVehicle = static_cast<CClientVehicleII*>(m_pClientManager->FromId(m_nVehicleNetworkIndex, ELEMENT_VEHICLE));

	m_nVehicleNetworkIndex = INVALID_NETWORK_ID;

	if (pVehicle != nullptr)
	{
		pVehicle->FreeSeat(m_nVehicleSeatIndex);
	}
}

void CClientHumanII::ExitVehicle(void)
{
	_glogverboseprintf(__gstr(__FUNCTION__));
	GetOccupiedVehicle()->FreeSeat(m_nVehicleSeatIndex);

	m_nVehicleSeatIndex = -1;
	m_nVehicleNetworkIndex = INVALID_NETWORK_ID;
}

bool CClientHumanII::WarpIntoVehicle(CClientVehicleII* pClientVehicle, uint8_t ucSeat)
{
	_glogverboseprintf(__gstr(__FUNCTION__));

	return true;
}

bool CClientHumanII::SetModel(uint32_t model)
{
	CClientEntityII::SetModel(model);

	Despawn();
	Spawn(m_Position, m_Rotation.z, (m_Type == ELEMENT_PLAYER) ? true : false);

	return true;
}

uint32_t CClientHumanII::GetModel()
{
	return CClientEntityII::GetModel();
}

void CClientHumanII::PlayAnim(const char* animName)
{
	if (m_MafiaHuman == nullptr) return;
}

void CClientHumanII::Shoot(bool state, const CVector3D& dstPos)
{
	if (m_MafiaHuman == nullptr) return;
}

void CClientHumanII::Jump()
{
	if (m_MafiaHuman == nullptr) return;
}

void CClientHumanII::ThrowGrenade(const CVector3D& dstPos)
{
	if (m_MafiaHuman == nullptr) return;
}

void CClientHumanII::Reload()
{
	if (m_MafiaHuman == nullptr) return;
}

void CClientHumanII::HolsterWeapon()
{
	if (m_MafiaHuman == nullptr) return;
}

void CClientHumanII::Crouch(bool state)
{
	if (m_MafiaHuman == nullptr) return;
}

M2::C_Human2* CClientHumanII::GetGameHuman()
{
	return m_MafiaHuman;
}

void CClientHumanII::ClearInventory()
{
	for (int i = 0; i < 8; i++)
	{
		auto item = items[i];

		item.weapId = item.ammo1 = item.ammo2 = 0;
	}
}

void CClientHumanII::GiveWeapon(unsigned short ucWeapon, unsigned short ucAmmo1, unsigned short ucAmmo2)
{
	if (m_MafiaHuman == nullptr) return;
}

bool CClientHumanII::HasWeapon(unsigned short ucWeapon)
{
	if (m_MafiaHuman == nullptr) return false;

	for (int i = 0; i < 8; i++)
	{
		InventoryItem item = items[i];

		if (item.weapId == ucWeapon) return true;
	}

	return false;
}

void CClientHumanII::SetActiveWeapon(unsigned short usWeapon)
{
	return;
}

int CClientHumanII::GetFirstEmptyWeaponIndex()
{
	for (int i = 0; i < 8; i++)
	{
		InventoryItem item = items[i];

		if (item.weapId == 0) return i;
	}

	return -1;
}

int CClientHumanII::GetIndexOfWeapon(unsigned short ucWeapon)
{
	for (int i = 0; i < 8; i++)
	{
		InventoryItem item = items[i];

		if (item.weapId == ucWeapon) return i;
	}

	return -1;
}

void CClientHumanII::TakeWeapon(unsigned short ucWeapon)
{
	if (m_MafiaHuman == nullptr) return;

}

void CClientHumanII::DropWeapon()
{
	if (m_MafiaHuman == nullptr) return;
}

int CClientHumanII::GetAnimationState()
{
	return 0;
}

int CClientHumanII::GetAnimationStateLocal()
{
	return 0;
}

void CClientHumanII::SetFromExistingEntity(M2::C_Human2* human) {
	m_MafiaHuman = human;
}

void CClientHumanII::SetBehavior(uint32_t iBehavior)
{
	return;
}

void CClientHumanII::CreateNetBlender()
{
	auto pBlender = new CNetBlenderHumanII(this);
	auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
	if (pMultiplayer != nullptr)
		pBlender->m_uiDelay = pMultiplayer->m_usSyncIntervalInMS + 0;
	m_pBlender = pBlender;
}