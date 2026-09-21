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
	// A new mission means the game already discarded every actor of the old one, so anything still waiting to be
	// removed points at freed memory
	g_pClientGame->m_DeferredVehicleRemovals.clear();

	auto mName = MafiaSDK::GetCurrentMissionName();

	MafiaSDK::GetIGraph()->SetAppName("Mafia Connected");

	// We don't want the game to be initialized in menu
	//if (!strcmp(mName, "00menu")) return;

	if (g_pClientGame->m_bFullReload) {
		g_pClientGame->OnPreStartInGame(false);
		g_pClientGame->OnStartInGame(false);

		auto pMultiplayer = g_pClientGame->GetMultiplayer();
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
	if (g_pClientGame->GetMultiplayer() != nullptr) {
		g_pClientGame->GetMultiplayer()->Disconnect(DISCONNECT_GRACEFUL);
	}
}

// SceneCreateActor
HOOKADDRESS g_ReturnSceneCreateActor;

static void HumanDoThrowCocotFromCar(MafiaSDK::C_Human* pHuman, MafiaSDK::C_Car* pCar, int iSeatID)
{
	if (g_pClientGame->m_bDoThrowCocotFromCarInvokedByGame)
	{
		CClientHuman* pClientHuman = g_pClientGame->m_pClientManager->FindHuman(pHuman);
		if (pClientHuman != nullptr)
		{
			CClientVehicle* pClientVehicle = g_pClientGame->m_pClientManager->FindVehicle(pCar);
			if (pClientVehicle != nullptr)
			{
				g_pClientGame->HumanJackVehicle(pClientHuman, pClientVehicle, iSeatID);
			}
		}
	}
}

// Returns false to stop the game's own Use_Actor from running. For a vehicle that's how an enter/exit the game
// starts is held back until the server has answered it (see HumanEnteringVehicle/HumanExitingVehicle).
static bool HumanUseActor(MafiaSDK::C_Human* pHuman, MafiaSDK::C_Actor* pActor, int iUnk1, int iUnk2, int iUnk3)
{
	// Our own replays (and calls made through the SDK on purpose) go straight through
	if (!g_pClientGame->m_bUseActorInvokedByGame)
		return true;

	CClientHuman* pClientHuman = g_pClientGame->m_pClientManager->FindHuman(pHuman);
	if (pClientHuman == nullptr)
		return true;

	CClientVehicle* pClientVehicle = g_pClientGame->m_pClientManager->FindVehicle((MafiaSDK::C_Car*)pActor);
	if (pClientVehicle == nullptr)
	{
		g_pClientGame->HumanUsingActor(pClientHuman, pActor, iUnk1, iUnk2, iUnk3);
		return true;
	}

	//_glogverboseprintf(_gstr("[GAME] HumanUseActor - Human %d used Vehicle %d with action %d. Extra2: %d, Extra 3: %d\n"), pClientHuman->GetId(), pClientVehicle->GetId(), iUnk1, iUnk2, iUnk3);

	if (iUnk1 == 2)
		return g_pClientGame->HumanExitingVehicle(pClientHuman, pClientVehicle, iUnk2, iUnk1, iUnk3);

	return g_pClientGame->HumanEnteringVehicle(pClientHuman, pClientVehicle, iUnk2, iUnk1, iUnk3);
}

static void CreateActor(MafiaSDK::C_Mission_Enum::ObjectTypes objectType)
{
	if (g_pClientGame->m_bCreateActorInvokedByGame)
	{
		//_glogprintf(_gstr("[GAME] CreateActor: %i"), objectType);

		// 2 = spawned player
		// 4 = spawned vehicle

		if (objectType == 11)
		{
			// Human
			//g_pClientGame->CreateGameHuman(pActor, objectType);
		}
	}
}

static void ModelOpen(uint32_t uiFrame, const char* pszModelName)
{
	//if (g_pClientGame->m_bModelOpenInvokedByGame)
	{
		//printf("[Model::Open] Frame=0x%X ModelName=%s\n", uiFrame, pszModelName);

		g_umapModelNames[uiFrame] = pszModelName;
	}
}

