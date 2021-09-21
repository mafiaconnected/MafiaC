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

#ifndef _ISND_Source_H_
#define _ISND_Source_H_

namespace MafiaSDK
{
	namespace ISND_Source_Enum
	{
		
	};

	struct S_wave_format 
	{
		DWORD unk1;
		DWORD channels;
		DWORD sample_rate;
		DWORD size;
	};

	class ISND_Source
	{
	public:
		ISND_Source()
		{
    	}

		~ISND_Source()
		{
		}

		void Open(const char* file, unsigned int unk1)
		{
			__asm 
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
				push unk1
				push file
				push edi
				call dword ptr ds : [eax + 0x4]
			}
		}

		void Close() 
		{
			__asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
				push edi
				call dword ptr ds : [eax + 0x8]
			}
		}

		BOOL IsOpen()
		{
			__asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
				push edi
				call dword ptr ds : [eax + 0xC]
			}
		}

		unsigned int GetPlayTime()
		{
			__asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
				push edi
				call dword ptr ds : [eax + 0x10]
			}
		}

		void GetFormat(S_wave_format* format) 
		{
			__asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
				push format
				push edi
				call dword ptr ds : [eax + 0x14]
			}
		}

		unsigned long Lock(void** buffer)
		{
			__asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
				push buffer
				push edi
				call dword ptr ds : [eax + 0x18]
			}
		}

		void Unlock()
		{
			__asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
				push edi
				call dword ptr ds : [eax + 0x1C]
			}
		}
	};
};

#endif