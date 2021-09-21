/*
    Copyright 2018 Dávid Svitana

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

#ifndef _CDOOR_H_
#define _CDOOR_H_

namespace MafiaSDK 
{    
    struct C_Door_Interface
    {
        C_Entity_Interface entity;
        PADDING(C_Door_Interface, _pad1, 0x4);
        unsigned int current_state;
        float open_angle;
        PADDING(C_Door_Interface, _pad2, 0xA6);
        BYTE open_direction;
        float open_percent_koef;
    };

    namespace C_Door_Enum 
    {
        enum States
        {
            Open = 2,
            Close = 3
        };

        enum FunctionAddresses 
        { 
            SetOpenAngle = 0x0043B810,
            SetState = 0x00439610 
        };
    };

    class C_Door : public C_Actor 
    {
    public:
        C_Door_Interface *GetInterface() 
        {
            return reinterpret_cast<C_Door_Interface *>(this);
        }

        void SetState(C_Door_Enum::States state, C_Actor* user, BOOL unk1, BOOL unk2) 
        {
            unsigned long funcAddress = C_Door_Enum::FunctionAddresses::SetState;

            __asm
            {
                push unk2
                push unk1
                push user
                push state
                mov ecx, this
                call funcAddress
            }
        }

        void SetOpenAngle(float angle) 
        {
            unsigned long funcAddress = C_Door_Enum::FunctionAddresses::SetOpenAngle;

            __asm
            {
                push angle
                mov ecx, this
                call funcAddress
            }
        }
    };
}; // namespace MafiaSDK

#endif
