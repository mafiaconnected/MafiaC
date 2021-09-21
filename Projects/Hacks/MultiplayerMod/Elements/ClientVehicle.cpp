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

MafiaSDK::C_Car* CClientVehicle::GetGameVehicle()
{
	return m_MafiaVehicle;
}

void CClientVehicle::Process(void)
{
	//if (!IsSyncer() && m_pBlender != nullptr && GetGameVehicle() != nullptr)
	//{
	//	m_pBlender->Interpolate();
	//}

	//GetGameVehicle()->AI(g_pClientGame->m_pTime->m_fDeltaTime);
	//GetGameVehicle()->Update(g_pClientGame->m_pTime->m_fDeltaTime);

	//auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
	//if (pMultiplayer != nullptr)
	//{
	//	_glogprintf(_gstr("Vehicle %i - IsSyncer: %i, IsLocal: %i, IsDirty: %i, IsCreatedFor: %i, m_bSendSync: %i, m_bRegistered: %i, m_RegisteredIndex"),
	//		GetId(),
	//		IsSyncer(),
	//		IsLocal(),
	//		IsDirty(g_pClientGame->GetActiveMultiplayer()->m_NetMachines.GetMachine(g_pClientGame->GetActiveMultiplayer()->m_iLocalIndex)),
	//		IsCreatedFor(g_pClientGame->GetActiveMultiplayer()->m_NetMachines.GetMachine(g_pClientGame->GetActiveMultiplayer()->m_iLocalIndex)),
	//		CanSendSync(-1),
	//		m_bSendSync,
	//		m_bRegistered,
	//		m_RegisteredIndex);
	//}

	CClientEntity::Process();
}

void CClientVehicle::Create(const GChar* model, const CVector3D& pos, float angle)
{
	if (m_MafiaVehicle != nullptr)
		return;

	auto pVehModel = (MafiaSDK::I3D_Model*)MafiaSDK::I3DGetDriver()->CreateFrame(MafiaSDK::I3D_Driver_Enum::FrameType::MODEL);

	UTF8String mdl(true, model);

	MafiaSDK::GetModelCache()->Open(pVehModel, mdl.CString(), NULL, NULL, NULL, NULL);

	pVehModel->SetName("SomeOrdinaryVehicles");
	pVehModel->SetScale({ 1, 1, 1 });
	pVehModel->SetWorldPos(CVecTools::ConvertToMafiaVec(pos));
	pVehModel->Update();

	m_MafiaVehicle = reinterpret_cast<MafiaSDK::C_Car*>(MafiaSDK::GetMission()->CreateActor(MafiaSDK::C_Mission_Enum::ObjectTypes::Car));
	m_MafiaVehicle->Init(pVehModel);
	m_MafiaVehicle->SetActive(true);

	MafiaSDK::GetMission()->GetGame()->AddTemporaryActor(m_MafiaVehicle);

	m_MafiaVehicle->GetInterface()->vehicle_interface.position = CVecTools::ConvertToMafiaVec(pos);

	auto rot = CVecTools::ComputeDirEuler(angle);

	CMatrix3x3 mat(rot);

	m_MafiaVehicle->GetInterface()->vehicle_interface.rot_forward = CVecTools::ConvertToMafiaVec(CVecTools::EulerToDir(mat[0]));
	m_MafiaVehicle->GetInterface()->vehicle_interface.rot_up = CVecTools::ConvertToMafiaVec(CVecTools::EulerToDir(mat[1]));
	m_MafiaVehicle->GetInterface()->vehicle_interface.rot_right = CVecTools::ConvertToMafiaVec(CVecTools::EulerToDir(mat[2]));

	auto IVehicle = m_MafiaVehicle->GetInterface()->vehicle_interface;
	_glogprintf(_gstr("Created new vehicle for element #%d:\n\tModel: %s\n\tPosition: {%f, %f, %f}\n\tAngle: %f\n\tHealth: %f\n\tEngine health: %f\n\tFuel: %f\n\tSound: %s\n\tEngine on: %s\n\tHorn: %s\n\tSiren: %s\n\tGear: %d\n\tEngine RPM: %f\n\tAcceleration: %f\n\tBrake: %f\n\tHandbrake: %f\n\tSpeed limit: %f\n\tClutch: %f\n\tWheel angle: %f"), GetId(), model, pos.x, pos.y, pos.z, angle, IVehicle.health, IVehicle.engine_health, IVehicle.fuel, IVehicle.sound_enabled ? L"Yes" : L"No", IVehicle.engine_on ? L"Yes" : L"No", IVehicle.horn ? L"Yes" : L"No", IVehicle.siren ? L"Yes" : L"No", IVehicle.gear, IVehicle.engine_rpm, IVehicle.accelerating, IVehicle.break_val, IVehicle.hand_break, IVehicle.speed_limit, IVehicle.clutch, IVehicle.wheel_angle);
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
	MafiaSDK::GetMission()->DelActor(GetGameVehicle());
}

