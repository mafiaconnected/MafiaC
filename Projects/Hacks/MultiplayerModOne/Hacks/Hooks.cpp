#include "stdafx.h"

#include "Hacks/Hooks.h"
#include "Hacks/Hacks.h"

#include <d3d9.h>
#include "Renderer.h"

#include "Elements/ClientHuman.h"
#include "Elements/ClientPlayer.h"
#include "Elements/ClientVehicle.h"
#include "ClientGame.h"
#include "LoadScreen.h"
#include "Multiplayer.h"
#include "../Utils/VectorTools.h"

using namespace Galactic3D;

std::unordered_map<uint32_t, std::string> g_umapModelNames;

extern decltype(Direct3DCreate9)* g_pDirect3DCreate9;
extern IDirect3D9* WINAPI HookDirect3DCreate9(UINT SDKVersion);

extern Direct3D9* g_pD3D9;
extern IDirect3DDevice9* g_pD3DDevice;
extern C2D* g_p2D;

static void OnGameInit()
{
	auto mName = MafiaSDK::GetCurrentMissionName();

	MafiaSDK::GetIGraph()->SetAppName("Mafia Connected");

	// We don't want the game to be initialized in menu
	//if (!strcmp(mName, "00menu")) return;

	if (g_pClientGame->m_bFullReload) {
		g_pClientGame->OnPreStartInGame(false);
		g_pClientGame->OnStartInGame(false);

		auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
		if (pMultiplayer != nullptr)
		{
			pMultiplayer->Join(); // elements/resources are delayed until this call
		}
	}

	CGameHacks::EnableGameMap(g_pClientGame->IsGameComponentEnabled(GAMECOMPONENT_BIGMAP));

	MafiaSDK::GetMission()->GetGame()->SetTrafficVisible(g_pClientGame->IsGameComponentEnabled(GAMECOMPONENT_TRAFFIC));

	if (!g_pClientGame->IsGameComponentEnabled(GAMECOMPONENT_BRIDGES)) {
		auto bridge1 = (MafiaSDK::C_Bridge*)MafiaSDK::GetMission()->FindActorByName("LLsklap01");
		auto bridge2 = (MafiaSDK::C_Bridge*)MafiaSDK::GetMission()->FindActorByName("sklapx01");

		if (bridge1) bridge1->Shutdown(true);
		if (bridge2) bridge2->Shutdown(true);
	}

	CArguments Args;
	Args.AddString(CString(false, mName).CString());
	bool bPreventDefault = false;
	g_pClientGame->m_pOnMapLoadedEventType->Trigger(Args, bPreventDefault);
	if (bPreventDefault)
		return;

	// Set full reload for next time (script can disable for map change)
	//g_pClientGame->m_bFullReload = true;
}

static void OnGameExit()
{
	if (g_pClientGame->GetActiveMultiplayer() != nullptr) {
		g_pClientGame->GetActiveMultiplayer()->Disconnect(DISCONNECT_GRACEFUL);
	}
}

__declspec(naked) void OnGameExit_Hook()
{
	__asm {
		pushad
		call OnGameExit
		popad
		retn
	}
}

// UseActor
HOOKADDRESS g_ReturnHumanUseActor;
HOOKVAR MafiaSDK::C_Human* g_pHumanUseActor_Human;
HOOKVAR MafiaSDK::C_Actor* g_pHumanUseActor_Actor;
HOOKVAR int g_pHumanUseActor_Unk1;
HOOKVAR int g_pHumanUseActor_Unk2;
HOOKVAR int g_pHumanUseActor_Unk3;
HOOKVAR bool g_bCancelHumanUseActor;

// DoThrowCocotFromCar
HOOKADDRESS g_ReturnHumanDoThrowCocotFromCar;
HOOKVAR MafiaSDK::C_Human* g_pHumanDoThrowCocotFromCar_Human;
HOOKVAR MafiaSDK::C_Car* g_pHumanDoThrowCocotFromCar_Car;
HOOKVAR int g_pHumanDoThrowCocotFromCar_SeatID;
HOOKVAR bool g_bCancelHumanDoThrowCocotFromCar;

