/*
Copyright 2018 Dávid Svitana

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

#ifndef _CI3D_Cache_H_
#define _CI3D_Cache_H_

namespace MafiaSDK
{
    namespace C_I3D_cache_9I3D_model_Enum
    {
        enum FunctionsAddresses : unsigned long
        {
            C_I3D_cache_9I3D_model = 0x00647DD0,
            Open = 0x004087E0
        };
    };

    class C_I3D_cache_9I3D_model_
    {
    public:
        bool Open(I3D_Model* model, char const * modelName, unsigned int unk1, void* unk2, void* unk3, void* unk4) 
        {
            unsigned long functionAddress = C_I3D_cache_9I3D_model_Enum::FunctionsAddresses::Open;

            __asm
            {
                push unk4
                push unk3
                push unk2
                push unk1
                push modelName
                push model
                mov ecx, this
                call functionAddress
            }
        }
    };

    extern C_I3D_cache_9I3D_model_* GetModelCache();
}

#endif