bool CClientVehicle::SetPosition(const CVector3D& vecPos)
{
	if (GetGameVehicle() == nullptr)
		return false;

	m_MafiaVehicle->GetInterface()->vehicle_interface.position = CVecTools::ConvertToMafiaVec(vecPos);
	CClientEntity::SetPosition(vecPos);
	return true;
}

bool CClientVehicle::GetPosition(CVector3D& vecPos)
{
	if (GetGameVehicle() == nullptr)
		return false;

	vecPos = CVecTools::ConvertFromMafiaVec(m_MafiaVehicle->GetInterface()->vehicle_interface.position);
	return true;
}

bool CClientVehicle::SetRotation(const CVector3D& vecRot)
{
	if (GetGameVehicle() == nullptr)
		return false;

	CMatrix3x3 mat(vecRot);

	m_MafiaVehicle->GetInterface()->vehicle_interface.rot_forward = CVecTools::ConvertToMafiaVec(CVecTools::EulerToDir(mat[0]));
	m_MafiaVehicle->GetInterface()->vehicle_interface.rot_up = CVecTools::ConvertToMafiaVec(CVecTools::EulerToDir(mat[1]));
	m_MafiaVehicle->GetInterface()->vehicle_interface.rot_right = CVecTools::ConvertToMafiaVec(CVecTools::EulerToDir(mat[2]));

	CClientEntity::SetRotation(vecRot);
	return true;
}

bool CClientVehicle::GetRotation(CVector3D& vecRot)
{
	if (GetGameVehicle() == nullptr)
		return false;

	auto front = CVecTools::DirToEuler(CVecTools::ConvertFromMafiaVec(m_MafiaVehicle->GetInterface()->vehicle_interface.rot_forward));
	auto up = CVecTools::DirToEuler(CVecTools::ConvertFromMafiaVec(m_MafiaVehicle->GetInterface()->vehicle_interface.rot_up));
	auto right = CVecTools::DirToEuler(CVecTools::ConvertFromMafiaVec(m_MafiaVehicle->GetInterface()->vehicle_interface.rot_right));

	CMatrix3x3 mat(front, up, right);

	vecRot = mat.GetEuler();

	return true;
}

bool CClientVehicle::SetRotationVelocity(const CVector3D& vecRotVel)
{
	if (GetGameVehicle() == nullptr)
		return false;

	m_MafiaVehicle->GetInterface()->vehicle_interface.rot_speed = CVecTools::ConvertToMafiaVec(vecRotVel);

	return true;
}

bool CClientVehicle::GetRotationVelocity(CVector3D& vecRotVel)
{
	if (GetGameVehicle() == nullptr)
		return false;

	vecRotVel = CVecTools::ConvertFromMafiaVec(m_MafiaVehicle->GetInterface()->vehicle_interface.rot_speed);

	return true;
}

