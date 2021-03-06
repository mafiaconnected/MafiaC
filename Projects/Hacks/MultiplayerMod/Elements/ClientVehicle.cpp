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
	if (!IsSyncer() && m_pBlender != nullptr && GetGameVehicle() != nullptr)
	{
		m_pBlender->Interpolate();
		GetGameVehicle()->GetInterface()->vehicle_interface.horn = m_Horn;
		GetGameVehicle()->GetInterface()->vehicle_interface.engine_rpm = m_EngineRPM;
		
		/*
	 	GetGameVehicle()->SetActive(true);
		//GetGameVehicle()->SetActState(0); // Note: Commeted as it caused vehicle jerking.
		GetGameVehicle()->Engine(0.083f, 0.083f, 0.083f);
		GetGameVehicle()->Update(g_pClientGame->m_pTime->m_fDeltaTime);
		*/
	}

	// Prevent vehicle damage.
	uint32_t uiVehicle = (uint32_t)GetGameVehicle();
	if (uiVehicle)
	{
		GetGameVehicle()->GetInterface()->vehicle_interface.health = 100.0f;
		GetGameVehicle()->GetInterface()->vehicle_interface.engine_health = 100.0f;
		*(uint32_t*)(uiVehicle + 3176) = 0; // flags
		for (int i = 0; i < 4; i++)
		{
			uint32_t uiWheel = *(uint32_t*)((*(uint32_t*)(uiVehicle + 3240)) + (4 * i));
			*(float*)(uiWheel + 0x18C) = 100.0f; // prevent wheel falling off
		}
	}

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

void CClientVehicle::Create(const GChar* model, const CVector3D& pos, const CVector3D& rot)
{
	if (m_MafiaVehicle != nullptr)
		return;

	auto pVehModel = (MafiaSDK::I3D_Model*)MafiaSDK::I3DGetDriver()->CreateFrame(MafiaSDK::I3D_Driver_Enum::FrameType::MODEL);

	UTF8String mdl(true, model);

	MafiaSDK::GetModelCache()->Open(pVehModel, mdl.CString(), NULL, NULL, NULL, NULL);

	pVehModel->SetName("SomeOrdinaryVehicles");
	pVehModel->SetScale({ 1, 1, 1 });

	//S_quat quat;
	//CVecTools::rotate(rot.z, rot.y, -M_PI, quat);
	//pVehModel->SetRot(quat);
	//pVehModel->SetWorldPos(CVecTools::ConvertToMafiaVec(pos));

	pVehModel->Update();

	m_MafiaVehicle = reinterpret_cast<MafiaSDK::C_Car*>(MafiaSDK::GetMission()->CreateActor(MafiaSDK::C_Mission_Enum::ObjectTypes::Car));
	SetPosition(pos);
	//CVector3D rot2(0, 0, 0);
	SetRotation(rot);
	SetHeading(rot.z);
	m_MafiaVehicle->Init(pVehModel);
	m_MafiaVehicle->SetActive(true);

	MafiaSDK::GetMission()->GetGame()->AddTemporaryActor(m_MafiaVehicle);

	m_pEntity = m_MafiaVehicle;

	auto IVehicle = m_MafiaVehicle->GetInterface()->vehicle_interface;

	_glogprintf(_gstr("Created new vehicle for element #%d:\n\tModel: %s\n\tPosition: {%f, %f, %f}\n\tRotation: {%f, %f, %f}\n\tHealth: %f\n\tEngine health: %f\n\tFuel: %f\n\tSound: %s\n\tEngine on: %s\n\tHorn: %s\n\tSiren: %s\n\tGear: %d\n\tEngine RPM: %f\n\tAcceleration: %f\n\tBrake: %f\n\tHandbrake: %f\n\tSpeed limit: %f\n\tClutch: %f\n\tWheel angle: %f"), GetId(), model, pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, IVehicle.health, IVehicle.engine_health, IVehicle.fuel, IVehicle.sound_enabled ? L"Yes" : L"No", IVehicle.engine_on ? L"Yes" : L"No", IVehicle.horn ? L"Yes" : L"No", IVehicle.siren ? L"Yes" : L"No", IVehicle.gear, IVehicle.engine_rpm, IVehicle.accelerating, IVehicle.break_val, IVehicle.hand_break, IVehicle.speed_limit, IVehicle.clutch, IVehicle.wheel_angle);
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
	MafiaSDK::GetMission()->GetGame()->RemoveTemporaryActor((MafiaSDK::C_Actor*)GetGameVehicle());
}

