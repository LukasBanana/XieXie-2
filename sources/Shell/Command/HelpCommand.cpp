/*
 * HelpCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"
#include "HelpPrinter.h"


void HelpCommand::Execute(StreamParser& input, Log& output)
{
    output.Messages(
        {
            "----- man page: -----",
            "usage:",
            "  xxc COMMAND+",
            "",
            "commands:",
        }
    );

    HelpPrinter printer;
    for (const auto& cmd : CommandFactory::Instance()->GetCommands())
        cmd->Help(printer);

    SCOPED_INDENT(output);
    printer.Flush(output);
}

void HelpCommand::Help(HelpPrinter& printer) const
{
    printer.Command("help", "prints this man page");
}



// ================================================================================