bool CClientVehicle::ReadCreatePacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntity::ReadCreatePacket(pStream))
		return false;

	CBinaryReader Reader(pStream);

	S_vector rotForward, rotUp, rotRight;

	relPos = m_Position;
	relRot = m_Rotation;

	CMatrix3x3 rotMat(m_Rotation);
	rotForward = CVecTools::ConvertToMafiaVec(CVecTools::EulerToDir(rotMat[0]));
	rotUp = CVecTools::ConvertToMafiaVec(CVecTools::EulerToDir(rotMat[1]));
	rotRight = CVecTools::ConvertToMafiaVec(CVecTools::EulerToDir(rotMat[2]));

	tVehicleCreatePacket Packet;

	if (pStream->Read(&Packet, sizeof(Packet)) != sizeof(Packet))
		return false;

	MafiaSDK::C_Vehicle_Interface IVehicle;
	if (GetGameVehicle() == nullptr)
	{
		Create(m_szModel, m_Position, CVecTools::DirToRotation180(m_Rotation));
		IVehicle = GetGameVehicle()->GetInterface()->vehicle_interface;
	}
	else
	{
		IVehicle = GetGameVehicle()->GetInterface()->vehicle_interface;
		IVehicle.position = CVecTools::ConvertToMafiaVec(m_Position);
		IVehicle.rot_forward = rotForward;
		IVehicle.rot_right = rotRight;
		IVehicle.rot_up = rotUp;
	}

	IVehicle.health = Packet.health;
	IVehicle.engine_health = Packet.engineHealth;
	IVehicle.fuel = Packet.fuel;

	IVehicle.sound_enabled = Packet.sound;
	IVehicle.engine_on = Packet.engineOn;
	IVehicle.horn = Packet.horn;
	IVehicle.siren = Packet.siren;
	IVehicle.lights = Packet.lights;

	IVehicle.gear = Packet.gear;
	IVehicle.engine_rpm = Packet.rpm;
	IVehicle.accelerating = Packet.accel;
	IVehicle.break_val = Packet.brake;
	IVehicle.hand_break = Packet.handBrake;
	IVehicle.speed_limit = Packet.speedLimit;
	IVehicle.clutch = Packet.clutch;
	IVehicle.wheel_angle = Packet.wheelAngle;

	IVehicle.speed = CVecTools::ConvertToMafiaVec(Packet.speed);
	IVehicle.rot_speed = CVecTools::ConvertToMafiaVec(Packet.rotSpeed);

	//if (gear != IVehicle.gear) {
	//	GetGameVehicle()->SetGear(gear);
	//}

	if (Packet.engineOn != IVehicle.engine_on) {
		GetGameVehicle()->SetEngineOn(Packet.engineOn, 2);
	}
	return true;
}

bool CClientVehicle::ReadSyncPacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntity::ReadSyncPacket(pStream))
		return false;

	if (GetGameVehicle() == nullptr)
		return false;

	auto IVehicle = GetGameVehicle()->GetInterface()->vehicle_interface;

	CBinaryReader Reader(pStream);

	int32_t iGear;
	bool bEngine;
	CVector3D speed, rotSpeed;

	Reader.ReadSingle(&IVehicle.health, 1);
	Reader.ReadSingle(&IVehicle.engine_health, 1);
	Reader.ReadSingle(&IVehicle.fuel, 1);

	Reader.ReadBoolean(IVehicle.sound_enabled);
	Reader.ReadBoolean(bEngine);
	Reader.ReadBoolean(IVehicle.horn);
	Reader.ReadBoolean(IVehicle.siren);
	Reader.ReadBoolean(IVehicle.lights);

	Reader.ReadInt32(&iGear, 1);
	Reader.ReadSingle(&IVehicle.engine_rpm, 1);
	Reader.ReadSingle(&IVehicle.accelerating, 1);
	Reader.ReadSingle(&IVehicle.break_val, 1);
	Reader.ReadSingle(&IVehicle.hand_break, 1);
	Reader.ReadSingle(&IVehicle.speed_limit, 1);
	Reader.ReadSingle(&IVehicle.clutch, 1);
	Reader.ReadSingle(&IVehicle.wheel_angle, 1);

	Reader.ReadVector3D(&speed, 1);
	Reader.ReadVector3D(&rotSpeed, 1);

	//relPos = m_RelativePosition;
	//relRot = m_RelativeRotation;

	IVehicle.speed = CVecTools::ConvertToMafiaVec(speed);
	IVehicle.rot_speed = CVecTools::ConvertToMafiaVec(rotSpeed);

	if (iGear != IVehicle.gear) {
		GetGameVehicle()->GearSnd();
		GetGameVehicle()->SetGear(iGear);
	}

	if (bEngine != IVehicle.engine_on) {
		GetGameVehicle()->SetEngineOn(bEngine, 2);
	}

	SetPosition(m_Position);
	SetRotation(m_Rotation);

	//_glogprintf(_gstr("Got sync packet for vehicle #%d:\n\tPosition: [%f, %f, %f]\n\tPos. difference: [%f, %f, %f]\n\tRotation: [%f, %f, %f]\n\tRot. difference: [%f, %f, %f]"), GetId(), vPos.x, vPos.y, vPos.z, vRelPos.x, vRelPos.y, vRelPos.z, vRot.x, vRot.y, vRot.z, vRelRot.x, vRelRot.y, vRelRot.z);

	return true;
}

