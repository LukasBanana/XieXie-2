/*
 * Main file
 * 
 * This file is part of the "XieXie2-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Shell.h"
#include "Console.h"


int main(int argc, char* argv[])
{
    /* Execute all program arguments as shell commands */
    if (argc > 1)
    {
        Shell shell;
        for (int i = 1; i < argc; ++i)
            shell.ExecuteCommand(std::string(argv[i]));
    }
    else
        Console::Message("Enter \"xxc --help\" for the man page");

    return 0;
}



// ================================================================================