#include "stdafx.h"

using namespace M2;

namespace M2
{
	m2sdk_callback g_gamemodule_callback = nullptr;

	DWORD GameStartDrive__Return;
	DWORD GameStartDrive_2__Return;
	DWORD GameStartDrive_3__Return;
	DWORD GameEndDrive__Return;
	DWORD _callDrive = 0x042CAC0;
	DWORD _callEnd = 0x99CE70;

	/**
	 * Hooking vehicle methods
	 */
	void __declspec(naked) GameStartDriveHook__1() {
		__asm call[_callDrive];
		__asm pushad;
		{
			m2sdk_event event = { 0 }; event.arg1 = (void*)M2::E_PlayerMessage::MESSAGE_MOD_BREAKIN_CAR;
			M2::TriggerHandler(M2_EVENT_MOD_MESSAGE, &event);
		}
		__asm popad;
		__asm jmp[GameStartDrive__Return];
	}

	void __declspec(naked) GameStartDriveHook__2() {
		__asm call[_callDrive];
		__asm pushad;
		{
			m2sdk_event event = { 0 }; event.arg1 = (void*)M2::E_PlayerMessage::MESSAGE_MOD_BREAKIN_CAR;
			M2::TriggerHandler(M2_EVENT_MOD_MESSAGE, &event);
		}
		__asm popad;
		__asm jmp[GameStartDrive_2__Return];
	}

	static M2::C_Car* car = nullptr;
	void __declspec(naked) GameStartDriveHook__3() {
		__asm call[_callDrive];
		__asm pushad
	{
		m2sdk_event event = { 0 }; event.arg1 = (void*)M2::E_PlayerMessage::MESSAGE_MOD_ENTER_CAR;
		M2::TriggerHandler(M2_EVENT_MOD_MESSAGE, &event);
	}
	__asm popad;
	__asm jmp[GameStartDrive_3__Return];
	}

	void __declspec(naked) GameEndDriveHook() {
		__asm call[_callEnd];
		__asm pushad;
		{
			m2sdk_event event = { 0 }; event.arg1 = (void*)M2::E_PlayerMessage::MESSAGE_MOD_LEAVE_CAR;
			M2::TriggerHandler(M2_EVENT_MOD_MESSAGE, &event);
		}
		__asm popad;
		__asm jmp[GameEndDrive__Return];

	}

	/**
	* Hooks for bugfixes
	*/
	void* _this;
	void _declspec(naked) FrameReleaseFix()
	{
		__asm
		{
			pushad;
			mov _this, esi;
		}

		//TODO: Check if _this != nullptr

		__asm
		{
			popad;
			retn;
		}
	}

	void _declspec(naked) FrameReleaseFix2()
	{
		//TODO: Check if _this != nullptr
		__asm
		{
			retn;
		}
	}

	/* Entity Messages */
	typedef bool(__cdecl* CScriptEntity__RecvMessage_t) (void* lua, void* a2, const char* function, M2::C_EntityMessage* message);
	CScriptEntity__RecvMessage_t onReceiveMessage;
	int OnReceiveMessageHook(void* lua, void* a2, const char* function, M2::C_EntityMessage* pMessage)
	{
		if (pMessage) {
			M2::C_Game* game = M2::C_Game::Get();
			if (game) {
				M2::C_Player2* player = game->GetLocalPed();
				if (player) {
					M2::C_Entity* entity = reinterpret_cast<M2::C_Entity*>(player);
					if (entity) {
						if (pMessage->m_dwReceiveGUID == entity->m_dwGUID) {
							m2sdk_event event = { 0 }; event.arg1 = (void*)pMessage;
							M2::TriggerHandler(M2_EVENT_GAME_MESSAGE, &event);
						}
					}
				}
			}
		}
		return onReceiveMessage(lua, a2, function, pMessage);
	}

	/* Player input process */
	DWORD CPlayer2__UpdateInput__Return;
	DWORD CPlayer2__UpdateInput__Call = 0x42ABE0;
	M2::C_Entity* player = nullptr;
	void __declspec(naked) CPlayer2__UpdateInput()
	{
		__asm call[CPlayer2__UpdateInput__Call];
		__asm mov player, ebx;
		__asm pushad;

		//TODO: Hook here

		__asm popad;
		__asm jmp[CPlayer2__UpdateInput__Return];
	}

