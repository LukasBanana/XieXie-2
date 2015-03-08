/*
 * CompileCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"
#include "SourceFile.h"
#include "Parser.h"
#include "Decorator.h"
#include "GraphGenerator.h"
#include "Program.h"
#include "ASTViewer.h"
#include "CFGViewer.h"


void CompileCommand::Execute(StreamParser& input, Log& output)
{
    ErrorReporter errorReporter;
    bool hasError = false;
    bool showAST = false;
    bool showCFG = false;

    /* Parse program */
    AbstractSyntaxTrees::Program program;
    SyntaxAnalyzer::Parser parser;
    
    /* Parse input filenames */
    while (input.Get() == "-f")
    {
        input.Accept();
        auto filename = input.Accept();

        if (output.verbose)
            output.Message("parse file \"" + filename + "\" ...");

        /* Parse source file */
        if (!parser.ParseSource(program, std::make_shared<SyntaxAnalyzer::SourceFile>(filename), errorReporter))
            hasError = true;
    }

    /* Parse options */
    while (true)
    {
        if (input.Get() == "-show-ast" && !showAST)
        {
            input.Accept();
            showAST = true;
        }
        else if (input.Get() == "-show-cfg" && !showCFG)
        {
            input.Accept();
            showCFG = true;
        }
        else
            break;
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
    if (!hasError)
    {
        if (output.verbose)
            output.Message("AST to CFG conversion ...");

        ControlFlowGraph::GraphGenerator converter;
        auto classTrees = converter.GenerateCFG(program);

        /* Show flow graph */
        if (showCFG)
        {
            ControlFlowGraph::CFGViewer viewer;

            size_t i = 0;
            for (const auto& tree : classTrees)
            {
                std::string path = "classtree." + ToStr(++i);
                if (output.verbose)
                    output.Message("dump CFG class tree \"" + path + "\"");
                viewer.ViewGraph(*tree, path);
            }
        }
    }

    /* Print out errors */
    errorReporter.Flush(output);

    if (!hasError)
        output.Success("compilation successful");
}



// ================================================================================