// CreateActor
HOOKADDRESS g_ReturnCreateActor;
HOOKVAR MafiaSDK::C_Actor* g_pCreateActor_Actor;
HOOKVAR int g_pCreateActor_Arg1;
HOOKVAR bool g_bCancelCreateActor;

// CarCar
HOOKADDRESS g_ReturnCarUpdate;
HOOKVAR MafiaSDK::C_Car* g_pCarUpdate_Car;
HOOKVAR bool g_bCancelCarUpdate;

// SceneCreateActor
HOOKADDRESS g_ReturnSceneCreateActor;
HOOKVAR MafiaSDK::C_Mission_Enum::ObjectTypes g_pSceneCreateActor_Type;
HOOKVAR DWORD g_pSceneCreateActor_Frame;
HOOKVAR bool g_bCancelSceneCreateActor;

// HumanSetAimPose
HOOKADDRESS g_ReturnHumanSetAimPose;
HOOKVAR MafiaSDK::C_Human* g_pHumanSetAimPose_Human;
HOOKVAR CVector3D* g_pvecHumanSetAimPose_Vec;
HOOKVAR bool g_bCancelHumanSetAimPose;

// HumanSetNormalPose
HOOKADDRESS g_ReturnHumanSetNormalPose;
HOOKVAR MafiaSDK::C_Human* g_pHumanSetNormalPose_Human;
HOOKVAR CVector3D* g_pvecHumanSetNormalPose_Vec;
HOOKVAR bool g_bCancelHumanSetNormalPose;

// HookModelOpen
HOOKADDRESS g_ReturnModelOpen;
HOOKVAR uint32_t g_pModelOpen_Arg1;
HOOKVAR uint32_t g_pModelOpen_Arg2;
HOOKVAR bool g_bCancelModelOpen;

RAWCODECALL HumanDoThrowCocotFromCar()
{
	if (g_pClientGame->m_bDoThrowCocotFromCarInvokedByGame)
	{
		CClientHuman* pClientHuman = g_pClientGame->m_pClientManager->FindHuman(g_pHumanDoThrowCocotFromCar_Human);
		if (pClientHuman != nullptr)
		{
			CClientVehicle* pClientVehicle = g_pClientGame->m_pClientManager->FindVehicle(g_pHumanDoThrowCocotFromCar_Car);
			if (pClientVehicle != nullptr)
			{
				bool bAllow = true;

				if (bAllow)
				{
					g_pClientGame->HumanJackVehicle(pClientHuman, pClientVehicle, g_pHumanDoThrowCocotFromCar_SeatID);
				}
				else
				{
					g_bCancelHumanDoThrowCocotFromCar = true;
				}
			}
		}
	}
}

RAWCODECALL HumanUseActor()
{
	if (g_pClientGame->m_bUseActorInvokedByGame)
	{
		CClientHuman* pClientHuman = g_pClientGame->m_pClientManager->FindHuman((MafiaSDK::C_Human*)g_pHumanUseActor_Human);
		if (pClientHuman != nullptr)
		{
			CClientVehicle* pClientVehicle = g_pClientGame->m_pClientManager->FindVehicle((MafiaSDK::C_Car*)g_pHumanUseActor_Actor);
			if (pClientVehicle != nullptr)
			{
				bool bAllow = true;

				if (bAllow)
				{
					if (g_pHumanUseActor_Unk1 == 2)
					{
						g_pClientGame->HumanExitingVehicle(pClientHuman, pClientVehicle, g_pHumanUseActor_Unk2, g_pHumanUseActor_Unk1, g_pHumanUseActor_Unk3);
					}
					else
					{
						g_pClientGame->HumanEnteringVehicle(pClientHuman, pClientVehicle, g_pHumanUseActor_Unk2, g_pHumanUseActor_Unk1, g_pHumanUseActor_Unk3);
					}
				}
				else
				{
					g_bCancelHumanUseActor = true;
				}
			}
			else 
			{
				g_pClientGame->HumanUsingActor(pClientHuman, g_pHumanUseActor_Actor, g_pHumanUseActor_Unk1, g_pHumanUseActor_Unk2, g_pHumanUseActor_Unk3);
			}
		}
	}
}