	/*
	DWORD _CHuman2__AddCommand;
	void __declspec(naked) CHuman2__AddCommand()
	{
		__asm mov     eax, [esp + 4];
		__asm push    esi;



		__asm pushad;

		static int cmdtype;
		__asm mov cmdtype, eax;

		static char* cmd;
		__asm mov edi, [esp + 16];
		__asm mov cmd, edi;

		mod_log("CHuman2__AddCommand: type %d humancmdptr %x\n", cmdtype, cmd);
		__asm popad;


		__asm jmp[_CHuman2__AddCommand];
	}*/

	DWORD _CHuman2__AddCommand;
	void __declspec(naked) CHuman2__AddCommand()
	{
		__asm mov     eax, [esp + 4];
		__asm push    esi;



		__asm pushad;

		static M2::E_Command cmdtype;
		__asm mov cmdtype, eax;

		static char* cmd;
		__asm mov edi, [esp + 16];
		__asm mov cmd, edi;

		m2sdk_log("CHuman2__AddCommand: type %d humancmdptr %x", cmdtype, cmd);
		__asm popad;


		__asm jmp[_CHuman2__AddCommand];
	}


	/* Game Module Implementation */
	DWORD __GameModuleInstall = 0x4F2F0A;
	void __declspec(naked) GameModuleInstall()
	{
		__asm {
			mov eax, [edx + 1Ch];
			push 0Ah;
		}
		__asm pushad;
		g_gamemodule_callback();
		__asm popad;
		__asm jmp[__GameModuleInstall];
	}

	DWORD __LoadCityPart;
	void __declspec(naked) LoadCityPartsHook()
	{
		__asm {
			push ebx;
			push ebp;
			push esi;
			push edi;
			mov edi, [ecx + 16];
		}
		__asm pushad;
		m2sdk_log("load city part\n");
		__asm popad;
		__asm jmp[__LoadCityPart];
	}


	// =======================================================================//
	// !
	// ! General mod implementation part
	// !
	// =======================================================================//

	void AttachHandler(m2sdk_event_id id, m2sdk_callback_event callback) {
		std::unordered_map<m2sdk_event_id, m2sdk_callback_event> pEvents;
		pEvents[id] = callback;
	}

	void DetachHandler(m2sdk_event_id id) {
		std::unordered_map<m2sdk_event_id, m2sdk_callback_event> pEvents;
		pEvents[id] = nullptr;
	}

	void TriggerHandler(m2sdk_event_id id, m2sdk_event* data) {
		std::unordered_map<m2sdk_event_id, m2sdk_callback_event> pEvents;
		auto pair = pEvents.find(id);
		if (pair != pEvents.end() && pair->second) {
			pair->second(data);
		}
	}

	//extern void m2sdk_preinit();

	void Initialize(m2sdk_callback callback) {
		g_gamemodule_callback = callback;
		//m2sdk_preinit();

		Mem::Initialize();

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
		*(BYTE*)0x09513EB = 0x75;
		*(BYTE*)0x0950D61 = 0x75;

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
		InitializeSDKHandlers();
	}

