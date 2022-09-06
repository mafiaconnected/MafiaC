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

void CClientHuman::UpdateGameMatrix(void)
{
	if (m_MafiaHuman->GetFrame() == nullptr)
		return;

	uint32_t frame = (uint32_t)m_MafiaHuman->GetFrame();
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

	m_MafiaHuman->GetFrame()->Update();
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

	UpdateGameMatrix();

	// Disable interpolation
	if (m_pBlender != nullptr)
		m_pBlender->ResetInterpolation();

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

	GetGameHuman()->GetInterface()->entity.rotation = CVecTools::ConvertToMafiaVec(CVecTools::ComputeDirVector(CVecTools::RadToDeg(heading)));

	UpdateGameMatrix();

	// Disable interpolation
	if (m_pBlender != nullptr)
		m_pBlender->ResetInterpolation();

	return true;
}

float CClientHuman::GetHeading()
{
	if (GetGameHuman() == nullptr)
		return 0.0f;

	CVector3D vecRot;
	GetRotation(vecRot);

	return CVecTools::DegToRad(CVecTools::DirToRotation360(CVecTools::EulerToDir(vecRot)));
}

bool CClientHuman::SetRotation(const CVector3D& vecRot)
{
	if (GetGameHuman() == nullptr)
		return false;

	GetGameHuman()->GetInterface()->entity.rotation = CVecTools::ConvertToMafiaVec(CVecTools::EulerToDir(vecRot));

	UpdateGameMatrix();

	// Disable interpolation
	if (m_pBlender != nullptr)
		m_pBlender->ResetInterpolation();

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

	g_pClientGame->m_bCreateActorInvokedByGame = false;
	m_MafiaHuman = reinterpret_cast<MafiaSDK::C_Human*>(MafiaSDK::GetMission()->CreateActor(isLocal ? MafiaSDK::C_Mission_Enum::ObjectTypes::Player : MafiaSDK::C_Mission_Enum::ObjectTypes::Enemy));
	g_pClientGame->m_bCreateActorInvokedByGame = true;

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

	//m_MafiaHuman->GetInterface()->entity.position = CVecTools::ConvertToMafiaVec(pos);
	//m_MafiaHuman->GetInterface()->entity.rotation = CVecTools::ConvertToMafiaVec(rot);

	SetPosition(pos);
	SetRotation(rot);

	m_pEntity = m_MafiaHuman;

	//_glogprintf(_gstr("Spawned human for element #%d:\n\tModel: %s\n\tPosition: {%f, %f, %f}\n\tAngle: %f"), GetId(), m_szModel, m_Position.x, m_Position.y, m_Position.z, angle);
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
	if (!CClientEntity::ReadCreatePacket(pStream))
		return false;

	MafiaSDK::C_Human_Interface* IHuman;

	tHumanCreatePacket Packet;
	if (pStream->Read(&Packet, sizeof(Packet)) != sizeof(Packet))
		return false;

	//_glogprintf(_gstr("Got create packet for element #%d:\n\tModel: %s\n\tPosition: [%f, %f, %f - %f, %f, %f]\n\tRotation: [%f, %f, %f - %f, %f, %f]\n", GetId(), m_szModel, m_Position.x, m_Position.y, m_Position.z, m_RelativePosition.x, m_RelativePosition.y, m_RelativePosition.z, m_Rotation.x, m_Rotation.y, m_Rotation.z, m_RelativeRotation.x, m_RelativeRotation.y, m_RelativeRotation.z));

	if (GetGameHuman() == nullptr)
	{
		bool isLocalPlayer = IsType(ELEMENT_PLAYER) && (GetSyncer() == g_pClientGame->GetActiveMultiplayer()->m_NetMachines.GetMachine(g_pClientGame->GetActiveMultiplayer()->m_iLocalIndex));
		Spawn(m_Position, CVecTools::DirToRotation180(m_Rotation), isLocalPlayer);
	}

	IHuman = GetGameHuman()->GetInterface();

	//auto pBlender = static_cast<CNetBlenderHuman*>(m_pBlender);
	//pBlender->SetTargetPosition(m_Position);
	//pBlender->SetTargetRotation(m_Rotation);

	IHuman->health = Packet.health;
	m_nVehicleNetworkIndex = Packet.vehicleNetworkIndex;
	m_nVehicleSeatIndex = Packet.seat;
	if (IHuman->carLeavingOrEntering == nullptr)
	{
		if (IHuman->playersCar == nullptr)
		{
			IHuman->animState = Packet.animationState;
			IHuman->isInAnimWithCar = false;
		}
		else
		{
			IHuman->animStateLocal = Packet.animStateLocal;
			IHuman->animState = Packet.animStateLocal;
			IHuman->isInAnimWithCar = true;
		}

		int32_t iAnimTimeLeft = *(int32_t*)(((uint32_t)IHuman) + 2772);
		*(int32_t*)(((uint32_t)IHuman) + 2772) = (int32_t)Packet.animStopTime;
		if (Packet.animStopTime <= 0)
		{
			if (iAnimTimeLeft > 0)
			{
				__asm
				{
					push 0
					mov ecx, IHuman
					mov eax, 0x57F830 // C_human::Do_Aimed
					call eax
				}
			}
		}

		IHuman->isDucking = Packet.isCrouching;
		IHuman->isAiming = Packet.isAiming;
		IHuman->isShooting = Packet.isShooting;
	}
	IHuman->inCarRotation = Packet.inCarRotation;
	SetActiveWeapon(Packet.weaponId);

	return true;
}