RAWCODECALL CreateActor()
{
	if (g_pClientGame->m_bCreateActorInvokedByGame)
	{
		//_glogprintf(_gstr("[GAME] CreateActor: %i"), g_pCreateActor_Arg1);

		// 2 = spawned player
		// 4 = spawned vehicle

		if (g_pCreateActor_Arg1 == 11)
		{
			// Human
			//g_pClientGame->CreateGameHuman(g_pCreateActor_Actor, g_pCreateActor_Arg1);
		}
	}
}

RAWCODECALL ModelOpen()
{
	//if (g_pClientGame->m_bModelOpenInvokedByGame)
	{
		//printf("[Model::Open] Frame=0x%X ModelName=%s\n", g_pModelOpen_Arg1, (char*)g_pModelOpen_Arg2);

		g_umapModelNames[(uint32_t)g_pModelOpen_Arg1] = (char*)g_pModelOpen_Arg2;
	}
}

RAWCODECALL CarUpdate()
{
	//if (!g_bTrafficEnabled)
	//	return;

	if (g_pClientGame->m_bCreateVehicleInvokedByGame)
	{
		if (g_pCarUpdate_Car == nullptr)
			return;

		CClientVehicle* pClientVehicle = g_pClientGame->m_pClientManager->FindVehicle(g_pCarUpdate_Car);

		if (pClientVehicle == nullptr)
		{
			//_glogprintf(_gstr("[GAME] Car::Update - Added Vehicle 0x%X Frame 0x%X"), (uint32_t)g_pCarUpdate_Car, (uint32_t)(g_pCarUpdate_Car->GetFrame()));
			//printf("Model Name for Create: %s\n", g_umapModelNames[(uint32_t)(g_pCarCar_Car->GetFrame())].c_str());

			g_pClientGame->OnTrafficCarCreate(g_pCarUpdate_Car);
		}
		else
		{
			CVector3D vecGamePosition;
			pClientVehicle->GetPosition(vecGamePosition);

			float fDistance = vecGamePosition.distance(pClientVehicle->m_vecCachedPositionForTraffic);
			if (fDistance >= 25.0f)
			{
				//_glogprintf(_gstr("[GAME] Car::Update - Respawned Vehicle 0x%X Frame 0x%X %f"), (uint32_t)g_pCarUpdate_Car, (uint32_t)(g_pCarUpdate_Car->GetFrame()), fDistance);
				//printf("Model Name for Respawn: %s\n", g_umapModelNames[(uint32_t)(g_pCarUpdate_Car->GetFrame())].c_str());

				g_pClientGame->OnTrafficCarRespawn(pClientVehicle, g_pCarUpdate_Car);
			}

			pClientVehicle->m_vecCachedPositionForTraffic = vecGamePosition;
		}
	}
}

