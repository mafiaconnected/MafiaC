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

	D3DXVECTOR3 origin;
	origin.x = pos.x;
	origin.y = pos.y;
	origin.z = pos.z;

	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);

	D3DVIEWPORT9 vp;
	if (g_pD3DDevice->GetViewport(&vp) != D3D_OK)
		return false;

	MafiaSDK::C_Game* pGame = MafiaSDK::GetMission()->GetGame();

	D3DXMATRIX camMat1 = *(D3DXMATRIX*)(*(uint32_t*)((((uint32_t)&pGame->GetInterface()->mCamera) + 4)) + 356);
	D3DXMATRIX camMat2 = *(D3DXMATRIX*)(*(uint32_t*)((((uint32_t)&pGame->GetInterface()->mCamera) + 4)) + 484);

	D3DXMATRIX Projection;
	D3DXMatrixPerspectiveFovLH(&Projection, 0.4f * 3.14f, ((float)vp.Width) / ((float)vp.Height), 1.0f, 1000.0f);

	D3DXMATRIX WVP1 = world * camMat1 * Projection;
	D3DXMATRIX WVP2 = world * camMat2 * Projection;

	D3DXVECTOR3 screenCoord1(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 screenCoord2(0.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&screenCoord1, &origin, &WVP1);
	D3DXVec3TransformCoord(&screenCoord2, &origin, &WVP2);

	CVector3D vecScreen;
	vecScreen.x = (screenCoord1.x + 0.5f) * (float)vp.Width;
	vecScreen.y = ((-screenCoord2.y + 1.0f) / 3.0f) * (float)vp.Height;
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
