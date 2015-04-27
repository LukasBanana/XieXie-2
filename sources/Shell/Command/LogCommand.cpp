/*
 * LogCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"


void LogCommand::Execute(StreamParser& input, Log& output)
{
    auto msg = input.Accept();

    if (input.Get() == "-W" || input.Get() == "--warn")
    {
        output.Warning(msg);
        input.Accept();
    }
    else if (input.Get() == "-E" || input.Get() == "--error")
    {
        output.Error(msg);
        input.Accept();
    }
    else
        output.Message(msg);
}

void LogCommand::Help(HelpPrinter& printer)
{
    printer.Command("log TEXT [OPT]", "prints TEXT as a log message");
    printer.Flag("-W, --warn", "prints a warning");
    printer.Flag("-E, --error", "prints an error");
}



// ================================================================================