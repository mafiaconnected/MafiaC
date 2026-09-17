/*
    Ported from reMafia's C_using_object.h (same author, MafiaOrbitCam/Vendors/reMafia).
    No addresses known for this class itself - only its nested S_using_object data layout,
    embedded by value in C_door, C_detector and C_traffic_element.
*/

#ifndef _CUSINGOBJECT_H_
#define _CUSINGOBJECT_H_

namespace MafiaSDK
{
    class C_UsingObject
    {
    public:
        struct S_using_object
        {
            S_vector position;
            float distance;
            int unk1;
            unsigned long flags;
            I3D_Frame* frame;
            C_Actor* actor;
            int unk5;
            int unk6;
            int unk7;
            int useTextID;
            int unk9;
        };
    };
};

#endif
