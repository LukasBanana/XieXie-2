/*
 * CompileCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CompileCommand.h"
#include "SourceStream.h"
#include "AppPath.h"
#include "FileHelper.h"

#include "Parser.h"
#include "Decorator.h"
#include "GraphGenerator.h"
#include "StdClassGenerator.h"
#include "CodeGenerators/XASM/XASMGenerator.h"

#include "Program.h"
#include "ASTViewer.h"
#include "CFGViewer.h"
#include "Optimizer.h"


#define _DEB_DISABLE_CFG_PER_DEFAULT_//!!!

void CompileCommand::Execute(StreamParser& input, Log& output)
{
    /* Parse input stream */
    ParseFilenames(input);
    ParseOptions(input);

    if (outputFilename_.empty())
    {
        output.Error("missing output filename for code generation");
        return;
    }

    ErrorReporter::showWarnings = options_.warnings;

    /* <----- COMPILATION -----> */

    ErrorReporter errorReporter;

    /* Parse program */
    AbstractSyntaxTrees::Program program;
    SyntaxAnalyzer::Parser parser;

    /* Generate built-in class declarations */
    ContextAnalyzer::StdClassGenerator::GenerateBuiltinClasses(program);
    
    /* Parse input filenames */
    std::vector<std::string> nextSources;

    while (!sources_.empty())
    {
        for (const auto& filename : sources_)
        {
            if (output.verbose)
                output.Message("parse file \"" + ExtractFilename(filename) + "\" ...");

            /* Parse source file */
            if (!parser.ParseSource(program, std::make_shared<SyntaxAnalyzer::SourceStream>(filename), errorReporter))
                hasError_ = true;
        }

        /* Collect import filenames */
        for (const auto& import : program.importFilenames)
        {
            /* Check if filename has already been parsed */
            auto importLower = ToLower(import);
            if (passedSources_.find(importLower) == passedSources_.end())
            {
                nextSources.push_back(import);
                passedSources_.insert(importLower);
            }
        }
        program.importFilenames.clear();

        /* Process next sources */
        sources_ = std::move(nextSources);
    }

    /* Decorate program */
    if (!hasError_)
    {
        if (output.verbose)
            output.Message("context analysis ...");

        ContextAnalyzer::Decorator decorator;
        if (!decorator.DecorateProgram(program, errorReporter))
            hasError_ = true;
    }

    /* Show syntax tree */
    if (options_.showAST)
    {
        AbstractSyntaxTrees::ASTViewer viewer(output);
        viewer.ViewProgram(program);
    }

    /* Transform to CFG */
    ControlFlowGraph::CFGProgramPtr cfgProgram;

    if (!hasError_)
    {
        #ifdef _DEB_DISABLE_CFG_PER_DEFAULT_//!!!
        if (options_.showCFG){
        #endif

        if (output.verbose)
            output.Message("AST to CFG conversion ...");

        ControlFlowGraph::GraphGenerator converter;
        cfgProgram = converter.GenerateCFG(program, errorReporter);

        if (errorReporter.HasErrors())
            hasError_ = true;
        else if (options_.optimize)
        {
            /* Optimize CFG */
            if (output.verbose)
                output.Message("optimize CFG ...");
            Optimization::Optimizer::OptimizeProgram(*cfgProgram);
        }

        #ifdef _DEB_DISABLE_CFG_PER_DEFAULT_//!!!!!
        }
        #endif
    }

    /* Generate assembler code */
    if (!hasError_ && cfgProgram)
    {
        #ifdef _DEB_DISABLE_CFG_PER_DEFAULT_//!!!
        if (options_.showCFG){
        #endif

        /* Create output file */
        std::string fileId;
        auto OutFile = [&]()
        {
            return outputFilename_ + fileId + ".xasm";
        };

        if (!options_.forceOverride && FileHelper::DoesFileExist(OutFile()))
        {
            /* Ask user to override file */
            output.Warning("output file \"" + OutFile() + "\" already exists! override? (y/n)");

            char answer = 0;
            std::cin >> answer;

            if (answer != 'y' && answer != 'Y')
            {
                /* Find available filename */
                size_t i = 0;
                do
                {
                    fileId = std::to_string(++i);
                }
                while (FileHelper::DoesFileExist(OutFile()));
            }
        }

        std::ofstream outputFile(OutFile());

        /* Generate code */
        if (output.verbose)
            output.Message("generate code file \"" + OutFile() + "\" ...");

        CodeGenerator::XASMGenerator generator(outputFile);
        if (!generator.GenerateAsm(*cfgProgram, errorReporter))
            hasError_ = true;

        #ifdef _DEB_DISABLE_CFG_PER_DEFAULT_//!!!!!
        }
        #endif
    }

    /* Show flow graph */
    if (options_.showCFG && cfgProgram)
    {
        ControlFlowGraph::CFGViewer viewer;

        size_t i = 0;
        for (const auto& tree : cfgProgram->classTrees)
        {
            if (output.verbose)
                output.Message("dump CFG class tree \"" + tree->GetClassDeclAST()->ident + "\"");
            viewer.ViewGraph(*tree, "cfg-dump/");
        }
    }

    /* Print out errors */
    errorReporter.Flush(output);

    if (!hasError_)
        output.Success("compilation successful");

    ErrorReporter::showWarnings = false;
}


/*
 * ======= Private: =======
 */

void CompileCommand::ParseFilenames(StreamParser& input)
{
    /* Get source filenames from input stream */
    while (input.Get() == "-f" || input.Get() == "--file")
    {
        /* Parse filename */
        input.Accept();
        auto filename = input.Accept();

        if (outputFilename_.empty())
            outputFilename_ = filename;

        /* Add filename to list */
        sources_.push_back(filename);
        passedSources_.insert(ToLower(filename));
    }
}

void CompileCommand::ParseOptions(StreamParser& input)
{
    /* Parse options */
    while (true)
    {
        if (input.Get() == "--show-ast" && !options_.showAST)
        {
            input.Accept();
            options_.showAST = true;
        }
        else if (input.Get() == "--show-cfg" && !options_.showCFG)
        {
            input.Accept();
            options_.showCFG = true;
        }
        else if ( ( input.Get() == "-O" || input.Get() == "--optimize" ) && !options_.optimize)
        {
            input.Accept();
            options_.optimize = true;
        }
        else if ( ( input.Get() == "-W" || input.Get() == "--warn" ) && !options_.warnings)
        {
            input.Accept();
            options_.warnings = true;
        }
        else if ( ( input.Get() == "-fo" || input.Get() == "--force-override" ) && !options_.forceOverride)
        {
            input.Accept();
            options_.forceOverride = true;
        }
        else if (input.Get() == "-out" || input.Get() == "--output")
        {
            input.Accept();
            outputFilename_ = input.Accept();
        }
        else
            break;
    }
}



// ================================================================================