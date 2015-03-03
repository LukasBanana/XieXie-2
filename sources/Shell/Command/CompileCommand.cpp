/*
 * CompileCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"
#include "Parser.h"
#include "Decorator.h"
#include "Program.h"
#include "SourceFile.h"
#include "ASTViewer.h"


void CompileCommand::Execute(StreamParser& input, Log& output)
{
    ErrorReporter errorReporter;
    bool hasError = false;
    bool showAST = false;

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

    if (input.Get() == "-show-ast")
    {
        input.Accept();
        showAST = true;
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

    /* Print out errors */
    errorReporter.Flush(output);

    if (!hasError)
        output.Success("compilation successful");
}



// ================================================================================