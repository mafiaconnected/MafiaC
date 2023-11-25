#include "stdafx.h"
#include "ScriptingFunctions.h"
#include "../ClientManager.h"
#include "../ClientGame.h"
#include "../Elements/ClientHuman.h"
#include "../Elements/ClientPlayer.h"
#include "../Elements/ClientVehicle.h"
#include <Utils/VectorTools.h>

static bool FunctionGameCreateDummyElement(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CVector3D pos = { 0, 0, 0 };
	if (!pState->CheckVector3D(0, pos))
		return false;

	CClientDummy* pClientDummy = reinterpret_cast<CClientDummy*>(pClientManager->Create(ELEMENT_DUMMY));
	pClientDummy->SetPosition(pos);
	pClientDummy->m_pResource = pState->GetResource();
	pClientManager->RegisterNetObject(pClientDummy);
	pState->ReturnObject(pClientDummy);

	return true;
}

static bool FunctionGameCreateObject(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CVector3D pos = { 0, 0, 0 };
	if (!pState->CheckVector3D(0, pos))
		return false;

	CVector3D rot = { 0, 0, 0 };
	if (!pState->CheckVector3D(1, rot))
		return false;

	CClientObject* pClientObject = reinterpret_cast<CClientObject*>(pClientManager->Create(ELEMENT_OBJECT));
	pClientObject->SetPosition(pos);
	pClientObject->SetRotation(rot);
	pClientObject->m_pResource = pState->GetResource();
	pClientManager->RegisterNetObject(pClientObject);
	pState->ReturnObject(pClientObject);

	return true;
}

static bool FunctionGameCreatePed(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	const GChar* mdl = pState->CheckString(0);
	if (!mdl) return false;

	CVector3D pos = { 0, 0, 0 };
	if (!pState->CheckVector3D(1, pos))
		return false;

	float angle = 0;
	if (!pState->CheckNumber(2, angle))
		return false;

	CClientHuman* pClientHuman = reinterpret_cast<CClientHuman*>(pClientManager->Create(ELEMENT_PED));
	pClientHuman->SetModel(mdl);
	pClientHuman->Spawn(pos, angle, false);
	pClientHuman->m_pResource = pState->GetResource();
	pClientManager->RegisterNetObject(pClientHuman);
	pState->ReturnObject(pClientHuman);

	return true;
}

static bool FunctionGameCreatePlayer(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CVector3D pos = { 0, 0, 0 };
	if (!pState->CheckVector3D(0, pos))
		return false;

	float angle = 0;
	if (!pState->CheckNumber(1, angle))
		return false;

	const GChar* mdl = pState->CheckString(2);
	if (!mdl) return false;

	CClientPlayer* pClientPlayer = reinterpret_cast<CClientPlayer*>(pClientManager->Create(ELEMENT_PLAYER));
	pClientPlayer->SetModel(mdl);
	//pClientPlayer->SetPosition(pos);
	//pClientPlayer->SetHeading(angle);
	pClientPlayer->Spawn(pos, angle, true);
	pClientManager->SetLocalPlayer(pClientPlayer);
	pClientPlayer->m_pResource = pState->GetResource();
	pClientManager->RegisterNetObject(pClientPlayer);
	pState->ReturnObject(pClientPlayer);

	return true;
}

static bool FunctionGameCreateVehicle(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	const GChar* mdl = pState->CheckString(0);
	if (!mdl) return false;

	CVector3D pos = { 0, 0, 0 };
	if (!pState->CheckVector3D(1, pos))
		return false;

	float angle = 0;
	if (!pState->CheckNumber(2, angle))
		return false;

	CClientVehicle* pClientVehicle = reinterpret_cast<CClientVehicle*>(pClientManager->Create(ELEMENT_VEHICLE));
	CVector3D rot = CVecTools::ComputeDirEuler(CVecTools::RadToDeg(angle));
	pClientVehicle->Create(mdl, pos, rot);
	pClientVehicle->m_pResource = pState->GetResource();
	pClientManager->RegisterNetObject(pClientVehicle);
	pClientVehicle->Release();

	pState->ReturnObject(pClientVehicle);
	return true;
}

static bool FunctionGameChangeMap(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	const GChar* mapName = pState->CheckString(0);
	if (!mapName) return false;
	UTF8String mapName2(true, mapName);

	bool bFullReload = true;
	if (!pState->CheckBoolean(1, bFullReload))
		return false;

	g_pClientGame->m_bFullReload = bFullReload;

	// Note (Sevenisko): had to use another func, because PatchJumpToGame works only on game load
	MafiaSDK::GetMission()->MapLoad(mapName2);

	return true;
}

