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

#ifndef _CDETECTOR_H_
#define _CDETECTOR_H_

namespace MafiaSDK
{
	struct C_Detector_Interface
	{
		PADDING(C_Detector_Interface, _pad0, 0x70);
		C_Program mProgram;
	};

	namespace C_Detector_Enum
	{
		enum FunctionAddresses
		{
			CreateColls = 0x004330B0
		};
	};

	class C_Detector : public C_Actor
	{
	public:
		C_Detector_Interface* GetInterface()
		{
			return reinterpret_cast<C_Detector_Interface*>(this);
		}
		
		C_Program* GetProgram() 
		{
			return &this->GetInterface()->mProgram;
		}
		
		void CreateColls(BOOL createColls)
		{
			unsigned long funcAddress = C_Detector_Enum::FunctionAddresses::CreateColls;

			__asm
			{
				push createColls
				mov ecx, this
				call funcAddress
			}
		}
	};
};

#endif