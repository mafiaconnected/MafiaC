#ifndef _CPLAYER_H_
#define _CPLAYER_H_

namespace MafiaSDK
{
	struct C_Player_Interface 
	{
		C_Human_Interface humanObject;
	};

	namespace C_Player_Enum
	{
		enum FunctionAddresses
		{
			Do_AB_OwnerNULL = 0x00594300,
			AI = 0x005937B0,
            SetAlpha = 0x00595E10,
            SetAimed = 0x005952F0,
            LockControls = 0x00595F00
		};
	};

	class C_Player : public C_Human
	{
	public:
		C_Player_Interface* GetInterface()
		{
			return reinterpret_cast<C_Player_Interface*>(this);
		}

		void Do_AB_OwnerNULL()
		{
			unsigned long funcAddress = C_Player_Enum::FunctionAddresses::Do_AB_OwnerNULL;

			__asm
			{
				mov ecx, this
				call funcAddress
			}
		}

        void SetAimed()
        {
            unsigned long funcAddress = C_Player_Enum::FunctionAddresses::SetAimed;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        void SetAlpha(float alpha)
        {
            unsigned long funcAddress = C_Player_Enum::FunctionAddresses::SetAlpha;

            __asm
            {
                push alpha
                mov ecx, this
                call funcAddress
            }
        }

		void AI(unsigned int delta)
		{
			unsigned long funcAddress = C_Player_Enum::FunctionAddresses::AI;

			__asm
			{
				push delta
				mov ecx, this
				call funcAddress
			}
		}

        void LockControls(BOOL lockState)
        {
            unsigned long funcAddress = C_Player_Enum::FunctionAddresses::LockControls;

            __asm
            {
                push lockState
                mov ecx, this
                call funcAddress
            }
        }
	};
};

#endif