MafiaSDK::C_Actor* SceneCreateActor(MafiaSDK::C_Mission_Enum::ObjectTypes type, DWORD frame)
{
	using ObjTypes = MafiaSDK::C_Mission_Enum::ObjectTypes;
	std::vector<ObjTypes> forbidden_objects = {
		//ObjTypes::Car,
		//ObjTypes::Dog,
		//ObjTypes::Enemy,
		//ObjTypes::Pumpar,
		//ObjTypes::Player,
		//ObjTypes::GhostObject,
		//ObjTypes::Script,
		//ObjTypes::Bottle,
		//ObjTypes::Clocks,
		//ObjTypes::RaceCamera,
		//ObjTypes::Model,
		//ObjTypes::Truck,
		//ObjTypes::Wagon,
	};

	MafiaSDK::I3D_Frame* frame_ex = (MafiaSDK::I3D_Frame*)frame;

	CArguments Args;
	Args.AddNumber((uint32_t)type);

	CString Name(false, frame_ex->GetInterface()->name);
	Args.AddString(Name); // Frame name

	CString Model(false, g_umapModelNames[(uint32_t)frame_ex].c_str());
	Args.AddString(Model); // Model name

	bool bPreventDefault = false;
	g_pClientGame->m_pOnAddActorEventType->Trigger(Args, bPreventDefault);
	if (bPreventDefault) {
		if (frame_ex)
			frame_ex->SetOn(false);
		return nullptr;
	}

	for (auto forbidden_type : forbidden_objects) {
		if (type == forbidden_type && frame != NULL) {
			if (frame_ex)
				frame_ex->SetOn(false);
			return nullptr;
		}
	}

	//if (type == ObjTypes::Trolley && frame != NULL) {
	//	if (!g_pClientGame->IsGameComponentEnabled(GAMECOMPONENT_TROLLEYS)) {
	//		if (frame_ex)
	//			frame_ex->SetOn(false);
	//		return nullptr;
	//	}
	//}

	//if (type == ObjTypes::Door && frame != NULL) {
	//	if (!g_pClientGame->IsGameComponentEnabled(GAMECOMPONENT_DOORS)) {
	//		if (frame_ex)
	//			frame_ex->SetOn(false);
	//		return nullptr;
	//	}
	//}

	//if (type == ObjTypes::Pumpar && frame != NULL) {
	//	if (!g_pClientGame->IsGameComponentEnabled(GAMECOMPONENT_FUELSTATIONS)) {
	//		if (frame_ex)
	//			frame_ex->SetOn(false);
	//		return nullptr;
	//	}
	//}

	//if (type == ObjTypes::Dog && frame != NULL) {
	//	if (!g_pClientGame->IsGameComponentEnabled(GAMECOMPONENT_DOGS)) {
	//		MafiaSDK::I3D_Frame* frame_ex = (MafiaSDK::I3D_Frame*)frame;
	//		if (frame_ex)
	//			frame_ex->SetOn(false);
	//		return nullptr;
	//	}
	//}

	//if (type == ObjTypes::Plane && frame != NULL) {
	//	if (!g_pClientGame->IsGameComponentEnabled(GAMECOMPONENT_PLANES)) {
	//		if (frame_ex)
	//			frame_ex->SetOn(false);
	//		return nullptr;
	//	}
	//}

	//if (type == ObjTypes::RailRoute && frame != NULL) {
	//	if (!g_pClientGame->IsGameComponentEnabled(GAMECOMPONENT_TRAINS)) {
	//		if (frame_ex)
	//			frame_ex->SetOn(false);
	//		return nullptr;
	//	}
	//}

	//if (type == ObjTypes::InitScript && frame != NULL) {
	//	if (!g_pClientGame->IsGameComponentEnabled(GAMECOMPONENT_SCRIPTS)) {
	//		if (frame_ex)
	//			frame_ex->SetOn(false);
	//		return nullptr;
	//	}
	//}

	//if (type == ObjTypes::Car && frame != NULL) {
	//	if (!g_pClientGame->IsGameComponentEnabled(GAMECOMPONENT_DEFAULTPARKEDCARS)) {
	//		if (frame_ex)
	//			frame_ex->SetOn(false);
	//		return nullptr;
	//	}
	//}

	//if (type == ObjTypes::TrafficSetup && frame != NULL) {
	//	if (!g_pClientGame->IsGameComponentEnabled(GAMECOMPONENT_TRAFFIC)) {
	//		if (frame_ex)
	//			frame_ex->SetOn(false);
	//		return nullptr;
	//	}
	//}

	//if (type == ObjTypes::PedestrianSetup && frame != NULL) {
	//	if (!g_pClientGame->IsGameComponentEnabled(GAMECOMPONENT_CIVILIANS)) {
	//		if (frame_ex)
	//			frame_ex->SetOn(false);
	//		return nullptr;
	//	}
	//}

	// Grab model from frame with g_umapModelNames[(uint32_t)frame_ex]
	MafiaSDK::C_Actor* actor = MafiaSDK::GetMission()->CreateActor(type);

	//auto pClientEntity = Strong<CClientEntity>::New(g_pClientGame->m_pClientManager->Create(ELEMENT_ELEMENT));
	//pClientEntity->m_pEntity = actor;
	//pClientEntity->m_Position = CVecTools::ConvertFromMafiaVec(actor->GetInterface()->entity.position);
	//pClientEntity->m_Rotation = CVecTools::ConvertFromMafiaVec(actor->GetInterface()->entity.rotation);
	//g_pClientGame->m_pClientManager->RegisterNetObject(pClientEntity);
	//{
	//	pClientEntity->GenerateGUID();
	//	pClientEntity->SetSyncer(pMultiplayer->m_NetMachines.GetMachine(pMultiplayer->m_iLocalIndex));
	//
	//	if (pMultiplayer->m_bNetworkedEntities)
	//	{
	//		pMultiplayer->EnqueuePeerElement(pClientEntity);
	//	}
	//}

	//if (g_pClientGame->IsGameComponentEnabled(GAMECOMPONENT_TRAFFIC)) {
	//	if ((type == ObjTypes::Car || type == ObjTypes::Traffic) && frame != NULL) {
	//		g_pClientGame->OnTrafficCarCreate((MafiaSDK::C_Car*)actor);
	//	}
	//}

	return actor;
}

