/** @file CPlayer2.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CEntity.hpp"
#include "CHuman2.hpp"

#include <functional>

namespace M2
{
    enum E_PlayerMovementType : BYTE
    {
        E_MOVEMENT_WALK = 0,
        E_MOVEMENT_JOG = 1,
        E_MOVEMENT_SPRINT = 2,
        E_MOVEMENT_IDLE = 3,
        E_MOVEMENT_STOPPING = 5
    };

    class S_InputCtrl
    {
    public:
        pad(S_InputCtrl, pad0, 0xA0);                           // 0000 - 00A0
        E_PlayerMovementType    m_ePlayerMovementState;         // 00A0 - 00A1
        pad(S_InputCtrl, pad1, 0x5F);                           // 00A1 - 0100
        bool                    m_bCrouching;                   // 0100 - 0104

    };

	class ICPlayer2
	{
	public:
        pad(ICPlayer2, pad0, 0x240);            // 0000 - 0240
        S_InputCtrl    m_pPlayerControls;       // 0240 - 0244
	};

	class C_Player2 : public ICPlayer2
	{
	public:
        bool IsControlLockFinished()
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x426FD0, this);
        }

		void LockControls(bool lock)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x438230, this, lock);
		}

        void UpdateMoveCmd(S_HumanCommandMoveDir *command, S_InputCtrl &input, Vector3 const &pos, Vector2 &dir, float unk)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x4268C0, this, command, &input, &pos, &dir, unk);
        }
	};

#ifdef MAFIA_SDK_IMPLEMENTATION
    namespace C_Player2_Hooks
    {
        void HookEnterCar(std::function<void(M2::C_Player2 *player, M2::C_Actor *car, char seat)>);

        namespace FunctionPointers
        {
            std::function<void(M2::C_Player2 *player, M2::C_Actor *car, char seat)> enterCar;
        };

        namespace Functions
        {
            inline void EnterCar(M2::C_Player2 *player, M2::C_Actor *car, char seat)
            {
                if (FunctionPointers::enterCar != nullptr) {
                    FunctionPointers::enterCar(player, car, seat);
                }
            }
        };

        namespace NakedFunctions
        {
            DWORD CPlayer__EnterCar__Call = 0x42CAC0;
            DWORD CPlayer__EnterCar_JumpBack = 0x437945;
            void __declspec(naked) CPlayer2__EnterCar()
            {
                __asm {
                    mov eax, dword ptr ss : [esp + 0x10]
                    mov ecx, dword ptr ds : [edi + 0x44]

                    pushad
                    push eax
                    push ecx
                    push esi
                    call Functions::EnterCar;
                    add esp, 0xC
                    popad

                    push eax
                    push ecx
                    mov ecx, esi
                    call CPlayer__EnterCar__Call
                    jmp CPlayer__EnterCar_JumpBack
                }
            }
        };

        void HookEnterCar(std::function<void(M2::C_Player2 *player, M2::C_Actor *car, char seat)> ptr)
        {
            FunctionPointers::enterCar = ptr;
            Mem::Hooks::InstallJmpPatch(0x437935, (DWORD)NakedFunctions::CPlayer2__EnterCar);
        }
    };
#endif
};
