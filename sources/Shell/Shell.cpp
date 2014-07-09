/*
 * Shell file
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Shell.h"
#include "Console.h"
#include "Version.h"

#include "Assembler.h"

#include <iostream>
#include <exception>
#include <sstream>


using namespace Console;

bool Shell::ExecuteCommandLine(const ArgList& args)
{
    if (args.empty())
        return false;
    
    /* Collect all options */
    try
    {
        for (auto it = args.begin(), end = args.end(); it != end; ++it)
        {
            const auto& arg = *it;

            if (arg.empty())
                continue;

            if (arg.front() == '-')
            {
                if (arg == "--version")
                    CmdVersion();
                else if (arg == "--help" || arg == "-h")
                    CmdHelp();
                else if (arg == "--assemble" || arg == "-asm")
                    CmdAssemble(it, end);
                else
                    Error("unknown command flag \"" + arg + "\"");
            }
            else if (arg == "input")
                WaitForUserInput();
            else if (arg == "help")
                CmdHelp();
            else
                Error("unknown command \"" + arg + "\"");
        }
    }
    catch (const std::exception& err)
    {
        Error(err.what());
    }

    return true;
}

void Shell::WaitForUserInput()
{
    std::string line, in;
    ArgList args;

    while (true)
    {
        /* Get command line input */
        std::getline(std::cin, line);
        std::istringstream stream(line);

        while (stream >> in)
        {
            if (in == "exit" || in == "q" || in == "quit" || in == "esc" || in == "escape")
                return;
            args.push_back(in);
        }

        /* Execute command line */
        ExecuteCommandLine(args);
        args.clear();
    }
}


/*
 * ======= Private: =======
 */

void Shell::CmdVersion()
{
    Messages(
        {
            "XieXie Compiler " + Version::AsString(),
            "Copyright (c) 2014  Lukas Hermanns"
        }
    );
}

void Shell::CmdHelp()
{
    //...
}

void Shell::CmdAssemble(ArgList::const_iterator& it, const ArgList::const_iterator& end)
{
    /* Get filename */
    ++it;
    if (it == end)
        throw std::runtime_error("missing filename to assembled source");

    auto filename = *it;

    /* Assemble code */
    Console::Message("assemble file \"" + filename + "\"");

    XieXie::Assembler assembler;

    if (assembler.AssembleFile(filename, filename + ".xbc"))
        Console::Success("assembling XASM file succeeded");
    else
        Console::Error("assembling XASM file failed");
}



// ================================================================================