static void CarUpdate(MafiaSDK::C_Car* pCar)
{
	//if (!g_bTrafficEnabled)
	//	return;

	if (g_pClientGame->m_bCreateVehicleInvokedByGame)
	{
		if (pCar == nullptr)
			return;

		CClientVehicle* pClientVehicle = g_pClientGame->m_pClientManager->FindVehicle(pCar);

		if (pClientVehicle == nullptr)
		{
			//_glogprintf(_gstr("[GAME] Car::Update - Added Vehicle 0x%X Frame 0x%X"), (uint32_t)pCar, (uint32_t)(pCar->GetFrame()));
			//_glogverboseprintf(_gstr("[GAME] Car::Update - Added traffic vehicle with model %s\n", g_umapModelNames[(uint32_t)(pCar->GetFrame())].c_str()));

			g_pClientGame->OnTrafficCarCreate(pCar);
		}
		else
		{
			CVector3D vecGamePosition;
			pClientVehicle->GetPosition(vecGamePosition);

			float fDistance = vecGamePosition.distance(pClientVehicle->m_vecCachedPositionForTraffic);
			if (fDistance >= 25.0f)
			{
				//_glogprintf(_gstr("[GAME] Car::Update - Respawned Vehicle 0x%X Frame 0x%X %f"), (uint32_t)pCar, (uint32_t)(pCar->GetFrame()), fDistance);
				//printf("Model Name for Respawn: %s\n", g_umapModelNames[(uint32_t)(pCar->GetFrame())].c_str());
				//_glogverboseprintf(_gstr("[GAME] Car::Update - Respawn traffic vehicle with model %s\n", g_umapModelNames[(uint32_t)(pCar->GetFrame())].c_str()));

				g_pClientGame->OnTrafficCarRespawn(pClientVehicle, pCar);
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

	g_pClientGame->OnActorAdded(type, Name.CString());

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

static void HumanSetAimPose(MafiaSDK::C_Human* pHuman, const S_vector& vecPose)
{
	if (g_pClientGame->m_bHumanSetAimPoseInvokedByGame)
	{
		CClientHuman* pLocalClientHuman = (CClientHuman*)g_pClientGame->m_pClientManager->m_pLocalPlayer.GetPointer();

		if (pLocalClientHuman != nullptr)
		{
			MafiaSDK::C_Human* pLocalGameHuman = pLocalClientHuman->GetGameHuman();

			if (pHuman == pLocalGameHuman)
			{
				pLocalClientHuman->m_vecCamera = CVecTools::ConvertFromMafiaVec(vecPose);
			}
		}
	}
}

static void HumanSetNormalPose(MafiaSDK::C_Human* pHuman, const S_vector& vecPose)
{
	if (g_pClientGame->m_bHumanSetNormalPoseInvokedByGame)
	{
		CClientHuman* pLocalClientHuman = (CClientHuman*)g_pClientGame->m_pClientManager->m_pLocalPlayer.GetPointer();

		if (pLocalClientHuman != nullptr)
		{
			MafiaSDK::C_Human* pLocalGameHuman = pLocalClientHuman->GetGameHuman();

			if (pHuman == pLocalGameHuman)
			{
				pLocalClientHuman->m_vecCamera = CVecTools::ConvertFromMafiaVec(vecPose);
			}
		}
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
	auto pMultiplayer = g_pClientGame->GetMultiplayer();
	if (pMultiplayer != nullptr)
	{
		auto humanTarget = g_pClientGame->m_pClientManager->FindHuman(target);
		pMultiplayer->SendHumanChangeWeapon(humanTarget, weapon);
	}
}

static void OnHumanWeaponDrop(MafiaSDK::C_Human* target)
{
	_glogprintf(_gstr("Human dropped the weapon"));
	auto pMultiplayer = g_pClientGame->GetMultiplayer();
	if (pMultiplayer != nullptr)
	{
		auto humanTarget = g_pClientGame->m_pClientManager->FindHuman(target);
		pMultiplayer->SendHumanDropWeapon(humanTarget);
	}
}

static void OnHumanShoot(const S_vector& pos)
{
	_glogprintf(_gstr("Player shooting to: {%f, %f, %f}"), pos.x, pos.y, pos.z);
	auto pMultiplayer = g_pClientGame->GetMultiplayer();
	if (pMultiplayer != nullptr)
	{
		pMultiplayer->SendLocalPlayerShoot(true, CVecTools::ConvertFromMafiaVec(pos));
	}
}

static void OnHumanThrowGrenade(MafiaSDK::C_Human* human, const S_vector& pos)
{
	auto pMultiplayer = g_pClientGame->GetMultiplayer();
	if (pMultiplayer == nullptr)
		return;

	// Only the local player's throws are ours to send; remote ones are replayed from the network
	auto pClientHuman = g_pClientGame->m_pClientManager->FindHuman(human);
	if (pClientHuman == nullptr || pClientHuman != g_pClientGame->m_pClientManager->m_pLocalPlayer.GetPointer())
		return;

	pMultiplayer->SendLocalPlayerThrowGrenade(CVecTools::ConvertFromMafiaVec(pos));
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
	MafiaSDK::C_Human_Hooks::HookHumanThrowGrenade(OnHumanThrowGrenade);
	MafiaSDK::C_Human_Hooks::HookUseActor(HumanUseActor);
	MafiaSDK::C_Human_Hooks::HookDoThrowCocotFromCar(HumanDoThrowCocotFromCar);
	MafiaSDK::C_Human_Hooks::HookHumanSetAimPose(HumanSetAimPose);
	MafiaSDK::C_Human_Hooks::HookHumanSetNormalPose(HumanSetNormalPose);

	// Mission Hooks
	MafiaSDK::C_Mission_Hooks::HookCreateActor(CreateActor);

	// Car Hooks
	MafiaSDK::C_Car_Hooks::HookOnUpdate(CarUpdate);

	// Engine Hooks
	MafiaSDK::I3D_Model_Hooks::HookOnOpen(ModelOpen);

	// Remove dropped clip
	new CHackJumpHack(g_pHack, (void*)MafiaSDK::C_Human_Enum::FunctionsAddresses::RemoveDroppedClipPatch, (void*)MafiaSDK::C_Human_Enum::FunctionsAddresses::RemoveDroppedClipPatchSkip, 6);

	// Disable local player weapon drop
	new CHackJumpHack(g_pHack, (void*)MafiaSDK::C_Human_Enum::FunctionsAddresses::Do_WeaponDrop, (void*)MafiaSDK::C_Human_Enum::FunctionsAddresses::Do_WeaponDropPatchSkip, 6);

	// Disable weapon drops
	new CHackJumpHack(g_pHack, (void*)MafiaSDK::C_Human_Enum::FunctionsAddresses::DisableWeaponDropsPatch, (void*)MafiaSDK::C_Human_Enum::FunctionsAddresses::DisableWeaponDropsPatchSkip, 6);

	// Game Exit
	MafiaSDK::C_Game_Hooks::HookOnGameExit(OnGameExit);

	// Note (Sevenisko): Currently unnecessary to work on
	new CHackJumpHack(g_pHack, (void*)UpdateProgress_Addr, &SetProgress_Hook, 6);

	// Fix exiting vehicle for passengers.
	new CHackValueHack(g_pHack, (void*)MafiaSDK::C_Human_Enum::FunctionsAddresses::ExitingVehiclePassengerFix, 1, (uint8_t)0xEB);

	// Hook SceneCreateActor
	g_ReturnSceneCreateActor = (void*)MafiaSDK::C_Mission_Enum::FunctionAddresses::SceneCreateActorReturn;
	new CHackJumpHack(g_pHack, (void*)MafiaSDK::C_Mission_Enum::FunctionAddresses::SceneCreateActor, HookSceneCreateActor, 8);

	// (From Mex) To fix the fullscreen game being suspended when using alt+tab
	new CHackNOPHack(g_pHack, (void*)MafiaSDK::C_Game_Patches::AltTabSuspendFix1, 7);
	new CHackNOPHack(g_pHack, (void*)MafiaSDK::C_Game_Patches::AltTabSuspendFix2, 7);
	new CHackNOPHack(g_pHack, (void*)MafiaSDK::C_Game_Patches::AltTabSuspendFix3, 7);


	// Fix bug where window border disappears and bugs out the bottom of the window
	//DWORD uiOldProt = 0;
	//DWORD uiOldProt2 = 0;
	//VirtualProtect((void*)0x1006EA60, 4, PAGE_EXECUTE_READWRITE, &uiOldProt);
	//uint32_t ui = 0x90CA0000;
	//new CHackValueHack(g_pHack, (void*)(0x1006EA60), 4, (unsigned char*)&ui);
	//VirtualProtect((void*)0x1006EA60, 4, uiOldProt, &uiOldProt2);
}