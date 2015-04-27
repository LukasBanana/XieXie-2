/*
 * VerboseCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"


void VerboseCommand::Execute(StreamParser& input, Log& output)
{
    if (input.Get() == "-on")
    {
        input.Accept();
        output.verbose = true;
    }
    else if (input.Get() == "-off")
    {
        input.Accept();
        output.verbose = false;
    }
    else
        output.verbose = true;
}

void VerboseCommand::Help(HelpPrinter& printer) const
{
    printer.Command("verbose [OPT]", "sets the log output to verbose or non-verbose");
    printer.Flag("-on", "enables colors (default)");
    printer.Flag("-off", "disables colors");
}



// ================================================================================