#include "stdafx.h"
#include "ScriptingFunctions.h"
#include "../ClientManager.h"
#include "../ClientGame.h"
#include "../Elements/ClientHuman.h"
#include "../Elements/ClientPlayer.h"
#include "../Elements/ClientVehicle.h"
#include <Utils/VectorTools.h>

static bool FunctionGameCreatePed(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman = reinterpret_cast<CClientHuman*>(pClientManager->Create(ELEMENT_PED));

	const GChar* mdl = pState->CheckString(0);
	if (!mdl) return false;

	CVector3D pos = { 0, 0, 0 };
	if (!pState->CheckVector3D(1, pos))
		return false;

	float angle = 0;
	if (!pState->CheckNumber(2, angle))
		return false;

	pClientHuman->SetModel(mdl);

	pClientHuman->Spawn(pos, angle, false);

	pClientHuman->m_pResource = pState->GetResource();
	pState->ReturnObject(pClientHuman);

	return true;
}

static bool FunctionGameCreatePlayer(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientPlayer* pClientPlayer = reinterpret_cast<CClientPlayer*>(pClientManager->Create(ELEMENT_PLAYER));

	const GChar* mdl = pState->CheckString(0);
	if (!mdl) return false;

	CVector3D pos = { 0, 0, 0 };
	if (!pState->CheckVector3D(1, pos))
		return false;

	float angle = 0;
	if (!pState->CheckNumber(2, angle))
		return false;

	pClientPlayer->SetModel(mdl);
	//pClientPlayer->SetPosition(pos);
	//pClientPlayer->SetHeading(angle);

	pClientPlayer->Spawn(pos, angle, true);

	pClientManager->SetLocalPlayer(pClientPlayer);

	pClientPlayer->m_pResource = pState->GetResource();
	pState->ReturnObject(pClientPlayer);

	return true;
}

static bool FunctionGameCreateVehicle(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle = reinterpret_cast<CClientVehicle*>(pClientManager->Create(ELEMENT_VEHICLE));

	const GChar* mdl = pState->CheckString(0);
	if (!mdl) return false;

	CVector3D pos = { 0, 0, 0 };
	if (!pState->CheckVector3D(1, pos))
		return false;

	float angle = 0;
	if (!pState->CheckNumber(2, angle))
		return false;

	CVector3D rot = CVecTools::ComputeDirEuler(angle);
	pClientVehicle->Create(mdl, pos, rot);

	pClientVehicle->m_pResource = pState->GetResource();
	pClientManager->RegisterObject(pClientVehicle);
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

	// Note (Sevenisko): had to use another func, because PatchJumpToGame works only on game load
	MafiaSDK::GetMission()->MapLoad(mapName2);

	return true;
}

static bool FunctionGameMessage(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle = reinterpret_cast<CClientVehicle*>(pClientManager->Create(ELEMENT_VEHICLE));

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
	bool state = true;
	if (!pState->CheckBoolean(0, state))
		return false;

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

	pDefineHandlers->Define(_gstr("GAME_UNKNOWN"), GAME_UNKNOWN);
	pDefineHandlers->Define(_gstr("GAME_MAFIA_ONE"), GAME_MAFIA_ONE);
	pDefineHandlers->Define(_gstr("GAME_MAFIA_TWO"), GAME_MAFIA_TWO);
	pDefineHandlers->Define(_gstr("GAME_MAFIA_THREE"), GAME_MAFIA_THREE);
	pDefineHandlers->Define(_gstr("GAME_MAFIA_ONE_DE"), GAME_MAFIA_ONE_DE);

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
	}

	{
		pGameNamespace->RegisterFunction(_gstr("createExplosion"), _gstr("vff"), FunctionGameCreateExplosion, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("fadeScreen"), _gstr("bf|i"), FunctionGameFadeScreen, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("setPlayerControl"), _gstr(""), FunctionSetPlayerControl, pClientGame);
	}

	{
		auto pHUDNamespace = pGameNamespace->AddNamespace(_gstr("hud"));
		pHUDNamespace->RegisterFunction(_gstr("message"), _gstr("si"), FunctionGameMessage, pClientManager);
		pHUDNamespace->RegisterFunction(_gstr("enableMap"), _gstr("b"), FunctionGameEnableMap, pClientManager);
		pHUDNamespace->RegisterFunction(_gstr("announce"), _gstr("sf"), FunctionGameAnnounce, pClientManager);
		pHUDNamespace->RegisterFunction(_gstr("showCountdown"), _gstr("i"), FunctionGameShowCountdown, pClientManager);
	}

	if (pClientGame->GetMultiplayer() == nullptr)
	{
		pGameNamespace->RegisterFunction(_gstr("setTrafficEnabled"), _gstr("b"), FunctionGameSetTrafficEnabled, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("changeMap"), _gstr("s"), FunctionGameChangeMap, pClientManager);

		pClientManager->m_pClientPlayerClass->RegisterConstructor(_gstr("tsvf"), FunctionGameCreatePlayer, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("createPlayer"), _gstr("svf"), FunctionGameCreatePlayer, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("setLocalPlayer"), _gstr("x"), FunctionGameSetLocalPlayer, pClientManager);
	}
}
