#include "stdafx.h"

using namespace MafiaSDK;

namespace MafiaSDK
{
    IGraph* GetIGraph()
    {
        return *(IGraph**)(IGraph_Constance);
    }

    bool IsWindowFocused()
    {
        return (HWND)GetIGraph()->GetMainHWND() == GetActiveWindow();
    }

    C_Indicators* GetIndicators()
    {
        return reinterpret_cast<C_Indicators*>(C_Indicators_Enum::GIndicators);
    }

    C_I3D_cache_9I3D_model_* GetModelCache()
    {
        return (C_I3D_cache_9I3D_model_*)C_I3D_cache_9I3D_model_Enum::FunctionsAddresses::C_I3D_cache_9I3D_model;
    }

    I3D_Driver* I3DGetDriver()
    {
        return *(I3D_Driver**)(I3D_Driver_Const);
    }

    namespace C_Game_Hooks
    {
        namespace FunctionsPointers
        {
            std::function<void()> gameTick;
            std::function<void()> gameDone;
            std::function<void()> gameInit;
            std::function<void()> localPlayerFallDown;
            std::function<void(C_Human*, S_vector)> humanOnShoot;
        }

        namespace NakedFunctions
        {
            __declspec(naked) void LocalPlayerFallDown()
            {
                __asm
                {
                    pushad
                    call Functions::LocalPlayerFallDown
                    popad

                    // 0x0057BAB1
                    mov eax, 0x005A545D
                    jmp eax
                }
            }

            __declspec(naked) void GameDone()
            {
                __asm
                {
                    pushad
                    call Functions::GameDone
                    popad

                    retn
                }
            }

            __declspec(naked) void GameInit()
            {
                __asm
                {
                    pushad
                    call Functions::GameInit
                    popad

                    retn
                }
            }

            __declspec(naked) void GameTick()
            {
                __asm
                {
                    pushad
                    call Functions::GameTick
                    popad

                    retn 4
                }
            }


        }
    }

    namespace C_Human_Hooks
    {
        namespace FunctionsPointers
        {
            std::function<int(MafiaSDK::C_Human*, int, const S_vector&, const S_vector&, const S_vector&, float, MafiaSDK::C_Actor*, unsigned long, MafiaSDK::I3D_Frame*)> humanHit;
            std::function<void(const S_vector&)> humanShoot;
            std::function<void(MafiaSDK::C_Human*, byte)> humanDoWeaponChange;
            std::function<void(MafiaSDK::C_Human*)> humanDoWeaponDrop;
        };

        namespace NakedFunctions
        {
            __declspec(naked) void HumanHitOne()
            {
                __asm
                {
                    MOV EAX, DWORD PTR SS : [ESP + 0x30]
                    MOV ECX, DWORD PTR SS : [ESP + 0x2C]
                    MOV EDI, DWORD PTR SS : [ESP + 0x28]
                    MOV EDX, DWORD PTR SS : [ESP + 0x24]
                    PUSH EAX; / Arg8 = 00000001
                    MOV EAX, DWORD PTR SS : [ESP + 0x24] ; |
                    PUSH ECX; | Arg7 = 000000CB
                    MOV ECX, DWORD PTR SS : [ESP + 0x24] ; |
                    PUSH EDI; | Arg6 = 0C691FC8 ASCII "xXb"
                    PUSH EDX; | Arg5 = 00000000
                    MOV EDX, DWORD PTR SS : [ESP + 0x28] ; |
                    PUSH EAX; | Arg4 = 00000001
                    MOV EAX, DWORD PTR SS : [ESP + 0x28] ; |
                    PUSH ECX; | Arg3 = 000000CB
                    PUSH EDX; | Arg2 = 00000000
                    PUSH EAX; | Arg1 = 00000001
                    PUSH ESI
                    CALL Functions::HumanHit
                    ADD ESP, 0x24

                    MOV EAX, 0x00507F10
                    JMP EAX
                }
            }

            __declspec(naked) void HumanHitTwo()
            {
                __asm
                {
                    MOV EAX, DWORD PTR SS : [ESP + 0x24]
                    MOV ECX, DWORD PTR SS : [ESP + 0x20]
                    MOV EDX, DWORD PTR SS : [ESP + 0x1C]
                    PUSH EAX; / Arg8 = 00001001
                    MOV EAX, DWORD PTR SS : [ESP + 0x1C] ; |
                    PUSH ECX; | Arg7 = 00001001
                    MOV ECX, DWORD PTR SS : [ESP + 0x1C] ; |
                    PUSH EDX; | Arg6 = 00000000
                    MOV EDX, DWORD PTR SS : [ESP + 0x1C] ; |
                    PUSH EAX; | Arg5 = 00001001
                    MOV EAX, DWORD PTR SS : [ESP + 0x1C] ; |
                    PUSH ECX; | Arg4 = 00001001
                    MOV ECX, DWORD PTR SS : [ESP + 0x1C] ; |
                    PUSH EDX; | Arg3 = 00000000
                    PUSH EAX; | Arg2 = 00001001
                    PUSH ECX; | Arg1 = 00001001
                    PUSH ESI
                    CALL Functions::HumanHit
                    ADD ESP, 0x24

                    MOV EAX, 0x0059425F
                    JMP EAX
                }
            }

            __declspec(naked) void HumanShoot()
            {
                __asm
                {
                    LEA ECX, DWORD PTR SS : [ESP + 0x38]

                    pushad
                    push ecx
                    call Functions::HumanShoot
                    add esp, 0x4
                    popad

                    PUSH ECX;
                    PUSH 1
                        MOV ECX, ESI
                        MOV EAX, 0x583590
                        CALL EAX

                        mov eax, 0x00591424
                        jmp eax
                }
            }

            __declspec(naked) void HumanDoWeaponChangeOne()
            {
                __asm
                {
                    PUSH ECX; / Arg2 = 00000000
                    PUSH EBP; | Arg1 = 00000000
                    MOV ECX, ESI; |
                    MOV EAX, 0x0057F550; \Game.0057F550
                    CALL EAX

                    pushad
                    mov ax, word ptr ds : [esi + 0x4A0]
                    push ax
                    push esi
                    call Functions::HumanDoWeaponChange
                    add esp, 0x6
                    popad

                    MOV EAX, 0x005903F6
                    JMP EAX
                }
            }


            __declspec(naked) void HumanDoWeaponChangeTwo()
            {
                __asm
                {
                    PUSH ECX; / Arg2 = 00000000
                    PUSH EBP; | Arg1 = 00000000
                    MOV ECX, ESI; |
                    MOV EAX, 0x0057F550; \Game.0057F550
                    CALL EAX

                    pushad
                    mov ax, word ptr ds : [esi + 0x4A0]
                    push ax
                    push esi
                    call Functions::HumanDoWeaponChange
                    add esp, 0x6
                    popad

                    MOV EAX, 0x00590303
                    JMP EAX
                }
            }

            __declspec(naked) void HumanDoWeaponDrop()
            {
                __asm
                {
                    MOV EAX, 0x00585D90
                    CALL EAX

                    pushad
                    push esi
                    call Functions::HumanDoWeaponDrop
                    add esp, 0x4
                    popad

                    MOV EAX, 0x00590252
                    JMP EAX
                }
            }
        };
    }

    namespace C_Indicators_Hooks
    {
        namespace FunctionsPointers
        {
            std::function<void()> afterAllDraw;
        }

        namespace NakedFunctions
        {
            __declspec(naked) void AfterAllDraw()
			{
				__asm
				{
					pushad
						call Functions::AfterAllDraw
					popad

					retn 0x0C
				}
			}
        }
    }
}