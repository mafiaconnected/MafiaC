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
    class ICCarActionEnter
    {

    };

    class C_CarActionEnter : public ICCarActionEnter
    {

    };

#ifdef MAFIA2_SDK_IMPLEMENTATION
    namespace C_CarActionEnter_Hooks
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
            extern void CCarActionEnter__TestAction__Hook();
        };

        extern void HookTestAction(std::function<bool(C_Car *)> ptr);
    };
#endif
}
