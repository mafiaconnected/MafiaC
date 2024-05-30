/** @file CCarOwnerDB.cpp
*  @brief Game's action wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/
#include <functional>

#pragma once

namespace M2
{
    class ICCarActionOpenHood
    {

    };

    class C_CarActionOpenHood : public ICCarActionOpenHood
    {

    };

#ifdef MAFIA2_SDK_IMPLEMENTATION
    namespace C_CarActionOpenHood_Hooks
    {
        extern void HookTestAction(std::function<bool(C_Car *)>);

        namespace FunctionPointers
        {
            extern std::function<bool(C_Car *)> testAction;
        };

        namespace Functions
        {
            inline bool TestAction(C_Car *car);
        };

        namespace NakedFunctions
        {
            extern void CCarActionOpenHood__TestAction__Hook();
        };

        extern void HookTestAction(std::function<bool(C_Car *)> ptr);
    };
#endif
}
