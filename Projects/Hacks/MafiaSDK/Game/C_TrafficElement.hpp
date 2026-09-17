/*
    Ported from reMafia's C_traffic_element.h (same author, MafiaOrbitCam/Vendors/reMafia).
    A single traffic/pedestrian instance spawned by a C_TrafficGenerator. Not itself a
    C_Actor - purely a data record the generator owns.
*/

#ifndef _CTRAFFICELEMENT_H_
#define _CTRAFFICELEMENT_H_

namespace MafiaSDK
{
    class C_TrafficElement
    {
    public:
        PADDING(C_TrafficElement, _pad0, 0x4);
        C_TrafficGenerator* generator;
        void* animMachine;
        bool isActive;
        S_vector position;
        S_vector direction;
        PADDING(C_TrafficElement, _pad1, 0x18);
        I3D_Frame* frame;
        I3D_Frame* headFrame;
        PADDING(C_TrafficElement, _pad2, 0x4);
        I3D_Frame* umbrellaFrame;
        PADDING(C_TrafficElement, _pad2_1, 0x4);
        float unk1;
        PADDING(C_TrafficElement, _pad2_2, 0x8);
        unsigned long targetNodeID;
        S_vector unk1v;
        PADDING(C_TrafficElement, _pad3, 0xC);
        unsigned long originNodeID;
        S_vector unkArr[4];
        PADDING(C_TrafficElement, _pad4_1, 0x8);
        I3D_Frame* unkFrame;
        float unk2;
        PADDING(C_TrafficElement, _pad4_2, 0x4);
        tDynamicCollObject collObject;
        PADDING(C_TrafficElement, _pad5, 0x44);
        bool hasCollisions;
        PADDING(C_TrafficElement, _pad5_1, 0x4);
        bool isWoman;
        PADDING(C_TrafficElement, _pad5_2, 0x13);
        int animIndex;
        int curAnimIndex;
        S_vector unk2v;
        PADDING(C_TrafficElement, _pad5_3, 0x34);
        unsigned long pedAnimType;
        PADDING(C_TrafficElement, _pad5_4, 0x4);
        C_UsingObject::S_using_object usingObject;
        PADDING(C_TrafficElement, _pad6, 0x4);
        unsigned short targetNodeType;
        PADDING(C_TrafficElement, _pad7, 0xA);
    };
};

#endif
