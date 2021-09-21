/*
	Copyright 2019 Dávid Svitana

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

#ifndef _CTEXTDB_H_
#define _CTEXTDB_H_

namespace MafiaSDK
{
	namespace G_TextDatabase_Enum
	{
		constexpr unsigned long CLASS_ADDRESS = 0x006D8714;

		enum FunctionsAddresses
		{
			LoadDatabase = 0x0060FA60,
			Unload = 0x0060FB10,
			GetTextOrNULL = 0x0060FB10,
			GetText = 0x0060FBA0
		};
	};

	class G_TextDatabase
	{
	public:
		void LoadDatabase(const char* file) 
		{
			unsigned long funcAddress = G_TextDatabase_Enum::FunctionsAddresses::LoadDatabase;

			__asm 
			{
				push file
				mov ecx, this
				call funcAddress
			}
		}

		void Unload()
		{
			unsigned long funcAddress = G_TextDatabase_Enum::FunctionsAddresses::Unload;

			__asm 
			{
				mov ecx, this
				call funcAddress
			}
		}

		const char* GetTextOrNULL(unsigned int textId) 
		{
			unsigned long funcAddress = G_TextDatabase_Enum::FunctionsAddresses::GetTextOrNULL;

			__asm 
			{
				push textId
				mov ecx, this
				call funcAddress
			}
		}

		const char* GetText(unsigned int textId) 
		{
			unsigned long funcAddress = G_TextDatabase_Enum::FunctionsAddresses::GetText;

			__asm 
			{
				push textId
				mov ecx, this
				call funcAddress
			}
		}
	};

	G_TextDatabase* GetTextDatabase()
	{
		return (G_TextDatabase*)G_TextDatabase_Enum::CLASS_ADDRESS;
	}
};

#endif
