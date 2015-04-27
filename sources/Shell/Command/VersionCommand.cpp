/*
 * VersionCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"
#include "Version.h"


void VersionCommand::Execute(StreamParser& input, Log& output)
{
    output.Blank();
    output.Message("  XieXie Compiler (XXC)");
    output.Message("  Version " + Version::AsString());
    output.Blank();
    output.Message("  Copyright (c) 2014-2015 Lukas Hermanns");
    output.Message("  All rights reserved.");
    output.Blank();
    output.Message("  This software may be modified and distributed under the terms");
    output.Message("  of the BSD license.  See the LICENSE file for details.");
    output.Blank();
}

void VersionCommand::Help(HelpPrinter& printer)
{
    printer.Command("version", "prints the version information");
}



// ================================================================================