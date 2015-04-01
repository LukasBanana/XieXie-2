/*
 * Main.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Shell.h"

#include <sstream>
#include <iostream>


int main(int argc, char* argv[])
{
    Shell shell;

    /* Execute all program arguments as shell commands */
    if (argc > 1)
    {
        /* Get argument stream */
        std::stringstream stream;

        for (int i = 1; i < argc; ++i)
            stream << argv[i] << std::endl;

        /* Execute argument stream */
        shell.Execute(stream);
    }
    else
        shell.Script(std::cin);
        //std::cout << "enter \"xxc help\" for the man page" << std::endl;

    return 0;
}



// ================================================================================