bool CClientVehicle::SetModel(const GChar* modelName)
{
	return true;
}

const GChar* CClientVehicle::GetModel()
{
	return CClientEntity::GetModel();
}

void CClientVehicle::UpdateGameMatrix(void)
{
	if (m_MafiaVehicle->GetFrame() == nullptr)
		return;

	uint32_t frame = (uint32_t)m_MafiaVehicle->GetFrame();
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

	m_MafiaVehicle->GetFrame()->Update();
}

bool CClientVehicle::SetPosition(const CVector3D& vecPos)
{
	if (GetGameVehicle() == nullptr)
		return false;

	CClientEntity::SetPosition(vecPos);
	m_MafiaVehicle->GetInterface()->entity.position = CVecTools::ConvertToMafiaVec(vecPos);
	m_MafiaVehicle->GetInterface()->vehicle_interface.position = CVecTools::ConvertToMafiaVec(vecPos);

	UpdateGameMatrix();

	//MafiaSDK::I3D_Model* pFrame = (MafiaSDK::I3D_Model*)GetGameVehicle()->GetFrame();
	//pFrame->SetWorldPos(CVecTools::ConvertToMafiaVec(vecPos));

	// Disable interpolation
	if (m_pBlender != nullptr)
		m_pBlender->ResetInterpolation();

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

	//GetGameVehicle()->GetInterface()->entity.rotation = CVecTools::ConvertToMafiaVec(vecRot);

	/*
	CMatrix3x3 mat(vecRot);

	m_RotationFront = mat.GetXAxis();
	m_RotationUp = mat.GetYAxis();
	m_RotationRight = mat.GetZAxis();

	m_MafiaVehicle->GetInterface()->vehicle_interface.rot_forward = CVecTools::ConvertToMafiaVec(mat.GetXAxis());
	m_MafiaVehicle->GetInterface()->vehicle_interface.rot_up = CVecTools::ConvertToMafiaVec(mat.GetYAxis());
	m_MafiaVehicle->GetInterface()->vehicle_interface.rot_right = CVecTools::ConvertToMafiaVec(mat.GetZAxis());

	//_glogprintf(_gstr("Vehicle SetRotation #%d:\n\tMafiaRotationFront: {%f, %f, %f}\n\tMafiaRotationUp: {%f, %f, %f}\n\tMafiaRotationRight: {%f, %f, %f}\n\tVec3Rotation: {%f, %f, %f}\n"), GetId(), mat[0].x, mat[0].y, mat[0].z, mat[1].x, mat[1].y, mat[1].z, mat[2].x, mat[2].y, mat[2].z, vecRot.x, vecRot.y, vecRot.z);
	*/

	CClientEntity::SetRotation(vecRot);

	//UpdateGameMatrix();

	// Disable interpolation
	if (m_pBlender != nullptr)
		m_pBlender->ResetInterpolation();

	return true;
}

bool CClientVehicle::GetRotation(CVector3D& vecRot)
{
	if (GetGameVehicle() == nullptr)
		return false;

	vecRot = CVecTools::ConvertFromMafiaVec(GetGameVehicle()->GetInterface()->entity.rotation);
	return true;

	/*
	CVector3D front = CVecTools::ConvertFromMafiaVec(m_MafiaVehicle->GetInterface()->vehicle_interface.rot_forward);
	CVector3D up = CVecTools::ConvertFromMafiaVec(m_MafiaVehicle->GetInterface()->vehicle_interface.rot_up);
	CVector3D right = CVecTools::ConvertFromMafiaVec(m_MafiaVehicle->GetInterface()->vehicle_interface.rot_right);

	CMatrix3x3 mat(front, up, right);
	vecRot = mat.GetEuler();
	vecRot = CVecTools::EulerToDir(vecRot);

	//_glogprintf(_gstr("Vehicle GetRotation #%d:\n\tMafiaRotationFront: {%f, %f, %f}\n\tMafiaRotationUp: {%f, %f, %f}\n\tMafiaRotationRight: {%f, %f, %f}\n\tVec3Rotation: {%f, %f, %f}\n"), GetId(), front.x, front.y, front.z, up.x, up.y, up.z, right.x, right.y, right.z, vecRot.x, vecRot.y, vecRot.z);
	return true;
	*/
}

