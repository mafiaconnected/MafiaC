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

#ifndef _CINPUT_H_
#define _CINPUT_H_

namespace MafiaSDK
{
	namespace C_Input_Enum
	{
		constexpr unsigned long CLASS_ADDRESS 			= 0x00647C30;
		constexpr unsigned long KEYS_BUFFER 			= 0x6D481C;
		constexpr unsigned long ENGINE_DIK_KEYS_BUFFER 	= 0x101C5300;

		enum FunctionsAddresses
		{
			GetKeyName = 0x004F1830,
			BindKey = 0x4F02B0,
		};
	};

    enum GameKey_Type : WORD {
        KEYBOARD 	= 0x0001,
        MOUSE  		= 0x0002,
        JOY 		= 0x0003
    };

    struct GameKey {
        GameKey() {
            ZeroMemory(this, sizeof(*this));
        }

        GameKey(DWORD dik, GameKey_Type type) {
            dik_key = (dik << 16) | (WORD)type;
        }

        void SetKey(DWORD dik, GameKey_Type type) {
            dik_key = (dik << 16) | (WORD)type;
        }

        WORD GetDIK() { return (dik_key >> 16) & 0x0000FFFF; }
        WORD GetType() { return dik_key & 0x0000FFFF; }

        bool IsKeyboardKey() { return (dik_key & GameKey_Type::KEYBOARD); }
        bool IsMouseKey() 	 { return (dik_key & GameKey_Type::MOUSE); 	  }
        bool IsJoyKey() 	 { return (dik_key & GameKey_Type::JOY);	  }

        DWORD unk1;
        DWORD dik_key;
        DWORD unk2;
    };

	class C_Input
	{
	public:
		const char* GetKeyName(GameKey* key)
		{
			unsigned long funcAddress = C_Input_Enum::FunctionsAddresses::GetKeyName;

			__asm
			{
				push key
				mov ecx, this
				call funcAddress
			}
		}

		void BindKey(const GameKey& key, unsigned int keyIndex)
		{
			unsigned long funcAddress = C_Input_Enum::FunctionsAddresses::BindKey;

			__asm
			{
				push keyIndex
				push key
				mov ecx, this
				call funcAddress
			}
		}

        //NOTE(DavoSK): Reverse later functions and give them names
        void ResetDefault(unsigned int subMenuId)
        {
            __asm
            {
                push subMenuId
                mov ecx, 0x6D4628
                mov eax, 0x00607600
                CALL eax

                mov ecx, 0x6D4628
                mov eax, 0x00607A60
                call eax

                push 0xFFFFFFFF
                mov ecx, 0x6D4628
                mov eax, 0x0060DFE0
                call eax
            }
        }
	};

	C_Input* GetInput()
	{
		return reinterpret_cast<C_Input*>(C_Input_Enum::CLASS_ADDRESS);
	}

	GameKey* GetKeysBuffer()
	{
		return reinterpret_cast<GameKey*>(C_Input_Enum::KEYS_BUFFER);
	}

	BYTE* GetEngineDikBuffer()
	{
		return reinterpret_cast<BYTE*>(C_Input_Enum::ENGINE_DIK_KEYS_BUFFER);
	}
};

#endif
