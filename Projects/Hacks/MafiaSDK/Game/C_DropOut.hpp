/*
    Ported from reMafia's Actors/C_DropOut.h/.cpp (same author, MafiaOrbitCam/Vendors/reMafia).
    A droppable/pickuppable physics prop (e.g. a dropped weapon).
*/

#ifndef _CDROPOUT_H_
#define _CDROPOUT_H_

namespace MafiaSDK
{
    namespace C_DropOut_Enum
    {
        enum FunctionAddresses
        {
            AI = 0x00443D80,
            Update = 0x00443DA0,
            Init = 0x00443B30,
            GameInit = 0x00443D10,
            GameDone = 0x00443D30,
            ChangeState = 0x00443DC0
        };
    };

    struct C_DropOut_Interface
    {
        C_PhThing_Interface phThing;							// 0-908 (approximate, see C_PhThing.hpp)
        unsigned long unk;
        bool isInactive;
        PADDING(C_DropOut_Interface, _pad0, 0x3);
    };

    class C_DropOut : public C_PhThing
    {
    public:
        C_DropOut_Interface* GetInterface()
        {
            return reinterpret_cast<C_DropOut_Interface*>(this);
        }

        void AI(unsigned int frameTime)
        {
            unsigned long funcAddress = C_DropOut_Enum::FunctionAddresses::AI;

            __asm
            {
                push frameTime
                mov ecx, this
                call funcAddress
            }
        }

        void Update(unsigned int frameTime)
        {
            unsigned long funcAddress = C_DropOut_Enum::FunctionAddresses::Update;

            __asm
            {
                push frameTime
                mov ecx, this
                call funcAddress
            }
        }

        bool Init(I3D_Frame* frame)
        {
            unsigned long funcAddress = C_DropOut_Enum::FunctionAddresses::Init;
            bool result = false;

            __asm
            {
                push frame
                mov ecx, this
                call funcAddress
                mov result, al
            }

            return result;
        }

        void GameInit()
        {
            unsigned long funcAddress = C_DropOut_Enum::FunctionAddresses::GameInit;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        void GameDone()
        {
            unsigned long funcAddress = C_DropOut_Enum::FunctionAddresses::GameDone;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        void ChangeState()
        {
            unsigned long funcAddress = C_DropOut_Enum::FunctionAddresses::ChangeState;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }
    };
};

#endif
