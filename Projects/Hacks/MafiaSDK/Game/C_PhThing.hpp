/*
    Ported from reMafia's Actors/C_PhThing.h/.cpp (same author, MafiaOrbitCam/Vendors/reMafia).
    Base class for physics-driven scenery objects (crates, drop-out items, public physics
    props). Overrides several C_Actor virtuals - since it doesn't declare any *new* virtuals
    of its own, this doesn't disturb C_Actor's vtable slot numbering.
*/

#ifndef _CPHTHING_H_
#define _CPHTHING_H_

namespace MafiaSDK
{
    // Opaque, not yet reverse-engineered - only ever passed around as pointers by InitBox/InitWheel.
    struct S_THING_BOX { };
    struct S_THING_WHEEL { };

    namespace C_PhThing_Enum
    {
        enum FunctionAddresses
        {
            AI = 0x0045A690,
            Update = 0x0045A7A0,
            Init = 0x00567540,
            SaveGameSave = 0x004603C0,
            SaveGameLoad = 0x004605A0,
            GameInit = 0x0045A120,
            GameDone = 0x0045A630,
            ChangeState = 0x0045ADC0,
            Collision = 0x0045F520,
            Hit = 0x0045F760,
            InitBox = 0x0045EE90,
            InitWheel = 0x0045ED30,
            ExternalImpuls = 0x00460000
        };
    };

    struct C_PhThing_Interface
    {
        C_Entity_Interface entity;								// 0-112
        int type;												// 112-116
        PADDING(C_PhThing_Interface, _pad0, 0x1);
        bool unkBool1;											// 117-118
        bool unkBool2;											// 118-119
        bool unkBool3;											// 119-120
        S_vector unkVec1;										// 120-132
        S_vector unkVec2;										// 132-144
        S_vector unkVec3WorldPos;								// 144-156
        PADDING(C_PhThing_Interface, _pad1, 0x4);
        S_vector worldRight;									// 160-172
        S_vector worldUp;										// 172-184
        tDynamicCollObject dynamicCollObject;					// 184-256 (approximate, tDynamicCollObject size estimated - see G_Collision.hpp)
        S_vector unkVec4;										// 256-268
        S_matrix worldMatrix;									// 268-332
        S_matrix inverseWorldMatrix;							// 332-396
        S_matrix inverseWorldMatrix2ZeroOrigin;				// 396-460
        S_matrix inverseWorldMatrix3ZeroOrigin;				// 460-524
        float unkFloat;											// 524-528
        float weight;											// 528-532
        S_vector velocity;										// 532-544
        S_vector unk1;											// 544-556
        S_vector maybeAngularVelocity;							// 556-568
        float unkFloat2;										// 568-572
        S_vector unkVec5;										// 572-584
        S_vector unkVec6;										// 584-596
        float gravity;											// 596-600
        float unkFloat4;										// 600-604
        float unkFloat5;										// 604-608
        float unkFloat3;										// 608-612
        PADDING(C_PhThing_Interface, _pad2, 0xD4);
        I3D_Sound* sound;										// 824-828
        S_vector maybeExternalForce;							// 828-840
        S_vector unkVec6b;										// 840-852
        PADDING(C_PhThing_Interface, _pad3, 0x24);
        bool unkBool;											// 888-889
        PADDING(C_PhThing_Interface, _pad4, 0x3);
        bool unkBool5;											// 892-893
        PADDING(C_PhThing_Interface, _pad5, 0x3);
        S_vector unkVec7;										// 896-908
    };

    class C_PhThing : public C_Actor
    {
    public:
        C_PhThing_Interface* GetInterface()
        {
            return reinterpret_cast<C_PhThing_Interface*>(this);
        }

        void InitBox(S_THING_BOX* box)
        {
            unsigned long funcAddress = C_PhThing_Enum::FunctionAddresses::InitBox;

            __asm
            {
                push box
                mov ecx, this
                call funcAddress
            }
        }

        void InitWheel(S_THING_WHEEL* wheel)
        {
            unsigned long funcAddress = C_PhThing_Enum::FunctionAddresses::InitWheel;

            __asm
            {
                push wheel
                mov ecx, this
                call funcAddress
            }
        }

        void ExternalImpuls(const S_vector& force, const S_vector& applyOffset)
        {
            unsigned long funcAddress = C_PhThing_Enum::FunctionAddresses::ExternalImpuls;

            __asm
            {
                push applyOffset
                push force
                mov ecx, this
                call funcAddress
            }
        }
    };
};

#endif
