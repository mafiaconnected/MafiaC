/*
    Copyright 2017 D�vid Svitana

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

#ifndef _CVEHICLE_H_
#define _CVEHICLE_H_

namespace MafiaSDK
{
    struct C_Vehicle_Interface {
		PADDING(C_Vehicle_Interface, _pad0, 0x68);
		bool roof;												// 104-105 - Needs more testing
		PADDING(C_Vehicle_Interface, _pad1, 0x14B);
		float engine_health;									// 436-440
		PADDING(C_Vehicle_Interface, _pad2, 0x34);
		float health;											// 492-496
		PADDING(C_Vehicle_Interface, _pad3, 0x28);
		DWORD first_mesh;										// 536-540
		DWORD last_mesh;										// 540-544
		PADDING(C_Vehicle_Interface, _pad4, 0x100);
		S_vector position;										// 800-812
		PADDING(C_Vehicle_Interface, _pad5, 0xF4);
		S_vector rot_speed;										// 1056-1068
		PADDING(C_Vehicle_Interface, _pad6, 0x4);
		bool horn;												// 1072-1073
		bool siren;												// 1073-1074
		bool sound_enabled;										// 1074-1075
		float hand_break;										// 1076-1080
		PADDING(C_Vehicle_Interface, _pad10, 0x6C);
		float speed_limit;										// 1188-1192
		PADDING(C_Vehicle_Interface, _pad11, 0x98);
		float accelerating;										// 1344-1348
		PADDING(C_Vehicle_Interface, _pad12, 0x4);
		float engine_rpm;										// 1352-1356
		PADDING(C_Vehicle_Interface, _pad13, 0x14);
		DWORD gear;												// 1376-1380
		PADDING(C_Vehicle_Interface, _pad14, 0x54);
		float break_val;										// 1464-1468
		PADDING(C_Vehicle_Interface, _pad15, 0x24);
		float clutch;											// 1504-1508
		PADDING(C_Vehicle_Interface, _pad16, 0x40);
		float wheel_angle;										// 1572-1576
		PADDING(C_Vehicle_Interface, _pad17, 0x604);
		bool engine_on;											// 3116-3117  
		float fuel;												// 3120-3124
		PADDING(C_Vehicle_Interface, _pad19, 0x8);
		S_vector rot_forward;									// 3132-3144
		S_vector rot_right;										// 3144-3156
		S_vector rot_up;										// 3156-3168
		PADDING(C_Vehicle_Interface, _pad20, 0x1330);
		S_vector speed;											// 8080-8092
		PADDING(C_Vehicle_Interface, _pad21, 0xC8);
		bool lights;											// 8292-8293
    };

    //const size_t Offset = offsetof(C_Vehicle_Interface, lights);
	//const size_t Size = sizeof(C_Vehicle_Interface::lights);

    namespace C_Vehicle_Enum
    {
        enum FunctionsAddresses
        {
			OpenWindow = 0x004D7A80,
			DoLights = 0x004D7CD0
        };
    };

    class C_Vehicle
    {
    public:
        C_Vehicle_Interface* GetInterface()
        {
            return reinterpret_cast<C_Vehicle_Interface*>(this);
        }

		void OpenWindow(int windowID, BOOL arg2)
		{
			unsigned long functionAddress = C_Vehicle_Enum::FunctionsAddresses::OpenWindow;
			__asm
			{
				mov ecx, this
				push windowID
				push arg2
				call functionAddress
			}
		}

		void DoLights(float arg1)
		{
			unsigned long functionAddress = C_Vehicle_Enum::FunctionsAddresses::DoLights;
			__asm
			{
				mov ecx, this
				push arg1
				call functionAddress
			}
		}
    };
};

#endif
