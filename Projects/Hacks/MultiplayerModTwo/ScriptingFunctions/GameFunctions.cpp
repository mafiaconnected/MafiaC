#include "stdafx.h"
#include "ScriptingFunctions.h"
#include "../ClientManager.h"
#include "../ClientGame.h"
#include "../Elements/ClientHuman.h"
#include "../Elements/ClientPlayer.h"
#include "../Elements/ClientVehicle.h"
#include <Utils/VectorTools.h>

extern SDL_Window* g_pWindow;

static bool FunctionGameCreatePedII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	uint32_t model = 0;
	if (!pState->CheckNumber(0, model))
		return false;

	CVector3D pos = { 0, 0, 0 };
	if (!pState->CheckVector3D(1, pos))
		return false;

	float angle = 0;
	if (!pState->CheckNumber(2, angle))
		return false;

	CClientHumanII* pClientHuman = reinterpret_cast<CClientHumanII*>(pClientManager->Create(ELEMENT_PED));
	pClientHuman->SetModel(model);
	pClientHuman->Spawn(pos, angle, false);
	pClientHuman->m_pResource = pState->GetResource();
	pState->ReturnObject(pClientHuman);

	return true;
}

static bool FunctionGameCreatePlayerII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	uint32_t model = 0;
	if (!pState->CheckNumber(0, model))
		return false;

	CVector3D pos = { 0, 0, 0 };
	if (!pState->CheckVector3D(1, pos))
		return false;

	float angle = 0;
	if (!pState->CheckNumber(2, angle))
		return false;

	CClientPlayerII* pClientPlayer = reinterpret_cast<CClientPlayerII*>(pClientManager->Create(ELEMENT_PLAYER));
	pClientPlayer->SetModel(model);
	//pClientPlayer->SetPosition(pos);
	//pClientPlayer->SetHeading(angle);
	pClientPlayer->Spawn(pos, angle, true);
	pClientManager->SetLocalPlayer(pClientPlayer);
	pClientPlayer->m_pResource = pState->GetResource();
	pState->ReturnObject(pClientPlayer);

	return true;
}

static bool FunctionGameCreateVehicleII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	uint32_t model = 0;
	if (!pState->CheckNumber(0, model))
		return false;

	CVector3D pos = { 0, 0, 0 };
	if (!pState->CheckVector3D(1, pos))
		return false;

	float angle = 0;
	if (!pState->CheckNumber(2, angle))
		return false;

	CClientVehicleII* pClientVehicle = reinterpret_cast<CClientVehicleII*>(pClientManager->Create(ELEMENT_VEHICLE));
	CVector3D rot = CVecToolsII::ComputeDirEuler(angle);
	pClientVehicle->Create(model, pos, rot);
	pClientVehicle->m_pResource = pState->GetResource();
	//pClientManager->RegisterObject(pClientVehicle);
	pClientVehicle->Release();

	pState->ReturnObject(pClientVehicle);
	return true;
}

static bool FunctionGameEnableMapII(IScriptState* pState, int argc, void* pUser)
{
	bool state = true;
	if (!pState->CheckBoolean(0, state))
		return false;

	// Set map state with game sdk
	return true;
}

static bool FunctionGameSetTrafficEnabledII(IScriptState* pState, int argc, void* pUser)
{
	bool state = true;
	if (!pState->CheckBoolean(0, state))
		return false;

	// Set traffic state with game sdk
	return true;
}

static bool FunctionGameCreateExplosionII(IScriptState* pState, int argc, void* pUser)
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

	// Create explosion with game sdk
	return true;
}

static bool FunctionGameSetLocalPlayerII(IScriptState* pState, int argc, void* pUser)
{
 	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientPlayerII* pClientPlayer;
	if (!pState->CheckClass(pClientManager->m_pClientPlayerClass, 0, false, &pClientPlayer))
		return false;

	// Set local player with game sdk
	return true;
}

static bool FunctionGameSetCameraLookAtII(IScriptState* pState, int argc, void* pUser)
{
	CVector3D camPos = { 0, 0, 0 };
	if (!pState->CheckVector3D(0, camPos))
		return false;

	CVector3D lookAtPos = { 0, 0, 0 };
	if (!pState->CheckVector3D(0, lookAtPos))
		return false;

	// Set camera lookat with game sdk
	return true;
}

