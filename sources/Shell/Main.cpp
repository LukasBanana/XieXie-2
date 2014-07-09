/*
 * Main file
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Shell.h"
#include "Console.h"


int main(int argc, char* argv[])
{
    /* Execute all program arguments as shell commands */
    if (argc > 1)
    {
        /* Get argument list */
        Shell::ArgList args;

        Shell shell;
        for (int i = 1; i < argc; ++i)
            args.push_back(argv[i]);

        /* Execute command line */
        shell.ExecuteCommandLine(args);
    }
    else
        Console::Message("Enter \"xxc --help\" for the man page");

    return 0;
}



// ================================================================================