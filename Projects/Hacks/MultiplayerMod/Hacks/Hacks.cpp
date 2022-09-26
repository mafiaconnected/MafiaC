#include "stdafx.h"

#include "Hacks/Hooks.h"
#include "Hacks/Hacks.h"

//#pragma region "ONE"

DWORD JumpBackMenu = 0x00594896;

__declspec(naked) void HookMultipleMenus()
{
    __asm {
        mov eax, 0x0a9
        jmp JumpBackMenu
    }
}

__declspec(naked) void RETN4() {
    __asm retn 0x4
}

void CGameHacks::EnableGameMap(bool state)
{
    *(BOOL*)(0x006C406C) = state;
}

void CGameHacks::InstallHacks()
{
    MafiaSDK::C_Game_Patches::PatchDisableLogos();
    MafiaSDK::C_Game_Patches::PatchDisableInventory();
    MafiaSDK::C_Game_Patches::PatchDisablePauseMenu();
    MafiaSDK::C_Game_Patches::PatchRemovePlayer();
    MafiaSDK::C_Game_Patches::PatchDisableGameMenu();
    MafiaSDK::C_Game_Patches::PatchDisableSuspendProcess();
    MafiaSDK::C_Game_Patches::PatchDisableGameScripting();
    MafiaSDK::C_Game_Patches::PatchCustomPlayerRespawning();
    MafiaSDK::C_Game_Patches::PatchJumpToGame("FREERIDE");

    //BYTE pCarPhys[] = { 0xE9, 0xF1, 0x00, 0x00, 0x90 };
    //MemoryPatcher::PatchAddress(0x0004E034A, pCarPhys, sizeof(pCarPhys));

    MemoryPatcher::InstallJmpHook(0x00594885, (DWORD)&HookMultipleMenus);

    // Patch global mutex (allows multiple game instances)
    MemoryPatcher::InstallNopPatch(0x5BEC27, 6);
    //new CHackNOPHack(g_pHack, 0x5BEC27, 6);

	// Force enable game map
	*(BOOL *)(0x006C406C) = true;
}