static bool FunctionGetGameII(IScriptState* pState, int argc, void* pUser)
{
	CClientGameII* pClientGame = (CClientGameII*)pUser;
	pState->ReturnNumber(GAME_MAFIA_ONE);
	return true;
}

static bool FunctionGetWidthII(IScriptState* pState, int argc, void* pUser)
{
	int width;
	int height;
	SDL_GetWindowSize(g_pWindow, &width, &height);

	pState->ReturnNumber(width);
	return true;
}

static bool FunctionGetHeightII(IScriptState* pState, int argc, void* pUser)
{
	int width;
	int height;
	SDL_GetWindowSize(g_pWindow, &width, &height);

	pState->ReturnNumber(height);
	return true;
}

static bool FunctionGetAspectRatioII(IScriptState* pState, int argc, void* pUser)
{
	int width;
	int height;
	SDL_GetWindowSize(g_pWindow, &width, &height);

	pState->ReturnNumber(width / height);
	return true;
}

static bool FunctionSetPlayerControlII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	bool bEnabled;
	if (!pState->CheckBoolean(0, bEnabled))
		return false;

	g_pClientGame->LockControls(!bEnabled);
	return true;
}

void CScriptingFunctionsII::RegisterGameDefines(Galactic3D::CDefineHandlers* pDefineHandlers)
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
}

void CScriptingFunctionsII::RegisterGameFunctions(Galactic3D::CScripting* pScripting, CClientGameII* pClientGame)
{
	auto pGameNamespace = pScripting->m_Global.GetNamespace(_gstr("mafia"));
	auto pClientManager = pClientGame->m_pClientManager;

	{
		pGameNamespace->AddProperty(pClientGame, _gstr("game"), ARGUMENT_INTEGER, FunctionGetGameII);
		pGameNamespace->AddProperty(pClientGame, _gstr("width"), ARGUMENT_INTEGER, FunctionGetWidthII);
		pGameNamespace->AddProperty(pClientGame, _gstr("height"), ARGUMENT_INTEGER, FunctionGetHeightII);
		pGameNamespace->AddProperty(pClientGame, _gstr("aspectRatio"), ARGUMENT_FLOAT, FunctionGetAspectRatioII);
	}

	{
		pClientManager->m_pClientHumanClass->RegisterConstructor(_gstr("tivf"), FunctionGameCreatePedII, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("createPed"), _gstr("ivf"), FunctionGameCreatePedII, pClientManager);

		pClientManager->m_pClientVehicleClass->RegisterConstructor(_gstr("tivf"), FunctionGameCreateVehicleII, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("createVehicle"), _gstr("ivf"), FunctionGameCreateVehicleII, pClientManager);
	}

	{
		pGameNamespace->RegisterFunction(_gstr("createExplosion"), _gstr("vff"), FunctionGameCreateExplosionII, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("setPlayerControl"), _gstr("b"), FunctionSetPlayerControlII, pClientGame);
		//pGameNamespace->RegisterFunction(_gstr("setCameraLookAt"), _gstr("vv"), FunctionSetCameraLookAt, pClientGame);
	}

	{
		auto pHUDNamespace = pGameNamespace->AddNamespace(_gstr("hud"));
		pHUDNamespace->RegisterFunction(_gstr("enableMap"), _gstr("b"), FunctionGameEnableMapII, pClientManager);
	}

	if (pClientGame->GetMultiplayer() == nullptr)
	{
		pGameNamespace->RegisterFunction(_gstr("setTrafficEnabled"), _gstr("b"), FunctionGameSetTrafficEnabledII, pClientManager);

		pClientManager->m_pClientPlayerClass->RegisterConstructor(_gstr("tivf"), FunctionGameCreatePlayerII, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("createPlayer"), _gstr("ivf"), FunctionGameCreatePlayerII, pClientManager);
		pGameNamespace->RegisterFunction(_gstr("setLocalPlayer"), _gstr("x"), FunctionGameSetLocalPlayerII, pClientManager);
	}
}
