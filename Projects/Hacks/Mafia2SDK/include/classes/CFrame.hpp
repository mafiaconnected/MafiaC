/** @file CFrame.hpp
*  @brief Game's class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
	class ICFrame
	{
	public:
	};

	class C_Frame : public ICFrame
	{
	public:
        int Clone()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x14DADC0, this);
        }

        void GetWorldMatrix(Utils::Matrix43 *mat)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x01410BC0, this, mat);
        }

		void MarkForNotify(int unk)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x14BA3D0, this, unk);
		}

        void NotifyEnableChange()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x1410F90, this);
        }

        void ProcessAllNotifications()
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x14AF6C0, this);
        }

		void SetName(const char *name)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x14BA350, this, name);
		}

        void Release()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x14E5BC0, this);
        }
	};
};
