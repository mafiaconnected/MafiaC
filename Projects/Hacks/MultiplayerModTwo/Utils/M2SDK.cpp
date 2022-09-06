#include "stdafx.h"

/*
using namespace Mafia2SDK;

namespace Mafia2SDK
{

	namespace C_Human2_Hooks
	{
		void HookSetupDeath(std::function<void(M2::C_Human2* human, M2::C_EntityMessageDamage* message)>);
		void HookDoDamage(std::function<void(M2::C_Human2 *human, M2::C_EntityMessageDamage *message)>);

		namespace FunctionPointers
		{
			std::function<void(M2::C_Human2* human, M2::C_EntityMessageDamage* message)> setupDeath;
			std::function<void(M2::C_Human2 *human, M2::C_EntityMessageDamage *message)> doDamage;
		};

		namespace Functions
		{
			inline void SetupDeath(M2::C_Human2* human, M2::C_EntityMessageDamage* message)
			{
				if (FunctionPointers::setupDeath != nullptr) {
					FunctionPointers::setupDeath(human, message);
				}
			}

			inline void DoDamage(M2::C_Human2* human, M2::C_EntityMessageDamage* message)
			{
				if (FunctionPointers::doDamage != nullptr) {
					FunctionPointers::doDamage(human, message);
				}
			}
		};

		namespace NakedFunctions
		{
			DWORD MineDeathHook_JumpBack = 0x00990CFF;
			DWORD _CHuman2__SetupDeath = 0x0098C160;
			inline void __declspec(naked) CHuman2__SetupDeath_Hook()
			{
				__asm {
					pushad
					push esi
					push ebp
					call Functions::SetupDeath
					add esp, 0x8
					popad

					push    esi
					mov     ecx, ebp
					call    _CHuman2__SetupDeath

					jmp MineDeathHook_JumpBack
				}
			}

			DWORD _CHuman2__DoDamage = 0x09907D0;
			DWORD _DoDamage__JumpBack = 0x042FC6F;
			inline void __declspec(naked) CHuman2__DoDamage__Hook()
			{
				__asm
				{
					pushad;
					push esi;
					push edi;
					call Functions::DoDamage;
					add esp, 0x8;
					popad;

					push edi;
					mov ecx, esi;
					call _CHuman2__DoDamage;

					jmp _DoDamage__JumpBack;
				}
			}
		};

		inline void HookSetupDeath(std::function<void(M2::C_Human2* human, M2::C_EntityMessageDamage* message)> ptr)
		{
			FunctionPointers::setupDeath = ptr;
			Mem::Hooks::InstallJmpPatch(0x00990CF7, (DWORD)NakedFunctions::CHuman2__SetupDeath_Hook);
		}

		inline void HookDoDamage(std::function<void(M2::C_Human2* human, M2::C_EntityMessageDamage* message)> ptr)
		{
			FunctionPointers::doDamage = ptr;
			Mem::Hooks::InstallJmpPatch(0x042FC63, (DWORD)NakedFunctions::CHuman2__DoDamage__Hook);
		}
	};

	namespace C_Player2_Hooks
	{
		inline void HookEnterCar(std::function<void(M2::C_Player2 *player, M2::C_Actor *car, char seat)>);

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

		inline void HookEnterCar(std::function<void(M2::C_Player2 *player, M2::C_Actor *car, char seat)> ptr)
		{
			FunctionPointers::enterCar = ptr;
			Mem::Hooks::InstallJmpPatch(0x437935, (DWORD)NakedFunctions::CPlayer2__EnterCar);
		}
	};

	namespace C_Human2CarWrapper_Hooks
	{
		inline void HookIsFreeToGetIn(std::function<bool(C_Car *)>);

		namespace FunctionPointers
		{
			std::function<bool(C_Car *)> isFreeToGetIn;
		};

		namespace Functions
		{
			inline bool TestAction(C_Car *car)
			{
				if (FunctionPointers::isFreeToGetIn != nullptr) {
					return FunctionPointers::isFreeToGetIn(car);
				}
				return false;
			}
		};

		namespace NakedFunctions
		{
			DWORD CHuman2CarWrapper__GetCar = 0x9235F0;
			C_Car *tryCar = nullptr;
			C_Human2CarWrapper *carWrapper;
			bool tryAnswer = false;
			inline void __declspec(naked) CHuman2CarWrapper__IsFreeToGetIn__Hook()
			{
				__asm {
					mov carWrapper, esi;
				}

				if (carWrapper != nullptr) {
					tryCar = carWrapper->GetCar();
					tryAnswer = Functions::TestAction(tryCar);
				}

				__asm {
					mov     al, tryAnswer
					pop     esi
					retn    8
				}
			}
		};

		inline void HookIsFreeToGetIn(std::function<bool(C_Car *)> ptr)
		{
			FunctionPointers::isFreeToGetIn = ptr;
			Mem::Hooks::InstallJmpPatch(0x956143, (DWORD)NakedFunctions::CHuman2CarWrapper__IsFreeToGetIn__Hook);
		}
	};

	namespace C_CarActionCloseHood_Hooks
	{
		inline void HookTestAction(std::function<bool(C_Car *)>);

		namespace FunctionPointers
		{
			std::function<bool(C_Car *)> testAction;
		};

		namespace Functions
		{
			inline bool TestAction(C_Car *car)
			{
				if (FunctionPointers::testAction != nullptr) {
					return FunctionPointers::testAction(car);
				}
				return false;
			}
		};

		namespace NakedFunctions
		{
			C_Car *tryHoodCloseCar = nullptr;
			bool tryHoodCloseAnswer;
			void __declspec(naked) CCarActionCloseHood__TestAction__Hook()
			{
				__asm
				{
					mov tryHoodCloseCar, esi;
				}

				tryHoodCloseAnswer = Functions::TestAction(tryHoodCloseCar);


				__asm {
					pop edi;
					mov al, tryHoodCloseAnswer;
					pop esi;
					retn 4;
				}
			}
		};

		inline void HookTestAction(std::function<bool(C_Car *)> ptr)
		{
			FunctionPointers::testAction = ptr;
			Mem::Hooks::InstallJmpPatch(0xA3F05C, (DWORD)NakedFunctions::CCarActionCloseHood__TestAction__Hook);
		}
	};

	namespace C_CarActionBreakIn_Hooks
	{
		inline void HookTestAction(std::function<bool(C_Car *)>);

		namespace FunctionPointers
		{
			std::function<bool(C_Car *)> testAction;
		};

		namespace Functions
		{
			inline bool TestAction(C_Car *car)
			{
				if (FunctionPointers::testAction != nullptr) {
					return FunctionPointers::testAction(car);
				}
				return false;
			}
		};

		namespace NakedFunctions
		{
			C_Car *tryCar = nullptr;
			bool tryAnswer = false;
			void __declspec(naked) CCarActionBreakIn__TestAction__Hook()
			{
				__asm
				{
					mov tryCar, esi;
				}

				tryAnswer = Functions::TestAction(tryCar);

				__asm {
					pop     edi;
					pop     esi;
					mov     al, tryAnswer;
					pop     ebx;
					retn    4;
				}
			}
		};

		inline void HookTestAction(std::function<bool(C_Car *)> ptr)
		{
			FunctionPointers::testAction = ptr;
			Mem::Hooks::InstallJmpPatch(0xA3F0A6, (DWORD)NakedFunctions::CCarActionBreakIn__TestAction__Hook);
		}
	};

	namespace C_CarActionEnter_Hooks
	{
		inline void HookTestAction(std::function<bool(C_Car *)>);

		namespace FunctionPointers
		{
			std::function<bool(C_Car *)> testAction;
		};

		namespace Functions
		{
			inline bool TestAction(C_Car *car)
			{
				if (FunctionPointers::testAction != nullptr) {
					return FunctionPointers::testAction(car);
				}
				return false;
			}
		};

		namespace NakedFunctions
		{
			C_Car *tryCar = nullptr;
			bool tryAnswer = false;
			void __declspec(naked) CCarActionEnter__TestAction__Hook()
			{
				__asm
				{
					mov tryCar, esi;
				}

				tryAnswer = Functions::TestAction(tryCar);

				__asm {
					pop     edi;
					mov     al, tryAnswer;
					pop     esi;
					add     esp, 0Ch;
					retn    4;
				}
			}
		};

		inline void HookTestAction(std::function<bool(C_Car *)> ptr)
		{
			FunctionPointers::testAction = ptr;
			Mem::Hooks::InstallJmpPatch(0xA3E8E1, (DWORD)NakedFunctions::CCarActionEnter__TestAction__Hook);
		}
	};

	namespace C_CarActionOpenHood_Hooks
	{
		inline void HookTestAction(std::function<bool(C_Car *)>);

		namespace FunctionPointers
		{
			std::function<bool(C_Car *)> testAction;
		};

		namespace Functions
		{
			inline bool TestAction(C_Car *car)
			{
				if (FunctionPointers::testAction != nullptr) {
					return FunctionPointers::testAction(car);
				}
				return false;
			}
		};

		namespace NakedFunctions
		{
			C_Car *tryHoodOpenCar = nullptr;
			bool tryHoodOpenAnswer;
			void __declspec(naked) CCarActionOpenHood__TestAction__Hook()
			{
				__asm
				{
					mov tryHoodOpenCar, esi;
				}

				tryHoodOpenAnswer = Functions::TestAction(tryHoodOpenCar);


				__asm {
					pop     edi;
					pop     esi;
					mov     al, tryHoodOpenAnswer;
					pop     ebx;
					retn    4;
				}
			}
		};

		inline void HookTestAction(std::function<bool(C_Car *)> ptr)
		{
			FunctionPointers::testAction = ptr;
			Mem::Hooks::InstallJmpPatch(0xA23088, (DWORD)NakedFunctions::CCarActionOpenHood__TestAction__Hook);
		}
	};

	namespace C_CarActionOpenTrunk_Hooks
	{
		inline void HookTestAction(std::function<bool(C_Car *)>);

		namespace FunctionPointers
		{
			std::function<bool(C_Car *)> testAction;
		};

		namespace Functions
		{
			inline bool TestAction(C_Car *car)
			{
				if (FunctionPointers::testAction != nullptr) {
					return FunctionPointers::testAction(car);
				}
				return false;
			}
		};

		namespace NakedFunctions
		{
			C_Car *tryCar = nullptr;
			bool tryAnswer = false;
			void __declspec(naked) CCarActionOpenTrunk__TestAction__Hook()
			{
				__asm
				{
					mov tryCar, esi;
				}

				tryAnswer = Functions::TestAction(tryCar);


				__asm {
					pop edi;
					pop esi;
					mov al, tryAnswer;
					pop ebx;
					retn 4;
				}
			}
		};

		inline void HookTestAction(std::function<bool(C_Car *)> ptr)
		{
			FunctionPointers::testAction = ptr;
			Mem::Hooks::InstallJmpPatch(0xA3EC95, (DWORD)NakedFunctions::CCarActionOpenTrunk__TestAction__Hook);
		}
	};

	namespace C_CarActionTankFuel_Hooks
	{
		inline void HookTestAction(std::function<bool(C_Car *)>);

		namespace FunctionPointers
		{
			std::function<bool(C_Car *)> testAction;
		};

		namespace Functions
		{
			inline bool TestAction(C_Car *car)
			{
				if (FunctionPointers::testAction != nullptr) {
					return FunctionPointers::testAction(car);
				}
				return false;
			}
		};

		namespace NakedFunctions
		{
			C_Car *tryCar = nullptr;
			bool tryAnswer = false;
			void __declspec(naked) CCarActionTankFuel__TestAction__Hook()
			{
				__asm
				{
					mov tryCar, esi;
				}

				tryAnswer = Functions::TestAction(tryCar);

				__asm {
					mov al, tryAnswer;
					pop esi;
					retn 4;
				}
			}
		};

		inline void HookTestAction(std::function<bool(C_Car *)> ptr)
		{
			FunctionPointers::testAction = ptr;
			Mem::Hooks::InstallJmpPatch(0xA23482, (DWORD)NakedFunctions::CCarActionTankFuel__TestAction__Hook);
		}
	};

	namespace C_CarActionThrowFrom_Hooks
	{
		inline void HookTestAction(std::function<bool(C_Car *)>);

		namespace FunctionPointers
		{
			std::function<bool(C_Car *)> testAction;
		};

		namespace Functions
		{
			inline bool TestAction(C_Car *car)
			{
				if (FunctionPointers::testAction != nullptr && car != nullptr) {
					return FunctionPointers::testAction(car);
				}
				return false;
			}
		};

		namespace NakedFunctions
		{
			C_Car *tryCar = nullptr;
			bool tryAnswer = false;
			void __declspec(naked) CCarActionThrowFrom__TestAction__Hook()
			{
				__asm
				{
					mov tryCar, esi;
				}

				tryAnswer = Functions::TestAction(tryCar);

				__asm {
					pop edi;
					mov al, tryAnswer;
					pop esi;
					retn 4;
				}
			}
		};

		inline void HookTestAction(std::function<bool(C_Car *)> ptr)
		{
			FunctionPointers::testAction = ptr;
			Mem::Hooks::InstallJmpPatch(0xA3F189, (DWORD)NakedFunctions::CCarActionThrowFrom__TestAction__Hook);
		}
	};
}
*/