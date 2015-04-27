/*
 * ReplyCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"
#include "Assembler.h"


using XieXie::Assembler;

void ReplyCommand::Execute(StreamParser& input, Log& output)
{
    using Flags = Assembler::QueryFlags;

    /* Parse input argument */
    auto filename = input.Accept();
    BitMask flags;

    while (true)
    {
        if (!flags(Flags::ExportAddresses) && input.Get() == "--exp-addr")
        {
            input.Accept();
            flags << Flags::ExportAddresses;
        }
        else if (!flags(Flags::ImportAddresses) && input.Get() == "--imp-addr")
        {
            input.Accept();
            flags << Flags::ImportAddresses;
        }
        else if (!flags(Flags::Invocations) && input.Get() == "--invk")
        {
            input.Accept();
            flags << Flags::Invocations;
        }
        else if ( !flags(Flags::SortByName) && ( input.Get() == "-S" || input.Get() == "--sort" ) )
        {
            input.Accept();
            flags << Flags::SortByName;
        }
        else
            break;
    }

    /* Show output */
    if (flags)
        Assembler::QueryByteCodeInformation(output, filename, flags);
    else
        output.Message("reply: nothing to be done");
}

void ReplyCommand::Help(HelpPrinter& printer) const
{
    printer.Command("reply FILE [OPT]", "shows information from the specified byte code file (*.xbc)");

    printer.Flags(
        {
            { "--exp-addr", "shows all export addresses" },
            { "--imp-addr", "shows all import addresses" },
            { "--invk",     "shows all invocations"      },
            { "-S, --sort", "sort output by names"       },
        }
    );
}



// ================================================================================