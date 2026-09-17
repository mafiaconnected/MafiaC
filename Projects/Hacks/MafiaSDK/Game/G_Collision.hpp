/*
    Ported from reMafia's g_collision.h/.cpp (same author, MafiaOrbitCam/Vendors/reMafia).
*/

#ifndef _GCOLLISION_H_
#define _GCOLLISION_H_

namespace MafiaSDK
{
    class C_Actor;

    // Used for both static scenery collision volumes and ad-hoc "dynamic" collision
    // tests (e.g. C_Actor::cbCollTester); referenced by several actor types.
    struct tDynamicCollObject
    {
        unsigned long unk1;
        I3D_Frame* attachedFrame;
        unsigned long unk2;
        I3D_bbox* boundBoxUnk1;
        S_vector position;
        float scale;
        PADDING(tDynamicCollObject, _pad0, 0x4);
        I3D_bbox boundBox;
        S_matrix* worldMatrix;
        S_matrix* localMatrix;
        C_Actor* attachedActor;
    };

    namespace G_Collision_Enum
    {
        constexpr unsigned long CLASS_ADDRESS = 0x00647F48;

        enum FunctionAddresses
        {
            LoadCollision = 0x005C2B70,
            // NOTE: these two addresses are copied as-given from reMafia's g_collision.cpp, but their
            // 0x001Cxxxx range doesn't fit the 0x005Cxxxx-0x005Dxxxx range every sibling function here
            // falls in (Mafia.exe's own code section) - looks like a dropped/mistyped leading digit in
            // that source. Verify against a disassembler before relying on DynInsert/DynErase.
            DynInsert = 0x001C36C0,
            DynErase = 0x001C38B0,
            TestSphereB = 0x005D5940,
            TestLineV = 0x005C6B10
        };
    };

    class G_Collision
    {
    public:
        void LoadCollision(const char* name, BOOL unk)
        {
            unsigned long funcAddress = G_Collision_Enum::FunctionAddresses::LoadCollision;

            __asm
            {
                push unk
                push name
                mov ecx, this
                call funcAddress
            }
        }

        void DynInsert(tDynamicCollObject* collObject)
        {
            unsigned long funcAddress = G_Collision_Enum::FunctionAddresses::DynInsert;

            __asm
            {
                push collObject
                mov ecx, this
                call funcAddress
            }
        }

        unsigned long DynErase(tDynamicCollObject* collObject)
        {
            unsigned long funcAddress = G_Collision_Enum::FunctionAddresses::DynErase;
            unsigned long result = 0;

            __asm
            {
                push collObject
                mov ecx, this
                call funcAddress
                mov result, eax
            }

            return result;
        }

        // pOutPos/pOutNormal receive the resolved position/surface normal when a hit occurs
        void TestSphereB(const S_vector& pos, const S_vector& unk, float distance, S_vector* pOutPos, S_vector* pOutNormal, unsigned long unkFlags, unsigned long flags)
        {
            unsigned long funcAddress = G_Collision_Enum::FunctionAddresses::TestSphereB;

            __asm
            {
                push flags
                push unkFlags
                push pOutNormal
                push pOutPos
                push distance
                push unk
                push pos
                mov ecx, this
                call funcAddress
            }
        }

        // vRelDestination is relative to pos; pOutPos/pOutNormal receive the resolved hit position/surface normal
        void TestLineV(const S_vector& pos, const S_vector& vRelDestination, S_vector* pOutPos, S_vector* pOutNormal, unsigned long unkFlags, unsigned long flags)
        {
            unsigned long funcAddress = G_Collision_Enum::FunctionAddresses::TestLineV;

            __asm
            {
                push flags
                push unkFlags
                push pOutNormal
                push pOutPos
                push vRelDestination
                push pos
                mov ecx, this
                call funcAddress
            }
        }
    };

    inline G_Collision* GetCollisionManager()
    {
        return reinterpret_cast<G_Collision*>(G_Collision_Enum::CLASS_ADDRESS);
    }
};

#endif
