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

#ifndef _BLOOD_H_
#define _BLOOD_H_

namespace MafiaSDK
{
	struct C_Blood_Interface
	{

	};

	namespace C_Blood_Enum
	{
		enum FunctionsAddresses
		{
			Tick = 0x00411E30,
			Init = 0x00411FB0
		};
	};

	class C_Blood
	{
	public:
		C_Blood()
		{
			//void* allocatedBlood = malloc(0x24);
			//__asm mov this, allocatedBlood
		}
				
		C_Blood_Interface* GetInterface()
		{
			return reinterpret_cast<C_Blood_Interface*>(this);
		}
		
		void Tick(float deltaTime)
		{
			unsigned long funcAddress = C_Blood_Enum::FunctionsAddresses::Tick;

			__asm
			{
				push deltaTime
				mov ecx, this
				call funcAddress
			}
		}
		
		void Init(S_vector & position, S_vector & rotation, float bloodRadius, int bloodType, BOOL doEnable)
		{
			unsigned long funcAddress = C_Blood_Enum::FunctionsAddresses::Init;

			__asm
			{
				push doEnable
				push bloodType
				push bloodRadius
				push rotation
				push position
				mov ecx, this
				call funcAddress
			}
		}
	};
};

#endif