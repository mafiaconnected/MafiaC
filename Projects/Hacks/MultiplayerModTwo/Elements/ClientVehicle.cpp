#include "stdafx.h"
#include "ClientManager.h"
#include "ClientGame.h"
#include "ClientVehicle.h"
#include "ClientEntity.h"
#include "Multiplayer.h"
#include <Utils/VectorTools.h>

using namespace Galactic3D;

CClientVehicle::CClientVehicle(CMafiaClientManager* pClientManager) : CClientEntity(pClientManager)
{
	m_Type = ELEMENT_VEHICLE;
	m_MafiaVehicle = nullptr;
}

Galactic3D::ReflectedClass* CClientVehicle::GetReflectedClass(void)
{
	return static_cast<CMafiaClientManager*>(m_pClientManager)->m_pClientVehicleClass;
}

M2::C_Car* CClientVehicle::GetGameVehicle()
{
	return m_MafiaVehicle;
}

void CClientVehicle::Process(void)
{
	if (!IsSyncer() && m_pBlender != nullptr && GetGameVehicle() != nullptr)
	{
		m_pBlender->Interpolate();
	}

	CClientEntity::Process();
}

void CClientVehicle::Create(const GChar* model, const CVector3D& pos, const CVector3D& rot)
{
	if (m_MafiaVehicle != nullptr)
		return;


}

void CClientVehicle::Remove()
{
	CNetObject::Remove();

	Delete();
}

void CClientVehicle::Delete()
{
	Despawn();
}

void CClientVehicle::Despawn()
{
	
}

bool CClientVehicle::SetModel(uint32_t model)
{
	return true;
}

uint32_t CClientVehicle::GetModel()
{
	return CClientEntity::GetModel();
}

bool CClientVehicle::SetPosition(const CVector3D& vecPos)
{
	if (GetGameVehicle() == nullptr)
		return false;

	CClientEntity::SetPosition(vecPos);
	GetGameVehicle()->SetPos(CVecTools::ConvertToMafiaVec(vecPos));

	// Disable interpolation
	if (m_pBlender != nullptr)
		m_pBlender->ResetInterpolation();

	return true;
}

bool CClientVehicle::GetPosition(CVector3D& vecPos)
{
	if (GetGameVehicle() == nullptr)
		return false;

	vecPos = CVecTools::ConvertFromMafiaVec(GetGameVehicle()->GetPos());
	return true;
}

bool CClientVehicle::SetRotation(const CVector3D& vecRot)
{
	if (GetGameVehicle() == nullptr)
		return false;

	CClientEntity::SetRotation(vecRot);
	GetGameVehicle()->SetRot(CVecTools::ConvertVec3ToMafiaQuat(vecRot));

	// Disable interpolation
	if (m_pBlender != nullptr)
		m_pBlender->ResetInterpolation();

	return true;
}

bool CClientVehicle::GetRotation(CVector3D& vecRot)
{
	if (GetGameVehicle() == nullptr)
		return false;

	vecRot = CVecTools::ConvertFromMafiaVec(GetGameVehicle()->GetRot().toEularAngles());
	return true;
}

bool CClientVehicle::SetHeading(float heading)
{
	if (GetGameVehicle() == nullptr)
		return false;

	GetGameVehicle()->SetRot(CVecTools::ConvertVec3ToMafiaQuat(CVecTools::ComputeDirVector(heading)));

	// Disable interpolation
	if (m_pBlender != nullptr)
		m_pBlender->ResetInterpolation();

	return true;
}

float CClientVehicle::GetHeading()
{
	if (GetGameVehicle() == nullptr)
		return false;

	return GetGameVehicle()->GetRot().z;
}

bool CClientVehicle::SetRotationVelocity(const CVector3D& vecRotVel)
{
	if (GetGameVehicle() == nullptr)
		return false;

	return true;
}

bool CClientVehicle::GetRotationVelocity(CVector3D& vecRotVel)
{
	if (GetGameVehicle() == nullptr)
		return false;

	return true;
}

bool CClientVehicle::ReadCreatePacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntity::ReadCreatePacket(pStream))
		return false;

	return true;
}

bool CClientVehicle::ReadSyncPacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntity::ReadSyncPacket(pStream))
		return false;

	if (GetGameVehicle() == nullptr)
		return false;

	return true;
}

