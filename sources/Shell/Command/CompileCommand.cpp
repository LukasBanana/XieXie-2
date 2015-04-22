/*
 * CompileCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CompileCommand.h"
#include "SourceStream.h"
#include "FileHelper.h"
#include "Platform/ConsoleManip.h"

#include "Parser.h"
#include "Decorator.h"
#include "GraphGenerator.h"
#include "StdCodeFactory.h"
#include "CodeGenerators/XASM/XASMGenerator.h"

#include "ASTViewer.h"
#include "CFGViewer.h"
#include "Optimizer.h"


#define _DEB_DISABLE_CFG_PER_DEFAULT_//!!!

using namespace AbstractSyntaxTrees;
using namespace SyntaxAnalyzer;
using namespace ControlFlowGraph;
using namespace Platform::ConsoleManip;

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
        if (input.Get() == "-out" || input.Get() == "--output")
        {
            input.Accept();
            outputFilename_ = input.Accept();
        }
        else if ( !( input.AcceptOption(options_.showAST, "--show-ast") ||
                     input.AcceptOption(options_.showTokens, "--show-tokens") ||
                     input.AcceptOption(options_.showCFG, "--show-cfg") ||
                     input.AcceptOption(options_.optimize, { "-O", "--optimize" }) ||
                     input.AcceptOption(options_.warnings, { "-W", "--warn" }) ||
                     input.AcceptOption(options_.forceOverride, { "-fo", "--force-override" }) ) )
        {
            break;
        }
    }
}

void CompileCommand::ParseProgram(AbstractSyntaxTrees::Program& program, Log& output)
{
    /* Initialize parser */
    Parser parser;

    std::stringstream tokenStream;
    if (options_.showTokens)
        parser.tokenStream = &tokenStream;

    /* Generate built-in class declarations */
    ContextAnalyzer::StdCodeFactory::GenerateBuiltinClasses(program);
    
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

    /* Show debug output */
    if (options_.showTokens)
    {
        output.Message("token stream dump:");
        ScopedColor scopedColor(output.stream, Color::Cyan);
        output.Message(tokenStream.str());
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
    for (const auto& ct : cfgProgram.classTrees)
    {
        const auto classDecl = ct->GetClassDeclAST();
        if (!classDecl->isBuiltin && !classDecl->isExtern && !classDecl->isModule)
        {
            if (output.verbose)
                output.Message("dump CFG class tree \"" + classDecl->ident + "\"");
            #ifdef _DEBUG
            viewer.ViewGraph(*ct, "cfg-dump/");
            #else
            viewer.ViewGraph(*ct);
            #endif
        }
    }
}



// ================================================================================