bool CClientVehicle::WriteCreatePacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntity::WriteCreatePacket(pStream))
		return false;

	if (GetGameVehicle() == nullptr)
		return false;

	auto IVehicle = GetGameVehicle()->GetInterface()->vehicle_interface;

	CBinaryWriter Writer(pStream);

	//CMatrix3x3 rotMat(CVecTools::ConvertFromMafiaVec(IVehicle.rot_forward), CVecTools::ConvertFromMafiaVec(IVehicle.rot_up), CVecTools::ConvertFromMafiaVec(IVehicle.rot_right));
	//CVector3D rot = rotMat.GetEuler();
	//Writer.WriteVector3D(&rot, 1);

	tVehicleCreatePacket Packet;

	Packet.health = IVehicle.health;
	Packet.engineHealth = IVehicle.engine_health;
	Packet.fuel = IVehicle.fuel;

	Packet.sound = IVehicle.sound_enabled;
	Packet.engineOn = IVehicle.engine_on;
	Packet.horn = IVehicle.horn;
	Packet.siren = IVehicle.siren;
	Packet.lights = IVehicle.lights;

	Packet.gear = IVehicle.gear;
	Packet.rpm = IVehicle.engine_rpm;
	Packet.accel = IVehicle.accelerating;
	Packet.brake = IVehicle.break_val;
	Packet.handBrake = IVehicle.hand_break;
	Packet.speedLimit = IVehicle.speed_limit;
	Packet.clutch = IVehicle.clutch;
	Packet.wheelAngle = IVehicle.wheel_angle;

	if (pStream->Write(&Packet, sizeof(Packet)) != sizeof(Packet))
		return false;

	return true;
}

bool CClientVehicle::WriteSyncPacket(Galactic3D::Stream* pStream)
{
	_glogprintf(_gstr("Writing sync packet for vehicle"));

	if (!CClientEntity::WriteSyncPacket(pStream))
		return false;

	if (GetGameVehicle() == nullptr)
		return false;

	auto IVehicle = GetGameVehicle()->GetInterface()->vehicle_interface;

	CBinaryWriter Writer(pStream);

	Writer.WriteSingle(&IVehicle.health, 1);
	Writer.WriteSingle(&IVehicle.engine_health, 1);
	Writer.WriteSingle(&IVehicle.fuel, 1);
	Writer.WriteBoolean(IVehicle.engine_on);

	Writer.WriteBoolean(IVehicle.sound_enabled);
	Writer.WriteBoolean(IVehicle.horn);
	Writer.WriteBoolean(IVehicle.siren);
	Writer.WriteBoolean(IVehicle.lights);

	Writer.WriteInt32((Sint32*)&IVehicle.gear, 1);
	Writer.WriteSingle(&IVehicle.engine_rpm, 1);
	Writer.WriteSingle(&IVehicle.accelerating, 1);
	Writer.WriteSingle(&IVehicle.break_val, 1);
	Writer.WriteSingle(&IVehicle.hand_break, 1);
	Writer.WriteSingle(&IVehicle.speed_limit, 1);
	Writer.WriteSingle(&IVehicle.clutch, 1);
	Writer.WriteSingle(&IVehicle.wheel_angle, 1);

	return true;
}

bool CClientVehicle::SetVelocity(const CVector3D& vecVel)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	m_MafiaVehicle->GetInterface()->vehicle_interface.speed = CVecTools::ConvertToMafiaVec(vecVel);

	return true;
}

bool CClientVehicle::GetVelocity(CVector3D& vecVel)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	vecVel = CVecTools::ConvertFromMafiaVec(m_MafiaVehicle->GetInterface()->vehicle_interface.speed);

	return true;
}

bool CClientVehicle::SetSpeed(float speed)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	CVector3D forward = CVecTools::ComputeDirVector(CVecTools::DirToRotation360(CVecTools::ConvertFromMafiaVec(m_MafiaVehicle->GetInterface()->entity.rotation)));

	CVector3D velocity = { forward.x * speed, forward.y * speed, forward.z * speed };

	SetVelocity(velocity);

	return true;
}

float CClientVehicle::GetSpeed()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	CVector3D velocity = { 0, 0, 0 };
	GetVelocity(velocity);

	CVector3D forward = CVecTools::ComputeDirVector(CVecTools::DirToRotation360(CVecTools::ConvertFromMafiaVec(m_MafiaVehicle->GetInterface()->entity.rotation)));

	float magnitude = sqrtf((velocity.x * forward.x) + (velocity.y * forward.y) + (velocity.z * forward.z));

	return magnitude;
}

