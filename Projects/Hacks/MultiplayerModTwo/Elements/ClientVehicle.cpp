#include "stdafx.h"
#include "ClientManager.h"
#include "ClientGame.h"
#include "ClientVehicle.h"
#include "ClientEntity.h"
#include "Multiplayer.h"
#include <Utils/VectorTools.h>

using namespace Galactic3D;

CClientVehicleII::CClientVehicleII(CMafiaClientManagerII* pClientManager) : CClientEntityII(pClientManager)
{
	m_Type = ELEMENT_VEHICLE;
}

Galactic3D::ReflectedClass* CClientVehicleII::GetReflectedClass()
{
	return static_cast<CMafiaClientManagerII*>(m_pClientManager)->m_pClientVehicleClass;
}

M2::C_Car* CClientVehicleII::GetGameVehicle()
{
	return m_MafiaVehicle;
}

void CClientVehicleII::Process()
{
	if (!IsSyncer() && m_pBlender != nullptr && GetGameVehicle() != nullptr)
	{
		m_pBlender->Interpolate();
	}

	CClientEntityII::Process();
}

void CClientVehicleII::Create(uint32_t model, const CVector3D& pos, const CVector3D& rot)
{
	if (m_MafiaVehicle != nullptr)
		return;
}

void CClientVehicleII::Remove()
{
	CNetObject::Remove();

	Delete();
}

void CClientVehicleII::Delete()
{
	Despawn();
}

void CClientVehicleII::Despawn()
{
	
}

bool CClientVehicleII::SetModel(uint32_t model)
{
	return true;
}

uint32_t CClientVehicleII::GetModel()
{
	return CClientEntityII::GetModel();
}

bool CClientVehicleII::SetPosition(const CVector3D& vecPos)
{
	if (GetGameVehicle() == nullptr)
		return false;

	CClientEntityII::SetPosition(vecPos);
	GetGameVehicle()->SetPos(CVecToolsII::ConvertToMafiaVec(vecPos));

	// Disable interpolation
	if (m_pBlender != nullptr)
		m_pBlender->ResetInterpolation();

	return true;
}

bool CClientVehicleII::GetPosition(CVector3D& vecPos)
{
	if (GetGameVehicle() == nullptr)
		return false;

	vecPos = CVecToolsII::ConvertFromMafiaVec(GetGameVehicle()->GetPos());
	return true;
}

bool CClientVehicleII::SetRotation(const CVector3D& vecRot)
{
	if (GetGameVehicle() == nullptr)
		return false;

	CClientEntityII::SetRotation(vecRot);
	GetGameVehicle()->SetRot(CVecToolsII::ConvertVec3ToMafiaQuat(vecRot));

	// Disable interpolation
	if (m_pBlender != nullptr)
		m_pBlender->ResetInterpolation();

	return true;
}

bool CClientVehicleII::GetRotation(CVector3D& vecRot)
{
	if (GetGameVehicle() == nullptr)
		return false;

	vecRot = CVecToolsII::ConvertFromMafiaVec(GetGameVehicle()->GetRot().toEularAngles());
	return true;
}

bool CClientVehicleII::SetHeading(float heading)
{
	if (GetGameVehicle() == nullptr)
		return false;

	GetGameVehicle()->SetRot(CVecToolsII::ConvertVec3ToMafiaQuat(CVecToolsII::ComputeDirVector(heading)));

	// Disable interpolation
	if (m_pBlender != nullptr)
		m_pBlender->ResetInterpolation();

	return true;
}

float CClientVehicleII::GetHeading()
{
	if (GetGameVehicle() == nullptr)
		return false;

	return GetGameVehicle()->GetRot().z;
}

bool CClientVehicleII::SetRotationVelocity(const CVector3D& vecRotVel)
{
	if (GetGameVehicle() == nullptr)
		return false;

	return true;
}

bool CClientVehicleII::GetRotationVelocity(CVector3D& vecRotVel)
{
	if (GetGameVehicle() == nullptr)
		return false;

	return true;
}

bool CClientVehicleII::ReadCreatePacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntityII::ReadCreatePacket(pStream))
		return false;

	return true;
}

bool CClientVehicleII::ReadSyncPacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntityII::ReadSyncPacket(pStream))
		return false;

	if (GetGameVehicle() == nullptr)
		return false;

	return true;
}

