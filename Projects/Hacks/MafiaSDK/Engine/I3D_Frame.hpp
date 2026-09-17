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
	/*
		Cross-checked against reMafia's I3D_frame (I3D_frame.h, same author,
		MafiaOrbitCam/Vendors/reMafia) - that source's own field declarations start counting
		from `m_iRefCount` as if it were offset 0, but I3D_frame has virtual functions, so
		the compiler puts an implicit vtable pointer at real offset 0 and everything
		reMafia declares is actually 4 bytes later than its own naive field order implies.
		Four independent checks confirm a uniform +4 correction: this SDK's pre-existing
		`position` (world position, e[12..14] of the world matrix) and `name` land exactly
		on reMafia's m_pSzName once shifted, and so do the pre-existing GetMatrix()/
		SetMatrix() (0x10 = start of the world matrix) and GetScale() (0x90 = m_aScale).

		`rotation` (originally guessed as 12 bytes right after `position`) does NOT hold up
		under this cross-check: those 12 bytes span the last float of the world matrix plus
		the first two floats of the *local* matrix, not any single meaningful vector - kept
		as-is here for compatibility since it's read/written directly elsewhere already, but
		treat it as suspect and prefer GetWorldDir()/GetLocalDir() below instead.
	*/
	struct I3D_Frame_Interface
	{
		unsigned long vtable;									// 0-4
		int refCount;											// 4-8
		unsigned long unk0;										// 8-12
		char* properties;										// 12-16
		PADDING(I3D_Frame_Interface, _pad1, 0x30);				// leading 3 rows of the world matrix
		S_vector position;										// 64-76 (world matrix translation row, e[12..14])
		S_vector rotation;										// 76-88 (see note above - likely not a valid vector)
		PADDING(I3D_Frame_Interface, _pad1b, 0x38);			// rest of the local matrix
		S_vector scale;											// 144-156
		S_quat rot;												// 156-172
		unsigned long flags;									// 172-176
		I3D_bound_volume localBoundVolume;						// 176-216
		I3D_bound_volume worldBoundVolume;						// 216-256
		char* name;												// 256-260
		char* modelName;										// 260-264
		I3D_CALLBACK* callback;								// 264-268
		I3D_Frame* owner;										// 268-272
		I3D_FRAME_TYPE frameType;								// 272-276
		PADDING(I3D_Frame_Interface, _pad2, 0x8);
		unsigned long childrenCount;							// 284-288
		I3D_Frame* prev;										// 288-292
		I3D_Frame* next;										// 292-296
		I3D_Frame* prevPrev;									// 296-300
		I3D_Frame* nextNext;									// 300-304
		PADDING(I3D_Frame_Interface, _pad3, 0x10);
		I3D_bbox dummyBBox;										// 320-344
		PADDING(I3D_Frame_Interface, _pad4, 0x2A);				// ~388 bytes total (reMafia's own estimate)
	};

	// Frame-flag bits (I3D_Frame_Interface::flags), ported from reMafia's I3D_frame.h.
	enum I3D_FRAME_FLAGS
	{
		FRMFLAGS_ON = 0x1,
		FRMFLAGS_VALID_WMATRIX = 0x20,
		FRMFLAGS_IN_FFTREE = 0x20000000,
		FRMFLAGS_UNK1 = 0x80000000
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

		void SetMatrix(S_matrix mat)
		{
			*(S_matrix*)((DWORD)this + 0x10) = mat;
			return;
		}

        void UpdateWMatrixProc()
        {
            DWORD update_mat = 0x0060fc30;

            __asm {
                mov ecx, this
                call update_mat
            }
        }

		/*
			The following are ported from reMafia's I3D_frame.h (same author,
			MafiaOrbitCam/Vendors/reMafia) and computed directly from the fields above
			rather than by calling into the game - prefer these over GetMatrix()/
			SetMatrix()/GetScale() where they overlap, and use GetWorldDir()/GetLocalDir()
			instead of the interface's own (likely-wrong, see the struct's comment) `rotation`.
		*/
		const S_matrix& GetWorldMat()
		{
			if (!(GetInterface()->flags & FRMFLAGS_VALID_WMATRIX))
				UpdateWMatrixProc();

			return *reinterpret_cast<S_matrix*>((DWORD)this + 0x10);
		}

		const S_matrix& GetLocalMat()
		{
			return *reinterpret_cast<S_matrix*>((DWORD)this + 0x50);
		}

		const S_vector& GetWorldDir()
		{
			return *reinterpret_cast<S_vector*>((DWORD)this + 0x30);
		}

		const S_vector& GetLocalDir()
		{
			return *reinterpret_cast<S_vector*>((DWORD)this + 0x70);
		}

		unsigned long GetChildrenCount()
		{
			return GetInterface()->childrenCount;
		}
    };
}

#endif