static bool FunctionGameMessage(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	const GChar* msg = pState->CheckString(0);
	if (!msg) return false;
	UTF8String message(true, msg);

	uint32_t color = 0xFFFFFFFF;
	if (!pState->CheckNumber(1, color))
		return false;

	MafiaSDK::GetIndicators()->ConsoleAddText(message.CString(), color);
	return true;
}

static bool FunctionGameFadeScreen(IScriptState* pState, int argc, void* pUser)
{
	bool fadeIn = false;
	if (!pState->CheckBoolean(0, fadeIn))
		return false;

	int32_t time = 250;

	if (!pState->CheckNumber(1, time))
		return false;

	uint32_t color = 0xFFFFFFFF;

	if (!pState->CheckNumber(2, color))
		return false;

	MafiaSDK::GetIndicators()->FadeInOutScreen(fadeIn, time, color);
	return true;
}

static bool FunctionGameEnableMap(IScriptState* pState, int argc, void* pUser)
{
	bool state = true;
	if (!pState->CheckBoolean(0, state))
		return false;

	MafiaSDK::GetIndicators()->MapEnable(state);
	return true;
}

static bool FunctionGameSetTrafficEnabled(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	bool state = true;
	if (!pState->CheckBoolean(0, state))
		return false;

	g_pClientGame->m_bLocalTrafficEnabled = state;
	MafiaSDK::GetMission()->GetGame()->SetTrafficVisible(state);
	return true;
}

static bool FunctionGameAnnounce(IScriptState* pState, int argc, void* pUser)
{
	const GChar* msg = pState->CheckString(0);
	if (!msg) return false;
	UTF8String message(true, msg);

	float time = 1.5f;
	if (!pState->CheckNumber(1, time))
		return false;
	
	MafiaSDK::GetIndicators()->RaceFlashText(message.CString(), time);
	return true;
}

static bool FunctionGameRenderText(IScriptState* pState, int argc, void* pUser)
{
	const GChar* msg = pState->CheckString(0);
	if (!msg) return false;
	UTF8String message(true, msg);

	CVector2D vecPos;
	if (!pState->CheckVector2D(1, vecPos))
		return false;

	CVector2D vecSize;
	if (!pState->CheckVector2D(2, vecSize))
		return false;

	int32_t iColour;
	if (!pState->CheckNumber(3, iColour))
		return false;

	bool bUnderlined;
	if (!pState->CheckBoolean(4, bUnderlined))
		return false;

	int32_t iFontStyle;
	if (!pState->CheckNumber(3, iFontStyle))
		return false;

	MafiaSDK::GetIndicators()->OutText(message, vecPos.x, vecPos.y, vecSize.x, vecSize.y, iColour, (int)bUnderlined, iFontStyle);
	return true;
}

static bool FunctionGameCreateSound(IScriptState* pState, int argc, void* pUser)
{
	const GChar* msg = pState->CheckString(0);
	if (!msg) return false;
	UTF8String message(true, msg);

	int32_t iSoundID = MafiaSDK::GetMission()->GetGame()->CreateStream(message.CString());

	pState->ReturnNumber(iSoundID);
	return true;
}

static bool FunctionGamePlaySound(IScriptState* pState, int argc, void* pUser)
{
	int32_t iSoundID = 0;
	if (!pState->CheckNumber(0, iSoundID))
		return false;

	MafiaSDK::GetMission()->GetGame()->PlayStream(iSoundID);
	return true;
}

static bool FunctionGamePauseSound(IScriptState* pState, int argc, void* pUser)
{
	int32_t iSoundID = 0;
	if (!pState->CheckNumber(0, iSoundID))
		return false;

	MafiaSDK::GetMission()->GetGame()->PauseStream(iSoundID);
	return true;
}

static bool FunctionGameDestroySound(IScriptState* pState, int argc, void* pUser)
{
	int32_t iSoundID = 0;
	if (!pState->CheckNumber(0, iSoundID))
		return false;

	MafiaSDK::GetMission()->GetGame()->DestroyStream(iSoundID);
	return true;
}

