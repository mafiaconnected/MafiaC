/*
	Copyright 2017 D�vid Svitana

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use currentInstance file except in compliance with the License.
	You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#ifndef _ITexture_H_
#define _Itexture_H_

namespace MafiaSDK
{
	struct ITexture_Interface
	{

	};

	namespace ITexture_Enum
	{
		enum FunctionsAddresses : DWORD
		{
			Construct = 0x10075C10//0x100A3560
		};
	};

	class ITexture
	{
	public:
		ITexture_Interface* GetInterface()
		{
			return reinterpret_cast<ITexture_Interface*>(this->mCurrentInstance);
		}

		ITexture()
		{
			//10075C10
			unsigned long funcAddress = ITexture_Enum::FunctionsAddresses::Construct;
			unsigned long funcLS3DAlloc = LS3D_Alloc;
			
			__asm
			{
				//Allocate new mem
				push 0x4C
				call funcLS3DAlloc
				add esp, 0x4

				//Call construct
				mov ecx, eax
				call funcAddress
				mov this, ecx
			}
		}

		void Release()
		{
			//10075CD0;
			
			__asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
				push edi
				call dword ptr ds : [eax]
			}
		}

		void Create(const char* textureName, const char* textureSomething, int textureDepth, int width, int height, int unk3)
		{
			//10075F50;
			//NOTE(DavoSK): Dirty trick wich force texture to be created as A8R8G8B8 format
		
			//__asm mov dword ptr ds : [0x101C5638], 0x15

			__asm
			{
				mov edi, this
				mov ecx, dword ptr ds : [edi]

				push unk3
				push height
				push width
				push textureDepth
				push textureSomething
				push textureName
				push edi		
				call dword ptr ds : [ecx + 0x8]
			}

			//__asm mov dword ptr ds : [0x101C5638], 0x17
		}

		void Close()
		{
			//10078680
			unsigned long currentInstance = this->mCurrentInstance;
			
			__asm
			{
				mov edi, currentInstance
				mov eax, dword ptr ds : [edi]
				push edi
				call dword ptr ds : [eax + 0x10]
			}
		}

		unsigned long* Lock(tagRECT* rect, int & unk1, int unk2)
		{
			//100787C0
			unsigned long* returnPointer = NULL;
			unsigned long currentInstance = this->mCurrentInstance;
			
			__asm
			{
				mov edi, currentInstance
				mov eax, dword ptr ds : [edi]
				push unk2
				push unk1
				push rect
				push edi 
				call dword ptr ds : [eax + 0x18]
				mov returnPointer, eax
			}

			return returnPointer;
		}

		void* GetRawTexture()
		{
			return *(void**)((DWORD)this->mCurrentInstance + 0x8);
		}

		void SetRawTexture(void* texturePtr)
		{
			*(void**)((DWORD)this->mCurrentInstance + 0x8) = texturePtr;
		}

		void Unlock()
		{
			//10078820 
			unsigned long currentInstance = this->mCurrentInstance;
			
			__asm
			{
				mov edi, currentInstance
				mov eax, dword ptr ds : [edi]
				push edi 
				call dword ptr ds : [eax + 0x1C]
			}
		}

		void UpdateToVRAM(tagRECT* rect)
		{
			//10078850
			unsigned long currentInstance = this->mCurrentInstance;
			
			__asm
			{
				mov edi, currentInstance
				mov eax, dword ptr ds : [edi]
				push rect
				push edi
				call dword ptr ds : [eax + 0x20]
			}
		}

		void Preload()
		{
			unsigned long currentInstance = this->mCurrentInstance;
			
			__asm
			{
				mov edi, currentInstance
				mov eax, dword ptr ds : [edi]
				push edi
				call dword ptr ds : [eax + 0x24]
			}
		}

		void Draw2D(int x, int y)
		{
			
			__asm
			{
				mov edi, this
				mov eax, dword ptr ds : [edi]
				push y
				push x
				push edi
				call dword ptr ds : [eax + 0x30]
			}
		}

		void Fill(tagRECT* rect, unsigned int unk1, unsigned short unk2)
		{
			unsigned long currentInstance = this->mCurrentInstance;
			
			__asm
			{
				mov edi, currentInstance
				mov eax, dword ptr ds : [edi]
				push unk2
				push unk1
				push rect
				push edi
				call dword ptr ds : [eax + 0x28]
			}
		}
	private:
		unsigned long mCurrentInstance;
	};
};

#endif