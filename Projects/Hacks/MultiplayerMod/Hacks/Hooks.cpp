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
extern C2D* g_p2D;

static void OnGameInit()
{
	auto mName = MafiaSDK::GetCurrentMissionName();

	MafiaSDK::GetIGraph()->SetAppName("Mafia Connected");

	// We don't want the game to be initialized in menu
	if (!strcmp(mName, "00menu")) return;

	g_pClientGame->OnPreStartInGame(false);
	g_pClientGame->OnStartInGame(false);

	auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
	if (pMultiplayer != nullptr)
	{
		pMultiplayer->Join(); // elements/resources are delayed until this call
	}

	CGameHacks::EnableGameMap(false);

	// Disable the traffic
	MafiaSDK::GetMission()->GetGame()->SetTrafficVisible(false);

	auto bridge1 = (MafiaSDK::C_Bridge*)MafiaSDK::GetMission()->FindActorByName("LLsklap01");
	auto bridge2 = (MafiaSDK::C_Bridge*)MafiaSDK::GetMission()->FindActorByName("sklapx01");

	if (bridge1) bridge1->Shutdown(true);
	if (bridge2) bridge2->Shutdown(true);

	CArguments Args;
	Args.AddString((const GChar*)mName);
	bool bPreventDefault = false;
	g_pClientGame->m_pOnMapLoadedEventType->Trigger(Args, bPreventDefault);
	if (bPreventDefault)
		return;
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

// UseActor
HOOKADDRESS g_ReturnHumanUseActor;
HOOKVAR MafiaSDK::C_Human* g_pHumanUseActor_Human;
HOOKVAR MafiaSDK::C_Actor* g_pHumanUseActor_Actor;
HOOKVAR int g_pHumanUseActor_Unk1;
HOOKVAR int g_pHumanUseActor_Unk2;
HOOKVAR int g_pHumanUseActor_Unk3;
HOOKVAR bool g_bCancelHumanUseActor;

// DoThrowCocotFromCar
HOOKADDRESS g_ReturnHumanDoThrowCocotFromCar;
HOOKVAR MafiaSDK::C_Human* g_pHumanDoThrowCocotFromCar_Human;
HOOKVAR MafiaSDK::C_Car* g_pHumanDoThrowCocotFromCar_Car;
HOOKVAR int g_pHumanDoThrowCocotFromCar_SeatID;
HOOKVAR bool g_bCancelHumanDoThrowCocotFromCar;

// CreateActor
HOOKADDRESS g_ReturnCreateActor;
HOOKVAR MafiaSDK::C_Actor* g_pCreateActor_Actor;
HOOKVAR int g_pCreateActor_Arg1;
HOOKVAR bool g_bCancelCreateActor;

RAWCODECALL HumanDoThrowCocotFromCar(void)
{
	if (g_pClientGame->m_bDoThrowCocotFromCarInvokedByGame)
	{
		CClientHuman* pClientHuman = g_pClientGame->m_pClientManager->FindHuman(g_pHumanDoThrowCocotFromCar_Human);
		if (pClientHuman != nullptr)
		{
			CClientVehicle* pClientVehicle = g_pClientGame->m_pClientManager->FindVehicle(g_pHumanDoThrowCocotFromCar_Car);
			if (pClientVehicle != nullptr)
			{
				bool bAllow = true;

				if (bAllow)
				{
					g_pClientGame->HumanJackVehicle(pClientHuman, pClientVehicle, g_pHumanDoThrowCocotFromCar_SeatID);
				}
				else
				{
					g_bCancelHumanDoThrowCocotFromCar = true;
				}
			}
		}
	}
}

RAWCODECALL HumanUseActor(void)
{
	if (g_pClientGame->m_bUseActorInvokedByGame) 
	{
		CClientHuman* pClientHuman = g_pClientGame->m_pClientManager->FindHuman((MafiaSDK::C_Human*)g_pHumanUseActor_Human);
		if (pClientHuman != nullptr)
		{
			CClientVehicle* pClientVehicle = g_pClientGame->m_pClientManager->FindVehicle((MafiaSDK::C_Car*)g_pHumanUseActor_Actor);
			if (pClientVehicle != nullptr)
			{
				bool bAllow = true;

				if (bAllow)
				{
					if (g_pHumanUseActor_Unk1 == 2)
					{
						g_pClientGame->HumanExitingVehicle(pClientHuman, pClientVehicle, g_pHumanUseActor_Unk2, g_pHumanUseActor_Unk1, g_pHumanUseActor_Unk3);
					}
					else
					{
						g_pClientGame->HumanEnteringVehicle(pClientHuman, pClientVehicle, g_pHumanUseActor_Unk2, g_pHumanUseActor_Unk1, g_pHumanUseActor_Unk3);
					}
				}
				else
				{
					g_bCancelHumanUseActor = true;
				}
			}
		}
	}
}

RAWCODECALL CreateActor(void)
{
	if (g_pClientGame->m_bCreateActorInvokedByGame) {
		_glogprintf(_gstr("CreateActor: %i"), g_pCreateActor_Arg1);
		
		// 2 = spawned player
		// 4 = spawned vehicle

		if (g_pCreateActor_Arg1 == 11)
		{
			// Human
			//g_pClientGame->CreateGameHuman(g_pCreateActor_Actor, g_pCreateActor_Arg1);
		}
	}
}

RAWCODE HookHumanDoThrowCocotFromCar(void)
{
	_asm
	{
		mov g_pHumanDoThrowCocotFromCar_Human, ecx
		mov eax, [esp + 4]
		mov g_pHumanDoThrowCocotFromCar_Car, eax
		mov eax, [esp + 8]
		mov g_pHumanDoThrowCocotFromCar_SeatID, eax
		pushad
	}

	g_bCancelHumanDoThrowCocotFromCar = false;
	HumanDoThrowCocotFromCar();
	if (g_bCancelHumanDoThrowCocotFromCar)
	{
		_asm
		{
			//popad
			//retn
		}
	}
	_asm
	{
		popad
		push 0xFFFFFFFF
		push 0x6206A2 // 0x587D70
		jmp g_ReturnHumanDoThrowCocotFromCar
	}
}

RAWCODE HookHumanUseActor(void)
{
	_asm
	{
		mov g_pHumanUseActor_Human, ecx
		mov eax, [esp + 4]
		mov g_pHumanUseActor_Actor, eax
		mov eax, [esp + 8]
		mov g_pHumanUseActor_Unk1, eax
		mov eax, [esp + 0x0C]
		mov g_pHumanUseActor_Unk2, eax
		mov eax, [esp + 0x10]
		mov g_pHumanUseActor_Unk3, eax
		pushad
	}
	g_bCancelHumanUseActor = false;
	HumanUseActor();
	if (g_bCancelHumanUseActor)
	{
		_asm
		{
			//popad
			//retn
		}
	}
	_asm
	{
		popad
		sub esp, 0xF8
		jmp g_ReturnHumanUseActor
	}
}

RAWCODE HookCreateActor(void)
{
	_asm
	{
		mov g_pCreateActor_Actor, ecx
		mov eax, [esp + 4]
		mov g_pCreateActor_Arg1, eax
		pushad
	}
	g_bCancelCreateActor = false;
	CreateActor();
	if (g_bCancelCreateActor)
	{
		_asm
		{
			//popad
			//retn
		}
	}
	_asm
	{
		popad
		mov eax, fs:0
		jmp g_ReturnCreateActor
	}
}

static void OnGameTick()
{
	g_pClientGame->DestroyUninitializedGameElements();
	TriggerHackEvent(HACKEVENT_PROCESS, nullptr);
}

static void OnLocalPlayerFallDown()
{

}

static int OnHumanHit(MafiaSDK::C_Human* target, int hitType, const S_vector& v1, const S_vector& v2, const S_vector& v3, float damage, MafiaSDK::C_Actor* attacker, unsigned int bodyPart, MafiaSDK::I3D_Frame* targetFrame)
{
	//auto humanTarget = g_pClientGame->m_pClientManager->FindHuman(target);
	//auto entityAttacker = g_pClientGame->m_pClientManager->FindEntity(attacker);

	//g_pClientGame->HumanHit(humanTarget, entityAttacker, CVecTools::ConvertFromMafiaVec(v1), CVecTools::ConvertFromMafiaVec(v2), CVecTools::ConvertFromMafiaVec(v3), hitType, damage, bodyPart);

	//if (humanTarget->GetHealth() <= 0) {
	//	CArguments args;
	//	args.AddObject(humanTarget);
	//	args.AddObject(humanAttacker);
	//	g_pClientGame->m_pOnHumanDeathEventType->Trigger(args);
	//
	//	auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
	//	if (pMultiplayer != nullptr)
	//	{
	//		pMultiplayer->SendHumanDeath(humanTarget, humanAttacker);
	//	}
	//}

	return 1;
}

static void OnHumanWeaponChange(MafiaSDK::C_Human* target, int8_t weapon)
{
	auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
	if (pMultiplayer != nullptr)
	{
		auto humanTarget = g_pClientGame->m_pClientManager->FindHuman(target);
		pMultiplayer->SendHumanChangeWeapon(humanTarget, weapon);
	}
}

static void OnHumanWeaponDrop(MafiaSDK::C_Human* target)
{
	_glogprintf(_gstr("Human dropped the weapon"));
	auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
	if (pMultiplayer != nullptr)
	{
		auto humanTarget = g_pClientGame->m_pClientManager->FindHuman(target);
		pMultiplayer->SendHumanDropWeapon(humanTarget);
	}
}

static void OnHumanShoot(const S_vector& pos)
{
	_glogprintf(_gstr("Player shooting to: {%f, %f, %f}"), pos.x, pos.y, pos.z);
	auto pMultiplayer = g_pClientGame->GetActiveMultiplayer();
	if (pMultiplayer != nullptr)
	{
		pMultiplayer->SendLocalPlayerShoot(CVecTools::ConvertFromMafiaVec(pos));
	}
}

static void OnRender2DStuff()
{
	if (g_pD3D9 && g_p2D != nullptr)
	{
		g_pD3D9->SetAllStatesAgain();

		g_p2D->SetViewport(CVector2D(0, 0), CVector2D((float)MafiaSDK::GetIGraph()->Scrn_sx(), (float)MafiaSDK::GetIGraph()->Scrn_sy()));
		if (g_p2D->Begin2D(g_pD3D9))
		{
			g_pClientGame->m_pGalacticFunctions->m_p2D = &g_pClientGame->m_p2D;
			g_pClientGame->OnRender2DStuff();
			g_p2D->End2D();
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

static int __fastcall SetProgress_Hook(DWORD _this, DWORD edx, float progress)
{
	CLoadScreen::UpdateProgress(progress);
	return progress;
}

static int __fastcall DrawProgress_Hook(DWORD _this, DWORD edx, float progress)
{
	CLoadScreen::Draw(&g_pClientGame->m_p2D);
	return 0;
}

DWORD UpdateProgress_Addr = 0x005DF357;
DWORD DrawProgress_Addr = 0x005DAC70;
DWORD HumanHit_Addr = 0x005762A0;

void CGameHooks::InstallHooks()
{
	// Game Hooks
    MafiaSDK::C_Game_Hooks::HookOnGameInit(OnGameInit);
    MafiaSDK::C_Game_Hooks::HookOnGameTick(OnGameTick);
    MafiaSDK::C_Game_Hooks::HookLocalPlayerFallDown(OnLocalPlayerFallDown);

	// Indicator Hooks
    MafiaSDK::C_Indicators_Hooks::HookAfterAllDraw(HookRender2DStuff);

	// Human Hooks
	MafiaSDK::C_Human_Hooks::HookOnHumanHit(OnHumanHit);
	MafiaSDK::C_Human_Hooks::HookHumanDoWeaponChange(OnHumanWeaponChange);
	MafiaSDK::C_Human_Hooks::HookHumanDoWeaponDrop(OnHumanWeaponDrop);
	MafiaSDK::C_Human_Hooks::HookOnHumanShoot(OnHumanShoot);

	// Remove dropped clip
	new CHackJumpHack(g_pHack, (void*)0x0058D4C6, (void*)0x0058D553, 6); 

	// Disable local player weapon drop
	new CHackJumpHack(g_pHack, (void*)0x00585D90, (void*)0x00585DCB, 6); 

	// Disable weapon drops
	new CHackJumpHack(g_pHack, (void*)0x0057FAA0, (void*)0x00580196, 6); 

	// Game Exit
	new CHackJumpHack(g_pHack, (void*)0x00612485, (void*)OnGameExit_Hook, 6);
	new CHackJumpHack(g_pHack, (void*)0x005A7F44, (void*)0x005A7F4B, 6);

	// Hook UseActor
	new CHackJumpHack(g_pHack, (void*)0x582180, HookHumanUseActor, 6);
	g_ReturnHumanUseActor = (void*)(0x582180 + 6);

	// Hook DoThrowCocotFromCar (jack/steal car)
	new CHackJumpHack(g_pHack, (void*)0x00587D70, HookHumanDoThrowCocotFromCar, 7);
	g_ReturnHumanDoThrowCocotFromCar = (void*)(0x00587D70 + 7);

	// Hook CreateActor
	new CHackJumpHack(g_pHack, (void*)0x0053F7D0, HookCreateActor, 6);
	g_ReturnCreateActor = (void*)(0x0053F7D0 + 6);

	// Note (Sevenisko): Currently unnecessary to work on
	new CHackJumpHack(g_pHack, (void*)UpdateProgress_Addr, &SetProgress_Hook, 6);

	// Fix exiting vehicle for passengers.
	new CHackValueHack(g_pHack, (void*)0x595040, 1, (uint8_t)0xEB);
}