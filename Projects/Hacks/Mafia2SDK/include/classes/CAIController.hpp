/** @file CAIController.hpp
*  @brief Game's ai class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CEntityPos.hpp"

namespace M2
{
    class ICAIController
    {
    public:
    };

    class C_AIController : public ICAIController
    {
    public:
        void NewCommand(void *cmd)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xF9F880, this, cmd);
        }
    };
};