bool CClientHuman::ReadSyncPacket(Galactic3D::Stream* pStream)
{
	if (!CClientEntity::ReadSyncPacket(pStream))
		return false;

	CVector3D vecPos = m_Position;
	CVector3D vecRot = m_Rotation;

	if (GetGameHuman() == nullptr)
		return false;

	auto IHuman = GetGameHuman()->GetInterface();

	tHumanSyncPacket Packet;
	if (pStream->Read(&Packet, sizeof(Packet)) != sizeof(Packet))
		return false;

	IHuman->health = Packet.health;
	m_nVehicleNetworkIndex = Packet.vehicleNetworkIndex;
	m_nVehicleSeatIndex = Packet.seat;
	if (IHuman->carLeavingOrEntering == nullptr)
	{
		if (IHuman->playersCar == nullptr)
		{
			IHuman->animState = Packet.animationState;
			IHuman->isInAnimWithCar = false;
		}
		else
		{
			IHuman->animStateLocal = Packet.animStateLocal;
			IHuman->animState = Packet.animStateLocal;
			IHuman->isInAnimWithCar = true;
		}

		int32_t iAnimTimeLeft = *(int32_t*)(((uint32_t)IHuman) + 2772);
		*(int32_t*)(((uint32_t)IHuman) + 2772) = (int32_t)Packet.animStopTime;
		if (Packet.animStopTime <= 0)
		{
			if (iAnimTimeLeft > 0)
			{
				__asm
				{
					push 0
					mov ecx, IHuman
					mov eax, 0x57F830 // C_human::Do_Aimed
					call eax
				}
			}
		}

		IHuman->isDucking = Packet.isCrouching;
		IHuman->isAiming = Packet.isAiming;
		IHuman->isShooting = Packet.isShooting;

		if (m_nVehicleNetworkIndex != INVALID_NETWORK_ID)
		{
			CClientVehicle* pVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(m_nVehicleNetworkIndex, ELEMENT_VEHICLE));
			if (pVehicle != nullptr && pVehicle->GetGameVehicle() != nullptr)
			{
				if (IHuman->playersCar == nullptr)
				{
					WarpIntoVehicle(pVehicle, 0); // TODO: Use correct seat?
				}
				else if (pVehicle->GetGameVehicle() != IHuman->playersCar)
				{
					RemoveFromVehicle();
					WarpIntoVehicle(pVehicle, 0); // TODO: Use correct seat?
				}
			}
		}
		else
		{
			if (IsInVehicle())
			{
				RemoveFromVehicle();
			}
		}
	}
	IHuman->inCarRotation = Packet.inCarRotation;
	SetActiveWeapon(Packet.weaponId);

	m_vecCamera = Packet.camera;
	if (m_vecCamera.GetLength() != 0.0f)
	{
		uint32_t uiCamera = (uint32_t)&m_vecCamera;
		g_pClientGame->m_bHumanSetAimPoseInvokedByGame = false;
		float x, y, z;
		x = m_vecCamera.x;
		y = m_vecCamera.y;
		z = m_vecCamera.z;
		uint32_t uiFunc = Packet.isAiming ? 0x579EA0 : 0x579630; // set aim pose / set normal pose
		__asm
		{
			push z
			push y
			push x
			mov ecx, IHuman
			mov eax, uiFunc
			call eax
		}
		g_pClientGame->m_bHumanSetAimPoseInvokedByGame = true;
	}

	//_glogprintf(L"Got sync packet for element #%d:\n\tPosition: [%f, %f, %f]\n\tPos. difference: [%f, %f, %f]\n\tRotation: [%f, %f, %f (%f, %f, %f)]\n\tRot. difference: [%f, %f, %f]\n\tHealth: %f\n\tVehicle index: %d\n\tVehicle seat index: %d\n\tDucking: %s\n\tAiming: %s\n\tAnim state: %d", GetId(), vPos.x, vPos.y, vPos.z, vRelPos.x, vRelPos.y, vRelPos.z, vRot.x, vRot.y, vRot.z, IHuman->entity.rotation.x, IHuman->entity.rotation.y, IHuman->entity.rotation.z, vRelRot.x, vRelRot.y, vRelRot.z, IHuman->health, m_nVehicleNetworkIndex, m_nVehicleSeatIndex, IHuman->isDucking ? L"Yes" : L"No", IHuman->isAiming ? L"Yes" : L"No", IHuman->animState);

	if (!IsSyncer())
	{
		auto pBlender = static_cast<CNetBlenderHuman*>(m_pBlender);

		pBlender->SetTargetPosition(vecPos);
		pBlender->SetTargetRotation(vecRot);
	}

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

	int32_t iStopAnimTime = *(int32_t*)(((uint32_t)IHuman) + 2772);

	tHumanCreatePacket Packet;

	Packet.health = IHuman->health;
	Packet.vehicleNetworkIndex = vehicleId;
	Packet.seat = m_nVehicleSeatIndex;
	Packet.isCrouching = IHuman->isDucking;
	Packet.isAiming = IHuman->isAiming;
	Packet.isShooting = IHuman->isShooting;
	Packet.animStateLocal = IHuman->animStateLocal;
	Packet.isInAnimWithCarLocal = IHuman->isInAnimWithCarLocal;
	Packet.animationState = IHuman->animState;
	Packet.isInAnimWithCar = IHuman->isInAnimWithCar;
	Packet.inCarRotation = IHuman->inCarRotation;
	Packet.animStopTime = iStopAnimTime;
	Packet.weaponId = *(int16_t*)(((uint32_t)IHuman) + 1184);

	if (pStream->Write(&Packet, sizeof(Packet)) != sizeof(Packet))
		return false;

	//_glogprintf(L"Sent create packet for element #%d:\n\tModel: %s\n\tPosition: [%f, %f, %f]\n\tRotation: [%f, %f, %f]\n\tHealth: %f\n\tVehicle index: %d\n\tVehicle seat index: %d\n\tDucking: %s\n\tAiming: %s\n\tAnim state: %d", GetId(), m_szModel, pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, IHuman->health, m_nVehicleNetworkIndex, m_nVehicleSeatIndex, IHuman->isDucking ? L"Yes" : L"No", IHuman->isAiming ? L"Yes" : L"No", IHuman->animState);

	return true;
}

