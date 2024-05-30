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
    class ICCarActionOpenTrunk
    {

    };

    class C_CarActionOpenTrunk : public ICCarActionOpenTrunk
    {

    };

#ifdef MAFIA2_SDK_IMPLEMENTATION
    namespace C_CarActionOpenTrunk_Hooks
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
            extern void CCarActionOpenTrunk__TestAction__Hook();
        };

        extern void HookTestAction(std::function<bool(C_Car *)> ptr);
    };
#endif
}
