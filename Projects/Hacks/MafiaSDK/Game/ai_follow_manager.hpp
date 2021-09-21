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
#ifndef _AI_FOLLOW_MANAGER_H_
#define _AI_FOLLOW_MANAGER_H_

namespace MafiaSDK
{
	struct ai_follow_manager_Interface
	{
	};

	namespace ai_follow_manager_Enum
	{
		enum FunctionAddresses
		{
			AddFollower = 0x00532320,
            DelFollower = 0x00532190
		};
	};

	class ai_follow_manager
	{
	public:
        void AddFollower(C_Actor* follower, C_Actor* to_follow) 
        {
            unsigned long funcAddress = ai_follow_manager_Enum::FunctionAddresses::AddFollower;

            __asm 
            {
                push to_follow
                push follower
                mov ecx, this
                call funcAddress
            }
        }

        void DelFollower(C_Actor* follower) 
        {
            unsigned long funcAddress = ai_follow_manager_Enum::FunctionAddresses::DelFollower;

            __asm 
            {
                push follower
                mov ecx, this
                call funcAddress
            }
        }
	};

	ai_follow_manager* GetFollowManager()
    {
        return reinterpret_cast<ai_follow_manager*>(0x646D6C);
    }
}

#endif