bool CClientHuman::WriteSyncPacket(Galactic3D::Stream* pStream)
{
	if (GetGameHuman() == nullptr)
		return false;

	auto IHuman = GetGameHuman()->GetInterface();

	GetPosition(m_Position);
	GetRotation(m_Rotation);

	if (!CClientEntity::WriteSyncPacket(pStream))
		return false;

	CVector3D diffPos = m_Position - prevPos;
	CVector3D diffRot = m_Rotation - prevRot;

	int vehicleId = INVALID_NETWORK_ID;

	CClientVehicle* pClientVehicle = GetEnteringExitingVehicle();
	if (pClientVehicle == nullptr)
	{
		pClientVehicle = GetOccupiedVehicle();
	}
	if (pClientVehicle != nullptr)
	{
		vehicleId = pClientVehicle->GetId();
	}

	int32_t iStopAnimTime = *(int32_t*)(((uint32_t)IHuman) + 2772);

	/*
	uint32_t uiGame0 = *(uint32_t*)0x63788C;
	uint32_t v54 = *(uint32_t*)(uiGame0 + 16);
	uint32_t v55 = *(uint32_t*)(v54 + 380);
	uint32_t v56 = v55 + 48;
	CVector3D vecCamera = *(CVector3D*)v56;
	//printf("vec: %f %f %f\n", vec.x, vec.y, vec.z);
	*/

	//uint32_t frame = *(uint32_t*)(((uint32_t)IHuman) + 1760);
	//CVector3D vecCamera(0,0,0);
	//vecCamera.z = *(float*)(((uint32_t)IHuman) + 0x5F4);
	//if (frame > 0)
	{
		//vecCamera = *(CVector3D*)(((uint32_t)IHuman) + 512);
	}

	tHumanSyncPacket Packet;

	Packet.health = IHuman->health;
	Packet.vehicleNetworkIndex = vehicleId;
	Packet.isCrouching = IHuman->isDucking;
	Packet.isAiming = IHuman->isAiming;
	Packet.isShooting = IHuman->isShooting;
	Packet.animStateLocal = IHuman->animStateLocal;
	Packet.isInAnimWithCarLocal = IHuman->isInAnimWithCarLocal;
	Packet.animationState = IHuman->animState;
	Packet.isInAnimWithCar = IHuman->isInAnimWithCar;
	Packet.inCarRotation = IHuman->inCarRotation;
	Packet.animStopTime = iStopAnimTime;
	Packet.weaponId = *(int16_t*)(((uint32_t)IHuman) + 1184);
	Packet.camera = m_vecCamera;

	if (pStream->Write(&Packet, sizeof(Packet)) != sizeof(Packet))
		return false;

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
	if (!IsSyncer() && m_pBlender != nullptr && GetGameHuman() != nullptr)
	{
		m_pBlender->Interpolate();
	}

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
		CClientHuman* pClientHuman = pClientVehicle->GetHumanInSeat(i);
		if (pClientHuman != nullptr && this == pClientHuman)
		{
			return i;
		}
	}

	return -1;
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

	if (GetGameHuman()->GetInterface()->carLeavingOrEntering != nullptr)
		return;

	CClientVehicle* pVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(m_nVehicleNetworkIndex, ELEMENT_VEHICLE));

	m_nVehicleNetworkIndex = INVALID_NETWORK_ID;

	if (pVehicle != nullptr)
	{
		pVehicle->FreeSeat(m_nVehicleSeatIndex);
	}

	if (GetGameHuman()->GetInterface()->playersCar != nullptr)
	{
		GetGameHuman()->Intern_FromCar();
	}
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
	//_glogprintf(_gstr("Setting element #%d (human) to shoot at [%f, %f, %f] with state: %d"), GetId(), dstPos.x, dstPos.y, dstPos.z, state);
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
	_glogprintf(_gstr("Setting element #%d (human) to throw grenade at [%f, %f, %f]"), GetId(), dstPos.x, dstPos.y, dstPos.z);
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
			if (this == m_pClientManager->m_pLocalPlayer.GetPointer()) MafiaSDK::GetIndicators()->ConsoleAddText("The inventory is full!", 0xFFFFFFFF);
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

