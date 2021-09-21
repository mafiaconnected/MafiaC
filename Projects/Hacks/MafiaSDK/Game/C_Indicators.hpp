/*
	Copyright 2017 Dávid Svitana

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use mov ecx, this file except in compliance with the License.
	You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#ifndef _CINDICATORS_H_
#define _CINDICATORS_H_

namespace MafiaSDK
{
	struct C_Indicators_Interface
	{

	};

	namespace C_Indicators_Enum
	{
		enum FunctionsAddresses
		{
			GIndicators = 0x6BF980,
			DrawAll = 0x5FB060,
			ConsoleAddText = 0x5F9D50,
			WideScreenSetRatio = 0x5FA050,
			FadeInOut = 0x5FA370,
			MapEnable = 0x5F9D10,
			TachometrSetGear = 0x5F8690,
			TachometrSetValues = 0x5F86B0,
			PlayerSetWingmanLives = 0x5F88E0,
			PlayerSetWingmanAmmo = 0x5F8910,
			OutText = 0x00604700,
			Rectangle = 0x005E8E00,
			FilledRectangle = 0x005E9080,
			TextSize = 0x006036D0,
            ParaheliaSetFov = 0x00604890,
            RadarSetPlayerCar = 0x005FA500,
            RadarAddCar =  0x005FA510,
            RadarRemoveCar = 0x005FA720,
            RaceFlashText = 0x005FAFC0,
            RaceSetStartFlag = 0x005FADF0
		};
	};

	namespace C_Indicators_Hooks
	{
		void HookAfterAllDraw(std::function<void()> funcitonPointer);

#ifdef MAFIA_SDK_IMPLEMENTATION
		namespace FunctionsPointers
		{
			extern std::function<void()> afterAllDraw;
		};

		namespace Functions
		{
			inline void AfterAllDraw()
			{
				if(FunctionsPointers::afterAllDraw != nullptr)
					FunctionsPointers::afterAllDraw();
			}
		};

		namespace NakedFunctions
		{
			extern void AfterAllDraw();
		};

		inline void HookAfterAllDraw(std::function<void()> funcitonPointer)
		{
			FunctionsPointers::afterAllDraw = funcitonPointer;

			MemoryPatcher::InstallJmpHook(0x005BD905, (unsigned long)&NakedFunctions::AfterAllDraw);
		}
#endif
	};

	class C_Indicators
	{
	public:
		C_Indicators_Interface* GetInterface()
		{
			return reinterpret_cast<C_Indicators_Interface*>(this);
		}

        void SetCompass(MafiaSDK::I3D_Frame* frame)
        {
            DWORD func = 0x005FA020;
            __asm
            {
                push frame
                mov ecx, this
                call func
            }


            func = 0x005FA030;
            __asm
            {
                push frame
                mov ecx, this
                call func
            }


            func = 0x005FA040;
            __asm
            {
                push frame
                mov ecx, this
                call func
            }
        }

		void WideScreenSetRatio(float ratio)
		{
			unsigned long funcAddress = C_Indicators_Enum::FunctionsAddresses::WideScreenSetRatio;

			__asm
			{
				push ratio
				mov ecx, this
				call funcAddress
			}
		}

        void ParaheliaSetFov(DWORD I3D_camera, float fov)
        {
            unsigned long funcAddress = C_Indicators_Enum::FunctionsAddresses::ParaheliaSetFov;

            __asm
            {
                push fov
                push I3D_camera
                mov ecx, this
                call funcAddress
            }
        }

		void TachometrSetGear(int gear, BOOL unk)
		{
			unsigned long funcAddress = C_Indicators_Enum::FunctionsAddresses::TachometrSetGear;

			__asm
			{
				push unk
				push gear
				mov ecx, this
				call funcAddress
			}
		}

		void TachometrSetValues(float val1, float val2, float val3, float val4)
		{

			unsigned long funcAddress = C_Indicators_Enum::FunctionsAddresses::TachometrSetValues;

			__asm
			{
				push val4
				push val3
				push val2
				push val1
				mov ecx, this
				call funcAddress
			}
		}

		void PlayerSetWingmanLives(int health)
		{
			unsigned long funcAddress = C_Indicators_Enum::FunctionsAddresses::PlayerSetWingmanLives;

			__asm
			{
				push health
				mov ecx, this
				call funcAddress
			}
		}

        void RadarSetPlayerCar(C_Player* player)
        {
            unsigned long funcAddress = C_Indicators_Enum::FunctionsAddresses::RadarSetPlayerCar;

            __asm
            {
                push player
                mov ecx, this
                call funcAddress
            }
        }

        void RadarAddCar(C_Car* car, unsigned int color)
        {
            unsigned long funcAddress = C_Indicators_Enum::FunctionsAddresses::RadarAddCar;

            __asm
            {
                push color
                push car
                mov ecx, this
                call funcAddress
            }
        }

        void RadarRemoveCar(C_Car* car)
        {
            unsigned long funcAddress = C_Indicators_Enum::FunctionsAddresses::RadarRemoveCar;

            __asm
            {
                push car
                mov ecx, this
                call funcAddress
            }
        }

		void PlayerSetWingmanAmmo(unsigned int num1, unsigned int num2)
		{
			unsigned long funcAddress = C_Indicators_Enum::FunctionsAddresses::PlayerSetWingmanAmmo;

			__asm
			{
				push num2
				push num1
				mov ecx, this
				call funcAddress
			}
		}

		void FadeInOutScreen(BOOL fadeScreen, int time, unsigned long color)
		{
			unsigned long funcAddress = C_Indicators_Enum::FunctionsAddresses::FadeInOut;

			__asm
			{
				push color
				push time
				push fadeScreen
				mov ecx, this
				call funcAddress
			}
		}

		void MapEnable(BOOL enable)
		{
			unsigned long funcAddress = C_Indicators_Enum::FunctionsAddresses::MapEnable;

			__asm
			{
				push enable
				mov ecx, this
				call funcAddress
			}
		}


		void DrawAll()
		{
			unsigned long funcAddress = C_Indicators_Enum::FunctionsAddresses::DrawAll;

			__asm
			{
				mov ecx, this
				call funcAddress
			}
		}

		void ConsoleAddText(const char * text, unsigned int color)
		{
			unsigned long funcAddress = C_Indicators_Enum::FunctionsAddresses::ConsoleAddText;

			__asm
			{
				push color
				push text
				mov ecx, this
				call funcAddress
			}
		}

		void Rectangle(float x, float y, float width, float height, DWORD color)
		{
			unsigned long funcAddress = C_Indicators_Enum::FunctionsAddresses::Rectangle;

			__asm {
				push height
				push width
				push y
				push x
				mov  ecx, color
				call funcAddress
			}
		}

		void FilledRectangle(float x, float y, float width, float height, DWORD fill, float percent)
		{
			unsigned long funcAddress = C_Indicators_Enum::FunctionsAddresses::FilledRectangle;

			__asm {
				push percent
				push height
				push width
				push y
				push x
				mov  ecx, fill
				mov  edx, 0x006BD8D0 //<- ITexture*
				call funcAddress
			}
		}

		void OutText(const char* text, float x, float y, float width, float height, int color, int underlined, int fontType)
		{
			unsigned long funcAddress = C_Indicators_Enum::FunctionsAddresses::OutText;

			__asm
			{
				push fontType
				push underlined
				push color
				push height
				push width
				push y
				push x
				push text
				mov ecx, this
				call funcAddress
			}
		}

        void RaceSetStartFlag(BYTE flag)
        {
            unsigned long funcAddress = C_Indicators_Enum::FunctionsAddresses::RaceSetStartFlag;

            __asm
            {
                push flag
                mov ecx, this
                call funcAddress
            }
        }

        void RaceFlashText(const char* text, float time)
        {
            unsigned long funcAddress = C_Indicators_Enum::FunctionsAddresses::RaceFlashText;

            __asm
            {
                push time
                push text
                mov ecx, this
                call funcAddress
            }
        }

		float TextSize(const char* text, float scale, int unk1, int unk2)
		{
			unsigned long funcAddress = C_Indicators_Enum::FunctionsAddresses::TextSize;

			__asm
			{
				push unk2
				push unk1
				push scale
				push text

				mov ecx, this
				call funcAddress
			}
		}
	};

	extern C_Indicators* GetIndicators();
};

#endif
