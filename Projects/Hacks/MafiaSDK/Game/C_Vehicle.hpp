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
        PADDING(C_Vehicle_Interface, _pad0, 0x1B4);
        float engine_health;
        PADDING(C_Vehicle_Interface, _pad1, 0x34);
        float health;
        PADDING(C_Vehicle_Interface, _pad2, 0x28);
        DWORD first_mesh;
        DWORD last_mesh;
        PADDING(C_Vehicle_Interface, _pad4, 0x100);
        S_vector position;
        PADDING(C_Vehicle_Interface, _pad5, 0xF4);
        S_vector rot_speed;
        PADDING(C_Vehicle_Interface, _pad6, 0x4);
        bool horn;
        bool siren;
        bool sound_enabled;
        float hand_break;
        PADDING(C_Vehicle_Interface, _pad10, 0x6C);
        float speed_limit;
        PADDING(C_Vehicle_Interface, _pad11, 0x98);
        float accelerating;
        PADDING(C_Vehicle_Interface, _pad12, 0x4);
        float engine_rpm;
        PADDING(C_Vehicle_Interface, _pad13, 0x14);
        DWORD gear;
        PADDING(C_Vehicle_Interface, _pad14, 0x54);
        float break_val;
        PADDING(C_Vehicle_Interface, _pad15, 0x24);
        float clutch;
        PADDING(C_Vehicle_Interface, _pad16, 0x40);
        float wheel_angle;
        PADDING(C_Vehicle_Interface, _pad17, 0x604);
        bool engine_on;
        float fuel;
        PADDING(C_Vehicle_Interface, _pad19, 0x8);
        S_vector rot_forward;
        S_vector rot_right;
        S_vector rot_up;
        PADDING(C_Vehicle_Interface, _pad20, 0x1330);
        S_vector speed;
        PADDING(C_Vehicle_Interface, _pad21, 0xC8);
        bool lights;
    };

    //const size_t Offset = offsetof(C_Vehicle_Interface, lights);
    //const size_t Offset = offsetof(C_Vehicle_Interface, siren);

    namespace C_Vehicle_Enum
    {
        enum FunctionsAddresses
        {

        };
    };

    class C_Vehicle
    {
    public:
        C_Vehicle_Interface* GetInterface()
        {
            return reinterpret_cast<C_Vehicle_Interface*>(this);
        }
    };
};

#endif