static bool FunctionGameShowCountdown(IScriptState* pState, int argc, void* pUser)
{
	int8_t flag = 0;
	if (!pState->CheckNumber(0, flag))
		return false;

	MafiaSDK::GetIndicators()->RaceSetStartFlag(flag);
	return true;
}

static bool FunctionGameGetMapName(IScriptState* pState, int argc, void* pUser)
{
	CString Name(false, MafiaSDK::GetCurrentMissionName());
	pState->ReturnString(Name);
	return true;
}

static bool FunctionGameCreateExplosion(IScriptState* pState, int argc, void* pUser)
{
	CVector3D pos = { 0, 0, 0 };
	if (!pState->CheckVector3D(0, pos))
		return false;

	float radius = 0;
	if (!pState->CheckNumber(1, radius))
		return false;

	float force = 0;
	if (!pState->CheckNumber(2, force))
		return false;

	MafiaSDK::GetMission()->GetGame()->NewExplosion(NULL, CVecTools::ConvertToMafiaVec(pos), radius, force, true, true, true, 1);
	return true;
}

static bool FunctionGameSetLocalPlayer(IScriptState* pState, int argc, void* pUser)
{
 	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientPlayer* pClientPlayer;
	if (!pState->CheckClass(pClientManager->m_pClientPlayerClass, 0, false, &pClientPlayer))
		return false;

	MafiaSDK::GetMission()->GetGame()->GetCamera()->SetCar(NULL);
	MafiaSDK::GetMission()->GetGame()->GetCamera()->SetMode(true, 1);
	MafiaSDK::GetMission()->GetGame()->GetCamera()->SetPlayer(pClientPlayer->GetGamePlayer());
	MafiaSDK::GetMission()->GetGame()->SetLocalPlayer(pClientPlayer->GetGamePlayer());
	return true;
}

static bool FunctionGetGame(IScriptState* pState, int argc, void* pUser)
{
	CClientGame* pClientGame = (CClientGame*)pUser;
	pState->ReturnNumber(GAME_MAFIA_ONE);
	return true;
}

static bool FunctionGetWidth(IScriptState* pState, int argc, void* pUser)
{
	pState->ReturnNumber(MafiaSDK::GetIGraph()->Scrn_sx());
	return true;
}

static bool FunctionGetHeight(IScriptState* pState, int argc, void* pUser)
{
	pState->ReturnNumber(MafiaSDK::GetIGraph()->Scrn_sy());
	return true;
}

static bool FunctionGetAspectRatio(IScriptState* pState, int argc, void* pUser)
{
	pState->ReturnNumber(MafiaSDK::GetIGraph()->Scrn_sx() / MafiaSDK::GetIGraph()->Scrn_sy());
	return true;
}

static bool FunctionSetPlayerControl(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	bool bEnabled;
	if (!pState->CheckBoolean(0, bEnabled))
		return false;

	g_pClientGame->LockControls(!bEnabled);
	return true;
}

static bool FunctionGameSetMoney(IScriptState* pState, int argc, void* pUser)
{
	int uiScore = 0;
	if (!pState->CheckNumber(0, uiScore))
		return false;

	MafiaSDK::GetMission()->GetGame()->ScoreSet(uiScore);
	return true;
}

static bool FunctionGameEnableMoney(IScriptState* pState, int argc, void* pUser)
{
	bool bEnabled = false;
	if (!pState->CheckBoolean(0, bEnabled))
		return false;

	MafiaSDK::GetMission()->GetGame()->ScoreOn(bEnabled);
	return true;
}

static bool FunctionGameSetDoorState(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	const GChar* name = pState->CheckString(0);
	if (!name) return false;
	UTF8String name2(true, name);

	uint32_t iState1 = false;
	if (!pState->CheckNumber(1, iState1))
		return false;

	bool bState2 = false;
	if (!pState->CheckBoolean(2, bState2))
		return false;

	bool bState3 = false;
	if (!pState->CheckBoolean(3, bState3))
		return false;

	MafiaSDK::C_Door* actor = (MafiaSDK::C_Door*)MafiaSDK::GetMission()->FindActorByName(name2);
	actor->SetState((MafiaSDK::C_Door_Enum::States)iState1, MafiaSDK::GetMission()->GetGame()->GetLocalPlayer(), bState2, bState3);
	return true;
}

