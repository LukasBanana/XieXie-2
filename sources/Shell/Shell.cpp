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


using namespace Console;

bool Shell::ExecuteCommand(const Command& cmd)
{
    if (cmd.IsEmpty())
        return false;
    
    /* Collect all options */
    const auto& args = cmd.GetArguments();

    for (auto it = args.begin(); it != args.end(); ++it)
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
                CmdAssemble(args);
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

    return true;
}

void Shell::WaitForUserInput()
{
    std::string input;

    while (true)
    {
        std::getline(std::cin, input);

        if (input == "exit" || input == "q" || input == "quit" || input == "esc" || input == "escape")
            break;

        ExecuteCommand(input);
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

void Shell::CmdAssemble(const Command::ArgumentListType& args)
{
    auto filename = args.back();

    Console::Message("assemble file \"" + filename + "\"");

    XieXie::Assembler assembler;

    if (assembler.AssembleFile(filename, filename + ".xbc"))
        Console::Success("assembling XASM file succeeded");
    else
        Console::Error("assembling XASM file failed");
}



// ================================================================================