
#include <functional>

#pragma once

namespace M2
{
    class ICCarActionThrowFrom
    {

    };

    class C_CarActionThrowFrom : public ICCarActionThrowFrom
    {

    };

#ifdef MAFIA2_SDK_IMPLEMENTATION
    namespace C_CarActionThrowFrom_Hooks
    {
        extern void HookTestAction(std::function<bool(C_Car *)>);

        namespace FunctionPointers
        {
            extern std::function<bool(C_Car *)> testAction;
        };

        namespace Functions
        {
            extern bool TestAction(C_Car *car);
        };

        namespace NakedFunctions
        {
            extern void CCarActionThrowFrom__TestAction__Hook();
        };

        extern void HookTestAction(std::function<bool(C_Car *)> ptr);
    };
#endif
}
