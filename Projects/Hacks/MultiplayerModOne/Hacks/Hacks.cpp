#include "stdafx.h"

#include "Hacks/Hooks.h"
#include "Hacks/Hacks.h"

//#pragma region "ONE"

__declspec(naked) void RETN4() {
    __asm retn 0x4
}

void CGameHacks::EnableGameMap(bool state)
{
    MafiaSDK::SetGameMapEnabled(state);
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

    MafiaSDK::C_Game_Patches::PatchAllowMultipleMenus();
    MafiaSDK::C_Game_Patches::PatchAllowMultipleInstances();
}