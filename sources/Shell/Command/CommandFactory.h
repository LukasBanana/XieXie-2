/*
 * CommandFactory.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SHELL_COMMAND_FACTORY_H__
#define __XX_SHELL_COMMAND_FACTORY_H__


#include "Command.h"

#include <memory>
#include <map>


//! Shell command factory singleton.
class CommandFactory
{

    public:
        
        CommandFactory(const CommandFactory&) = delete;
        CommandFactory& operator = (const CommandFactory&) = delete;

        using CommandMap = std::map<std::string, std::unique_ptr<Command>>;

        //! Returns the instance of this singleton.
        static CommandFactory* Instance();
        
        /**
        Registers the new command under the specified name.
        \throws std::invalid_argument If the name is already used for another command.
        */
        void RegisterCommand(const std::string& name, std::unique_ptr<Command>&& command);

        //! Returns the command with the specified name, or null if there is no such command.
        Command* FindCommand(const std::string& name) const;

        //! Returns the map of all commands.
        const CommandMap& GetCommands() const
        {
            return commands_;
        }

    private:
        
        CommandFactory();

        void EstablishCommands();

        CommandMap commands_;

};


//! Declares an implementation of the "Command" interface
#define DECL_COMMAND_IMPL(name)                                         \
    class name##Command : public Command                                \
    {                                                                   \
        public:                                                         \
            void Execute(StreamParser& input, Log& output) override;    \
            void Help(HelpPrinter& printer) override;                   \
    }

DECL_COMMAND_IMPL( Log      );
DECL_COMMAND_IMPL( Pause    );
DECL_COMMAND_IMPL( Prompt   );
DECL_COMMAND_IMPL( Assemble );
DECL_COMMAND_IMPL( Reply    );
DECL_COMMAND_IMPL( Version  );
DECL_COMMAND_IMPL( Help     );
DECL_COMMAND_IMPL( Verbose  );
DECL_COMMAND_IMPL( Color    );

#undef DECL_COMMAND_IMPL


#endif



// ================================================================================