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

static bool FunctionGetScreenFromWorldPosition(IScriptState* pState, int argc, void* pUser)
{
	CVector3D pos;
	if (!pState->CheckVector3D(0, pos))
		return false;

	D3DDEVICE_CREATION_PARAMETERS cparams;
	RECT rect;
	g_pD3DDevice->GetCreationParameters(&cparams);
	GetWindowRect(cparams.hFocusWindow, &rect);

	int iWindowWidth = rect.right - rect.left;
	int iWindowHeight = rect.bottom - rect.top;

	D3DXMATRIX matrix;
	if (g_pD3DDevice->GetTransform(D3DTS_WORLD, &matrix) != D3D_OK)
		return false;

	CVector4D clipCoords;
	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[1] + pos.z * matrix[2] + matrix[3];
	clipCoords.y = pos.x * matrix[4] + pos.y * matrix[5] + pos.z * matrix[6] + matrix[7];
	clipCoords.z = pos.x * matrix[8] + pos.y * matrix[9] + pos.z * matrix[10] + matrix[11];
	clipCoords.w = pos.x * matrix[12] + pos.y * matrix[13] + pos.z * matrix[14] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	CVector3D NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	CVector3D vecScreen;
	vecScreen.x = (iWindowWidth / 2 * NDC.x) + (NDC.x + iWindowWidth / 2);
	vecScreen.y = -(iWindowHeight / 2 * NDC.y) + (NDC.y + iWindowHeight / 2);
	vecScreen.z = 0.0f;
	
	pState->ReturnVector3D(vecScreen);
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
	pScripting->m_Global.RegisterFunction(_gstr("setChatWindowEnabled"), _gstr("b"), FunctionSetChatWindowEnabled, pClientGame);

	pScripting->m_Global.RegisterFunction(_gstr("isScancodePressed"), _gstr("i"), FunctionIsScancodePressed);
	pScripting->m_Global.RegisterFunction(_gstr("isKeyDown"), _gstr("i"), FunctionIsKeyDown);

	//pScripting->m_Global.RegisterFunction(_gstr("getPeds"), _gstr(""), FunctionGetHumans);
	//pScripting->m_Global.RegisterFunction(_gstr("getPlayers"), _gstr(""), FunctionGetPlayers);
	//pScripting->m_Global.RegisterFunction(_gstr("getVehicles"), _gstr(""), FunctionGetVehicles);

	pScripting->m_Global.RegisterFunction(_gstr("getScreenFromWorldPosition"), _gstr("v"), FunctionGetScreenFromWorldPosition, pClientGame);
}
