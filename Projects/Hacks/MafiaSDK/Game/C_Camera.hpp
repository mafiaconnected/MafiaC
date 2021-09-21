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

#ifndef _CCAMERA_H_
#define _CCAMERA_H_

namespace MafiaSDK
{
	struct C_Camera_Interface
	{
		I3D_Frame cameraFrame;
	};

	namespace C_Camera_Enum
	{
		enum FunctionsAddresses
		{
			Construct = 0x005ECF90,
			Destruct = 0x005ED000,
			Reset = 0x005ED040,
			GameDone = 0x005ED120,
			SetCamera = 0x005ED150,
			SetPlayer = 0x005ED170,
			SetCar = 0x005ED190,
			Link = 0x005ED1E0,
			SetSwing = 0x005ED210,
			LookAround = 0x005ED330,
			LookFromEyes = 0x005ED3E0,
			SetMode = 0x005ED400,
			LezeDoToyoty = 0x005ED460,
			Tick = 0x005ED4C0,
			WantPosCar = 0x005F2100,
			WantPos = 0x005F2580,
			Begin = 0x005F2EA0,
			End = 0x005F345,
			EnableFreeLook = 0x005F34C0,
			FreeLook = 0x005F3520,
			FreeLookBase = 0x005F3810,
			LockAt = 0x005F39F0,
			CumName = 0x005F3A70,
			Unlock = 0x005F3FD0,
			PushMode = 0x005F3FF0,
			PopMode = 0x005F4220,
			DialogLook = 0x005F42E0,
			DialogStart = 0x005F4370,
			DialogStop = 0x005F4650,
			IsInDialog = 0x005F4690,
			AddRaceCamera = 0x005F4870,
			RemoveRaceCamera = 0x005F48D0,
			SetLookTo = 0x005F4A00,
			SetSniperFov = 0x005F4A70,
			SetSniperMode = 0x005F4AB0
		};
	}

	class C_Camera
	{
	public:
		C_Camera_Interface* GetInterface()
		{
			return reinterpret_cast<C_Camera_Interface*>(this);
		}

		C_Camera(void)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::Construct;;

			__asm
			{
				mov ecx, this
				call funcAddress
			}
		}

		~C_Camera(void)
		{

			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::Destruct;

			__asm
			{
				mov ecx, this
				call funcAddress
			}
		}

		void Reset(void)
		{

			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::Reset;

			__asm
			{
				mov ecx, this
				call funcAddress
			}
		}

		void GameDone(void)
		{

			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::GameDone;

			__asm
			{
				mov ecx, this
				call funcAddress
			}
		}

		void SetCamera(unsigned long I3D_Camera)
		{

			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::SetCamera;

			__asm
			{
				push I3D_Camera
				mov ecx, this
				call funcAddress
			}
		}

		void SetPlayer(C_Human* human)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::SetPlayer;

			__asm
			{
				push human
				mov ecx, this
				call funcAddress
			}
		}

		void Tick(float dtime)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::Tick;

			__asm
			{
				push dtime
				mov ecx, this
				call funcAddress
			}
		}

		void SetCar(C_Car* car)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::SetCar;

			__asm
			{
				push car
				mov ecx, this
				call funcAddress
			}
		}

		void Link(unsigned long I3D_frame)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::Link;

			__asm
			{
				push I3D_frame
				mov ecx, this
				call funcAddress
			}
		}

		void SetSwing(BOOL enable, float angle, unsigned long I3D_frame)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::SetSwing;
			float devided = angle / 100.0f;

			__asm
			{
				push I3D_frame
				push devided
				push enable
				mov ecx, this
				call funcAddress
			}
		}

		void LookAround(int val)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::LookAround;

			__asm
			{
				push val
				mov ecx, this
				call funcAddress
			}
		}

		void LookFromEyes(void)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::LookFromEyes;

			__asm
			{
				mov ecx, this
				call funcAddress
			}
		}

		void SetMode(BOOL enabled, int typeval)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::SetMode;

			__asm
			{
				push enabled
				push typeval
				mov ecx, this
				call funcAddress
			}
		}

		void LezeDoToyoty(BOOL leze)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::LezeDoToyoty;

			__asm
			{
				push leze
				mov ecx, this
				call funcAddress
			}
		}

		void FreeLook(BOOL unk1, BOOL unk2)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::FreeLook;

			__asm
			{
				push unk1
				push unk2
				mov ecx, this
				call funcAddress
			}
		}

		void FreeLookBase(float ohnisko, float rychlost, S_vector & vPozice, S_vector &vRotace1, S_vector &vRotace2)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::FreeLookBase;

			__asm
			{
				push vRotace2
				push vRotace1
				push vPozice
				push rychlost
				push ohnisko
				mov ecx, this
				call funcAddress
			}
		}

		void LockAt(S_vector const &vPos, S_vector const &vRot)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::LockAt;

			__asm
			{
				push vRot
				push vPos
				mov ecx, this
				call funcAddress
			}
		}

		void CumNane(C_Human* C_humanOne, C_Human* C_humanTwo)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::CumName;

			__asm
			{
				push C_humanOne
				push C_humanTwo
				mov ecx, this
				call funcAddress
			}
		}

		void Unlock(void)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::Unlock;

			__asm
			{
				push 0
				mov ecx, this
				call funcAddress
			}
		}

		void PushMode(void)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::PushMode;

			__asm
			{
				mov ecx, this
				call funcAddress
			}
		}

		void PopMode(BOOL bPop)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::PopMode;

			__asm
			{
				push bPop
				mov ecx, this
				call funcAddress
			}
		}

		void DialogStart(C_Human* human, int sholder)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::DialogStart;

			__asm
			{
				push sholder
				push human
				mov ecx, this
				call funcAddress
			}
		}

		void DialogLook(C_Human* humanOne,C_Human* humanTwo)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::DialogLook;

			__asm
			{
				push humanTwo
				push humanOne
				mov ecx, this
				call funcAddress
			}
		}

		void DialogStop(void)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::DialogStop;

			__asm
			{
				mov ecx, this
				call funcAddress
			}
		}

		BOOL IsInDialog(void)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::IsInDialog;
			
			__asm
			{
				mov ecx, this
				call funcAddress
			
			}
		}

		void AddRaceCamera(unsigned long G_RaceCamera)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::AddRaceCamera;

			__asm
			{
				push G_RaceCamera
				mov ecx, this
				call funcAddress
			}
		}

		void RemoveRaceCamera(unsigned long G_RaceCamera)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::RemoveRaceCamera;

			__asm
			{
				push G_RaceCamera
				mov ecx, this
				call funcAddress
			}
		}

		void SetLookTo(S_vector const &vVec, unsigned long I3D_Frame)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::SetLookTo;

			__asm
			{
				push I3D_Frame
				push vVec
				mov ecx, this
				call funcAddress

			}
		}

		void SetSniperFov(float fFov)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::SetSniperFov;

			__asm
			{
				push fFov
				mov ecx, this
				call funcAddress
			}
		}

		void SetSniperMode(BOOL bEnabled)
		{
			unsigned long funcAddress = C_Camera_Enum::FunctionsAddresses::SetSniperMode;

			__asm
			{
				push bEnabled
				mov ecx, this
				call funcAddress
			}
		}
	};
};

#endif