bool CClientVehicle::SetHeading(float heading)
{
	if (GetGameVehicle() == nullptr)
		return false;

	CClientEntity::SetHeading(heading);

	//GetGameVehicle()->GetInterface()->entity.rotation = CVecTools::ConvertToMafiaVec(CVecTools::ComputeDirVector(heading));
	CVector3D rot = CVecTools::ComputeDirEuler(heading);
	SetRotation(rot);

	CVector3D front;
	CVector3D up;
	CVector3D right;
	GetVehicleRotation(front, up, right);
	SetVehicleRotation(rot, up, right);

	UpdateGameMatrix();

	// Disable interpolation
	if (m_pBlender != nullptr)
		m_pBlender->ResetInterpolation();

	return true;
}

float CClientVehicle::GetHeading()
{
	if (GetGameVehicle() == nullptr)
		return false;

	return CVecTools::ConvertFromMafiaVec(GetGameVehicle()->GetInterface()->entity.rotation).z;
}

bool CClientVehicle::SetVehicleRotation(const CVector3D& rotationFront, const CVector3D& rotationUp, const CVector3D& rotationRight)
{
	if (GetGameVehicle() == nullptr)
		return false;

	//m_RotationFront = rotationFront;
	//m_RotationUp = rotationUp;
	//m_RotationRight = rotationRight;

	m_MafiaVehicle->GetInterface()->vehicle_interface.rot_forward = CVecTools::ConvertToMafiaVec(rotationFront);
	m_MafiaVehicle->GetInterface()->vehicle_interface.rot_up = CVecTools::ConvertToMafiaVec(rotationUp);
	m_MafiaVehicle->GetInterface()->vehicle_interface.rot_right = CVecTools::ConvertToMafiaVec(rotationRight);

	UpdateGameMatrix();

	//_glogprintf(_gstr("Vehicle SetVehicleRotation #%d:\n\tMafiaRotationFront: {%f, %f, %f}\n\tMafiaRotationUp: {%f, %f, %f}\n\tMafiaRotationRight: {%f, %f, %f}\n"), GetId(), rotationFront.x, rotationFront.y, rotationFront.z, rotationUp.x, rotationUp.y, rotationUp.z, rotationRight.x, rotationRight.y, rotationRight.z);
	return true;
}

bool CClientVehicle::GetVehicleRotation(CVector3D& rotationFront, CVector3D& rotationUp, CVector3D& rotationRight)
{
	if (GetGameVehicle() == nullptr)
		return false;

	rotationFront = CVecTools::ConvertFromMafiaVec(m_MafiaVehicle->GetInterface()->vehicle_interface.rot_forward);
	rotationUp = CVecTools::ConvertFromMafiaVec(m_MafiaVehicle->GetInterface()->vehicle_interface.rot_up);
	rotationRight = CVecTools::ConvertFromMafiaVec(m_MafiaVehicle->GetInterface()->vehicle_interface.rot_right);

	//_glogprintf(_gstr("Vehicle GetVehicleRotation #%d:\n\tMafiaRotationFront: {%f, %f, %f}\n\tMafiaRotationUp: {%f, %f, %f}\n\tMafiaRotationRight: {%f, %f, %f}\n"), GetId(), rotationFront.x, rotationFront.y, rotationFront.z, rotationUp.x, rotationUp.y, rotationUp.z, rotationRight.x, rotationRight.y, rotationRight.z);
	return true;
}

bool CClientVehicle::SetRotationVelocity(const CVector3D& vecRotVel)
{
	if (GetGameVehicle() == nullptr)
		return false;

	GetGameVehicle()->GetInterface()->vehicle_interface.rot_speed = CVecTools::ConvertToMafiaVec(vecRotVel);

	return true;
}

bool CClientVehicle::GetRotationVelocity(CVector3D& vecRotVel)
{
	if (GetGameVehicle() == nullptr)
		return false;

	vecRotVel = CVecTools::ConvertFromMafiaVec(GetGameVehicle()->GetInterface()->vehicle_interface.rot_speed);

	return true;
}

