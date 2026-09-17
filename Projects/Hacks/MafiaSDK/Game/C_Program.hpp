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

#ifndef _CPROGRAM_H_
#define _CPROGRAM_H_

namespace MafiaSDK
{
	// Ported from reMafia's C_program.h (same author, MafiaOrbitCam/Vendors/reMafia). Kept
	// as a self-contained addition: nothing in MafiaSDK previously relied on this struct
	// having any particular size (it was empty), so filling it in here is purely additive.
	struct C_Program_Interface
	{
		struct S_Instruction
		{
			unsigned long command;
			void* arguments;
		};

		void* vtable;
		char* programName;
		char* sourceCode;
		vc6_vector<S_Instruction> instructions;
		int curInstructionID;
		int curLine;
		int curInstructionIndex;
		int unk8;
		int unk9;
		void* subcall;
		int unk11;
		int unk12;
		int unk13;
		int unk14;
		C_Actor* userActor;
		float* variables;
		int floatCount;
		I3D_Frame** frameVariables;
		int frameCount;
		C_Actor** actorVariables;
		int actorCount;
		int unk15;
		bool unk16;
		bool unk17;
		PADDING(C_Program_Interface, _pad0, 0x2);
		int unk18;
	};

	namespace C_Program_Enum
	{
		enum FunctionAddresses
		{
			SetSourceCode = 0x00461530,
			Init = 0x00461580,
			Done = 0x004616A0
		};
	};

	class C_Program
	{
	public:
		C_Program_Interface* GetInterface()
		{
			return reinterpret_cast<C_Program_Interface*>(this);
		}

		void SetSourceCode(const char* sourceCode)
		{
			unsigned long funcAddress = C_Program_Enum::FunctionAddresses::SetSourceCode;

			__asm
			{
				push sourceCode
				mov ecx, this
				call funcAddress
			}
		}

		void Init()
		{
			unsigned long funcAddress = C_Program_Enum::FunctionAddresses::Init;

			__asm
			{
				mov ecx, this
				call funcAddress
			}
		}

		void Done()
		{
			unsigned long funcAddress = C_Program_Enum::FunctionAddresses::Done;

			__asm
			{
				mov ecx, this
				call funcAddress
			}
		}
	};
};

#endif