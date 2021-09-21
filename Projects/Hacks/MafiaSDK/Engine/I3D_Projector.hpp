/*
	Copyright 2017 Dávid Svitana

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this->mCurrentInstance file except in compliance with the License.
	You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#ifndef _I3D_Projector_H_
#define _I3D_Projector_H_

namespace MafiaSDK
{
	struct I3D_Projector_Interface
	{

	};

	namespace I3D_Projector_Enum
	{
		enum FunctionsAddresses
		{
			Construct = 0x10045E30
		};
	};

	class I3D_Projector : public I3D_Frame
	{
	public:
		I3D_Projector_Interface* GetInterface()
		{
			return reinterpret_cast<I3D_Projector_Interface*>(this->mCurrentInstance);
		}

		I3D_Projector()
		{
			unsigned long funcAddress = I3D_Projector_Enum::FunctionsAddresses::Construct;
			unsigned long funcLS3DAlloc = LS3D_Alloc;
			unsigned long returnInstance = NULL;
			
			__asm
			{
				push 0x264
				call funcLS3DAlloc
				add esp, 0x4

				mov ecx, eax
				call funcAddress
				mov returnInstance, eax
			}
			
			this->mCurrentInstance = returnInstance;
		}
		
		void SetTexture(ITexture* projectorTexture)
		{
			unsigned long currentInstance = this->mCurrentInstance;
			
			__asm
			{
				mov edi, currentInstance
				mov eax, dword ptr ds : [edi]
				push projectorTexture
				push edi
				call dword ptr ds : [eax + 0x70]
			}
		}
		
	private:
		unsigned long mCurrentInstance;
	};
};

#endif