bool CClientVehicle::ReadCreatePacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntity::ReadCreatePacket(pStream))
		return false;

	relPos = m_Position;
	relRot = m_Rotation;

	tVehicleCreatePacket Packet;

	if (pStream->Read(&Packet, sizeof(Packet)) != sizeof(Packet))
		return false;

	m_RotationFront = Packet.rotationFront;
	m_RotationUp = Packet.rotationUp;
	m_RotationRight = Packet.rotationRight;

	//CMatrix3x3 mat3(m_RotationFront, m_RotationUp, m_RotationRight);
	GChar szModel[64];
	wmemcpy(szModel, m_szModel, 64);
	CVector3D vecPos = m_Position;
	CVector3D vecRot = m_Rotation;

	if (GetGameVehicle() == nullptr)
	{
		Create(szModel, vecPos, vecRot);
	}
	else
	{
		SetPosition(vecPos);
		SetRotation(vecRot);
	}
	//SetVehicleRotation(m_RotationFront, m_RotationUp, m_RotationRight);

	auto pBlender = static_cast<CNetBlenderVehicle*>(m_pBlender);

	pBlender->SetTargetPosition(vecPos);
	pBlender->SetTargetRotation(m_RotationFront, m_RotationUp, m_RotationRight);
	pBlender->SetTargetSpeed(Packet.speed, Packet.rotSpeed);

	auto pGameVehicle = &GetGameVehicle()->GetInterface()->vehicle_interface;
	auto pGameCar = GetGameVehicle();

	pGameVehicle->health = Packet.health;
	pGameVehicle->engine_health = Packet.engineHealth;
	pGameVehicle->fuel = Packet.fuel;
	pGameVehicle->sound_enabled = Packet.sound;
	//pGameVehicle->engine_on = Packet.engineOn;
	m_Horn = Packet.horn;
	//pGameVehicle->horn = Packet.horn;
	pGameVehicle->siren = Packet.siren;
	pGameVehicle->lights = Packet.lights;
	//pGameVehicle->gear = Packet.gear;
	//pGameVehicle->engine_rpm = Packet.rpm;
	m_EngineRPM = Packet.rpm;
	pGameVehicle->accelerating = Packet.accel;
	pGameVehicle->break_val = Packet.brake;
	pGameVehicle->hand_break = Packet.handBrake;
	pGameVehicle->speed_limit = Packet.speedLimit;
	pGameVehicle->clutch = Packet.clutch;

	//if (gear != IVehicle.gear) {
	//	GetGameVehicle()->SetGear(gear);
	//}

	if (Packet.engineOn)
	{
		if (!GetEngine())
		{
			SetEngine(true);
		}
	}
	else
	{
		if (!pGameVehicle->engine_on)
		{
			SetEngine(false);
		}
	}

	//GetGameVehicle()->SetActive(true);
	GetGameVehicle()->SetActState(0);
	//GetGameVehicle()->Engine(0.083f, 0.083f, 0.083f);
	//GetGameVehicle()->SetColsOn(TRUE);
	//GetGameVehicle()->Update(g_pClientGame->m_pTime->m_fDeltaTime);
	return true;
}

bool CClientVehicle::ReadSyncPacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntity::ReadSyncPacket(pStream))
		return false;

	if (GetGameVehicle() == nullptr)
		return false;

	CVector3D vecPos = m_Position;
	CVector3D vecRot = m_Rotation;

	auto pGameVehicle = &GetGameVehicle()->GetInterface()->vehicle_interface;
	auto pGameCar = GetGameVehicle();

	tVehicleSyncPacket Packet;
	if (pStream->Read(&Packet, sizeof(Packet)) != sizeof(Packet))
		return false;

	m_RotationFront = Packet.rotationFront;
	m_RotationUp = Packet.rotationUp;
	m_RotationRight = Packet.rotationRight;

	pGameVehicle->health = Packet.health;
	pGameVehicle->engine_health = Packet.engineHealth;
	pGameVehicle->fuel = Packet.fuel;
	pGameVehicle->sound_enabled = Packet.sound;
	//pGameVehicle->engine_on = Packet.engineOn;
	//pGameVehicle->horn = Packet.horn;
	m_Horn = Packet.horn;
	pGameVehicle->siren = Packet.siren;
	pGameVehicle->lights = Packet.lights;
	//pGameVehicle->gear = Packet.gear;
	//pGameVehicle->engine_rpm = Packet.rpm;
	m_EngineRPM = Packet.rpm;
	pGameVehicle->accelerating = Packet.accel;
	pGameVehicle->break_val = Packet.brake;
	pGameVehicle->hand_break = Packet.handBrake;
	pGameVehicle->speed_limit = Packet.speedLimit;
	pGameVehicle->clutch = Packet.clutch;
	pGameVehicle->wheel_angle = Packet.wheelAngle;

	if (Packet.gear != GetGear())
	{
		SetGear(Packet.gear);
	}

	if (Packet.engineOn)
	{
		if (!GetEngine())
		{
			SetEngine(true);
		}
	}
	else
	{
		if (!pGameVehicle->engine_on)
		{
			SetEngine(false);
		}
	}

	m_RelativePosition = Packet.speed;
	m_RelativeRotation = Packet.rotSpeed;

	//GetGameVehicle()->SetActive(true);
	//GetGameVehicle()->SetActState(0); // Note: Commeted as it caused vehicle jerking.
	//GetGameVehicle()->Engine(0.083f, 0.083f, 0.083f);

	if (!IsSyncer())
	{
		auto pBlender = static_cast<CNetBlenderVehicle*>(m_pBlender);
		
		pBlender->SetTargetPosition(vecPos);
		pBlender->SetTargetRotation(m_RotationFront, m_RotationUp, m_RotationRight);
		pBlender->SetTargetSpeed(Packet.speed, Packet.rotSpeed);
		//pBlender->SetTargetEngineRPM(Packet.rpm);
		//pBlender->SetTargetWheelAngle(Packet.wheelAngle); 
	}

	//_glogprintf(_gstr("Got sync packet for vehicle #%d:\n\tPosition: [%f, %f, %f]\n\tPos. difference: [%f, %f, %f]\n\tRotation: [%f, %f, %f]\n\tRot. difference: [%f, %f, %f]"), GetId(), m_Position.x, m_Position.y, m_Position.z, m_RelativePosition.x, m_RelativePosition.y, m_RelativePosition.z, m_Rotation.x, m_Rotation.y, m_Rotation.z, m_RelativeRotation.x, m_RelativeRotation.y, m_RelativeRotation.z);

	return true;
}

