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
#ifndef _AI_ACTION_MANAGER_H_
#define _AI_ACTION_MANAGER_H_

namespace MafiaSDK
{
	struct ai_action_manager_Interface
	{
	};

    struct ai_action 
    {
        DWORD action_id;
    };

	namespace ai_action_manager_Enum
	{
		enum FunctionAddresses
		{
			NewFollow = 0x00524D30,
            NewTurnTo = 0x00524DC0,
            AddJob =  0x00526AF0,
            AddIdle = 0x005271B0
		};
	};

	class ai_action_manager
	{
	public:
        ai_action* NewFollow(C_Actor* to_follow, float distance, int unk1, int unk2, int unk3, int unk4)
        {
            unsigned long funcAddress = ai_action_manager_Enum::FunctionAddresses::NewFollow;

            __asm 
            {
                push unk4
                push unk3
                push unk2
                push unk1
                push distance
                push to_follow
                mov ecx, this
                call funcAddress
            }
        }
        
        void NewTurnTo(C_Actor* to_turn, unsigned int action_id) 
        {
            unsigned long funcAddress = ai_action_manager_Enum::FunctionAddresses::NewTurnTo;

            __asm 
            {
                push action_id
                push to_turn
                mov ecx, this
                call funcAddress
            }
        }

        void AddJob(ai_action* job) 
        {
            unsigned long funcAddress = ai_action_manager_Enum::FunctionAddresses::AddJob;

            __asm 
            {
                push job
                mov ecx, this
                call funcAddress
            }
        }

        void AddIdle(ai_action* idle)
        {
             unsigned long funcAddress = ai_action_manager_Enum::FunctionAddresses::AddIdle;

            __asm 
            {
                push idle
                mov ecx, this
                call funcAddress
            }
        }
	};
}

#endif