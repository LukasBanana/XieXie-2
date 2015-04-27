/*
 * AssembleCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"
#include "Assembler.h"


void AssembleCommand::Execute(StreamParser& input, Log& output)
{
    ErrorReporter errorReporter;
    XieXie::Assembler assembler;

    /* Parse input filenames */
    while (input.Get() == "-f" || input.Get() == "--file")
    {
        input.Accept();
        auto filename = input.Accept();

        if (output.verbose)
            output.Message("assemble file \"" + filename + "\" ...");

        /* Assemble source file */
        if (!assembler.Assemble(filename, filename + ".xbc", errorReporter))
            errorReporter.Flush(output);
        else
            output.Success("assembling successful");
    }
}



// ================================================================================