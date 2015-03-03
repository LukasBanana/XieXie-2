/*
 * HelpCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"


//!TODO! -> extend this man page !!!

void HelpCommand::Execute(StreamParser& input, Log& output)
{
    output.Messages(
        {
            "----- man page: -----",
            "usage:",
            "  xxc COMMAND+",
            "",
            "commands:",
            "  compile          compiles several files",
            "    -f FILE        specifies a file to compile",
            "    -show-ast      prints the abstract syntax tree (AST)",
        }
    );
}



// ================================================================================