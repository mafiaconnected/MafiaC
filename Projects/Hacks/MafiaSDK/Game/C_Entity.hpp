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
    struct C_Entity_Interface
    {
        unsigned long vtable;
        PADDING(C_Entity_Interface, _pad0, 0xC);
        unsigned long objectType;
        PADDING(C_Entity_Interface, _pad1, 0x10);
        S_vector position;
        S_vector rotation;
        PADDING(C_Entity_Interface, _pad2, 0x21);
        bool isActive;
        bool doRemove;
        PADDING(C_Entity_Interface, _pad3, 0x9);
        I3D_Frame *frame;
    };

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