	void InitializeSDKHandlers()
	{
		C_Door_Hooks::HookSolveContact([&](C_Door* instance, S_ContactEventInfo const& ev, E_DoorContactType contactType) {
			//instance->Lock();
			});

		C_CarActionOpenHood_Hooks::HookTestAction([&](C_Car* car) {
			m2sdk_event event = { 0 }; {
				event.arg1 = (void*)car;
			}

			M2::TriggerHandler(M2_EVENT_CAR_HOOD_OPEN_REQUEST, &event);
			return (bool)event.arg5;
			});

		C_CarActionCloseHood_Hooks::HookTestAction([&](C_Car* car) {
			m2sdk_event event = { 0 }; {
				event.arg1 = (void*)car;
			}

			M2::TriggerHandler(M2_EVENT_CAR_HOOD_CLOSE_REQUEST, &event);
			return (bool)event.arg5;
			});

		C_CarActionOpenTrunk_Hooks::HookTestAction([&](C_Car* car) {
			m2sdk_event event = { 0 }; {
				event.arg1 = (void*)car;
			}

			M2::TriggerHandler(M2_EVENT_CAR_TRUNK_OPEN_REQUEST, &event);
			return (bool)event.arg5;
			});

		C_CarActionCloseTrunk_Hooks::HookTestAction([&](C_Car* car) {
			m2sdk_event event = { 0 }; {
				event.arg1 = (void*)car;
			}

			M2::TriggerHandler(M2_EVENT_CAR_TRUNK_CLOSE_REQUEST, &event);
			return (bool)event.arg5;
			});

		C_CarActionEnter_Hooks::HookTestAction([&](C_Car* car) {
			return true;
			});

		C_CarActionBreakIn_Hooks::HookTestAction([&](C_Car* car) {
			return false;
			});

		C_CarActionTankFuel_Hooks::HookTestAction([&](C_Car* car) {
			m2sdk_event event = { 0 }; {
				event.arg1 = (void*)car;
			}

			M2::TriggerHandler(M2_EVENT_CAR_FUELTANK_REQUEST, &event);
			return (bool)event.arg5;
			});

		C_CarActionThrowFrom_Hooks::HookTestAction([&](C_Car* car) {
			return false;
			});

		C_Human2CarWrapper_Hooks::HookIsFreeToGetIn([&](C_Car* car) {
			m2sdk_event event = { 0 }; {
				event.arg1 = (void*)car;
			}

			M2::TriggerHandler(M2_EVENT_CAR_ENTER_REQUEST, &event);
			return (bool)event.arg5;
			});

		C_Player2_Hooks::HookEnterCar([&](C_Player2* player, C_Actor* car, char seat) {
			m2sdk_event event = { 0 }; {
				event.arg1 = (void*)player;
				event.arg2 = (void*)car;
				event.arg3 = (void*)seat;
			}

			M2::TriggerHandler(M2_EVENT_CAR_ENTER, &event);
			});

		C_Human2_Hooks::HookSetupDeath([&](C_Human2* human, C_EntityMessageDamage* message) {
			if (human == reinterpret_cast<C_Human2*>(C_Game::Get()->GetLocalPed())) {
				m2sdk_log("The player just died\n");
			}
			else {
				m2sdk_log("An human just died\n");
			}
			});

		C_Human2_Hooks::HookDoDamage([&](C_Human2* human, C_EntityMessageDamage* message) {
			printf("damage lol\n");
			});
	}

	void M2::Free() {}

	namespace C_Human2_Hooks
	{
		inline void HookSetupDeath(std::function<void(M2::C_Human2* human, M2::C_EntityMessageDamage* message)>);
		inline void HookDoDamage(std::function<void(M2::C_Human2 *human, M2::C_EntityMessageDamage *message)>);

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

	namespace C_Door_Hooks
	{
		void HookSolveContact(std::function<void(C_Door*, S_ContactEventInfo const&, E_DoorContactType)>);
		DWORD HookSolveContact__Return;

		namespace FunctionPointers
		{
			std::function<void(C_Door*, S_ContactEventInfo const&, E_DoorContactType)> solveContact;
		}

		namespace Functions
		{
			inline void SolveContact(C_Door* thisInstance, S_ContactEventInfo const& ev, E_DoorContactType contactType)
			{
				if (FunctionPointers::solveContact != nullptr) {
					FunctionPointers::solveContact(thisInstance, ev, contactType);
				}
			}
		}

		namespace NakedFunctions
		{
			inline void __declspec(naked) SolveContact()
			{
				__asm {
					sub esp, 10h;
					mov eax, [ecx + 358h];

					pushad;
				}

				static C_Door* instance = nullptr;
				static S_ContactEventInfo info;
				static void* infoPtr = nullptr;
				static E_DoorContactType eventType;

				__asm {
					mov instance, ecx;

					mov ecx, [esp + 24];
					mov infoPtr, ecx;

					mov eax, [esp + 28];
					mov eventType, eax;
				}

				if (infoPtr) {
					memcpy(&info, infoPtr, sizeof infoPtr);
				}

				Functions::SolveContact(instance, info, eventType);

				__asm {
					popad;
					jmp[HookSolveContact__Return];
				}
			}
		}

		inline void HookSolveContact(std::function<void(C_Door*, S_ContactEventInfo const& ev, E_DoorContactType contactType)> ptr)
		{
			FunctionPointers::solveContact = ptr;

			HookSolveContact__Return = Mem::Hooks::InstallNotDumbJMP(0x4F1190, (M2_Address)NakedFunctions::SolveContact, 9);
		}
	}
}