bool CClientVehicle::WriteCreatePacket(Galactic3D::Stream* pStream)
{
	if (GetGameVehicle() == nullptr)
		return false;

	GetPosition(m_Position);
	GetRotation(m_Rotation);
	//GetVehicleRotation(m_RotationFront, m_RotationUp, m_RotationRight);

	if (!CClientEntity::WriteCreatePacket(pStream))
		return false;

	auto IVehicle = GetGameVehicle()->GetInterface()->vehicle_interface;

	tVehicleCreatePacket Packet;

	Packet.rotationFront = m_RotationFront;
	Packet.rotationUp = m_RotationUp;
	Packet.rotationRight = m_RotationRight;

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
	GetVelocity(Packet.speed);
	GetRotationVelocity(Packet.rotSpeed);

	if (Packet.engineOn == true) {
		GetGameVehicle()->SetEngineOn(true, 2);
	}

	if (pStream->Write(&Packet, sizeof(Packet)) != sizeof(Packet))
		return false;

	return true;
}

bool CClientVehicle::WriteSyncPacket(Galactic3D::Stream* pStream)
{
	if (GetGameVehicle() == nullptr)
		return false;

	GetPosition(m_Position);
	GetRotation(m_Rotation);
	GetVehicleRotation(m_RotationFront, m_RotationUp, m_RotationRight);

	if (!CClientEntity::WriteSyncPacket(pStream))
		return false;

	auto IVehicle = GetGameVehicle()->GetInterface()->vehicle_interface;

	tVehicleSyncPacket Packet;

	Packet.rotationFront = m_RotationFront;
	Packet.rotationUp = m_RotationUp;
	Packet.rotationRight = m_RotationRight;

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
	Packet.speed = CVecTools::ConvertFromMafiaVec(IVehicle.speed);
	Packet.rotSpeed = CVecTools::ConvertFromMafiaVec(IVehicle.rot_speed);

	if (pStream->Write(&Packet, sizeof(Packet)) != sizeof(Packet))
		return false;

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

bool CClientVehicle::SetGear(uint32_t gear)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	m_MafiaVehicle->GetInterface()->vehicle_interface.gear = gear;

	return true;
}

uint32_t CClientVehicle::GetGear()
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

bool CClientVehicle::SetEngineRPM(float engineRPM)
{
	if (m_MafiaVehicle == nullptr)
		return false;

	m_MafiaVehicle->GetInterface()->vehicle_interface.engine_rpm = engineRPM;

	return true;
}

float CClientVehicle::GetEngineRPM()
{
	if (m_MafiaVehicle == nullptr)
		return -1;

	return m_MafiaVehicle->GetInterface()->vehicle_interface.engine_rpm;
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

void CClientVehicle::CreateNetBlender()
{
	auto pBlender = new CNetBlenderVehicle(this);
	auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
	if (pMultiplayer != nullptr)
		pBlender->m_uiDelay = pMultiplayer->m_usSyncIntervalInMS + 70;
	m_pBlender = pBlender;
}