bool CClientVehicleII::WriteCreatePacket(Galactic3D::Stream* pStream)
{
	if (GetGameVehicle() == nullptr)
		return false;

	GetPosition(m_Position);
	GetRotation(m_Rotation);

	if (!CClientEntityII::WriteCreatePacket(pStream))
		return false;

	return true;
}

bool CClientVehicleII::WriteSyncPacket(Galactic3D::Stream* pStream)
{
	if (GetGameVehicle() == nullptr)
		return false;

	GetPosition(m_Position);
	GetRotation(m_Rotation);

	if (!CClientEntityII::WriteSyncPacket(pStream))
		return false;

	return true;
}

bool CClientVehicleII::SetVelocity(const CVector3D& vecVel)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

bool CClientVehicleII::GetVelocity(CVector3D& vecVel)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

bool CClientVehicleII::SetSpeed(float speed)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

float CClientVehicleII::GetSpeed()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	float magnitude = 0.0;

	return magnitude;
}

bool CClientVehicleII::SetFuel(float fuel)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

float CClientVehicleII::GetFuel()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return 0.0;
}

bool CClientVehicleII::SetGear(uint32_t gear)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

uint32_t CClientVehicleII::GetGear()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return 0;
}

bool CClientVehicleII::SetSpeedLimit(float speedLimit)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

float CClientVehicleII::GetSpeedLimit()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return 0.0f;
}

bool CClientVehicleII::SetEngineHealth(float engineHealth)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

float CClientVehicleII::GetEngineHealth()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return 0.0f;
}

bool CClientVehicleII::SetHealth(float health)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

float CClientVehicleII::GetHealth()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return 100.0f;
}

bool CClientVehicleII::SetEngineRPM(float engineRPM)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

float CClientVehicleII::GetEngineRPM()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return 0.0f;
}

bool CClientVehicleII::SetWheelAngle(float wheelAngle)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

float CClientVehicleII::GetWheelAngle()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return 0.0f;
}

bool CClientVehicleII::SetOdometer(float odometer)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

float CClientVehicleII::GetOdometer()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return 0.0f;
}

bool CClientVehicleII::SetRoof(bool state)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return false;
}

bool CClientVehicleII::GetRoof()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return false;
}

bool CClientVehicleII::SetLocked(bool state)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return false;
}

bool CClientVehicleII::GetLocked()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return false;
}

bool CClientVehicleII::SetLights(bool state)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

bool CClientVehicleII::GetLights()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return false;
}

bool CClientVehicleII::SetEngine(bool state)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

bool CClientVehicleII::GetEngine()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return false;
}

bool CClientVehicleII::Explode()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

bool CClientVehicleII::IsSeatOccupied(unsigned char ucSeat)
{
	return GetHumanInSeat(ucSeat) != nullptr;
}

CClientHumanII* CClientVehicleII::GetHumanInSeat(unsigned char ucSeat)
{
	if (m_MafiaVehicle == nullptr)
		return nullptr;

	if (m_pOccupants[ucSeat] == nullptr)
		return nullptr;

	return m_pOccupants[ucSeat].GetPointer();
}

bool CClientVehicleII::AssignSeat(CClientHumanII* pHuman, unsigned char ucSeat)
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

bool CClientVehicleII::FreeSeat(unsigned char ucSeat)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	if (!IsSeatOccupied(ucSeat))
		return false;

	m_pOccupants[ucSeat] = nullptr;
	return true;
}

bool CClientVehicleII::SetSiren(bool state)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	m_Siren = state;
	GetGameVehicle()->SetSiren(state);
	return true;
}

bool CClientVehicleII::GetSiren()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return m_Siren;
}

bool CClientVehicleII::Repair()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return true;
}

void CClientVehicleII::SetFromExistingEntity(M2::C_Car* car) {
	m_MafiaVehicle = car;
}

void CClientVehicleII::CreateNetBlender()
{
	auto pBlender = new CNetBlenderVehicleII(this);
	auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
	if (pMultiplayer != nullptr)
		pBlender->m_uiDelay = pMultiplayer->m_usSyncIntervalInMS + 70;
	m_pBlender = pBlender;
}
