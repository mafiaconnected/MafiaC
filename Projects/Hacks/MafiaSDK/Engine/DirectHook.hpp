#ifndef _DIRECT_HOOK_
#define _DIRECT_HOOK_

namespace MafiaSDK 
{
	namespace Direct 
	{
	#ifdef MAFIA_SDK_IMPLEMENTATION
		namespace FunctionsPointers
		{
			std::function<void(IDirect3DDevice8*)> hook_function;
		};

		namespace Functions
		{
			void AfterDeviceCreated()  
			{
			   	if(FunctionsPointers::hook_function != nullptr)
			   		FunctionsPointers::hook_function(*reinterpret_cast<IDirect3DDevice8**>(D3D_DEVICE_PTR));
			}
		};

		namespace NakedFunctions
		{
			unsigned long jump_back = 0x1006F6E6;
			__declspec(naked) void Hook_DeviceCreate()
			{
			    __asm
			    {
			        pushad
			            call Functions::AfterDeviceCreated
			        popad

			        jmp jump_back
			    }
			}
		};
		
		inline void HookAfterCreated(std::function<void(IDirect3DDevice8*)> function) 
		{
			if(function != nullptr)
				FunctionsPointers::hook_function = function;

			 MemoryPatcher::InstallJmpHook(0x1006F6D8, (DWORD)&NakedFunctions::Hook_DeviceCreate);
		}
	#endif
	}
};

#endif