/*
	Copyright 2017 Dávid Svitana

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

#ifndef _CDROP_IN_WEAPON_H_
#define _CDROP_In_WEAPON_H_

namespace MafiaSDK
{
	struct C_Drop_In_Weapon_Interface
	{
		PADDING(C_Drop_In_Weapon_Interface, _pad0, 0x38C);
		S_GameItem DropItem;
	};

	namespace C_Drop_In_Weapon_Enum
	{
		enum FunctionsAddresses
		{
			Construct = 0x004435B0,
			Destruct = 0x00443620
		};
	};

	class C_Drop_In_Weapon : public C_Actor
	{
	public:
		C_Drop_In_Weapon_Interface* GetInterface()
		{
			return reinterpret_cast<C_Drop_In_Weapon_Interface*>(this);
		}
		
		~C_Drop_In_Weapon()
		{
			unsigned long funcAddr = C_Drop_In_Weapon_Enum::FunctionsAddresses::Destruct;

			__asm
			{
				mov ecx, this
				call funcAddr
			}
		}

		C_Drop_In_Weapon()
		{
			unsigned long funcAddr = C_Drop_In_Weapon_Enum::FunctionsAddresses::Construct;
			unsigned long funcAlloc = LS3D_Alloc;
			void* allocated = malloc(0x3D4);
			__asm
			{		
				mov ecx, allocated
				call funcAddr
				mov this, eax
			}
		}
		
		void Init(I3D_Frame* frame) 
		{
			__asm
			{
				push frame
				mov edi, this
				mov eax, [edi]
				mov ecx, edi
				call dword ptr ds : [ eax + 0x48 ]
			}
		}
	};
};

#endif