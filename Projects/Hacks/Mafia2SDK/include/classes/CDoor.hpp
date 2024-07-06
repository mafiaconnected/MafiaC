/** @file CDoor.cpp
*  @brief Game's doors wrapper
*
*
*  @author DavoSK
*/

#include <functional>

#pragma once

namespace M2
{
    struct S_ContactEventInfo
    {

    };

    enum E_DoorContactType
    {
        CONTACT_TYPE_TOUCH = 10,
        CONTACT_TYPE_USE = 42 //Either kick or open
    };
    class C_Door
    {
    public:

        void AILock()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x44A700, this);
        }

        void AIUnlock()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x44A740, this);
        }

        void Close()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x4EAD30, this);
        }

        void DisableAction()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x44A850, this);
        }

        void EnableAction()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x44A7E0, this);
        }

        bool IsLocked()
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x44A780, this);
        }

        void Kick(const Vector3 & position)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x4F5110, this, position);
        }

        void Lock()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x4F10C0, this);
        }

        void Open(const Vector3 & position, bool unk1, unsigned int unk2)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x4EAC80, this, &position, unk1, unk2);
        }

        void StartLockpick()
        {
            Mem::InvokeFunction<Mem::call_this, bool>(0x4F1140, this);
        }

        void StopLockpick()
        {
            Mem::InvokeFunction<Mem::call_this, bool>(0x4F1160, this, true);
        }

        void ToggleOpen(const Vector3 & position, bool unk1)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x4EAD70, this, position, unk1);
        }

        void Unlock()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x4F10F0, this);
        }
    };

#ifdef MAFIA2_SDK_IMPLEMENTATION

    namespace C_Door_Hooks
    {
        extern void HookSolveContact(std::function<void(C_Door *, S_ContactEventInfo const&, E_DoorContactType)>);

        namespace FunctionPointers
        {
            extern std::function<void(C_Door *, S_ContactEventInfo const&, E_DoorContactType)> solveContact;
        }

        namespace Functions
        {
            extern void SolveContact(C_Door *thisInstance, S_ContactEventInfo const& ev, E_DoorContactType contactType);
        }

        namespace NakedFunctions
        {
            extern void SolveContact();
        }

        extern void HookSolveContact(std::function<void(C_Door *, S_ContactEventInfo const& ev, E_DoorContactType contactType)> ptr);
    }
#endif
};
