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

static void OnGameInitII()
{
	
	g_pClientGame->OnPreStartInGame(false);
	g_pClientGame->OnStartInGame(false);

	auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
	if (pMultiplayer != nullptr)
	{
		pMultiplayer->Join(); // elements/resources are delayed until this call
	}
}

static void OnGameExitII()
{

}

__declspec(naked) void OnGameExit_HookII()
{
    __asm {
        pushad
        call OnGameExitII
        popad
        retn
    }
}

static void OnGameTickII()
{
	g_pClientGame->DestroyUninitializedGameElements();
	TriggerHackEvent(HACKEVENT_PROCESS, nullptr);
}

static void OnRender2DStuffII()
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

RAWCODECALL Render2DStuffII()
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

		OnRender2DStuffII();

		pStateBlock->Apply();
	}
}

RAWCODE HookRender2DStuffII()
{
	_asm
	{
		pushad
	}
	Render2DStuffII();
	_asm
	{
		popad
		xor eax, eax
		ret
	}
}

void CGameHooksII::InstallHooks()
{
	//M2::AttachHandler(M2_EVENT_CAR_ENTER, [](m2sdk_event* data) {
	//	auto ped = (M2::C_Human2*)data->arg1;
	//	auto car = (M2::C_Car*)data->arg2;
	//	auto seat = (uint32_t)data->arg3;
	//
	//	g_pClientGame->HumanEnteredVehicle(g_pClientGame->m_pClientManager->FindHuman(ped), g_pClientGame->m_pClientManager->FindVehicle(car), seat, 0, 0);
	//	printf("[game-event] ped entered the car on seat: %d\n", seat);
	//});

	//M2::AttachHandler(M2_EVENT_CAR_ENTER_REQUEST, [](m2sdk_event* data) {
	//	auto ped = (M2::C_Human2*)data->arg1;
	//	auto car = (M2::C_Car*)data->arg2;
	//	auto seat = (uint32_t)data->arg3;
	//
	//	g_pClientGame->HumanEnteringVehicle(g_pClientGame->m_pClientManager->FindHuman(ped), g_pClientGame->m_pClientManager->FindVehicle(car), seat, 0, 0);
	//	printf("[game-event] ped entering the car on seat: %d\n", seat);
	//});

    M2::AttachHandler(M2_EVENT_MOD_MESSAGE, [](m2sdk_event* data) {
        auto message = (int)data->arg1;

        switch (message) {
            case M2::E_PlayerMessage::MESSAGE_MOD_ENTER_CAR: {
                auto ped = (M2::C_Human2*)data->arg1;
                auto car = (M2::C_Car*)data->arg2;
                auto seat = (uint32_t)data->arg3;
                g_pClientGame->HumanEnteringVehicle(g_pClientGame->m_pClientManager->FindHuman(ped), g_pClientGame->m_pClientManager->FindVehicle(car), seat, 0, 0);
            } 
            break;

            case M2::E_PlayerMessage::MESSAGE_MOD_BREAKIN_CAR: {
                // Todo
            } 
            break;

            case M2::E_PlayerMessage::MESSAGE_MOD_LEAVE_CAR: {
                auto ped = (M2::C_Human2*)data->arg1;
                auto car = (M2::C_Car*)data->arg2;
                auto seat = (uint32_t)data->arg3;
                g_pClientGame->HumanExitingVehicle(g_pClientGame->m_pClientManager->FindHuman(ped), g_pClientGame->m_pClientManager->FindVehicle(car), seat, 0, 0);
            }
            break;
        }
        });

    M2::AttachHandler(M2_EVENT_GAME_MESSAGE, [](m2sdk_event* data) {
        auto message = (M2::C_EntityMessage*)data->arg1;

        switch (message->m_dwMessage) {
        case M2::E_HumanMessage::MESSAGE_GAME_ENTER_EXIT_VEHICLE_DONE: {
            auto ped = (M2::C_Human2*)data->arg1;
            auto car = (M2::C_Car*)data->arg2;
            auto seat = (uint32_t)data->arg3;
            g_pClientGame->HumanExitedVehicle(g_pClientGame->m_pClientManager->FindHuman(ped), g_pClientGame->m_pClientManager->FindVehicle(car), seat, 0, 0);
        } break;

        default: {
            mod_log("[game-event] unknown event %d\n", (int)message->m_dwMessage);
        } break;
        }
    });

    M2::AttachHandler(M2_EVENT_CAR_ENTER_REQUEST, [](m2sdk_event* data) {
        data->arg5 = (void*)true;
    });

    M2::AttachHandler(M2_EVENT_CAR_HOOD_OPEN_REQUEST, [](m2sdk_event* data) {
        data->arg5 = (void*)true;
    });

    M2::AttachHandler(M2_EVENT_CAR_HOOD_CLOSE_REQUEST, [](m2sdk_event* data) {
        data->arg5 = (void*)true;
        });

    M2::AttachHandler(M2_EVENT_CAR_TRUNK_OPEN_REQUEST, [](m2sdk_event* data) {
        data->arg5 = (void*)true;
    });

    M2::AttachHandler(M2_EVENT_CAR_TRUNK_CLOSE_REQUEST, [](m2sdk_event* data) {
        data->arg5 = (void*)true;
    });

    M2::AttachHandler(M2_EVENT_CAR_FUELTANK_REQUEST, [](m2sdk_event* data) {
        data->arg5 = (void*)true;
    });

    M2::AttachHandler(M2_EVENT_CAR_ENTER, [](m2sdk_event* data) {
        auto ped = (M2::C_Human2*)data->arg1;
        auto car = (M2::C_Car*)data->arg2;
        auto seat = (uint32_t)data->arg3;
        g_pClientGame->HumanEnteredVehicle(g_pClientGame->m_pClientManager->FindHuman(ped), g_pClientGame->m_pClientManager->FindVehicle(car), seat, 0, 0);
    });

    M2::C_Player2_Hooks::HookAddSpecCommand([&](M2::E_SpecCommand type) {
        switch (type)
        {
            case M2::E_SpecCommand::HOLSTER:
                // OnPlayerHolsterWeapon
                break;

            case M2::E_SpecCommand::RELOAD:
                // OnPlayerReloadWeapon
                break;

            case M2::E_SpecCommand::SHOT:
                // OnPlayerShoot
                break;

            case M2::E_SpecCommand::SWITCH_NEXT:
                // OnPlayerSwitchNextWeapon
                break;

            case M2::E_SpecCommand::SWITCH_PREV:
                // OnPlayerSwitchPreviousWeapon
                break;
        }
    });
}