/*
 * CompileCommand.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SHELL_COMPILE_COMMAND_H__
#define __XX_SHELL_COMPILE_COMMAND_H__


#include "Command.h"

#include <string>
#include <vector>
#include <set>


class CompileCommand : public Command
{

    public:
        
        void Execute(StreamParser& input, Log& output) override;

    private:
        
        struct Options
        {
            bool hasError       = false;
            bool showAST        = false;
            bool showCFG        = false;
            bool optimize       = false;
            bool warnings       = false;
            bool forceOverride  = false;
        };

        void ParseFilenames(StreamParser& input);
        void ParseOptions(StreamParser& input);

        std::vector<std::string>    sources_;
        std::set<std::string>       passedSources_;
        std::string                 outputFilename_;

        Options                     options_;

        bool                        hasError_ = false;

};


#endif



// ================================================================================