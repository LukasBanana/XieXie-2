/*
 * Shell header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SHELL_H__
#define __XX_SHELL_H__


#include "Command.h"


class Shell
{
    
    public:
        
        bool ExecuteCommand(const Command& cmd);

        void WaitForUserInput();

    private:
        
        void CmdVersion();
        void CmdHelp();

};


#endif



// ================================================================================