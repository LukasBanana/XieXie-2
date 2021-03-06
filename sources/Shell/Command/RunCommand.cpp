/*
 * RunCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"

#include <xiexie/xiexie.h>


using namespace XieXie;

void RunCommand::Execute(StreamParser& input, Log& output)
{
    auto filename = input.Accept();

    auto exitCode = RunFromFile(
        filename,
        CompileFlags::Warn,
        VirtualMachine::Stack::defaultSize,
        &output.stream
    );

    if (exitCode != VirtualMachine::ExitCodes::Success)
        output.Error(VirtualMachine::ExitCodeString(exitCode));
}

void RunCommand::Help(HelpPrinter& printer) const
{
    printer.Command("run FILE", "runs a program directly from file");
}



// ================================================================================