void CClientHuman::SetActiveWeapon(unsigned short usWeapon)
{
	uint32_t uiHuman = (uint32_t)GetGameHuman();
	if (uiHuman == 0)
		return;

	if (*(uint16_t*)(uiHuman + 1184) == usWeapon)
		return;

	GiveWeapon(usWeapon, 32000, 32000);

	*(uint16_t*)(uiHuman + 1184) = usWeapon; // uint16 +1184 = active weapon id

	__asm
	{
		push 1
		push 1
		mov ecx, uiHuman
		mov eax, 0x57F550 // Human::ChangeWeapon
		call eax
	}
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

bool CClientHuman::IsShooting()
{
	return GetGameHuman()->GetInterface()->isShooting;
}

void CClientHuman::SetFromExistingEntity(MafiaSDK::C_Human* human) {
	m_MafiaHuman = human;
}

void CClientHuman::SetBehavior(uint32_t iBehavior)
{
	m_MafiaHuman->SetBehavior((MafiaSDK::C_Human_Enum::BehaviorStates)iBehavior);
}

void CClientHuman::CreateNetBlender()
{
	auto pBlender = new CNetBlenderHuman(this);
	auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
	if (pMultiplayer != nullptr)
		pBlender->m_uiDelay = pMultiplayer->m_usSyncIntervalInMS + 20;
	m_pBlender = pBlender;
}