RAWCODECALL HumanSetAimPose()
{
	if (g_pClientGame->m_bHumanSetAimPoseInvokedByGame)
	{
		CClientHuman* pLocalClientHuman = (CClientHuman*)g_pClientGame->m_pClientManager->m_pLocalPlayer.GetPointer();

		if (pLocalClientHuman != nullptr)
		{
			MafiaSDK::C_Human* pLocalGameHuman = pLocalClientHuman->GetGameHuman();

			if (g_pHumanSetAimPose_Human == pLocalGameHuman)
			{
				pLocalClientHuman->m_vecCamera = *g_pvecHumanSetAimPose_Vec;
			}
		}
	}
}

RAWCODECALL HumanSetNormalPose()
{
	if (g_pClientGame->m_bHumanSetNormalPoseInvokedByGame)
	{
		CClientHuman* pLocalClientHuman = (CClientHuman*)g_pClientGame->m_pClientManager->m_pLocalPlayer.GetPointer();

		if (pLocalClientHuman != nullptr)
		{
			MafiaSDK::C_Human* pLocalGameHuman = pLocalClientHuman->GetGameHuman();

			if (g_pHumanSetNormalPose_Human == pLocalGameHuman)
			{
				pLocalClientHuman->m_vecCamera = *g_pvecHumanSetNormalPose_Vec;
			}
		}
	}
}

RAWCODE HookHumanDoThrowCocotFromCar()
{
	_asm
	{
		mov g_pHumanDoThrowCocotFromCar_Human, ecx
		mov eax, [esp + 4]
		mov g_pHumanDoThrowCocotFromCar_Car, eax
		mov eax, [esp + 8]
		mov g_pHumanDoThrowCocotFromCar_SeatID, eax
		pushad
	}

	g_bCancelHumanDoThrowCocotFromCar = false;
	HumanDoThrowCocotFromCar();
	if (g_bCancelHumanDoThrowCocotFromCar)
	{
		_asm
		{
			//popad
			//retn
		}
	}
	_asm
	{
		popad
		push 0xFFFFFFFF
		push 0x6206A2 // 0x587D70
		jmp g_ReturnHumanDoThrowCocotFromCar
	}
}

RAWCODE HookHumanUseActor()
{
	_asm
	{
		mov g_pHumanUseActor_Human, ecx
		mov eax, [esp + 4]
		mov g_pHumanUseActor_Actor, eax
		mov eax, [esp + 8]
		mov g_pHumanUseActor_Unk1, eax
		mov eax, [esp + 0x0C]
		mov g_pHumanUseActor_Unk2, eax
		mov eax, [esp + 0x10]
		mov g_pHumanUseActor_Unk3, eax
		pushad
	}
	g_bCancelHumanUseActor = false;
	HumanUseActor();
	if (g_bCancelHumanUseActor)
	{
		_asm
		{
			//popad
			//retn
		}
	}
	_asm
	{
		popad
		sub esp, 0xF8
		jmp g_ReturnHumanUseActor
	}
}

RAWCODE HookCreateActor()
{
	_asm
	{
		mov eax, [esp + 4]
		mov g_pCreateActor_Arg1, eax
		pushad
	}
	g_bCancelCreateActor = false;
	CreateActor();
	if (g_bCancelCreateActor)
	{
		_asm
		{
			//popad
			//retn
		}
	}
	_asm
	{
		popad
		mov eax, fs:0
		jmp g_ReturnCreateActor
	}
}

