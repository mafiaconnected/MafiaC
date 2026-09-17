/*
    Ported from reMafia's Actors/C_traffic_generator.h/.cpp (same author,
    MafiaOrbitCam/Vendors/reMafia). Spawns/despawns pedestrians (C_traffic_element) around
    the player within its spawn radii.
*/

#ifndef _CTRAFFICGENERATOR_H_
#define _CTRAFFICGENERATOR_H_

namespace MafiaSDK
{
    class C_TrafficElement; // defined in Game/C_TrafficElement.hpp

    namespace C_TrafficGenerator_Enum
    {
        enum FunctionAddresses
        {
            AI = 0x0044EDB0,
            SaveData = 0x0044D670,
            LoadData = 0x0044D820,
            DuplicateFrom = 0x0044D510,
            SaveGameGetSize = 0x0044DE00,
            SaveGameSave = 0x0044DE20,
            SaveGameLoad = 0x0044E120,
            GameInit = 0x0044E500,
            GameDone = 0x0044E7F0
        };
    };

    struct C_TrafficGenerator_Interface
    {
        C_Entity_Interface entity;								// 0-112
        PADDING(C_TrafficGenerator_Interface, _pad0, 0x4);
        bool unk1;
        float spawnRadius;
        int unk1b;
        C_TrafficElement* trafficElements;
        int numSpawnedPeds;
        int numElements;
        float despawnRadius;
        float innerSpawnRadius;
        float outerSpawnRadius;
        float innerSpawnRadius2;
        char** modelNames;
        int numPeds;
        unsigned long* densityRatios;
        int rngState;
        int maxSpawnedPeds;
        int maxPedsCount;
        int randomPedID;
        int unk3;
    };

    class C_TrafficGenerator : public C_Actor
    {
    public:
        C_TrafficGenerator_Interface* GetInterface()
        {
            return reinterpret_cast<C_TrafficGenerator_Interface*>(this);
        }

        void AI(unsigned int frameTime)
        {
            unsigned long funcAddress = C_TrafficGenerator_Enum::FunctionAddresses::AI;

            __asm
            {
                push frameTime
                mov ecx, this
                call funcAddress
            }
        }

        bool SaveData(C_Chunk* chunk)
        {
            unsigned long funcAddress = C_TrafficGenerator_Enum::FunctionAddresses::SaveData;
            bool result = false;

            __asm
            {
                push chunk
                mov ecx, this
                call funcAddress
                mov result, al
            }

            return result;
        }

        bool LoadData(C_Chunk* chunk)
        {
            unsigned long funcAddress = C_TrafficGenerator_Enum::FunctionAddresses::LoadData;
            bool result = false;

            __asm
            {
                push chunk
                mov ecx, this
                call funcAddress
                mov result, al
            }

            return result;
        }

        void DuplicateFrom(C_Actor* other)
        {
            unsigned long funcAddress = C_TrafficGenerator_Enum::FunctionAddresses::DuplicateFrom;

            __asm
            {
                push other
                mov ecx, this
                call funcAddress
            }
        }

        void GameInit()
        {
            unsigned long funcAddress = C_TrafficGenerator_Enum::FunctionAddresses::GameInit;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        void GameDone()
        {
            unsigned long funcAddress = C_TrafficGenerator_Enum::FunctionAddresses::GameDone;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }
    };
};

#endif
