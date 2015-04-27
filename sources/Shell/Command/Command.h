/*
 * Command.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SHELL_COMMAND_H__
#define __XX_SHELL_COMMAND_H__


#include "Command.h"
#include "StreamParser.h"
#include "Log.h"
#include "HelpPrinter.h"


//! Shell command interface.
class Command
{
    
    public:
        
        virtual ~Command()
        {
        }

        //! Executes this shell command.
        virtual void Execute(StreamParser& input, Log& output) = 0;

        //! Adds the help document to the specified help printer.
        virtual void Help(HelpPrinter& printer) = 0;

};


#endif



// ================================================================================