RAWCODE HookCarUpdate()
{
	_asm
	{
		mov		g_pCarUpdate_Car, ecx
		pushad
	}
	CarUpdate();
	_asm
	{
		popad
		mov     eax, fs:0
		jmp		g_ReturnCarUpdate
	}
}

// Unfinished
RAWCODE HookSceneCreateActor()
{
	__asm {
		push edi
		push eax
		call SceneCreateActor
		add esp, 0x8
		jmp g_ReturnSceneCreateActor
	}
}

RAWCODE HookModelOpen()
{
	_asm
	{
		mov eax, [esp + 4]
		mov g_pModelOpen_Arg1, eax
		mov eax, [esp + 8]
		mov g_pModelOpen_Arg2, eax
		pushad
	}
	g_bCancelModelOpen = false;
	ModelOpen();
	if (g_bCancelModelOpen)
	{
		_asm
		{
			//popad
			//retn
		}
	}
	__asm {
		popad
		push    ebx
		mov     ebx, [esp + 8]
		jmp		g_ReturnModelOpen
	}
}

RAWCODE HookHumanSetAimPose()
{
	_asm
	{
		mov g_pHumanSetAimPose_Human, ecx
		mov g_pvecHumanSetAimPose_Vec, esp
		add g_pvecHumanSetAimPose_Vec, 4
		pushad
	}
	g_bCancelHumanSetAimPose = false;
	HumanSetAimPose();
	if (g_bCancelHumanSetAimPose)
	{
		_asm
		{
			popad
			retn
		}
	}
	_asm
	{
		popad
		sub		esp, 0xB4
		jmp		g_ReturnHumanSetAimPose
	}
}

RAWCODE HookHumanSetNormalPose()
{
	_asm
	{
		mov g_pHumanSetNormalPose_Human, ecx
		mov g_pvecHumanSetNormalPose_Vec, esp
		add g_pvecHumanSetNormalPose_Vec, 4
		pushad
	}
	g_bCancelHumanSetNormalPose = false;
	HumanSetNormalPose();
	if (g_bCancelHumanSetNormalPose)
	{
		_asm
		{
			popad
			retn
		}
	}
	_asm
	{
		popad
		sub		esp, 0xC0
		jmp		g_ReturnHumanSetNormalPose
	}
}

static void OnGameTick()
{
	g_pClientGame->DestroyUninitializedGameElements();
	TriggerHackEvent(HACKEVENT_PROCESS, nullptr);
}

static void OnLocalPlayerFallDown()
{

}

static int OnHumanHit(MafiaSDK::C_Human* target, int hitType, const S_vector& v1, const S_vector& v2, const S_vector& v3, float damage, MafiaSDK::C_Actor* attacker, unsigned int bodyPart, MafiaSDK::I3D_Frame* targetFrame)
{
	auto humanTarget = g_pClientGame->m_pClientManager->FindHuman(target);

	/*
	auto entityAttacker = nullptr;

	if (attacker->GetInterface()->entity.objectType == MafiaSDK::C_Mission_Enum::ObjectTypes::Enemy || attacker->GetInterface()->entity.objectType == MafiaSDK::C_Mission_Enum::ObjectTypes::Player) {
		entityAttacker = g_pClientGame->m_pClientManager->FindHuman((MafiaSDK::C_Human*)attacker);
	}
	else if (attacker->GetInterface()->entity.objectType == MafiaSDK::C_Mission_Enum::ObjectTypes::Car) 
	{
		entityAttacker = g_pClientGame->m_pClientManager->FindVehicle((MafiaSDK::C_Car*)attacker);
		g_pClientGame->HumanHit(humanTarget, CVecTools::ConvertFromMafiaVec(v1), CVecTools::ConvertFromMafiaVec(v2), CVecTools::ConvertFromMafiaVec(v3), hitType, damage, bodyPart);
	}
	*/
		
	g_pClientGame->HumanHit(humanTarget, CVecTools::ConvertFromMafiaVec(v1), CVecTools::ConvertFromMafiaVec(v2), CVecTools::ConvertFromMafiaVec(v3), hitType, damage, bodyPart);

	return 1;
}

