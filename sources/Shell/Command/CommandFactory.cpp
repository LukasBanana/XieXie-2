/*
 * CommandFactory.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"
#include "CompileCommand.h"
#include "MakeUnique.h"


namespace CommandFactory
{


std::unique_ptr<Command> InstantiateCommand(const std::string& cmdName)
{
    if (cmdName == "log")
        return MakeUnique<LogCommand>();
    if (cmdName == "prompt")
        return MakeUnique<PromptCommand>();
    if (cmdName == "pause")
        return MakeUnique<PauseCommand>();
    if (cmdName == "compile" || cmdName == "C")
        return MakeUnique<CompileCommand>();
    if (cmdName == "assemble" || cmdName == "A")
        return MakeUnique<AssembleCommand>();
    if (cmdName == "version")
        return MakeUnique<VersionCommand>();
    if (cmdName == "help")
        return MakeUnique<HelpCommand>();
    if (cmdName == "verbose")
        return MakeUnique<VerboseCommand>();
    if (cmdName == "reply")
        return MakeUnique<ReplyCommand>();
    if (cmdName == "color")
        return MakeUnique<ColorCommand>();
    return nullptr;
}


} // /namespace CommandFactory



// ================================================================================
