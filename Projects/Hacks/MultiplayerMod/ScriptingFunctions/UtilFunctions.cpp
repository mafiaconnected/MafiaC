#include "stdafx.h"
#include "ScriptingFunctions.h"
#include "../Elements/Elements.h"
#include "../ClientManager.h"

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
	//if (!bEnabled)
	//	g_pClientGame->m_pChatWindow->FlushBuffers();
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
	CVector3D vecWorld;
	if (!pState->CheckVector3D(0, vecWorld))
		return false;

	// From Oakwood
	D3DXVECTOR3 input = { vecWorld.x, vecWorld.y, vecWorld.z };
	D3DXVECTOR3 out;
	D3DVIEWPORT9 viewport;
	g_pD3DDevice->GetViewport(&viewport);
	D3DXMATRIX projection, view, world;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &view);
	D3DXMatrixIdentity(&world);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &projection);
	D3DXVec3Project(&out, &input, &viewport, &projection, &view, &world);
	
	CVector3D vecScreen;
	vecScreen.x = out.x;
	vecScreen.y = out.y;
	vecScreen.z = out.z;

	pState->ReturnVector3D(vecScreen);

	// From GTA Connected
	// Get the static view matrix as D3DXMATRIX
	//CMatrix4x4 m ( (float*)SELECTADDRESS_GTA(0x7095F0, 0x7E4EF8, 0xB6FA2C, SELECTADDRESS_ADDRESS_NOTIMPLEMENTED, 0x7095F0, 0x7E4F00, 0xB720AC, SELECTADDRESS_ADDRESS_NOTIMPLEMENTED, SELECTADDRESS_ADDRESS_NOTIMPLEMENTED) );
	//D3DXMATRIX projection;

	// Get the static virtual screen (x,y)-sizes
	//unsigned int uiLenX = MafiaSDK::GetIGraph()->Scrn_sx();
	//unsigned int uiLenY = MafiaSDK::GetIGraph()->Scrn_sy();

	// Do a transformation
	//CVector3D vecScreen;
	//vecScreen.x = vecWorld.z * m.m_vec[2].x + vecWorld.y * m.m_vec[1].x + vecWorld.x * m.m_vec[0].x + m.m_vec[3].x;
	//vecScreen.y = vecWorld.z * m.m_vec[2].y + vecWorld.y * m.m_vec[1].y + vecWorld.x * m.m_vec[0].y + m.m_vec[3].y;
	//vecScreen.z = vecWorld.z * m.m_vec[2].z + vecWorld.y * m.m_vec[1].z + vecWorld.x * m.m_vec[0].z + m.m_vec[3].z;

	// Get the correct screen coordinates
	//float fRecip = 1.0f / vecScreen.m_Z;
	//vecScreen.m_X *= fRecip * uiLenX;
	//vecScreen.m_Y *= fRecip * uiLenY;

	

	return true;
}

void CScriptingFunctions::RegisterUtilFunctions(Galactic3D::CScripting* pScripting, CClientGame* pClientGame)
{
	pScripting->m_Global.RegisterFunction(_gstr("setChatWindowEnabled"), _gstr("b"), FunctionSetChatWindowEnabled, pClientGame);

	pScripting->m_Global.RegisterFunction(_gstr("isScancodePressed"), _gstr("i"), FunctionIsScancodePressed);
	pScripting->m_Global.RegisterFunction(_gstr("isKeyDown"), _gstr("i"), FunctionIsKeyDown);

	pScripting->m_Global.RegisterFunction(_gstr("getPeds"), _gstr(""), FunctionGetHumans);
	pScripting->m_Global.RegisterFunction(_gstr("getPlayers"), _gstr(""), FunctionGetPlayers);
	pScripting->m_Global.RegisterFunction(_gstr("getVehicles"), _gstr(""), FunctionGetVehicles);

	pScripting->m_Global.RegisterFunction(_gstr("getScreenFromWorldPosition"), _gstr("v"), FunctionGetScreenFromWorldPosition, pClientGame);
}