static bool FunctionGameGetDoorState(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	const GChar* name = pState->CheckString(0);
	if (!name) return false;
	UTF8String name2(true, name);

	MafiaSDK::C_Door* actor = (MafiaSDK::C_Door*)MafiaSDK::GetMission()->FindActorByName(name2);
	pState->ReturnNumber(actor->GetInterface()->current_state);
	return true;
}

static bool FunctionGameGetDoorOpenAngle(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	const GChar* name = pState->CheckString(0);
	if (!name) return false;
	UTF8String name2(true, name);

	MafiaSDK::C_Door* actor = (MafiaSDK::C_Door*)MafiaSDK::GetMission()->FindActorByName(name2);
	pState->ReturnNumber(actor->GetInterface()->open_angle);
	return true;
}

static bool FunctionGameGetDoorOpenDirection(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	const GChar* name = pState->CheckString(0);
	if (!name) return false;
	UTF8String name2(true, name);

	MafiaSDK::C_Door* actor = (MafiaSDK::C_Door*)MafiaSDK::GetMission()->FindActorByName(name2);
	pState->ReturnNumber(actor->GetInterface()->open_direction);
	return true;
}

static bool FunctionGameSetDoorOpenAngle(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	const GChar* name = pState->CheckString(0);
	if (!name) return false;
	UTF8String name2(true, name);

	float fAngle = false;
	if (!pState->CheckNumber(1, fAngle))
		return false;

	MafiaSDK::C_Door* actor = (MafiaSDK::C_Door*)MafiaSDK::GetMission()->FindActorByName(name2);
	actor->SetOpenAngle(fAngle);
	return true;
}

static bool FunctionGameSetActorState(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	const GChar* name = pState->CheckString(0);
	if (!name) return false;
	UTF8String name2(true, name);

	bool bActive = false;
	if (!pState->CheckBoolean(1, bActive))
		return false;

	MafiaSDK::GetMission()->FindActorByName(name2)->SetActive(bActive);
	return true;
}

static bool FunctionGameSetActorActState(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	const GChar* name = pState->CheckString(0);
	if (!name) return false;
	UTF8String name2(true, name);

	uint32_t iState = false;
	if (!pState->CheckNumber(1, iState))
		return false;

	MafiaSDK::GetMission()->FindActorByName(name2)->SetActState(iState);
	return true;
}

static bool FunctionGameUnloadActor(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	const GChar* name = pState->CheckString(0);
	if (!name) return false;
	UTF8String name2(true, name);

	MafiaSDK::C_Actor* actor = MafiaSDK::GetMission()->FindActorByName(name2);
	MafiaSDK::GetMission()->UnloadActor(actor);
	return true;
}

static bool FunctionGameSetVehicleTachometerValues(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	float fUnknown1;
	if (!pState->CheckNumber(1, fUnknown1))
		return false;

	float fUnknown2;
	if (!pState->CheckNumber(2, fUnknown2))
		return false;

	float fUnknown3;
	if (!pState->CheckNumber(3, fUnknown3))
		return false;

	float fUnknown4;
	if (!pState->CheckNumber(4, fUnknown4))
		return false;

	MafiaSDK::GetIndicators()->TachometrSetValues(fUnknown1, fUnknown2, fUnknown3, fUnknown4);
	return true;
}

static bool FunctionGameAddVehicleToRadar(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;
	if (!pState->CheckClass(pClientManager->m_pClientVehicleClass, 0, false, &pClientVehicle))
		return false;

	uint32_t iColour = 0;
	if (!pState->CheckNumber(1, iColour))
		return false;

	MafiaSDK::GetIndicators()->RadarAddCar(pClientVehicle->GetGameVehicle(), iColour);
	return true;
}

static bool FunctionGameRemoveVehicleFromRadar(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;
	if (!pState->CheckClass(pClientManager->m_pClientVehicleClass, 0, false, &pClientVehicle))
		return false;

	uint32_t iColour = 0;
	if (!pState->CheckNumber(1, iColour))
		return false;

	MafiaSDK::GetIndicators()->RadarRemoveCar(pClientVehicle->GetGameVehicle());
	return true;
}

static bool FunctionGameSetPlayerVehicleOnRadar(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;
	if (!pState->CheckClass(pClientManager->m_pClientVehicleClass, 0, false, &pClientVehicle))
		return false;

	uint32_t iColour = 0;
	if (!pState->CheckNumber(1, iColour))
		return false;

	MafiaSDK::GetIndicators()->RadarSetPlayerCar(MafiaSDK::GetMission()->GetGame()->GetLocalPlayer());
	return true;
}

