/*
 * Shell.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Shell.h"
#include "CommandFactory.h"


void Shell::Execute(StreamParser parser)
{
    try
    {
        while (parser)
        {
            /* Read next command */
            auto cmdName = parser.Get();
            auto cmd = CommandFactory::InstantiateCommand(cmdName);

            /* Execute command */
            if (cmd)
            {
                parser.Accept();
                cmd->Execute(parser, log_);
            }
            else
            {
                log_.Error("unknown command: " + cmdName);
                parser.Accept();
            }
        }
    }
    catch (const std::exception& err)
    {
        log_.Error(err.what());
    }
}



// ================================================================================