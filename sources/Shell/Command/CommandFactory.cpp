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

void CommandFactory::RegisterCommand(const std::initializer_list<std::string>& names, std::unique_ptr<Command>&& command)
{
    for (const auto& name : names)
    {
        if (!FindCommand(name))
            commandMap_[name] = command.get();
        else
            throw std::invalid_argument("\"" + name + "\" is already a registered shell command");
    }
    commands_.emplace_back(std::forward<std::unique_ptr<Command>>(command));
}

Command* CommandFactory::FindCommand(const std::string& name) const
{
    auto it = commandMap_.find(name);
    return it != commandMap_.end() ? it->second : nullptr;
}


/*
 * ======= Private: =======
 */

void CommandFactory::EstablishCommands()
{
    RegisterCommand({ "compile",  "C" }, MakeUnique< CompileCommand  >());
    RegisterCommand({ "assemble", "A" }, MakeUnique< AssembleCommand >());
    RegisterCommand({ "log"           }, MakeUnique< LogCommand      >());
    RegisterCommand({ "prompt"        }, MakeUnique< PromptCommand   >());
    RegisterCommand({ "pause"         }, MakeUnique< PauseCommand    >());
    RegisterCommand({ "version"       }, MakeUnique< VersionCommand  >());
    RegisterCommand({ "help"          }, MakeUnique< HelpCommand     >());
    RegisterCommand({ "verbose"       }, MakeUnique< VerboseCommand  >());
    RegisterCommand({ "reply"         }, MakeUnique< ReplyCommand    >());
    RegisterCommand({ "color"         }, MakeUnique< ColorCommand    >());
}



// ================================================================================
