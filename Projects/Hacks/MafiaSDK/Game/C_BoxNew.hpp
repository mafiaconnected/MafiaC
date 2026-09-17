/*
    Ported from reMafia's Actors/C_box_new.h/.cpp (same author, MafiaOrbitCam/Vendors/reMafia).
    A physical, hittable/collidable box or wheel-shaped prop.
*/

#ifndef _CBOXNEW_H_
#define _CBOXNEW_H_

namespace MafiaSDK
{
    namespace C_BoxNew_Enum
    {
        enum FunctionAddresses
        {
            Init = 0x00567540,
            SaveData = 0x00567AB0,
            DuplicateFrom = 0x00567C20,
            GameInit = 0x00567650
        };
    };

    struct C_BoxNew_Interface
    {
        C_PhThing_Interface phThing;							// 0-908 (approximate, see C_PhThing.hpp)
        bool isBox;
        PADDING(C_BoxNew_Interface, _pad0, 0x3);
        float unk0;
        float unk1;
        float mass;
        float friction;
        float unk4;
        bool wasHitted;
        bool unk2;
        PADDING(C_BoxNew_Interface, _pad1, 0x2);
    };

    class C_BoxNew : public C_PhThing
    {
    public:
        C_BoxNew_Interface* GetInterface()
        {
            return reinterpret_cast<C_BoxNew_Interface*>(this);
        }

        bool Init(I3D_Frame* frame)
        {
            unsigned long funcAddress = C_BoxNew_Enum::FunctionAddresses::Init;
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

        bool SaveData(C_Chunk* chunk)
        {
            unsigned long funcAddress = C_BoxNew_Enum::FunctionAddresses::SaveData;
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
            unsigned long funcAddress = C_BoxNew_Enum::FunctionAddresses::DuplicateFrom;

            __asm
            {
                push other
                mov ecx, this
                call funcAddress
            }
        }

        void GameInit()
        {
            unsigned long funcAddress = C_BoxNew_Enum::FunctionAddresses::GameInit;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }
    };
};

#endif
