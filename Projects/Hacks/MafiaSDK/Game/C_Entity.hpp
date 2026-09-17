/*
    Copyright 2017 Dávid Svitana

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

#ifndef _CENTITY_H_
#define _CENTITY_H_

namespace MafiaSDK
{
    // Cross-checked field-for-field against reMafia's C_actor (Actors/C_actor.h, same
    // author, MafiaOrbitCam/Vendors/reMafia) - every field this SDK already named
    // (objectType/position/rotation/isActive/doRemove/frame) landed at the exact same
    // byte offset there, so the previously-unnamed padding gaps below are filled in
    // from that source with high confidence. Field names kept as-is for compatibility.
    struct C_Entity_Interface
    {
        unsigned long vtable;									// 0-4
        unsigned long flags;									// 4-8
        unsigned long unk1;										// 8-12
        unsigned short refCount;								// 12-14
        PADDING(C_Entity_Interface, _pad0, 0x2);
        unsigned long objectType;								// 16-20
        unsigned long unk2;										// 20-24
        unsigned long id;										// 24-28
        unsigned long state;									// 28-32
        bool unkBool;											// 32-33
        PADDING(C_Entity_Interface, _pad1, 0x3);
        S_vector position;										// 36-48
        S_vector rotation;										// 48-60
        S_vector truePosition;									// 60-72
        S_quat trueRotation;									// 72-88
        I3D_Frame *previousFrame;								// 88-92
        bool isOn;												// 92-93
        bool isActive;											// 93-94
        bool doRemove;											// 94-95
        bool unk3;												// 95-96
        PADDING(C_Entity_Interface, _pad2, 0x2);
        bool unk4;												// 98-99
        bool isHidden;											// 99-100
        bool isInited;											// 100-101
        PADDING(C_Entity_Interface, _pad3, 0x3);
        I3D_Frame *frame;										// 104-108
        bool unk5;												// 108-109
        PADDING(C_Entity_Interface, _pad4, 0x3);
    };

	//const size_t Offset = offsetof(C_Entity_Interface, frame);
	//const size_t Size = sizeof(C_Entity_Interface::frame);

    namespace C_Entity_Enum
    {
        enum FunctionsAddresses
        {

        };
    };

    class C_Entity
    {
    public:
        C_Entity_Interface* GetInterface()
        {
            return reinterpret_cast<C_Entity_Interface*>(this);
        }
    };
};

#endif
