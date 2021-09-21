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

#ifndef _I3D_Frame_H_
#define _I3D_Frame_H_

namespace MafiaSDK
{
	struct I3D_Frame_Interface
	{
        PADDING(I3D_Frame_Interface, _pad1, 0x40);
		//S_matrix frame_matrix;
		S_vector position;
		S_vector rotation;
        PADDING(I3D_Frame_Interface, _pad2, 0xA8);
        char* name;
	};

    namespace I3D_Frame_Enum
    {
        enum FunctionsAddresses
        {   
            SetScale = 0x0047A750,
            SetPos = 0x00460980,
            SetRot = 0x0043BCE0
        };
    };

    class I3D_Frame
    {
    public:												
        virtual ~I3D_Frame(void);								
        virtual void __stdcall SetWorldPos(struct S_vector const &);								
        virtual void __stdcall SetWorldPosDir(struct S_vector const &, struct S_vector const &, float);								
        virtual void __stdcall SetDir(struct S_vector const &, float);				
        virtual void __stdcall SetWorldDir(struct S_vector const &, float);								
        virtual void nullsub_5(void);							
        virtual void __stdcall Update(void);							
        virtual void __stdcall SetCallback(unsigned int(__stdcall *)(class I3D_frame *, enum  LS3D_CALLBACK_MESSAGE, unsigned int, unsigned int), unsigned int);
        virtual enum  LS3D_RESULT __stdcall SetProperty(char const *);								
        virtual void __stdcall SetOn(bool);								
        virtual enum  LS3D_RESULT __stdcall SetName(char const *);
        virtual enum  LS3D_RESULT __stdcall LinkTo(class I3D_frame *, unsigned int);
        virtual class I3D_frame * __stdcall GetChild(int)const;								
        virtual enum  LS3D_RESULT __stdcall EnumFrames(enum  I3DENUMRET(__stdcall *)(class I3D_frame *, unsigned int), unsigned int, unsigned int, char const *)const;
        virtual class I3D_frame * __stdcall FindChildFrame(char const *, unsigned int)const;
        virtual enum  LS3D_RESULT __stdcall Duplicate(class I3D_frame * const);

        /* Non VTable functions */
        /* Some methods are implmented in Game.exe not in engine ! */
        I3D_Frame_Interface* GetInterface()
        {
            return reinterpret_cast<I3D_Frame_Interface*>(this);
        }

        void SetScale(const S_vector& scale)
        {
            unsigned long addressFunc = I3D_Frame_Enum::FunctionsAddresses::SetScale;

            __asm
            {
                push scale
                mov ecx, this
                call addressFunc
            }

            this->Update();
        }

        S_vector GetScale()
        {
            return *(S_vector*)((DWORD)this + 0x90);
        }

        void SetRot(const S_quat& rot)
        {
            unsigned long addressFunc = I3D_Frame_Enum::FunctionsAddresses::SetRot;

            __asm
            {
                push rot
                mov ecx, this
                call addressFunc
            }

            this->Update();
        }

        S_matrix GetMatrix() 
        {
            return *(S_matrix*)((DWORD)this + 0x10);
        }

        void UpdateWMatrixProc()
        {
            DWORD update_mat = 0x0060fc30;

            __asm {
                mov ecx, this
                call update_mat
            }
        }
    };
}

#endif
