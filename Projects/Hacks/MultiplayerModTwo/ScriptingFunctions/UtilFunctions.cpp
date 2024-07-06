#include "stdafx.h"
#include "ScriptingFunctions.h"
#include "../Elements/Elements.h"
#include "../ClientManager.h"
#include "ClientGame.h"

// For SetChatWindowEnabled
#include <Multiplayer/ChatWindow.h>
#include <Multiplayer/CmdWindow.h>

#include "D3dx9math.h"
#include "IDirect3D9Hook.h"
#include "CHookedDirect3D9Util.h"

#include <Utils/VectorTools.h>

extern IDirect3DDevice9* g_pD3DDevice;

static bool FunctionSetChatWindowEnabledII(IScriptState* pState, int argc, void* pUser)
{
	bool bEnabled;
	if (!pState->CheckBoolean(0, bEnabled))
		return false;

	g_pClientGame->m_pChatWindow->SetEnabled(bEnabled);
	if (g_pClientGame->m_pCmdWindow->IsEnabled())
	{
		g_pClientGame->m_pCmdWindow->Disable();
		g_pClientGame->m_pCmdWindow->FlushBuffers();
	}

	return true;
}

static bool FunctionIsScancodePressedII(IScriptState* pState, int argc, void* pUser)
{
	Sint32 Scancode;
	if (!pState->CheckNumber(0, Scancode))
		return false;
	if (Scancode >= SDL_NUM_SCANCODES)
		return pState->Error(_gstr("Invalid scancode"));
	const Uint8* pKeyboardState = SDL_GetKeyboardState(NULL);
	pState->ReturnBoolean(pKeyboardState[Scancode] != 0);
	return true;
}

static bool FunctionIsKeyDownII(IScriptState* pState, int argc, void* pUser)
{
	Sint32 Key;
	if (!pState->CheckNumber(0, Key))
		return false;
	auto Scancode = SDL_GetScancodeFromKey(Key);
	const Uint8* pKeyboardState = SDL_GetKeyboardState(NULL);
	pState->ReturnBoolean(pKeyboardState[Scancode] != 0);
	return true;
}

static bool FunctionGetVehiclesII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;
	auto pArray = new CArgumentArray();
	for (auto pElement : pClientManager->m_rgpVehicles)
	{
		if (pElement != NULL && pElement.GetPointer() != nullptr)
		{
			pArray->AddObject((CClientVehicleII*)pElement.GetPointer());
		}
	}

	pState->ReturnAndOwn(pArray);
	return true;
}

static bool FunctionGetHumansII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;
	auto pArray = new CArgumentArray();
	for (auto pElement : pClientManager->m_rgpPeds)
	{
		if (pElement != NULL && pElement.GetPointer() != nullptr)
		{
			pArray->AddObject((CClientHumanII*)pElement.GetPointer());
		}
	}

	for (auto pElement : pClientManager->m_rgpPlayers)
	{
		if (pElement != NULL && pElement.GetPointer() != nullptr)
		{
			pArray->AddObject((CClientPlayerII*)pElement.GetPointer());
		}
	}

	pState->ReturnAndOwn(pArray);
	return true;
}

static bool FunctionGetPlayersII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;
	auto pArray = new CArgumentArray();
	for (auto pElement : pClientManager->m_rgpPlayers)
	{
		if (pElement != NULL && pElement.GetPointer() != nullptr)
		{
			pArray->AddObject((CClientPlayerII*)pElement.GetPointer());
		}
	}

	pState->ReturnAndOwn(pArray);
	return true;
}

static bool FunctionGetCameraPositionII(IScriptState* pState, int argc, void* pUser)
{
	M2::C_Camera* pCamera = M2::C_GameCamera::Get()->GetCamera(1);
	pState->ReturnVector3D(CVecToolsII::ConvertFromMafiaVec(pCamera->GetPos()));
	return true;
}

void CScriptingFunctionsII::RegisterUtilDefines(Galactic3D::CDefineHandlers* pDefineHandlers)
{
	pDefineHandlers->Define(_gstr("CLIENT_VERSION_MAJOR"), LAUNCHER_MAJOR);
	pDefineHandlers->Define(_gstr("CLIENT_VERSION_MINOR"), LAUNCHER_MINOR);
	pDefineHandlers->Define(_gstr("CLIENT_VERSION_PATCH"), LAUNCHER_PATCH);
	pDefineHandlers->Define(_gstr("CLIENT_VERSION_BUILD"), LAUNCHER_BUILD);
	//pDefineHandlers->Define("CLIENT_VERSION", CLIENT_VERSION_MAJOR+"."+CLIENT_VERSION_MINOR+"."+CLIENT_VERSION_PATCH+"."+CLIENT_VERSION_BUILD);
}

void CScriptingFunctionsII::RegisterUtilFunctions(Galactic3D::CScripting* pScripting, CClientGameII* pClientGame)
{
	auto pClientManager = pClientGame->m_pClientManager;

	pScripting->m_Global.RegisterFunction(_gstr("setChatWindowEnabled"), _gstr("b"), FunctionSetChatWindowEnabledII, pClientGame);

	pScripting->m_Global.RegisterFunction(_gstr("isScancodePressed"), _gstr("i"), FunctionIsScancodePressedII);
	pScripting->m_Global.RegisterFunction(_gstr("isKeyDown"), _gstr("i"), FunctionIsKeyDownII);

	//pScripting->m_Global.RegisterFunction(_gstr("getPeds"), _gstr(""), FunctionGetHumans);
	//pScripting->m_Global.RegisterFunction(_gstr("getPlayers"), _gstr(""), FunctionGetPlayers);
	//pScripting->m_Global.RegisterFunction(_gstr("getVehicles"), _gstr(""), FunctionGetVehicles);

	//pScripting->m_Global.RegisterFunction(_gstr("getScreenFromWorldPosition"), _gstr("v"), FunctionGetScreenFromWorldPosition, pClientGame);
	
	auto pCameraNamespace = pScripting->m_Global.AddNamespace(_gstr("camera"));
	pCameraNamespace->AddProperty(pClientManager, _gstr("position"), ARGUMENT_VECTOR3D, FunctionGetCameraPositionII);
	//pCameraNamespace->AddProperty(pClientManager, _gstr("lookAtPosition"), ARGUMENT_VECTOR3D, FunctionGetCameraLookAtPosition);

	//pCameraNamespace->AddProperty(pClientManager, _gstr("fov"), ARGUMENT_FLOAT, FunctionGetCameraFieldOfView, FunctionSetCameraFieldOfView);
}
