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
//extern std::unordered_map<m2sdk_event_id, m2sdk_callback_event> g_events;

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

		int width = 0;
		int height = 0;

		if (M2::C_MafiaFramework::GetActive())
		{
			HWND pWindow = GetActiveWindow();
			RECT rect;

			if (GetWindowRect(pWindow, &rect))
			{
				width = rect.right - rect.left;
				height = rect.bottom - rect.top;
			}

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

	//M2::Initialize([]);

	Mem::Initialize();

	/*
	// Hooking game module registering
	Mem::Hooks::InstallJmpPatch(0x4F2F05, (DWORD)GameModuleInstall, 5);

	// vehicle hooks
	GameStartDrive__Return = Mem::Hooks::InstallNotDumbJMP(0x043B305, (M2_Address)GameStartDriveHook__1);
	GameStartDrive_2__Return = Mem::Hooks::InstallNotDumbJMP(0x43B394, (M2_Address)GameStartDriveHook__2);
	GameStartDrive_3__Return = Mem::Hooks::InstallNotDumbJMP(0x437940, (M2_Address)GameStartDriveHook__3);
	GameEndDrive__Return = Mem::Hooks::InstallNotDumbJMP(0x43BAAD, (M2_Address)GameEndDriveHook);

	// Crash fix on C_Frame::Release
	Mem::Hooks::InstallJmpPatch(0x14E5BC0, (DWORD)FrameReleaseFix);
	Mem::Hooks::InstallJmpPatch(0x12F0DB0, (DWORD)FrameReleaseFix2);

	//_CHuman2__AddCommand = (DWORD)Mem::Hooks::InstallNotDumbJMP(0x94D400, (DWORD)CHuman2__AddCommand, 5);
	__LoadCityPart = (DWORD)Mem::Hooks::InstallNotDumbJMP(0x4743C0, (DWORD)LoadCityPartsHook, 5);

	// Entity Messages hooks
	onReceiveMessage = (CScriptEntity__RecvMessage_t)Mem::Hooks::InstallJmpPatch(0x117BCA0, (DWORD)OnReceiveMessageHook);

	// Player input hook
	CPlayer2__UpdateInput__Return = Mem::Hooks::InstallNotDumbJMP(0x43BD42, (M2_Address)CPlayer2__UpdateInput);

	// noop the CreateMutex, allow to run multiple instances
	Mem::Hooks::InstallJmpPatch(0x00401B89, 0x00401C16);

	// Always use vec3
	*(BYTE *)0x09513EB = 0x75;
	*(BYTE *)0x0950D61 = 0x75;

	// Disable game controlling engine state and radio
	//Mem::Hooks::InstallJmpPatch(0x956362, 0x9563B6); // When leaving car
	//Mem::Hooks::InstallJmpPatch(0x95621A, 0x956333); // When entering car

	// Disable game pause when minimized or in background
	Mem::Hooks::InstallJmpPatch(0xAC6D2B, 0xAC6F79);
	Mem::Hooks::InstallJmpPatch(0xAC6E57, 0xAC6F79);

	// Disable game creating vehicle (common/police) map icons
	Mem::Hooks::InstallJmpPatch(0x9CC219, 0x9CC220);//C_Car::OnActivate
	Mem::Hooks::InstallJmpPatch(0x9CC53C, 0x9CC543);//C_Car::OnDeactivate
	Mem::Hooks::InstallJmpPatch(0x4DCABD, 0x4DCAC4);//C_Car::SetSeatOwner
	Mem::Hooks::InstallJmpPatch(0x4DCC7D, 0x4DCC8A);//C_Car::SetSeatOwner

	// Prevent game controlling wipers
	Mem::Hooks::InstallJmpPatch(0x4877F1, 0x487892);//C_Car::UpdateIdleFX
	Mem::Hooks::InstallJmpPatch(0xA151CB, 0xA151D4);//C_Car::InitTuning

	// Disable shop loading
	//Mem::Utilites::PatchAddress(0x4731A0, 0x0004C2);
	//Mem::Utilites::PatchAddress(0xAC4B80, 0x0004C2);

	// Disable garages
	//Mem::Utilites::PatchAddress(0xCD6E90, 0xC300B0);

	// Disable loading screen
	Mem::Utilites::PatchAddress(0x08CA820, 0xC300B0); // mov al, 0; retn

	// Disable DLC loadings (NONO, WE NEED DLCs !)
	//Mem::Utilites::PatchAddress(0x11A62C0, 0xC300B0); // mov al, 0; retn
	//InitializeSDKHandlers();
	*/

	/*
	M2::Initialize([]() {
		/*
		M2::AttachHandler(M2_EVENT_MOD_MESSAGE, [](m2sdk_event* data) {
			auto message = (int)data->arg1;

			switch (message) {
				case M2::E_PlayerMessage::MESSAGE_MOD_ENTER_CAR: {
					auto ped = (M2::C_Human2*)data->arg1;
					auto car = (M2::C_Car*)data->arg2;
					auto seat = (uint32_t)data->arg3;
					g_pClientGame->HumanEnteringVehicle(g_pClientGame->m_pClientManager->FindHuman(ped), g_pClientGame->m_pClientManager->FindVehicle(car), seat, 0, 0);
				} break;

				case M2::E_PlayerMessage::MESSAGE_MOD_BREAKIN_CAR: {
					// Todo
				} break;

				case M2::E_PlayerMessage::MESSAGE_MOD_LEAVE_CAR: {
					auto ped = (M2::C_Human2*)data->arg1;
					auto car = (M2::C_Car*)data->arg2;
					auto seat = (uint32_t)data->arg3;
					g_pClientGame->HumanExitingVehicle(g_pClientGame->m_pClientManager->FindHuman(ped), g_pClientGame->m_pClientManager->FindVehicle(car), seat, 0, 0);
				} break;
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
					//mod_log("[game-event] unknown event %d\n", (int)message->m_dwMessage);
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
		*/

		/*
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
	});
	*/
}