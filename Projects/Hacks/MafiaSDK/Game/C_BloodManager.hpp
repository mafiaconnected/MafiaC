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

#ifndef _BLOODMANAGER_H_
#define _BLOODMANAGER_H_

namespace MafiaSDK
{
	struct C_BloodManager_Interface
	{

	};

	namespace C_BloodManger_Enum
	{
		enum FunctionsAddresses
		{
			GameInit = 0x00411E30,
			GameDone = 0x00412050, 
			Tick = 0x004120A0,
			AddBlood = 0x00412140
		};
	};

	class C_BloodManager
	{
	public:				
		C_BloodManager_Interface* GetInterface()
		{
			return reinterpret_cast<C_BloodManager_Interface*>(this);
		}
		
		void GameInit()
		{
			unsigned long funcAddress = C_BloodManger_Enum::FunctionsAddresses::GameInit;

			__asm
			{
				mov ecx, this
				call funcAddress
			}
		}
		
		void GameDone()
		{
			unsigned long funcAddress = C_BloodManger_Enum::FunctionsAddresses::GameDone;

			__asm
			{
				mov ecx, this
				call funcAddress
			}
		}
		
		void Tick(float deltaTime)
		{
			unsigned long funcAddress = C_BloodManger_Enum::FunctionsAddresses::Tick;

			__asm
			{
				push deltaTime
				mov ecx, this
				call funcAddress
			}
		}
		
		void AddBlood(S_vector & position, S_vector & rotation, float bloodRadius, int bloodType, BOOL doEnable)
		{
			unsigned long funcAddress = C_BloodManger_Enum::FunctionsAddresses::AddBlood;

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