/*
 * CompileCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CompileCommand.h"
#include "SourceStream.h"
#include "FileHelper.h"

#include "Parser.h"
#include "Decorator.h"
#include "GraphGenerator.h"
#include "StdClassGenerator.h"
#include "CodeGenerators/XASM/XASMGenerator.h"

#include "ASTViewer.h"
#include "CFGViewer.h"
#include "Optimizer.h"


#define _DEB_DISABLE_CFG_PER_DEFAULT_//!!!

using namespace AbstractSyntaxTrees;
using namespace SyntaxAnalyzer;
using namespace ControlFlowGraph;

void CompileCommand::Execute(StreamParser& input, Log& output)
{
    /* Parse input stream */
    ReadFilenames(input);
    ReadOptions(input);

    if (outputFilename_.empty())
    {
        output.Error("missing output filename for code generation");
        return;
    }

    ErrorReporter::showWarnings = options_.warnings;

    /* Parse program */
    Program program;
    ParseProgram(program, output);

    /* Decorate program */
    if (!hasError_)
        DecorateProgram(program, output);

    /* Show syntax tree (AST) */
    if (options_.showAST)
        ShowAST(program, output);

    #ifdef _DEB_DISABLE_CFG_PER_DEFAULT_//!!!
    if (options_.showCFG){
    #endif
        
    /* Transform to CFG */
    CFGProgramPtr cfgProgram;
    if (!hasError_)
        cfgProgram = GenerateCFG(program, output);

    /* Generate assembler code */
    if (!hasError_ && cfgProgram)
        GenerateCode(*cfgProgram, output);

    /* Show flow graph (CFG) */
    if (options_.showCFG && cfgProgram)
        ShowCFG(*cfgProgram, output);

    #ifdef _DEB_DISABLE_CFG_PER_DEFAULT_//!!!!!
    }
    #endif

    /* Print out errors */
    errorReporter_.Flush(output);

    if (!hasError_)
        output.Success("compilation successful");

    ErrorReporter::showWarnings = false;
}


/*
 * ======= Private: =======
 */

void CompileCommand::ReadFilenames(StreamParser& input)
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

void CompileCommand::ReadOptions(StreamParser& input)
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

void CompileCommand::ParseProgram(AbstractSyntaxTrees::Program& program, Log& output)
{
    Parser parser;

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
            if (!parser.ParseSource(program, std::make_shared<SourceStream>(filename), errorReporter_))
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
}

void CompileCommand::DecorateProgram(AbstractSyntaxTrees::Program& program, Log& output)
{
    if (output.verbose)
        output.Message("context analysis ...");

    /* Decoreate AST */
    ContextAnalyzer::Decorator decorator;
    if (!decorator.DecorateProgram(program, errorReporter_))
        hasError_ = true;
}

CFGProgramPtr CompileCommand::GenerateCFG(Program& program, Log& output)
{
    if (output.verbose)
        output.Message("graph generation ...");

    GraphGenerator converter;
    auto cfgProgram = converter.GenerateCFG(program, errorReporter_);

    if (errorReporter_.HasErrors())
        hasError_ = true;
    else if (options_.optimize)
    {
        if (output.verbose)
            output.Message("optimization ...");

        /* Optimize CFG */
        Optimization::Optimizer::OptimizeProgram(*cfgProgram);

        #if 1//!!!TEMPORARY!!!
        for (auto& ct : cfgProgram->classTrees)
        {
            for (auto bb : ct->GetRootBasicBlocks())
                bb.second->Clean();
        }
        #endif
    }

    return cfgProgram;
}

void CompileCommand::GenerateCode(const CFGProgram& cfgProgram, Log& output)
{
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
    if (!generator.GenerateAsm(cfgProgram, errorReporter_))
        hasError_ = true;
}

void CompileCommand::ShowAST(Program& program, Log& output)
{
    ASTViewer viewer(output);
    viewer.ViewProgram(program);
}

void CompileCommand::ShowCFG(const CFGProgram& cfgProgram, Log& output)
{
    CFGViewer viewer;

    size_t i = 0;
    for (const auto& tree : cfgProgram.classTrees)
    {
        if (output.verbose)
            output.Message("dump CFG class tree \"" + tree->GetClassDeclAST()->ident + "\"");
        #ifdef _DEBUG
        viewer.ViewGraph(*tree, "cfg-dump/");
        #else
        viewer.ViewGraph(*tree);
        #endif
    }
}



// ================================================================================