/*
 * CompileCommand.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SHELL_COMPILE_COMMAND_H__
#define __XX_SHELL_COMPILE_COMMAND_H__


#include "Command.h"
#include "Program.h"
#include "ErrorReporter.h"
#include "CFGProgram.h"

#include <string>
#include <vector>
#include <set>


using AbstractSyntaxTrees::Program;
using ControlFlowGraph::CFGProgram;
using ControlFlowGraph::CFGProgramPtr;

class CompileCommand : public Command
{

    public:
        
        void Execute(StreamParser& input, Log& output) override;

        void Help(HelpPrinter& printer) const override;

    private:
        
        struct Options
        {
            bool        hasError       = false;
            
            bool        showAST        = false;
            bool        showTokens     = false;
            bool        showCFG        = false;
            bool        showAsm        = false;

            bool        optimize       = false;
            bool        warnings       = false;
            bool        forceOverride  = false;
            
            std::string cfgDumpPath;
        };

        /* === Functions === */

        void ReadArgs(StreamParser& input);

        void ParseProgram(Program& program, Log& output);
        void DecorateProgram(Program& program, Log& output);
        CFGProgramPtr GenerateCFG(Program& program, Log& output);
        void GenerateCode(const CFGProgram& cfgProgram, Log& output);

        void ShowAST(Program& program, Log& output);
        void ShowCFG(const CFGProgram& cfgProgram, Log& output);

        /**
        Returns an available output filename, for the specified input filename.
        If the 'forceOverride' option is disabled, the user is asked to override an existing file.
        */
        std::string OutputFilename(
            const std::string& inputFilename, const std::string& fileExt, Log& output
        ) const;

        /* === Members === */

        ErrorReporter               errorReporter_;

        std::vector<std::string>    sources_;
        std::set<std::string>       passedSources_;
        std::string                 outputFilename_;

        Options                     options_;

        bool                        hasError_ = false;

};


#endif



// ================================================================================