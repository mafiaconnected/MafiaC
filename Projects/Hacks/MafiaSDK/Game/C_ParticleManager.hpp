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

#ifndef _CPARTICLE_MANAGER_H_
#define _CPARTICLE_MANAGER_H_

namespace MafiaSDK
{
    struct C_ParticleManager_Interface
    {

    };

    namespace C_ParticleManager_Enum
    {
        enum FunctionsAddresses
        {
            Construct = 0x00449080,
            Destruct = 0x00449100,
            Mission_Start = 0x00449180,
            Mission_Done = 0x004491D0,
            LoadData = 0x00449380,
            CreateParticle = 0x00449690,
            CreateParticleEx = 0x004496E0,
            ValidateParticleData = 0x0044A0D0,
            DestroyParticle	 = 0x0044A240,
        };
    };

    class C_ParticleManager
    {
    public:
        C_ParticleManager_Interface* GetInterface()
        {
            return reinterpret_cast<C_ParticleManager_Interface*>(this);
        }
        
    public:
        C_ParticleManager()
        {
            unsigned long funcAddr = C_ParticleManager_Enum::FunctionsAddresses::Construct;

            __asm
            {
                call funcAddr
                mov this, eax
            }
        }

        ~C_ParticleManager()
        {
            unsigned long funcAddr = C_ParticleManager_Enum::FunctionsAddresses::Destruct;

            __asm
            {
                mov ecx, this
                call funcAddr
            }

        }

        void Mission_Start()
        {
            unsigned long funcAddr = C_ParticleManager_Enum::FunctionsAddresses::Mission_Start;

            __asm
            {
                mov ecx, this
                call funcAddr
            }
        }

        void Mission_Done()
        {
            unsigned long funcAddr = C_ParticleManager_Enum::FunctionsAddresses::Mission_Done;

            __asm
            {
                mov ecx, this
                call funcAddr
            }
        }

        void LoadData()
        {
            unsigned long funcAddr = C_ParticleManager_Enum::FunctionsAddresses::LoadData;

            __asm
            {
                mov ecx, this
                call funcAddr
            }
        }

        int CreateParticle(int particleId, S_vector & vPos, S_vector & vRot, I3D_Frame* I3D_Frame)
        {
            unsigned long funcAddr = C_ParticleManager_Enum::FunctionsAddresses::CreateParticle;
            int iReturnId = 0;

            __asm
            {
                push I3D_Frame
                push 0
                push vPos
                push vRot
                push particleId
                mov ecx, this
                call funcAddr
                mov iReturnId, eax
            }

            return iReturnId;
        }


        void FindGuid(int iArg1, int & iArg2)
        {

        }

        void DestroyParticle(int iParticleId)
        {
            unsigned long funcAddr = C_ParticleManager_Enum::FunctionsAddresses::DestroyParticle;

            __asm
            {
                push iParticleId
                mov ecx, this
                call funcAddr
            }
        }
    };
};

#endif