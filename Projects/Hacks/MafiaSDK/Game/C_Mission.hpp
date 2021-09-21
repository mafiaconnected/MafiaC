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

#ifndef _CMISSION_H_
#define _CMISSION_H_

namespace MafiaSDK
{
    namespace C_Mission_Enum
    {
        enum FunctionAddresses
        {
            CLASS_CMISSION = C_MISSION_CLASS,
            CreateActor = 0x0053F7D0,
            AddActor = 0x0053FEA0,
            UnloadActor = 0x00540080,
            DelActor = 0x00540130,
            DelActors = 0x00540240,
            FindActorByName = 0x00540490,
            Open = 0x005409D0,
            Close = 0x005405E0
        };

		enum MissionID
		{
			FREERIDE = 29,
			FREERIDE_NOC = 30,
            TUTORIAL = 17,
		};

        enum ObjectTypes
        {
            GhostObject = 1,
            Player = 2,
            Car = 4,
            Detector = 5,
            Door = 6,
            Trolley = 8,
            SpecialIDK = 9,
            Bottle = 10,
            TrafficSetup = 12,
            PedestrianSetup = 18,
            Unknown = 20,
            Dog = 21,
            Plane = 22,
            RailRoute = 24,
            Pumpar = 25,
            Enemy = 27,
            RaceCamera = 28,
            Wagons = 30,
            Clocks = 34,
            Physical = 35,
            Truck = 36,
            InitScript = 155
        };
    }

    struct C_Mission_Interface
    {
        PADDING(C_Mission_Interface, _pad0, 0x24);
        C_Game * mGame;
    };

    class C_Mission
    {
    public:
        C_Mission_Interface* GetMissionInterface()
        {
            return reinterpret_cast<C_Mission_Interface*>(this);
        }

        C_Game* GetGame()
        {
            return GetMissionInterface()->mGame;
        }

        C_Actor* CreateActor(C_Mission_Enum::ObjectTypes actorType)
        {
            unsigned long funcAddress = C_Mission_Enum::FunctionAddresses::CreateActor;

            __asm
            {
                mov ecx, this
                push actorType
                call funcAddress
               
            }
        }

        void MapLoad(const char* mapName)
        {
            strcpy((char*)0x00647BA0, mapName);
            *(BYTE*)(0x00647B9C) = 1;
        }

        void AddActor(C_Actor* actor, BOOL unk)
        {
            unsigned long funcAddress = C_Mission_Enum::FunctionAddresses::AddActor;

            __asm
            {
                push unk
                push actor
                mov ecx, this
                call funcAddress
            }
        }

        void UnloadActor(C_Actor* actor)
        {
            unsigned long funcAddress = C_Mission_Enum::FunctionAddresses::UnloadActor;

            __asm
            {
                push actor
                mov ecx, this
                call funcAddress
            }
        }

        void DelActor(C_Actor* actor)
        {
            unsigned long funcAddress = C_Mission_Enum::FunctionAddresses::DelActor;

            __asm
            {
                push actor
                mov ecx, this
                call funcAddress
            }
        }

        void DelActors()
        {
            unsigned long funcAddress = C_Mission_Enum::FunctionAddresses::DelActors;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        C_Actor* FindActorByName(const char* actorName)
        {
            unsigned long funcAddress = C_Mission_Enum::FunctionAddresses::FindActorByName;

            __asm
            {
                push actorName
                mov ecx, this
                call funcAddress
            }
        }

        void Close()
        {
            unsigned long funcAddress = C_Mission_Enum::FunctionAddresses::Close;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        void Open(const char* mission_name, BOOL unk1, unsigned int unk2, BOOL unk3)
        {
            unsigned long funcAddress = C_Mission_Enum::FunctionAddresses::Open;

            __asm
            {
                push unk3
                push unk2
                push unk1
                push mission_name
                mov ecx, this
                call funcAddress
            }
        }
    };

    inline C_Mission* GetMission()
    {
        return *reinterpret_cast<C_Mission**>(C_Mission_Enum::FunctionAddresses::CLASS_CMISSION);
    }

    inline C_Mission_Enum::MissionID GetCurrentMissionID() {
        return (C_Mission_Enum::MissionID)*(int*)(0x6BD8A8);
    }

    inline char* GetCurrentMissionName() {
        DWORD addr = (DWORD)GetModuleHandle(NULL) + 0x00247E60;
        return *(char**)(addr);
    }
}

#endif