static void OnHumanWeaponChange(MafiaSDK::C_Human* target, int8_t weapon)
{
	auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
	if (pMultiplayer != nullptr)
	{
		auto humanTarget = g_pClientGame->m_pClientManager->FindHuman(target);
		pMultiplayer->SendHumanChangeWeapon(humanTarget, weapon);
	}
}

static void OnHumanWeaponDrop(MafiaSDK::C_Human* target)
{
	_glogprintf(_gstr("Human dropped the weapon"));
	auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
	if (pMultiplayer != nullptr)
	{
		auto humanTarget = g_pClientGame->m_pClientManager->FindHuman(target);
		pMultiplayer->SendHumanDropWeapon(humanTarget);
	}
}

static void OnHumanShoot(const S_vector& pos)
{
	_glogprintf(_gstr("Player shooting to: {%f, %f, %f}"), pos.x, pos.y, pos.z);
	auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
	if (pMultiplayer != nullptr)
	{
		pMultiplayer->SendLocalPlayerShoot(true, CVecTools::ConvertFromMafiaVec(pos));
	}
}

static void OnRender2DStuff()
{
	if (g_pD3D9 && g_p2D != nullptr)
	{
		g_pD3D9->SetAllStatesAgain();

		g_p2D->SetViewport(CVector2D(0, 0), CVector2D((float)MafiaSDK::GetIGraph()->Scrn_sx(), (float)MafiaSDK::GetIGraph()->Scrn_sy()));
		if (g_p2D->Begin2D(g_pD3D9))
		{
			g_pClientGame->m_pGalacticFunctions->m_p2D = &g_pClientGame->m_p2D;
			g_pClientGame->OnRender2DStuff();
			g_p2D->End2D();
		}
	}
}

RAWCODECALL Render2DStuff()
{
	if (g_pD3D9)
	{
		IDirect3DDevice9* pD3DDevice = g_pD3D9->m_pD3DDevice;
		if (pD3DDevice == nullptr)
			return;

		CComPtr<IDirect3DStateBlock9> pStateBlock;

		HRESULT hResult = pD3DDevice->CreateStateBlock(D3DSBT_ALL, &pStateBlock);

		pStateBlock->Capture();

		D3DMATERIAL9 mtrl;
		ZeroMemory(&mtrl, sizeof(mtrl));
		mtrl.Ambient.r = 1.0f;
		mtrl.Ambient.g = 1.0f;
		mtrl.Ambient.b = 1.0f;
		mtrl.Ambient.a = 1.0f;
		mtrl.Diffuse.r = 1.0f;
		mtrl.Diffuse.g = 1.0f;
		mtrl.Diffuse.b = 1.0f;
		mtrl.Diffuse.a = 1.0f;
		pD3DDevice->SetMaterial(&mtrl);

		pD3DDevice->SetPixelShader(nullptr);
		pD3DDevice->SetVertexShader(nullptr);

		OnRender2DStuff();

		pStateBlock->Apply();
	}
}

RAWCODE HookRender2DStuff()
{
	_asm
	{
		pushad
	}
	Render2DStuff();
	_asm
	{
		popad
		xor eax, eax
		ret
	}
}

static int __fastcall SetProgress_Hook(DWORD _this, DWORD edx, float progress)
{
	CLoadScreen::UpdateProgress(progress);
	return progress;
}

static int __fastcall DrawProgress_Hook(DWORD _this, DWORD edx, float progress)
{
	CLoadScreen::Draw(&g_pClientGame->m_p2D);
	return 0;
}

DWORD UpdateProgress_Addr = 0x005DF357;
DWORD DrawProgress_Addr = 0x005DAC70;
DWORD HumanHit_Addr = 0x005762A0;