static bool FunctionGameSetActorAI(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	const GChar* name = pState->CheckString(0);
	if (!name) return false;
	UTF8String name2(true, name);

	uint32_t iUnknown1 = false;
	if (!pState->CheckNumber(1, iUnknown1))
		return false;

	uint32_t iUnknown2 = false;
	if (!pState->CheckNumber(2, iUnknown2))
		return false;

	uint32_t iUnknown3 = false;
	if (!pState->CheckNumber(3, iUnknown3))
		return false;

	uint32_t iUnknown4 = false;
	if (!pState->CheckNumber(4, iUnknown4))
		return false;

	MafiaSDK::GetMission()->FindActorByName(name2)->ForceAI(iUnknown1, iUnknown2, iUnknown3, iUnknown4);
	return true;
}

/*
static bool FunctionAudioSetWorldPosition(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	Galactic3D::Audio::CSound* pAudioObject;

	if (!pState->GetThis(Galactic3D::Audio::CSound, &pAudioObject))
		return false;

	CVector3D pos = { 0, 0, 0 };
	if (!pState->CheckVector3D(0, pos))
		return false;

	CVector3D orient = { 0, 0, 0 };
	if (!pState->CheckVector3D(1, orient))
		return false;

	CVector3D vel = { 0, 0, 0 };
	if (!pState->CheckVector3D(2, vel))
		return false;

	BASS_ChannelSet3DPosition(pAudioObject->m_pAudioInstance, pos, orient, vel);
	return true;
}
*/

void CScriptingFunctions::RegisterGameDefines(Galactic3D::CDefineHandlers* pDefineHandlers)
{
	pDefineHandlers->Define(_gstr("NONE"), 0);

	pDefineHandlers->Define(_gstr("MAX_PEDS"), MAX_PEDS);
	pDefineHandlers->Define(_gstr("MAX_VEHICLES"), MAX_VEHICLES);

	pDefineHandlers->Define(_gstr("ELEMENT_ELEMENT"), ELEMENT_ELEMENT);
	pDefineHandlers->Define(_gstr("ELEMENT_ENTITY"), ELEMENT_ENTITY);
	pDefineHandlers->Define(_gstr("ELEMENT_PED"), ELEMENT_PED);
	pDefineHandlers->Define(_gstr("ELEMENT_PLAYER"), ELEMENT_PLAYER);
	pDefineHandlers->Define(_gstr("ELEMENT_VEHICLE"), ELEMENT_VEHICLE);
	pDefineHandlers->Define(_gstr("ELEMENT_DUMMY"), ELEMENT_DUMMY);
	pDefineHandlers->Define(_gstr("ELEMENT_OBJECT"), ELEMENT_OBJECT);

	pDefineHandlers->Define(_gstr("GAME_UNKNOWN"), GAME_UNKNOWN);
	pDefineHandlers->Define(_gstr("GAME_MAFIA_ONE"), GAME_MAFIA_ONE);
	pDefineHandlers->Define(_gstr("GAME_MAFIA_TWO"), GAME_MAFIA_TWO);
	pDefineHandlers->Define(_gstr("GAME_MAFIA_THREE"), GAME_MAFIA_THREE);
	pDefineHandlers->Define(_gstr("GAME_MAFIA_ONE_DE"), GAME_MAFIA_ONE_DE);

	// Script Compatibility
	pDefineHandlers->Define(_gstr("GAME_GTA_III"), 1);
	pDefineHandlers->Define(_gstr("GAME_GTA_VC"), 2);
	pDefineHandlers->Define(_gstr("GAME_GTA_SA"), 3);
	pDefineHandlers->Define(_gstr("GAME_GTA_IV"), 5);

	pDefineHandlers->Define(_gstr("WEAPON_KNUCKLEDUSTER"), 2);
	pDefineHandlers->Define(_gstr("WEAPON_KNIFE"), 3);
	pDefineHandlers->Define(_gstr("WEAPON_BASEBALLBAT"), 4);
	pDefineHandlers->Define(_gstr("WEAPON_MOLOTOV"), 5);
	pDefineHandlers->Define(_gstr("WEAPON_COLTDETECTIVESPECIAL"), 6);
	pDefineHandlers->Define(_gstr("WEAPON_SW_MAGNUM"), 7);
	pDefineHandlers->Define(_gstr("WEAPON_SW_MP"), 8);
	pDefineHandlers->Define(_gstr("WEAPON_COLT1911"), 9);
	pDefineHandlers->Define(_gstr("WEAPON_TOMMYGUN"), 10);
	pDefineHandlers->Define(_gstr("WEAPON_PUMPSHOTGUN"), 11);
	pDefineHandlers->Define(_gstr("WEAPON_SAWEDOFF"), 12);
	pDefineHandlers->Define(_gstr("WEAPON_SPRINGFIELD"), 13);
	pDefineHandlers->Define(_gstr("WEAPON_MOSINNAGANT"), 13);
	pDefineHandlers->Define(_gstr("WEAPON_GRENADE"), 15);
	pDefineHandlers->Define(_gstr("WEAPON_BUCKET"), 17);
	pDefineHandlers->Define(_gstr("WEAPON_STEELBAR"), 20);
	pDefineHandlers->Define(_gstr("WEAPON_CROWBAR"), 25);
	pDefineHandlers->Define(_gstr("WEAPON_WOODENPLANK"), 28);
	pDefineHandlers->Define(_gstr("WEAPON_BOTTLE"), 29);
	pDefineHandlers->Define(_gstr("WEAPON_SWORD"), 31);
	pDefineHandlers->Define(_gstr("WEAPON_DOGSHEAD"), 32);

	pDefineHandlers->Define(_gstr("PEDBEHAVIOR_NOREACT"), MafiaSDK::C_Human_Enum::BehaviorStates::DoesntReactOnWeapon);
	pDefineHandlers->Define(_gstr("PEDBEHAVIOR_NOREACT_GUARDPLAYER"), MafiaSDK::C_Human_Enum::BehaviorStates::DoesntReactGuard);
	pDefineHandlers->Define(_gstr("PEDBEHAVIOR_REACTALL"), MafiaSDK::C_Human_Enum::BehaviorStates::ReactsAll);
	pDefineHandlers->Define(_gstr("PEDBEHAVIOR_REACTTOATTACKS_ATTACK"), MafiaSDK::C_Human_Enum::BehaviorStates::ReactsOnAttacksAttack);
	pDefineHandlers->Define(_gstr("PEDBEHAVIOR_REACTTOATTACKS_AWAYORATTACK"), MafiaSDK::C_Human_Enum::BehaviorStates::ReactsOnAttacksAwayOrAttack);
	pDefineHandlers->Define(_gstr("PEDBEHAVIOR_REACTTOPLAYER_AWAY"), MafiaSDK::C_Human_Enum::BehaviorStates::ReactsPlayerMovesAway);
	pDefineHandlers->Define(_gstr("PEDBEHAVIOR_REACTTOPLAYERHIT_ATTACK"), MafiaSDK::C_Human_Enum::BehaviorStates::ReactsPlayerOnHit);
}

