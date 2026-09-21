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

    namespace C_Game_Patches
    {
        namespace NakedFunctions
        {
            DWORD JumpBackMenu = 0x00594896;

            __declspec(naked) void AllowMultipleMenus()
            {
                __asm {
                    mov eax, 0x0a9
                    jmp JumpBackMenu
                }
            }
        }
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
            std::function<void()> gameExit;
        }

        namespace NakedFunctions
        {
            __declspec(naked) void GameExit()
            {
                __asm
                {
                    pushad
                    call Functions::GameExit
                    popad

                    retn
                }
            }

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
            std::function<bool(MafiaSDK::C_Human*, MafiaSDK::C_Actor*, int, int, int)> useActor;
            std::function<void(MafiaSDK::C_Human*, MafiaSDK::C_Car*, int)> doThrowCocotFromCar;
            std::function<void(MafiaSDK::C_Human*, const S_vector&)> humanSetAimPose;
            std::function<void(MafiaSDK::C_Human*, const S_vector&)> humanSetNormalPose;
            std::function<void(MafiaSDK::C_Human*, const S_vector&)> humanThrowGrenade;
            std::function<bool(MafiaSDK::C_Human*)> humanVehicleAim;
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

            void* useActorReturn;

            // Human::Use_Actor(C_Actor* actor, int unk1, int unk2, int unk3)
            __declspec(naked) void UseActor()
            {
                __asm
                {
                    // Load the arguments after pushad: EBX/ESI are callee-saved, so clobbering them before it would
                    // leak the argument values back to Use_Actor's caller once popad "restores" them.
                    pushad

                    mov eax, [esp + 0x20 + 4]
                    mov edx, [esp + 0x20 + 8]
                    mov ebx, [esp + 0x20 + 0x0C]
                    mov esi, [esp + 0x20 + 0x10]

                    push esi
                    push ebx
                    push edx
                    push eax
                    push ecx
                    call Functions::UseActor
                    add esp, 0x14

                    // false = don't run the game's own Use_Actor. It's a thiscall taking four stack arguments, so
                    // skipping it means returning to the caller here and popping them (0x10) the way it would have
                    test al, al
                    jz skipUseActor

                    popad
                    sub esp, 0xF8
                    jmp useActorReturn

                skipUseActor:
                    popad
                    xor eax, eax
                    ret 0x10
                }
            }

            void* doThrowCocotFromCarReturn;

            // Human::Do_ThrowCocotFromCar(C_Car* car, int seatId)
            __declspec(naked) void DoThrowCocotFromCar()
            {
                __asm
                {
                    mov eax, [esp + 4]
                    mov edx, [esp + 8]
                    pushad

                    push edx
                    push eax
                    push ecx
                    call Functions::DoThrowCocotFromCar
                    add esp, 0x0C

                    popad
                    push 0xFFFFFFFF
                    push 0x6206A2 // 0x587D70
                    jmp doThrowCocotFromCarReturn
                }
            }

            void* setAimPoseReturn;

            // Human::PoseSetPoseAimed(S_vector pose)
            __declspec(naked) void SetAimPose()
            {
                __asm
                {
                    lea eax, [esp + 4]
                    pushad

                    push eax
                    push ecx
                    call Functions::HumanSetAimPose
                    add esp, 8

                    popad
                    sub esp, 0xB4
                    jmp setAimPoseReturn
                }
            }

            void* setNormalPoseReturn;

            // Human::PoseSetPoseNormal(S_vector pose)
            __declspec(naked) void SetNormalPose()
            {
                __asm
                {
                    lea eax, [esp + 4]
                    pushad

                    push eax
                    push ecx
                    call Functions::HumanSetNormalPose
                    add esp, 8

                    popad
                    sub esp, 0xC0
                    jmp setNormalPoseReturn
                }
            }

            DWORD humanThrowGrenadeReturn = 0x00583A68;
            DWORD humanPersonAnim = 0x00573E50;

            // Grenade throw inside Human::Do_Shoot. Replaces the 18 bytes at 0x00583A56..0x00583A67; the throw
            // target is stored at human + 0x200
            __declspec(naked) void HumanThrowGrenade()
            {
                __asm
                {
                    lea eax, [esi + 0x200]
                    pushad

                    push eax
                    push esi
                    call Functions::HumanThrowGrenade
                    add esp, 8

                    popad
                    push ebx
                    push 1
                    push ebx
                    mov ecx, esi
                    mov dword ptr [esi + 0x74], 0xA3
                    call dword ptr [humanPersonAnim] // indirect, so eax stays untouched like the original rel32 call
                    jmp humanThrowGrenadeReturn
                }
            }

            DWORD humanVehicleAimReturn = 0x0057B5CF;

            // The store of the in-vehicle aim in the human's update. Replaces the 12 bytes at 0x0057B5C3..0x0057B5CE
            // (fstp [esi+0x5F4] / fld [esi+0x5F0]). Skipping the store still has to pop the x87 stack the way the fstp
            // did, or every skipped frame leaves one more value on it.
            __declspec(naked) void HumanVehicleAim()
            {
                __asm
                {
                    pushad
                    push esi
                    call Functions::HumanVehicleAim
                    add esp, 4
                    test al, al
                    popad // leaves the flags from the test alone
                    je skipStore

                    fstp dword ptr [esi + 0x5F4]
                    jmp storeDone

                skipStore:
                    fstp st(0)

                storeDone:
                    fld dword ptr [esi + 0x5F0]
                    jmp humanVehicleAimReturn
                }
            }
        };
    }

    namespace C_Mission_Hooks
    {
        namespace FunctionsPointers
        {
            std::function<void(C_Mission_Enum::ObjectTypes)> createActor;
        }

        namespace NakedFunctions
        {
            void* createActorReturn;

            // C_Mission::CreateActor(C_Mission_Enum::ObjectTypes actorType)
            __declspec(naked) void CreateActor()
            {
                __asm
                {
                    mov eax, [esp + 4]
                    pushad

                    push eax
                    call Functions::CreateActor
                    add esp, 4

                    popad
                    mov eax, fs:0
                    jmp createActorReturn
                }
            }
        };
    }

    namespace C_Car_Hooks
    {
        namespace FunctionsPointers
        {
            std::function<void(C_Car*)> onUpdate;
        }

        namespace NakedFunctions
        {
            void* updateReturn;

            // C_Car::Update(float dt)
            __declspec(naked) void Update()
            {
                __asm
                {
                    pushad

                    push ecx
                    call Functions::OnUpdate
                    add esp, 4

                    popad
                    mov eax, fs:0
                    jmp updateReturn
                }
            }
        };
    }

    namespace I3D_Model_Hooks
    {
        namespace FunctionsPointers
        {
            std::function<void(uint32_t, const char*)> onOpen;
        }

        namespace NakedFunctions
        {
            void* openReturn;

            // I3D_Model::Open(char const* modelName, unsigned int, callback, void*)
            __declspec(naked) void Open()
            {
                __asm
                {
                    mov eax, [esp + 4]
                    mov edx, [esp + 8]
                    pushad

                    push edx
                    push eax
                    call Functions::OnOpen
                    add esp, 8

                    popad
                    push    ebx
                    mov     ebx, [esp + 8]
                    jmp		openReturn
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

