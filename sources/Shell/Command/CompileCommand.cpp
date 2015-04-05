/*
 * CompileCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"
#include "SourceStream.h"

#include "Parser.h"
#include "Decorator.h"
#include "GraphGenerator.h"
#include "CodeGenerators/XASM/XASMGenerator.h"

#include "Program.h"
#include "ASTViewer.h"
#include "CFGViewer.h"
#include "Optimizer.h"


#define _DEB_DISABLE_CFG_PER_DEFAULT_//!!!

static bool DoesFileExist(const std::string& filename)
{
    std::ifstream file(filename);
    return file.good();
}

void CompileCommand::Execute(StreamParser& input, Log& output)
{
    ErrorReporter errorReporter;

    bool hasError       = false;
    bool showAST        = false;
    bool showCFG        = false;
    bool optimize       = false;
    bool forceOverride  = false;

    std::string outputFilename;

    /* Parse program */
    AbstractSyntaxTrees::Program program;
    SyntaxAnalyzer::Parser parser;
    
    /* Parse input filenames */
    while (input.Get() == "-f" || input.Get() == "--file")
    {
        input.Accept();
        auto filename = input.Accept();

        if (outputFilename.empty())
            outputFilename = filename;

        if (output.verbose)
            output.Message("parse file \"" + filename + "\" ...");

        /* Parse source file */
        if (!parser.ParseSource(program, std::make_shared<SyntaxAnalyzer::SourceStream>(filename), errorReporter))
            hasError = true;
    }

    /* Parse options */
    while (true)
    {
        if (input.Get() == "--show-ast" && !showAST)
        {
            input.Accept();
            showAST = true;
        }
        else if (input.Get() == "--show-cfg" && !showCFG)
        {
            input.Accept();
            showCFG = true;
        }
        else if ( ( input.Get() == "-O" || input.Get() == "--optimize" ) && !optimize)
        {
            input.Accept();
            optimize = true;
        }
        else if ( ( input.Get() == "-fo" || input.Get() == "--force-override" ) && !forceOverride)
        {
            input.Accept();
            forceOverride = true;
        }
        else if (input.Get() == "-out" || input.Get() == "--output")
        {
            input.Accept();
            outputFilename = input.Accept();
        }
        else
            break;
    }

    if (outputFilename.empty())
    {
        output.Error("missing output filename for code generation");
        return;
    }

    /* Decorate program */
    if (!hasError)
    {
        if (output.verbose)
            output.Message("context analysis ...");

        ContextAnalyzer::Decorator decorator;
        if (!decorator.DecorateProgram(program, errorReporter))
            hasError = true;
    }

    /* Show syntax tree */
    if (showAST)
    {
        AbstractSyntaxTrees::ASTViewer viewer(output);
        viewer.ViewProgram(program);
    }

    /* Transform to CFG */
    std::vector<std::unique_ptr<ControlFlowGraph::ClassTree>> classTrees;

    if (!hasError)
    {
        #ifdef _DEB_DISABLE_CFG_PER_DEFAULT_//!!!
        if (showCFG){
        #endif

        if (output.verbose)
            output.Message("AST to CFG conversion ...");

        ControlFlowGraph::GraphGenerator converter;
        classTrees = converter.GenerateCFG(program, errorReporter);

        if (errorReporter.HasErrors())
            hasError = true;
        else if (optimize)
        {
            /* Optimize CFG */
            if (output.verbose)
                output.Message("optimize CFG ...");
            Optimization::Optimizer::OptimizeProgram(classTrees);
        }

        #ifdef _DEB_DISABLE_CFG_PER_DEFAULT_//!!!!!
        }
        #endif
    }

    /* Generate assembler code */
    if (!hasError)
    {
        #ifdef _DEB_DISABLE_CFG_PER_DEFAULT_//!!!
        if (showCFG){
        #endif

        /* Create output file */
        std::string fileId;
        auto OutFile = [&outputFilename, &fileId]()
        {
            return outputFilename + fileId + ".xasm";
        };

        if (!forceOverride && DoesFileExist(OutFile()))
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
                while (DoesFileExist(OutFile()));
            }
        }

        std::ofstream outputFile(OutFile());

        /* Generate code */
        if (output.verbose)
            output.Message("generate code file \"" + OutFile() + "\" ...");

        CodeGenerator::XASMGenerator generator(outputFile);
        if (!generator.GenerateAsm(classTrees, errorReporter))
            hasError = true;

        #ifdef _DEB_DISABLE_CFG_PER_DEFAULT_//!!!!!
        }
        #endif
    }

    /* Show flow graph */
    if (showCFG)
    {
        ControlFlowGraph::CFGViewer viewer;

        size_t i = 0;
        for (const auto& tree : classTrees)
        {
            if (output.verbose)
                output.Message("dump CFG class tree \"" + tree->GetClassDeclAST()->ident + "\"");
            viewer.ViewGraph(*tree, "cfg-dump/");
        }
    }

    /* Print out errors */
    errorReporter.Flush(output);

    if (!hasError)
        output.Success("compilation successful");
}



// ================================================================================