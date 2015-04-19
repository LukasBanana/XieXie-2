/*
 * Shell.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Shell.h"
#include "CommandFactory.h"
#include "Version.h"
#include "ConsoleManip.h"
#include "StringModifier.h"
#include "MakeUnique.h"

#include "SourceStream.h"
#include "Parser.h"
#include "Decorator.h"
#include "GraphGenerator.h"
#include "Program.h"
#include "StdCodeFactory.h"

#include <sstream>


using namespace Platform::ConsoleManip;

void Shell::Execute(StreamParser parser)
{
    try
    {
        while (parser)
        {
            /* Read next command */
            auto cmdName = parser.Get();
            auto cmd = CommandFactory::InstantiateCommand(cmdName);

            /* Execute command */
            if (cmd)
            {
                parser.Accept();
                cmd->Execute(parser, log_);
            }
            else
            {
                log_.Error("unknown command: " + cmdName);
                parser.Accept();
            }
        }
    }
    catch (const std::exception& err)
    {
        log_.Error(err.what());
    }
}

// Define the code for the default script
static void AppendDefaultScript(std::stringstream& sstr)
{
    sstr
        << "class XieXieScript { " << std::endl
        << "void print(int val) { Intrinsics.printInt(val) }" << std::endl
        << "void print(float val) { Intrinsics.printFloat(val) }" << std::endl
        << "void print(String val) { Intrinsics.print(val.pointer()) }" << std::endl
        << "void print(int[] val) { foreach v : val { print(v) } }" << std::endl
        << "void print(float[] val) { foreach v : val { print(v) } }" << std::endl
        << "void print(String[] val) { foreach v : val { print(v) } }" << std::endl
        << "String str(int val) { return \"\" }" << std::endl
        << "String str(float val) { return \"\" }" << std::endl
        << "static void main() {" << std::endl
        << "    XieXieScript script := new XieXieScript()" << std::endl
        << "}" << std::endl
        << "init() {" << std::endl
    ;
}

void Shell::Script(std::istream& stream, bool appendDefaultScript)
{
    /* Print info at start-up */
    std::cout << "XieXie Compiler " << Version::AsString() << std::endl;
    std::cout << "Copyright (c) 2014-2015 Lukas Hermanns" << std::endl;

    /* Read and print each line from input stream */
    std::string line;
    auto source = MakeUnique<std::stringstream>();

    size_t lineNum = 0;

    if (appendDefaultScript)
        AppendDefaultScript(*source);

    while (!stream.eof())
    {
        /* Print line number */
        {
            ScopedColor color(std::cout, Color::Green | Color::Blue);
            std::cout << NumberOffset(++lineNum, 10, '0') << "  ";
        }

        /* Print line */
        {
            ScopedColor color(std::cout, Color::Cyan);
            std::getline(stream, line);
            *source << line << std::endl;
        }
    }

    if (appendDefaultScript)
        *source << "}}" << std::endl;

    /* Compile and run script */
    ErrorReporter errorReporter;
    bool hasError = false;

    /* Generate built-in class declarations */
    AbstractSyntaxTrees::Program program;
    SyntaxAnalyzer::Parser parser;

    ContextAnalyzer::StdCodeFactory::GenerateBuiltinClasses(program);

    /* Parse program */
    if (!parser.ParseSource(program, std::make_shared<SyntaxAnalyzer::SourceStream>(std::move(source)), errorReporter))
        hasError = true;

    /* Decorate program */
    if (!hasError)
    {
        ContextAnalyzer::Decorator decorator;
        if (!decorator.DecorateProgram(program, errorReporter))
            hasError = true;
    }
    
    /* Generate CFG */
    /*ControlFlowGraph::GraphGenerator converter;
    auto classTrees = converter.GenerateCFG(program, errorReporter);

    if (errorReporter.HasErrors())
        hasError = true;*/

    /* Print out errors */
    errorReporter.Flush(log_);
}



// ================================================================================