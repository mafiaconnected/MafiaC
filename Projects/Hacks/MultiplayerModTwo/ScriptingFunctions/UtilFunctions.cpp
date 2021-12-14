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

static bool FunctionSetChatWindowEnabled(IScriptState* pState, int argc, void* pUser)
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

static bool FunctionIsScancodePressed(IScriptState* pState, int argc, void* pUser)
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

static bool FunctionIsKeyDown(IScriptState* pState, int argc, void* pUser)
{
	Sint32 Key;
	if (!pState->CheckNumber(0, Key))
		return false;
	auto Scancode = SDL_GetScancodeFromKey(Key);
	const Uint8* pKeyboardState = SDL_GetKeyboardState(NULL);
	pState->ReturnBoolean(pKeyboardState[Scancode] != 0);
	return true;
}

static bool FunctionGetVehicles(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;
	auto pArray = new CArgumentArray();
	for (auto pElement : pClientManager->m_rgpVehicles)
	{
		if (pElement != NULL && pElement.GetPointer() != nullptr)
		{
			pArray->AddObject((CClientVehicle*)pElement.GetPointer());
		}
	}

	pState->ReturnAndOwn(pArray);
	return true;
}

static bool FunctionGetHumans(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;
	auto pArray = new CArgumentArray();
	for (auto pElement : pClientManager->m_rgpPeds)
	{
		if (pElement != NULL && pElement.GetPointer() != nullptr)
		{
			pArray->AddObject((CClientHuman*)pElement.GetPointer());
		}
	}

	for (auto pElement : pClientManager->m_rgpPlayers)
	{
		if (pElement != NULL && pElement.GetPointer() != nullptr)
		{
			pArray->AddObject((CClientPlayer*)pElement.GetPointer());
		}
	}

	pState->ReturnAndOwn(pArray);
	return true;
}

static bool FunctionGetPlayers(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;
	auto pArray = new CArgumentArray();
	for (auto pElement : pClientManager->m_rgpPlayers)
	{
		if (pElement != NULL && pElement.GetPointer() != nullptr)
		{
			pArray->AddObject((CClientPlayer*)pElement.GetPointer());
		}
	}

	pState->ReturnAndOwn(pArray);
	return true;
}

static bool FunctionGetCameraPosition(IScriptState* pState, int argc, void* pUser)
{
	M2::C_Camera* pCamera = M2::C_GameCamera::Get()->GetCamera(1);
	pState->ReturnVector3D(CVecTools::ConvertFromMafiaVec(pCamera->GetPos()));
	return true;
}

void CScriptingFunctions::RegisterUtilDefines(Galactic3D::CDefineHandlers* pDefineHandlers)
{
	pDefineHandlers->Define(_gstr("CLIENT_VERSION_MAJOR"), LAUNCHER_MAJOR);
	pDefineHandlers->Define(_gstr("CLIENT_VERSION_MINOR"), LAUNCHER_MINOR);
	pDefineHandlers->Define(_gstr("CLIENT_VERSION_PATCH"), LAUNCHER_PATCH);
	pDefineHandlers->Define(_gstr("CLIENT_VERSION_BUILD"), LAUNCHER_BUILD);
	//pDefineHandlers->Define("CLIENT_VERSION", CLIENT_VERSION_MAJOR+"."+CLIENT_VERSION_MINOR+"."+CLIENT_VERSION_PATCH+"."+CLIENT_VERSION_BUILD);
}

void CScriptingFunctions::RegisterUtilFunctions(Galactic3D::CScripting* pScripting, CClientGame* pClientGame)
{
	auto pClientManager = pClientGame->m_pClientManager;

	pScripting->m_Global.RegisterFunction(_gstr("setChatWindowEnabled"), _gstr("b"), FunctionSetChatWindowEnabled, pClientGame);

	pScripting->m_Global.RegisterFunction(_gstr("isScancodePressed"), _gstr("i"), FunctionIsScancodePressed);
	pScripting->m_Global.RegisterFunction(_gstr("isKeyDown"), _gstr("i"), FunctionIsKeyDown);

	//pScripting->m_Global.RegisterFunction(_gstr("getPeds"), _gstr(""), FunctionGetHumans);
	//pScripting->m_Global.RegisterFunction(_gstr("getPlayers"), _gstr(""), FunctionGetPlayers);
	//pScripting->m_Global.RegisterFunction(_gstr("getVehicles"), _gstr(""), FunctionGetVehicles);

	//pScripting->m_Global.RegisterFunction(_gstr("getScreenFromWorldPosition"), _gstr("v"), FunctionGetScreenFromWorldPosition, pClientGame);
	
	auto pCameraNamespace = pScripting->m_Global.AddNamespace(_gstr("camera"));
	pCameraNamespace->AddProperty(pClientManager, _gstr("position"), ARGUMENT_VECTOR3D, FunctionGetCameraPosition);
	//pCameraNamespace->AddProperty(pClientManager, _gstr("lookAtPosition"), ARGUMENT_VECTOR3D, FunctionGetCameraLookAtPosition);

	//pCameraNamespace->AddProperty(pClientManager, _gstr("fov"), ARGUMENT_FLOAT, FunctionGetCameraFieldOfView, FunctionSetCameraFieldOfView);
}
