/*
	Copyright 2017 D�vid Svitana

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


#ifndef _CACTOR_H_
#define _CACTOR_H_

namespace MafiaSDK
{
	// Defined later in Game/C_Program.hpp - only needed here as a pointer/return type.
	class C_Program;

	// Placeholder for the hit-type argument of C_Actor::Hit - not yet reverse-engineered
	// (matches reMafia's own E_hit_type, which is equally empty there).
	enum E_hit_type
	{
	};

	struct C_Actor_Interface
	{
		C_Entity_Interface entity;
	};

	namespace C_Actor_Enum
	{
		enum FunctionAddresses
		{
			Activate = 0x47AFE0,
			SetActState = 0x406DA0,
			ForceAI = 0x00523D30,
		};

		// Non-virtual constructor reMafia calls (with the vtable pointer saved/restored around
		// it) when it needs to run the game's own C_actor field-initialisation logic.
		constexpr unsigned long Construct = 0x00405D20;
	};

	class C_Actor
	{
	public:
		enum E_act_state
		{
			IDLE,
			UNK2,
			MOVING,
		};

		C_Actor_Interface* GetInterface()
		{
			return reinterpret_cast<C_Actor_Interface*>(this);
		}

        I3D_Frame* GetFrame()
        {
            return *reinterpret_cast<I3D_Frame**>((DWORD)this + 0x68);
        }

		/*
			Virtual function table, cross-checked against reMafia's C_actor (Actors/C_actor.h) -
			C_Actor::Init below already called through the vtable manually via [eax+0x48] before
			this was added (offset 0x48 = slot 18 = Init's position here), which matches exactly
			and gives good confidence in the rest of this ordering. Garbage1-13 are unidentified/
			unused reserved slots that must stay in place to keep every later slot's offset correct.
		*/
		virtual int Garbage1() { return -1; }
		virtual int Garbage2(int garbage) { return 0; }
		virtual int Garbage3(int garbage) { return 0; }
		virtual int Garbage4(int garbage) { return 1; }
		virtual int Garbage5(int garbage, int garbage2) { return 0; }
		virtual int Garbage6(int garbage, int garbage2) { return 0; }
		virtual int Garbage7(char garbage) { return 0; }
		virtual int Garbage8(char garbage) { return 0; }
		virtual int Garbage9(char garbage) { return 0; }
		virtual int Garbage10() { return 1; }
		virtual int Garbage11(int garbage, int garbage2) { return 0; }
		virtual int Garbage12(int garbage) { return 0; }
		virtual void Garbage13() { }

		virtual void AI(unsigned int frameTime) { }
		virtual void Update(unsigned int frameTime) { }
		virtual void NetDirect(unsigned int frameTime) { }
		virtual ~C_Actor() { }
		virtual void Release() { }
		virtual bool Init(I3D_Frame* frame) { return false; }
		virtual bool SaveData(C_Chunk* chunk) { return false; }
		virtual bool LoadData(C_Chunk* chunk) { return false; }
		virtual void DuplicateFrom(C_Actor* other) { }
		virtual int SaveGameGetSize() { return 0; }
		virtual bool SaveGameSave(void** buf) { return false; }
		virtual bool SaveGameLoad(void** buf) { return false; }
		virtual void GameInit() { }
		virtual void GameDone() { }
		virtual void ChangeState() { }
		virtual void GetActorSpeed(S_vector& speed) { }
		virtual void Death() { }
		virtual bool Collision(C_Actor* actor, const S_vector& unk1, const S_vector& unk2, const S_vector& unk3, float unk, I3D_Frame* otherFrame) { return false; }
		virtual bool Hit(E_hit_type hitType, const S_vector& unk1, const S_vector& unk2, const S_vector& unk3, float damage, C_Actor* attacker, unsigned int unk, I3D_Frame* frame) { return false; }
		virtual int GetMaterialID(I3D_Frame* frame) { return 0; }
		virtual bool IsPossibleColl(C_Actor* actor, S_vector unk1, S_vector unk2, float unk, float& unk2Out, C_Actor** actorOut) { return false; }
		virtual bool GetBoundPoints2D(void* boundPoints, I3D_Frame* frame) { return false; }
		virtual C_Program* GetProgram() { return nullptr; }
		virtual bool cbCollTester(tDynamicCollObject* dynamicCollObj) { return false; }
		virtual void SoundsChange(int unk) { }
		virtual void ChangeSceneInit() { }
		virtual void ChangeSceneDone() { }

		void SetActState(int state)
		{
			unsigned long funcAddress = C_Actor_Enum::FunctionAddresses::SetActState;

			__asm
			{
				mov ecx, this
				push state
				call funcAddress
			}
		}

		void SetActive( BOOL active )
		{
			unsigned long funcAddress = C_Actor_Enum::FunctionAddresses::Activate;
			__asm
			{
				mov ecx, this
				push active
				call funcAddress
			}
		}

		void ForceAI(unsigned int unk1, unsigned int unk2, unsigned int unk3, unsigned int unk4) 
		{
			unsigned long funcAddress = C_Actor_Enum::FunctionAddresses::ForceAI;
			__asm
			{
				push unk4
				push unk3
				push unk2
				push unk1
				mov ecx, this
				call funcAddress
			}
		}
	};
};

#endif
