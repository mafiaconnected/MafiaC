/** @file CMafiaFramework.cpp
*  @brief Game's GameGuiModule wrapper
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
	class ICMafiaFramework
	{
	public:
	};

	class C_MafiaFramework : public GameClassWrapper<C_MafiaFramework, ICMafiaFramework, 0x1ABFE30>
	{
	public:
		int GetTimeFromStart()
		{
			return Mem::InvokeFunction<Mem::call_this, int>(0x058C070, this);
		}

		void SetActive(bool active)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x58C0B0, this, active);
		}

		static bool GetActive()
		{
			return Mem::InvokeFunction<Mem::call_this, int>(0x58C0B0);
		}

		void SetAudioSilent(bool silent)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x05985E0, this, silent);
		}

		void SetMinimized(bool minimized)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x058C090, this, minimized);
		}

		static bool GetMinimized()
		{
			return Mem::InvokeFunction<Mem::call_this, bool>(0x058C0F0);
		}

		void SetPaused(bool paused)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x058C120, this, paused);
		}

		static bool GetPaused()
		{
			return Mem::InvokeFunction<Mem::call_this, bool>(0x058C0F0);
		}
	};
};
