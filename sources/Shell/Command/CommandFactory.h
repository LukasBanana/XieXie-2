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


//! Declares an implementation of the "Command" interface
#define DECL_COMMAND_IMPL(name)                                         \
    class name##Command : public Command                                \
    {                                                                   \
        public:                                                         \
            void Execute(StreamParser& input, Log& output) override;    \
    }


namespace CommandFactory
{


std::unique_ptr<Command> InstantiateCommand(const std::string& cmdName);


}; // /namespace CommandFactory


DECL_COMMAND_IMPL( Log     );
DECL_COMMAND_IMPL( Pause   );
DECL_COMMAND_IMPL( Prompt  );
DECL_COMMAND_IMPL( Compile );
DECL_COMMAND_IMPL( Version );
DECL_COMMAND_IMPL( Help    );
DECL_COMMAND_IMPL( Verbose );
DECL_COMMAND_IMPL( Color   );


#endif



// ================================================================================