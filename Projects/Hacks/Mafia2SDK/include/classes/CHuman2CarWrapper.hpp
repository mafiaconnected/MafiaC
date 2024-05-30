/**
 * @file CHuman2CarWrapper.hpp
 * @brief Wrapper for human and car relation
 * @author DavoSK, Inlife
 */

#pragma once

namespace M2
{
    class ICHuman2CarWrapper
    {

    };

    class  C_Human2CarWrapper : ICHuman2CarWrapper
    {
    public:
        C_Human2CarWrapper()
        {
            Mem::InvokeFunction<Mem::call_this, DWORD*>(0x009560F0, this);
        }

        void Init(M2::C_Actor* actor)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x00956120, this, actor);
        }

        C_Car *GetCar()
        {
            return Mem::InvokeFunction<Mem::call_this, C_Car*>(0x9235F0, this);
        }

        float GetSpeedFloat()
        {
            return Mem::InvokeFunction<Mem::call_this, float>(0x00940FE0, this);
        }

        bool IsCabriolet()
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x00940320, this);
        }
    };


    namespace C_Human2CarWrapper_Hooks
    {
        extern void HookIsFreeToGetIn(std::function<bool(C_Car *)>);

#ifdef MAFIA2_SDK_IMPLEMENTATION
        namespace FunctionPointers
        {
            extern std::function<bool(C_Car *)> isFreeToGetIn;
        };

        namespace Functions
        {
            extern inline bool TestAction(C_Car *car);
        };

        namespace NakedFunctions
        {
            extern void CHuman2CarWrapper__IsFreeToGetIn__Hook();
        };

        extern void HookIsFreeToGetIn(std::function<bool(C_Car *)> ptr);

#endif
    };

}