bool CClientVehicle::WriteCreatePacket(Galactic3D::Stream* pStream)
{
	if (GetGameVehicle() == nullptr)
		return false;

	GetPosition(m_Position);
	GetRotation(m_Rotation);

	if (!CClientEntity::WriteCreatePacket(pStream))
		return false;

	return true;
}

bool CClientVehicle::WriteSyncPacket(Galactic3D::Stream* pStream)
{
	if (GetGameVehicle() == nullptr)
		return false;

	GetPosition(m_Position);
	GetRotation(m_Rotation);

	if (!CClientEntity::WriteSyncPacket(pStream))
		return false;

	return true;
}

bool CClientVehicle::SetVelocity(const CVector3D& vecVel)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

bool CClientVehicle::GetVelocity(CVector3D& vecVel)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

bool CClientVehicle::SetSpeed(float speed)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

float CClientVehicle::GetSpeed()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	float magnitude = 0.0;

	return magnitude;
}

bool CClientVehicle::SetFuel(float fuel)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

float CClientVehicle::GetFuel()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return 0.0;
}

bool CClientVehicle::SetGear(uint32_t gear)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

uint32_t CClientVehicle::GetGear()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return 0;
}

bool CClientVehicle::SetSpeedLimit(float speedLimit)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

float CClientVehicle::GetSpeedLimit()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return 0.0f;
}

bool CClientVehicle::SetEngineHealth(float engineHealth)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

float CClientVehicle::GetEngineHealth()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return 0.0f;
}

bool CClientVehicle::SetHealth(float health)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

float CClientVehicle::GetHealth()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return 100.0f;
}

bool CClientVehicle::SetEngineRPM(float engineRPM)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

float CClientVehicle::GetEngineRPM()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return 0.0f;
}

bool CClientVehicle::SetWheelAngle(float wheelAngle)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

float CClientVehicle::GetWheelAngle()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return 0.0f;
}

bool CClientVehicle::SetOdometer(float odometer)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

float CClientVehicle::GetOdometer()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return 0.0f;
}

bool CClientVehicle::SetRoof(bool state)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return false;
}

bool CClientVehicle::GetRoof()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return false;
}

bool CClientVehicle::SetLocked(bool state)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return false;
}

bool CClientVehicle::GetLocked()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return false;
}

bool CClientVehicle::SetLights(bool state)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

bool CClientVehicle::GetLights()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return false;
}

bool CClientVehicle::SetEngine(bool state)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

bool CClientVehicle::GetEngine()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return false;
}

bool CClientVehicle::Explode()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

bool CClientVehicle::IsSeatOccupied(unsigned char ucSeat)
{
	return GetHumanInSeat(ucSeat) != nullptr;
}

CClientHuman* CClientVehicle::GetHumanInSeat(unsigned char ucSeat)
{
	if (m_MafiaVehicle == nullptr)
		return nullptr;

	if (m_pOccupants[ucSeat] == nullptr)
		return nullptr;

	return m_pOccupants[ucSeat].GetPointer();
}

bool CClientVehicle::AssignSeat(CClientHuman* pHuman, unsigned char ucSeat)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	if (pHuman == nullptr)
		return false;

	if (IsSeatOccupied(ucSeat))
		return false;

	m_pOccupants[ucSeat] = pHuman;
	return true;
}

bool CClientVehicle::FreeSeat(unsigned char ucSeat)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	if (!IsSeatOccupied(ucSeat))
		return false;

	m_pOccupants[ucSeat] = nullptr;
	return true;
}

bool CClientVehicle::SetSiren(bool state)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	m_Siren = state;
	GetGameVehicle()->SetSiren(state);
	return true;
}

bool CClientVehicle::GetSiren()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return m_Siren;
}

bool CClientVehicle::Repair()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

void CClientVehicle::SetFromExistingEntity(M2::C_Car* car) {
	m_MafiaVehicle = car;
}

void CClientVehicle::CreateNetBlender()
{
	auto pBlender = new CNetBlenderVehicle(this);
	auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
	if (pMultiplayer != nullptr)
		pBlender->m_uiDelay = pMultiplayer->m_usSyncIntervalInMS + 70;
	m_pBlender = pBlender;
}
