/*
    Ported from reMafia's Actors/C_public_physics.h/.cpp (same author,
    MafiaOrbitCam/Vendors/reMafia). No fields of its own beyond C_PhThing's.
*/

#ifndef _CPUBLICPHYSICS_H_
#define _CPUBLICPHYSICS_H_

namespace MafiaSDK
{
    namespace C_PublicPhysics_Enum
    {
        enum FunctionAddresses
        {
            GameInit = 0x0047C3F0
        };
    };

    class C_PublicPhysics : public C_PhThing
    {
    public:
        void GameInit()
        {
            unsigned long funcAddress = C_PublicPhysics_Enum::FunctionAddresses::GameInit;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }
    };
};

#endif
