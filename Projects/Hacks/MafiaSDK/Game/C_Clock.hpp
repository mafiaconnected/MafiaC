/*
    Ported from reMafia's Actors/C_clock.h/.cpp (same author, MafiaOrbitCam/Vendors/reMafia).
    Drives the hour/minute/second-hand frames of an in-world clock model from the real
    system clock (see AI()/GameInit() below).
*/

#ifndef _CCLOCK_H_
#define _CCLOCK_H_

namespace MafiaSDK
{
    struct C_Clock_Interface
    {
        C_Entity_Interface entity;								// 0-112
        I3D_Frame* hourHand;									// 112-116 ("MalaRucicka" = "small hand")
        I3D_Frame* minuteHand;									// 116-120 ("VelkaRucicka" = "big hand")
        I3D_Frame* secondHand;									// 120-124 ("SekundovaRucicka")
        float second;											// 124-128
        float hour;												// 128-132
        float minute;											// 132-136
    };

    class C_Clock : public C_Actor
    {
    public:
        C_Clock_Interface* GetInterface()
        {
            return reinterpret_cast<C_Clock_Interface*>(this);
        }
    };
};

#endif