void CScriptingFunctions::RegisterGameFunctions(Galactic3D::CScripting* pScripting, CClientGame* pClientGame)
{
	auto pGameNamespace = pScripting->m_Global.GetNamespace(_gstr("mafia"));
	auto pClientManager = pClientGame->m_pClientManager;

	pGameNamespace->AddProperty(pClientManager, _gstr("mapName"), ARGUMENT_STRING, FunctionGameGetMapName);

	{
		pGameNamespace->AddProperty(pClientGame, _gstr("game"), ARGUMENT_INTEGER, FunctionGetGame);
		pGameNamespace->AddProperty(pClientGame, _gstr("width"), ARGUMENT_INTEGER, FunctionGetWidth);
		pGameNamespace->AddProperty(pClientGame, _gstr("height"), ARGUMENT_INTEGER, FunctionGetHeight);
		pGameNamespace->AddProperty(pClientGame, _gstr("aspectRatio"), ARGUMENT_FLOAT, FunctionGetAspectRatio);
	}

	{
		pClientManager->m_pClientHumanClass->RegisterConstructor(_gstr("tsvf"), FunctionGameCreatePed, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("createPed"), _gstr("svf"), FunctionGameCreatePed, pClientManager);

		pClientManager->m_pClientVehicleClass->RegisterConstructor(_gstr("tsvf"), FunctionGameCreateVehicle, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("createVehicle"), _gstr("svf"), FunctionGameCreateVehicle, pClientManager);

		pClientManager->m_pClientDummyClass->RegisterConstructor(_gstr("tv"), FunctionGameCreateDummyElement, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("createDummyElement"), _gstr("v"), FunctionGameCreateDummyElement, pClientManager);

		pClientManager->m_pClientDummyClass->RegisterConstructor(_gstr("tv"), FunctionGameCreateObject, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("createObject"), _gstr("v"), FunctionGameCreateObject, pClientManager);
	}

	{
		pGameNamespace->RegisterFunction(_gstr("createExplosion"), _gstr("vff"), FunctionGameCreateExplosion, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("fadeCamera"), _gstr("bf|i"), FunctionGameFadeScreen, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("setPlayerControl"), _gstr("b"), FunctionSetPlayerControl, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("createSound"), _gstr("s"), FunctionGameCreateSound, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("playSound"), _gstr("i"), FunctionGamePlaySound, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("pauseSound"), _gstr("i"), FunctionGamePauseSound, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("destroySound"), _gstr("i"), FunctionGameDestroySound, pClientManager);
	}

	{
		auto pHUDNamespace = pGameNamespace->AddNamespace(_gstr("hud"));
		pHUDNamespace->RegisterFunction(_gstr("message"), _gstr("si"), FunctionGameMessage, pClientManager);
		pHUDNamespace->RegisterFunction(_gstr("enableMap"), _gstr("b"), FunctionGameEnableMap, pClientManager);
		pHUDNamespace->RegisterFunction(_gstr("announce"), _gstr("sf"), FunctionGameAnnounce, pClientManager);
		pHUDNamespace->RegisterFunction(_gstr("renderText"), _gstr("szzibi"), FunctionGameRenderText, pClientManager);
		pHUDNamespace->RegisterFunction(_gstr("showCountdown"), _gstr("i"), FunctionGameShowCountdown, pClientManager);
		pHUDNamespace->RegisterFunction(_gstr("setMoney"), _gstr("i"), FunctionGameSetMoney, pClientManager);
		pHUDNamespace->RegisterFunction(_gstr("enableMoney"), _gstr("b"), FunctionGameEnableMoney, pClientManager);
		pHUDNamespace->RegisterFunction(_gstr("setVehicleTachometerValues"), _gstr("ffff"), FunctionGameSetVehicleTachometerValues, pClientManager);
		pHUDNamespace->RegisterFunction(_gstr("addVehicleToRadar"), _gstr("xi"), FunctionGameAddVehicleToRadar, pClientManager);
		pHUDNamespace->RegisterFunction(_gstr("removeVehicleFromRadar"), _gstr("x"), FunctionGameRemoveVehicleFromRadar, pClientManager);
		pHUDNamespace->RegisterFunction(_gstr("setPlayerVehicleOnRadar"), _gstr(""), FunctionGameSetPlayerVehicleOnRadar, pClientManager);
	}

	{
		pGameNamespace->RegisterFunction(_gstr("getDoorState"), _gstr("s"), FunctionGameGetDoorState, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("getDoorOpenAngle"), _gstr("s"), FunctionGameGetDoorOpenAngle, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("setDoorState"), _gstr("sibb"), FunctionGameSetDoorState, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("setDoorOpenAngle"), _gstr("sf"), FunctionGameSetDoorOpenAngle, pClientManager);
	}

	{
		pGameNamespace->RegisterFunction(_gstr("setActorState"), _gstr("sb"), FunctionGameSetActorState, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("setActorActState"), _gstr("si"), FunctionGameSetActorActState, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("setActorAI"), _gstr("siiii"), FunctionGameSetActorAI, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("unloadActor"), _gstr("s"), FunctionGameUnloadActor, pClientManager);
	}

	if (pClientGame->GetMultiplayer() == nullptr)
	{
		pGameNamespace->RegisterFunction(_gstr("setTrafficEnabled"), _gstr("b"), FunctionGameSetTrafficEnabled, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("changeMap"), _gstr("s|b"), FunctionGameChangeMap, pClientManager);

		pClientManager->m_pClientPlayerClass->RegisterConstructor(_gstr("tvfs"), FunctionGameCreatePlayer, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("createPlayer"), _gstr("vfs"), FunctionGameCreatePlayer, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("setLocalPlayer"), _gstr("x"), FunctionGameSetLocalPlayer, pClientManager);
	}

	//g_pClientGame->m_pAudioScriptingFunctions->m_pSoundClass->RegisterFunction(_gstr("setWorldPosition"), _gstr("v"), FunctionAudioSetWorldPosition, pClientManager);
}