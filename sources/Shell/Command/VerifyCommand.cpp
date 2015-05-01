/*
 * VerifyCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"
#include "ErrorReporter.h"

#include <xiexie/xiexie.h>
#include <fstream>


using namespace XieXie;

void VerifyCommand::Execute(StreamParser& input, Log& output)
{
    /*
    Setup compilation configuration, and skip "assembly",
    since this command shall not create any output file!
    */
    CompileConfig config;
    config.flags = CompileFlags::Warn;

    if (output.verbose)
        config.flags |= CompileFlags::Verbose;

    /* Parse input filenames */
    while (input.Get() == "-f" || input.Get() == "--file")
    {
        input.Accept();
        auto filename = input.Accept();

        /* Open file stream */
        auto file = std::make_shared<std::ifstream>(filename);
        if (!file->good())
        {
            output.Error("reading source file \"" + filename + "\" failed");
            return;
        }
        else
            config.sources.push_back({ filename, file });
    }

    /* Verify code for syntactical and contextual correctness */
    if (Compile(config, &output.stream))
        output.Success("verification successful");
    else
        output.Error("verification failed", false);
}

void VerifyCommand::Help(HelpPrinter& printer) const
{
    printer.Command("verify, V [OPT+]", "verifies the code for syntactical and contextual correctness");
    printer.Flag("-f, --file FILE", "specifies a file to verify");
}



// ================================================================================