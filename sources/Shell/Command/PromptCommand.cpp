/*
 * PromptCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"
#include "Shell.h"


void PromptCommand::Execute(StreamParser& input, Log& output)
{
    if (output.verbose)
        output.Message("command prompt:");

    /* Execute command prompt with new shell instance */
    Shell shell;
    shell.Execute(std::cin);
}

void PromptCommand::Help(HelpPrinter& printer)
{
    printer.Command("prompt", "enters the command line prompt inside the application");
}



// ================================================================================