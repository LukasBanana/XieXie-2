/*
 * CommandFactory.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"
#include "CompileCommand.h"
#include "MakeUnique.h"


CommandFactory::CommandFactory()
{
    EstablishCommands();
}

CommandFactory* CommandFactory::Instance()
{
    static CommandFactory instance;
    return &instance;
}

void CommandFactory::RegisterCommand(const std::string& name, std::unique_ptr<Command>&& command)
{
    if (!FindCommand(name))
        commands_[name] = std::move(command);
    else
        throw std::invalid_argument("\"" + name + "\" is already a registered shell command");
}

Command* CommandFactory::FindCommand(const std::string& name) const
{
    auto it = commands_.find(name);
    return it != commands_.end() ? it->second.get() : nullptr;
}


/*
 * ======= Private: =======
 */

void CommandFactory::EstablishCommands()
{
    RegisterCommand("log",      MakeUnique< LogCommand      >());
    RegisterCommand("prompt",   MakeUnique< PromptCommand   >());
    RegisterCommand("pause",    MakeUnique< PauseCommand    >());
    RegisterCommand("compile",  MakeUnique< CompileCommand  >());
    RegisterCommand("assemble", MakeUnique< AssembleCommand >());
    RegisterCommand("version",  MakeUnique< VersionCommand  >());
    RegisterCommand("help",     MakeUnique< HelpCommand     >());
    RegisterCommand("verbose",  MakeUnique< VerboseCommand  >());
    RegisterCommand("reply",    MakeUnique< ReplyCommand    >());
    RegisterCommand("color",    MakeUnique< ColorCommand    >());
}



// ================================================================================
