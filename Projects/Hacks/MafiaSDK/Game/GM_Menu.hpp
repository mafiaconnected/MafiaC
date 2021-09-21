/*
Copyright 2018 Dávid Svitana

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _CGAME_MENU_H_
#define _CGAME_MENU_H_

namespace MafiaSDK
{
	struct GM_Menu_Interface
	{
	
	};

	namespace GM_Menu_Enum
	{
		enum FunctionsAddresses
		{
			GM_Menu_CLASS = 0x6BD890,
			Construct = 0x005EA1F0,
			FindComponentByID = 0x005EB2F0,
			SetTextID = 0x005EAFD0,
			GetTextID = 0x005EAFFD,
			SetText = 0x005EB010,
			SetString = 0x005EB030,
			GetText = 0x005EB050,
			MenuClose = 0x005EAE10,
			MenuCloseAll = 0x005EAE40,
			Destroy = 0x005EA690,
			PridejComponentPod = 0x005EB7D0
		};
	};

	class GM_Menu;
	namespace GM_Menu_Hooks
	{
		void HookOnMenuItemClick(std::function<void(unsigned int)> funcitonPointer);
		void OnMenuCreated(GM_Menu* menu);

#ifdef MAFIA_SDK_IMPLEMENTATION
		namespace FunctionsPointers
		{
			std::function<void(unsigned int)> onMenuItemClick;
			std::function<void(GM_Menu*)> onMenuCreated;
			std::function<void(GM_Menu*, unsigned long)> onMenuItemHover;
		};

		namespace Functions
		{
			inline void OnMenuItemClick(unsigned int componentId)
			{
				if (FunctionsPointers::onMenuItemClick != nullptr)
					FunctionsPointers::onMenuItemClick(componentId);
			}

			inline void OnMenuCreated(GM_Menu* menu)
			{
				if (FunctionsPointers::onMenuCreated != nullptr)
					FunctionsPointers::onMenuCreated(menu);
			}

			inline void OnMenuItemHover(GM_Menu* menu, unsigned long component)
			{
				if (FunctionsPointers::onMenuItemHover != nullptr)
					FunctionsPointers::onMenuItemHover(menu, component);
			}
		};

		namespace NakedFunctions
		{
			unsigned long on_menu_itemclick_back = 0x005EAB46;
			__declspec(naked) void OnMenuItemClick_Hook() 
			{
				__asm 
				{
					pushad
						push eax
						call Functions::OnMenuItemClick
						add esp, 0x4
					popad

					mov edx, dword ptr ds : [esi]
					push eax
					mov ecx, esi
					call dword ptr ds : [edx + 0x24]

					jmp on_menu_itemclick_back
				}
			}

			unsigned long on_menu_itemhover_back = 0x005EAB03;
			__declspec(naked) void OnMenuItemHover_Hook()
			{
				__asm
				{
					
					mov eax, dword ptr ds : [esi]
					push ebp
					push edi
					mov ecx, esi
					call dword ptr ds : [eax + 0x2C]

					//ebp, edi
					pushad
					push ebp
					push esi
					call Functions::OnMenuItemHover
					add esp, 0x8
					popad

					jmp on_menu_itemhover_back
				}
			}

			unsigned long on_menu_created_back = 0x005EA320;
			__declspec(naked) void OnMenuCreated_Hook()
			{
				__asm
				{
					mov edx, dword ptr ds : [esi]
					mov ecx, esi
					call dword ptr ds : [edx + 0x4]

					pushad
					push esi
					call Functions::OnMenuCreated
					add esp, 0x4
					popad


					jmp on_menu_created_back
				}
			}
		};

		inline void HookOnMenuItemClick(std::function<void(unsigned int)> functionPointer)
		{
			FunctionsPointers::onMenuItemClick = functionPointer;
			MemoryPatcher::InstallJmpHook(0x005EAB3E, (unsigned long)&NakedFunctions::OnMenuItemClick_Hook);
		}

		inline void HookOnMenuItemHover(std::function<void(GM_Menu*, unsigned long)> functionPointer)
		{
			FunctionsPointers::onMenuItemHover = functionPointer;
			MemoryPatcher::InstallJmpHook(0x005EAAFA, (unsigned long)&NakedFunctions::OnMenuItemHover_Hook);
		}

		inline void HookOnMenuCreated(std::function<void(GM_Menu*)> functionPointer)
		{
			FunctionsPointers::onMenuCreated = functionPointer;
			MemoryPatcher::InstallJmpHook(0x005EA319, (unsigned long)&NakedFunctions::OnMenuCreated_Hook);
		}
#endif
	};

	class GM_Menu
	{
	public:
		GM_Menu_Interface * GetInterface()
		{
			return reinterpret_cast<GM_Menu_Interface*>(this);
		}

		GM_Menu()
		{
			unsigned long funcAddr = GM_Menu_Enum::FunctionsAddresses::Construct;
			void* allocated = malloc(0x34);
			__asm
			{
				mov ecx, allocated
				call funcAddr
				mov this, eax
			}
		}

		unsigned long FindComponentByID(unsigned int id) 
		{
			unsigned long funcAddr = GM_Menu_Enum::FindComponentByID;

			__asm 
			{
				push id
				mov ecx, this
				call funcAddr
			}
		}

		void SetTextID(unsigned int component_id, unsigned int id)
		{
			unsigned long funcAddr = GM_Menu_Enum::SetTextID;

			__asm
			{
				push id
				push component_id
				mov ecx, this
				call funcAddr
			}
		}

		unsigned int GetTextID(unsigned int component_id)
		{
			unsigned long funcAddr = GM_Menu_Enum::GetTextID;

			__asm
			{
				push component_id
				mov ecx, this
				call funcAddr
			}
		}

		char* GetText(unsigned int component_id)
		{
			unsigned long funcAddr = GM_Menu_Enum::GetText;

			__asm
			{
				push component_id
				mov ecx, this
				call funcAddr
			}
		}

		/* 
		* Used for setting text of component's like text edit 
		*/
		void SetText(unsigned int component_id, const char* text)
		{
			unsigned long funcAddr = GM_Menu_Enum::SetText;

			__asm
			{
				push text
				push component_id
				mov ecx, this
				call funcAddr
			}
		}

		/* 
		* Function used for setting images bitmap file
		* and others properties of compoenent
		*/
		void SetString(unsigned int component_id, const char* text)
		{
			unsigned long funcAddr = GM_Menu_Enum::SetString;

			__asm
			{
				push text
				push component_id
				mov ecx, this
				call funcAddr
			}
		}

		void MenuClose(unsigned int component_id)
		{
			unsigned long funcAddr = GM_Menu_Enum::MenuClose;

			__asm
			{
				push component_id
				mov ecx, this
				call funcAddr
			}
		}

		void MenuCloseAll(unsigned int component_id, unsigned int unk)
		{
			unsigned long funcAddr = GM_Menu_Enum::MenuCloseAll;

			__asm
			{
				push unk
				push component_id
				mov ecx, this
				call funcAddr
			}
		}

		void Destroy()
		{
			unsigned long funcAddr = GM_Menu_Enum::Destroy;

			__asm
			{
				mov ecx, this
				call funcAddr
			}
		}

		unsigned int PridejComponentPod(unsigned int unk1, unsigned int unk2, unsigned int unk3, unsigned int unk4, unsigned int unk5, unsigned int unk6)
		{
			unsigned long funcAddr = GM_Menu_Enum::PridejComponentPod;

			__asm
			{
				push unk6
				push unk5
				push unk4
				push unk3
				push unk2
				push unk1
				mov ecx, this
				call funcAddr
			}
		}

		void ReturnFromMenuExecute(int return_number) 
		{
			*(unsigned long*)(0x006BD8A8) = return_number;
		}

		unsigned int GetComponentID(unsigned long component)
		{
			return *(unsigned int*)(component + 0x4);
		}
	};

	inline GM_Menu* GetGMMenu()
	{
		return *reinterpret_cast<GM_Menu**>(GM_Menu_Enum::FunctionsAddresses::GM_Menu_CLASS);
	}

};

#endif
