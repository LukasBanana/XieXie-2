/*
 * CommandFactory.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"


namespace CommandFactory
{


template <typename T, typename... Args> std::unique_ptr<T> Make(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

std::unique_ptr<Command> InstantiateCommand(const std::string& cmdName)
{
    if (cmdName == "log")
        return Make<LogCommand>();
    if (cmdName == "prompt")
        return Make<PromptCommand>();
    if (cmdName == "pause")
        return Make<PauseCommand>();
    if (cmdName == "compile" || cmdName == "c")
        return Make<CompileCommand>();
    if (cmdName == "assemble" || cmdName == "asm")
        return Make<AssembleCommand>();
    if (cmdName == "version")
        return Make<VersionCommand>();
    if (cmdName == "help")
        return Make<HelpCommand>();
    if (cmdName == "verbose")
        return Make<VerboseCommand>();
    if (cmdName == "color")
        return Make<ColorCommand>();
    return nullptr;
}


} // /namespace CommandFactory



// ================================================================================