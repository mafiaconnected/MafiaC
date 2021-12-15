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

extern decltype(Direct3DCreate9)* g_pDirect3DCreate9;
extern IDirect3D9* WINAPI HookDirect3DCreate9(UINT SDKVersion);

extern Direct3D9* g_pD3D9;
extern IDirect3DDevice9* g_pD3DDevice;
extern C2D* g_p2DII;
extern SDL_Window* g_pWindow;

static void OnGameInit()
{
	
	g_pClientGame->OnPreStartInGame(false);
	g_pClientGame->OnStartInGame(false);

	auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
	if (pMultiplayer != nullptr)
	{
		pMultiplayer->Join(); // elements/resources are delayed until this call
	}
}

static void OnGameExit()
{

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

static void OnGameTick()
{
	g_pClientGame->DestroyUninitializedGameElements();
	TriggerHackEvent(HACKEVENT_PROCESS, nullptr);
}

static void OnRender2DStuff()
{
	if (g_pD3D9 && g_p2DII != nullptr)
	{
		g_pD3D9->SetAllStatesAgain();

		int width;
		int height;
		SDL_GetWindowSize(g_pWindow, &width, &height);

		g_p2DII->SetViewport(CVector2D(0, 0), CVector2D((float)width, (float)height));
		if (g_p2DII->Begin2D(g_pD3D9))
		{
			g_pClientGame->m_pGalacticFunctions->m_p2D = &g_pClientGame->m_p2D;
			g_pClientGame->OnRender2DStuff();
			g_pClientGame->m_pOnDrawnHUDEventType->Trigger();
			g_p2DII->End2D();
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

void CGameHooksII::InstallHooks()
{
	M2::AttachHandler(M2_EVENT_CAR_ENTER, [](m2sdk_event* data) {
		auto ped = (M2::C_Human2*)data->arg1;
		auto car = (M2::C_Car*)data->arg2;
		auto seat = (uint32_t)data->arg3;

		g_pClientGame->HumanEnteredVehicle(g_pClientGame->m_pClientManager->FindHuman(ped), g_pClientGame->m_pClientManager->FindVehicle(car), seat, 0, 0);
		printf("[game-event] ped entered the car on seat: %d\n", seat);
	});

	M2::AttachHandler(M2_EVENT_CAR_ENTER_REQUEST, [](m2sdk_event* data) {
		auto ped = (M2::C_Human2*)data->arg1;
		auto car = (M2::C_Car*)data->arg2;
		auto seat = (uint32_t)data->arg3;

		g_pClientGame->HumanEnteringVehicle(g_pClientGame->m_pClientManager->FindHuman(ped), g_pClientGame->m_pClientManager->FindVehicle(car), seat, 0, 0);
		printf("[game-event] ped entering the car on seat: %d\n", seat);
	});
}