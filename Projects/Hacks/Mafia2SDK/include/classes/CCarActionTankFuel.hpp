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
    class ICCarActionTankFuel
    {

    };

    class C_CarActionTankFuel : public ICCarActionTankFuel
    {

    };

#ifdef MAFIA2_SDK_IMPLEMENTATION
    namespace C_CarActionTankFuel_Hooks
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
            extern void CCarActionTankFuel__TestAction__Hook();
        };

        extern void HookTestAction(std::function<bool(C_Car *)> ptr);
    };
#endif
}