bool CClientVehicle::SetFuel(float fuel)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	m_MafiaVehicle->GetInterface()->vehicle_interface.fuel = fuel;

	return true;
}

float CClientVehicle::GetFuel()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return m_MafiaVehicle->GetInterface()->vehicle_interface.fuel;
}

bool CClientVehicle::SetGear(float gear)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	m_MafiaVehicle->GetInterface()->vehicle_interface.gear = gear;

	return true;
}

float CClientVehicle::GetGear()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return m_MafiaVehicle->GetInterface()->vehicle_interface.gear;
}

bool CClientVehicle::SetSpeedLimit(float speedLimit)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	m_MafiaVehicle->GetInterface()->vehicle_interface.speed_limit = speedLimit;

	return true;
}

float CClientVehicle::GetSpeedLimit()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return m_MafiaVehicle->GetInterface()->vehicle_interface.speed_limit;
}

bool CClientVehicle::SetEngineHealth(float engineHealth)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	m_MafiaVehicle->GetInterface()->vehicle_interface.engine_health = engineHealth;

	return true;
}

float CClientVehicle::GetEngineHealth()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return m_MafiaVehicle->GetInterface()->vehicle_interface.engine_health;
}

bool CClientVehicle::SetHealth(float health)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	m_MafiaVehicle->GetInterface()->vehicle_interface.health = health;

	return true;
}

float CClientVehicle::GetHealth()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return m_MafiaVehicle->GetInterface()->vehicle_interface.health;
}

bool CClientVehicle::SetWheelAngle(float wheelAngle)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	m_MafiaVehicle->GetInterface()->vehicle_interface.wheel_angle = wheelAngle;

	return true;
}

float CClientVehicle::GetWheelAngle()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return m_MafiaVehicle->GetInterface()->vehicle_interface.wheel_angle;
}

bool CClientVehicle::SetOdometer(float odometer)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	//m_MafiaVehicle->GetInterface()->vehicle_interface.odometer = odometer;

	return true;
}

float CClientVehicle::GetOdometer()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	//return m_MafiaVehicle->GetInterface()->vehicle_interface.odometer;
	return 0.0f;
}

bool CClientVehicle::SetRoof(bool state)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	//m_MafiaVehicle->GetInterface()->vehicle_interface.roof = state;
	return false;
}

bool CClientVehicle::GetRoof()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	//return m_MafiaVehicle->GetInterface()->vehicle_interface.roof;
	return false;
}

bool CClientVehicle::SetLocked(bool state)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	//m_MafiaVehicle->GetInterface()->vehicle_interface.locked = state;
	return false;
}

bool CClientVehicle::GetLocked()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	//return m_MafiaVehicle->GetInterface()->vehicle_interface.locked;
	return false;
}

bool CClientVehicle::SetLights(bool state)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	auto* pVehicleInterface = &m_MafiaVehicle->GetInterface()->vehicle_interface;
	pVehicleInterface->lights = state;
	return true;
}

bool CClientVehicle::GetLights()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	auto* pVehicleInterface = &m_MafiaVehicle->GetInterface()->vehicle_interface;
	return pVehicleInterface->lights;
}

bool CClientVehicle::SetEngine(bool state)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	m_MafiaVehicle->SetEngineOn(state, 1);
	return true;
}

bool CClientVehicle::GetEngine()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	auto* pVehicleInterface = &m_MafiaVehicle->GetInterface()->vehicle_interface;
	return pVehicleInterface->engine_on;
}

bool CClientVehicle::Explode()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	m_MafiaVehicle->CarExplosion(200);
	return true;
}

bool CClientVehicle::IsSeatOccupied(unsigned char ucSeat)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	auto pHuman = m_pOccupants[ucSeat].GetPointer();

	if (pHuman == nullptr)
		return false;

	return true;
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

	m_MafiaVehicle->GetInterface()->vehicle_interface.siren = state;
	return true;
}

bool CClientVehicle::GetSiren()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	return m_MafiaVehicle->GetInterface()->vehicle_interface.siren;
}

bool CClientVehicle::Repair()
{
	if (m_MafiaVehicle == nullptr)
		return false;

	m_MafiaVehicle->RepairPosition(true);
	return true;
}

void CClientVehicle::SetFromExistingEntity(MafiaSDK::C_Car* car) {
	m_MafiaVehicle = car;
}