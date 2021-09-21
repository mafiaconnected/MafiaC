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

#ifndef _I3D_Light_H_
#define _I3D_Light_H_

namespace MafiaSDK
{
	namespace I3D_Light_Enum
	{
		enum FunctionsAddresses
		{
			Construct = 0x100201F0,
			Destruct = 0x10020760,
			SetLightType = 0x10020800,
			SetColor = 0x10020930,
			SetColorFloat = 0x100209E0,
			SetPower = 0x10020A90,
			SetRange = 0x10020B20,
			SetCone = 0x10020C30,
			Update = 0x10020D10,
			SetOn = 0x10021400,
			SetMode = 0x10021440,
			Duplicate = 0x10021490,
			SaveImage = 0x10021531,
			LoadImage = 0x10021550
		};
	};

	class I3D_Light : public I3D_Frame
	{
	public:
		I3D_Light()
		{
			unsigned long funcAddress = I3D_Light_Enum::FunctionsAddresses::Construct;
			unsigned long funcLS3DAlloc = LS3D_Alloc;

			__asm
			{
				push 0x1F0
				call funcLS3DAlloc
				add esp, 0x4

				mov ecx, eax
				call funcAddress
				mov this, eax
			}
		}

		~I3D_Light()
		{
			__asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
				push 1
				call dword ptr ds : [eax + 0x40]
			}
		}

		void SetLightType(int lightType)
		{
			__asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
				push lightType
				push edi
				call dword ptr ds : [eax + 0x50]
			}
		}

		void SetColor(S_vector & lightColor)
		{
			__asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
				push lightColor
				push edi
				call dword ptr ds : [eax + 0x58]
			}
		}

		void SetPower(float lightPower)
		{
			__asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
				push lightPower
				push edi
				call dword ptr ds : [eax + 0x5C]
			}
		}

		void SetMode(int lightMode)
		{
			__asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
				push lightMode
				push edi
				call dword ptr ds : [eax + 0x60]
			}
		}

		void SetRange(float unk1, float unk2)
		{
			__asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
				push unk1
				push unk2
				push edi
				call dword ptr ds : [eax + 0x64]
			}
		}

		void SetCone(float unk1, float unk2)
		{
			__asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
				push unk1
				push unk2
				push edi
				call dword ptr ds : [eax + 0x68]
			}
		}
	};
};

#endif