void CGameHooks::InstallHooks()
{
	// Game Hooks
	MafiaSDK::C_Game_Hooks::HookOnGameInit(OnGameInit);
	MafiaSDK::C_Game_Hooks::HookOnGameTick(OnGameTick);
	MafiaSDK::C_Game_Hooks::HookLocalPlayerFallDown(OnLocalPlayerFallDown);

	// Indicator Hooks
	MafiaSDK::C_Indicators_Hooks::HookAfterAllDraw(HookRender2DStuff);

	// Human Hooks
	MafiaSDK::C_Human_Hooks::HookOnHumanHit(OnHumanHit);
	MafiaSDK::C_Human_Hooks::HookHumanDoWeaponChange(OnHumanWeaponChange);
	MafiaSDK::C_Human_Hooks::HookHumanDoWeaponDrop(OnHumanWeaponDrop);
	MafiaSDK::C_Human_Hooks::HookOnHumanShoot(OnHumanShoot);

	// Remove dropped clip
	new CHackJumpHack(g_pHack, (void*)0x0058D4C6, (void*)0x0058D553, 6);

	// Disable local player weapon drop
	new CHackJumpHack(g_pHack, (void*)0x00585D90, (void*)0x00585DCB, 6);

	// Disable weapon drops
	new CHackJumpHack(g_pHack, (void*)0x0057FAA0, (void*)0x00580196, 6);

	// Game Exit
	new CHackJumpHack(g_pHack, (void*)0x00612485, (void*)OnGameExit_Hook, 6);
	new CHackJumpHack(g_pHack, (void*)0x005A7F44, (void*)0x005A7F4B, 6);

	// Hook UseActor
	new CHackJumpHack(g_pHack, (void*)0x582180, HookHumanUseActor, 6);
	g_ReturnHumanUseActor = (void*)(0x582180 + 6);

	// Hook DoThrowCocotFromCar (jack/steal car)
	new CHackJumpHack(g_pHack, (void*)0x00587D70, HookHumanDoThrowCocotFromCar, 7);
	g_ReturnHumanDoThrowCocotFromCar = (void*)(0x00587D70 + 7);

	// Hook CreateActor
	new CHackJumpHack(g_pHack, (void*)0x0053F7D0, HookCreateActor, 6);
	g_ReturnCreateActor = (void*)(0x0053F7D0 + 6);

	// Note (Sevenisko): Currently unnecessary to work on
	new CHackJumpHack(g_pHack, (void*)UpdateProgress_Addr, &SetProgress_Hook, 6);

	// Fix exiting vehicle for passengers.
	new CHackValueHack(g_pHack, (void*)0x595040, 1, (uint8_t)0xEB);

	// Hook Human::SetAimPose
	new CHackJumpHack(g_pHack, (void*)0x579EA0, HookHumanSetAimPose, 6);
	g_ReturnHumanSetAimPose = (void*)(0x579EA0 + 6);

	// Hook Human::SetNormalPose
	new CHackJumpHack(g_pHack, (void*)0x579630, HookHumanSetNormalPose, 6);
	g_ReturnHumanSetNormalPose = (void*)(0x579630 + 6);

	// Hook SceneCreateActor
	g_ReturnSceneCreateActor = (void*)(0x00544B07);
	new CHackJumpHack(g_pHack, (void*)0x00544AFF, HookSceneCreateActor, 8);

	// Hook Car::Update
	new CHackJumpHack(g_pHack, (void*)0x41FAC0, HookCarUpdate, 6);
	g_ReturnCarUpdate = (void*)(0x41FAC0 + 6);

	// Hook I3D_Model::Open
	new CHackJumpHack(g_pHack, (void*)0x100335A0, HookModelOpen, 5);
	g_ReturnModelOpen = (void*)(0x100335A0 + 5);

	// (From Mex) To fix the fullscreen game being suspended when using alt+tab
	new CHackNOPHack(g_pHack, (void*)0x1006DBF7, 7);
	new CHackNOPHack(g_pHack, (void*)0x1006DD1D, 7);
	new CHackNOPHack(g_pHack, (void*)0x1006DB2B, 7);


	// Fix bug where window border disappears and bugs out the bottom of the window
	//DWORD uiOldProt = 0;
	//DWORD uiOldProt2 = 0;
	//VirtualProtect((void*)0x1006EA60, 4, PAGE_EXECUTE_READWRITE, &uiOldProt);
	//uint32_t ui = 0x90CA0000;
	//new CHackValueHack(g_pHack, (void*)(0x1006EA60), 4, (unsigned char*)&ui);
	//VirtualProtect((void*)0x1006EA60, 4, uiOldProt, &uiOldProt2);
}