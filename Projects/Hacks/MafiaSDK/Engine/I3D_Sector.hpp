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

#ifndef _I3D_Sector_H_
#define _I3D_Sector_H_

namespace MafiaSDK
{
	struct I3D_Sector_Interface
	{
		
	};

	namespace I3D_Sector_Enum
	{
		enum FunctionsAddresses
		{
			SetWeatherSystemParam = 0x1004BCE0 // LS3DF.dll base (0x10000000) + 0x4BCE0
		};

		/*
			Rain/weather-system parameter IDs for SetWeatherSystemParam() below. Values 0, 3-10
			and 14 are confirmed from MultiplayerModOne's own WEATHER_* MafiaScript defines
			(GameFunctions.cpp's RegisterGameDefines) and match the documented `weather_setparam`
			MafiaScript command (ON, COLOR_L/COLOR_H, SPEED, LEN, WIDTH, MAX_DIST, MAX_HEIGHT,
			MAX_CNT, MODE). DIR_X/DIR_Y/DIR_Z at 11-13 are inferred - they exactly fill the only
			gap in that list, immediately before MODE, and the doc separately lists a 3-axis rain
			direction - but they're not yet confirmed against the real engine values. 1 and 2 are
			unaccounted for by any known name; don't assume they're unused without checking.

			All of these except ON/MAX_CNT/MODE are documented as float ranges (e.g. SPEED 20-40,
			LEN 0.5-1, WIDTH 0.02-0.05) despite SetWeatherSystemParam() taking a raw uint32_t -
			use the float overload below for those, which just bit-reinterprets the value.
		*/
		enum WS_PARAM
		{
			WS_PARAM_ON         = 0,
			WS_PARAM_UNKNOWN1   = 1,
			WS_PARAM_UNKNOWN2   = 2,
			WS_PARAM_COLOR_L    = 3,
			WS_PARAM_COLOR_H    = 4,
			WS_PARAM_SPEED      = 5,
			WS_PARAM_LEN        = 6,
			WS_PARAM_WIDTH      = 7,
			WS_PARAM_MAX_DIST   = 8,
			WS_PARAM_MAX_HEIGHT = 9,
			WS_PARAM_MAX_CNT    = 10,
			WS_PARAM_DIR_X      = 11,
			WS_PARAM_DIR_Y      = 12,
			WS_PARAM_DIR_Z      = 13,
			WS_PARAM_MODE       = 14
		};
	};

	class I3D_Sector : public I3D_Frame
	{
	public:
		I3D_Sector_Interface* GetInterface()
		{
			return reinterpret_cast<I3D_Sector_Interface*>(this);
		}
		
		void AddLight(I3D_Light* lightToAdd)
		{
			__asm
			{
				mov eax, this
				mov ecx, dword ptr ds : [eax]
				push lightToAdd
				push eax
				call dword ptr ds : [ecx + 0x50]
			}
		}
		
		void DeleteLight(I3D_Light* lightToRemove)
		{
			__asm
			{
				mov eax, this
				mov ecx, dword ptr ds : [eax]
				push lightToRemove
				push eax
				call dword ptr ds : [ecx + 0x54]
			}
		}

		void AddSound(I3D_Sound* soundToAdd)
		{
			__asm
			{
				mov eax, this
				mov ecx, dword ptr ds : [eax]
				push soundToAdd
				push eax
				call dword ptr ds : [ecx + 0x58]
			}
		}

		void RemoveSound(I3D_Sound* soundToRemove)
		{
			__asm
			{
				mov eax, this
				mov ecx, dword ptr ds : [eax]
				push soundToRemove
				push eax
				call dword ptr ds : [ecx + 0x5C]
			}
		}

		// Not a vtable call - a direct function in LS3DF.dll (see FunctionsAddresses above).
		void __stdcall SetWeatherSystemParam(I3D_Sector_Enum::WS_PARAM eParam, uint32_t uiValue)
		{
			unsigned long addressFunc = I3D_Sector_Enum::FunctionsAddresses::SetWeatherSystemParam;

			__asm
			{
				push uiValue
				push eParam
				mov ecx, this
				call addressFunc
			}
		}

		// Convenience for the params documented as floats (SPEED, LEN, WIDTH, MAX_DIST,
		// MAX_HEIGHT, DIR_X/Y/Z, and probably COLOR_L/COLOR_H) - bit-reinterprets fValue into
		// the raw uint32_t the real function takes. Use the uint32_t overload directly for
		// true integer params (ON, MAX_CNT, MODE).
		void SetWeatherSystemParam(I3D_Sector_Enum::WS_PARAM eParam, float fValue)
		{
			SetWeatherSystemParam(eParam, *reinterpret_cast<uint32_t*>(&fValue));
		}
	};
}

#endif