/*
 * HelpCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"


void HelpCommand::Execute(StreamParser& input, Log& output)
{
    output.Messages(
        {
            "----- man page: -----",
            "usage:",
            "  xxc COMMAND+",
            "",
            "commands:",
            "  assemble, A [OPT+]   assembles several files",
            "    -f, --file FILE    specifies a file to assemble to 'FILE.xbc'",
            "",
            "  compile, C [OPT+]    compiles several files",
            "    -f, --file FILE    specifies a file to compile to 'FILE.xasm'",
            "    -show-ast          prints the abstract syntax tree (AST)",
            "    -show-cfg          writes the control flow graph (CFG) to a *.vg (DOT) file",
            "    -O, --optimize     enables all optimization passes",
            "    -W, --warn         enables all warnings",
            "",
            "  color [OPT]          enables/ disables console color manipulation (by default disabled)",
            "    -on                enables colors (default)",
            "    -off               disables colors",
            "",
            "  help                 prints this man page",
            "",
            "  log TEXT [OPT]       prints TEXT as a log message",
            "    -W, --warn         prints a warning",
            "    -E, --error        prints an error",
            "",
            "  pause                pauses the application",
            "",
            "  prompt               enters the command line prompt inside the application",
            "",
            "  verbose [OPT]        sets the log output to verbose or non-verbose",
            "    -on                enables colors (default)",
            "    -off               disables colors",
            "",
            "  version              prints the version information",
        }
    );
}



// ================================================================================