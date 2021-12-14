#include "stdafx.h"

#include "Hacks/Hooks.h"
#include "Hacks/Hacks.h"

__declspec(naked) void RETN4() {
    __asm retn 0x4
}

void CGameHacks::InstallHacks()
{

}