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

#ifndef _I3D_Sector_H_
#define _I3D_Sector_H_

namespace MafiaSDK
{
	struct I3D_Sector_Interface
	{
		
	};

	namespace I3D_Sector_Enum
	{
		enum FunctionsAddresses
		{
			
		};
	};

	class I3D_Sector : public I3D_Frame
	{
	public:
		I3D_Sector_Interface* GetInterface()
		{
			return reinterpret_cast<I3D_Sector_Interface*>(this);
		}
		
		void AddLight(I3D_Light* lightToAdd)
		{
			__asm
			{
				mov eax, this
				mov ecx, dword ptr ds : [eax]
				push lightToAdd
				push eax
				call dword ptr ds : [ecx + 0x50]
			}
		}
		
		void DeleteLight(I3D_Light* lightToRemove)
		{
			__asm
			{
				mov eax, this
				mov ecx, dword ptr ds : [eax]
				push lightToRemove
				push eax
				call dword ptr ds : [ecx + 0x54]
			}
		}

		void AddSound(I3D_Sound* soundToAdd)
		{
			__asm
			{
				mov eax, this
				mov ecx, dword ptr ds : [eax]
				push soundToAdd
				push eax
				call dword ptr ds : [ecx + 0x58]
			}
		}

		void RemoveSound(I3D_Sound* soundToRemove)
		{
			__asm
			{
				mov eax, this
				mov ecx, dword ptr ds : [eax]
				push soundToRemove
				push eax
				call dword ptr ds : [ecx + 0x5C]
			}
		}
	};
}

#endif