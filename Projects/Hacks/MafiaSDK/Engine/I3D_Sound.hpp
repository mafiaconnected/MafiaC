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

#ifndef _I3D_Sound_H_
#define _I3D_Sound_H_

namespace MafiaSDK
{
	struct I3D_Sound_Interface
	{
		PADDING(I3D_Sound_Interface, _pad0, 0x160);
		ISND_Source* sound_source;
	};

	namespace I3D_Sound_Enum
	{
		enum SoundType
		{
            SPATIAL = 1,
            STATIC = 3
		};
	};

	class I3D_Sound : public I3D_Frame
	{
	public:
		I3D_Sound()
		{
    	}

		~I3D_Sound()
		{
		}

		I3D_Sound_Interface* GetSoundInterface()
		{
			return reinterpret_cast<I3D_Sound_Interface*>(this);
		}

        unsigned long Open(const char* file, unsigned int unk1, void* callback, void* unk2) 
        {
            __asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
                push unk2
                push callback
                push unk1
                push file
				push edi
				call dword ptr ds : [eax + 0x50]
			}
        }

        bool IsPlaying() 
        {
            __asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
                push 0
				push edi
				call dword ptr ds : [eax + 0x54]
			}
        }

        void SetSoundType(I3D_Sound_Enum::SoundType sound_type) 
        {
            __asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
                push sound_type
				push edi
				call dword ptr ds : [eax + 0x58]
			}
        }

        void SetRange(float unk1, float unk2, float unk3, float unk4)
        {
            __asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
                push unk4
                push unk3
                push unk2
                push unk1
				push edi
				call dword ptr ds : [eax + 0x5C]
			}
        }

        void SetCone(float unk1, float unk2)
        {
            __asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
                push unk2
                push unk1
				push edi
				call dword ptr ds : [eax + 0x60]
			}
        }

        void SetOutVol(float out_vol)
        {
            __asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
                push out_vol
				push edi
				call dword ptr ds : [eax + 0x64]
			}
        }

        void SetVolume(float volume)
        {
            __asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
                push volume
				push edi
				call dword ptr ds : [eax + 0x68]
			}
        }

        void SetLoop(BOOL do_loop)
        {
            __asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
                push do_loop
				push edi
				call dword ptr ds : [eax + 0x6C]
			}
        }
	};
};

#endif