/*
 * PauseCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"


void PauseCommand::Execute(StreamParser& input, Log& output)
{
    system("pause");
}

void PauseCommand::Help(HelpPrinter& printer)
{
    printer.Command("pause", "pauses the application");
}



// ================================================================================