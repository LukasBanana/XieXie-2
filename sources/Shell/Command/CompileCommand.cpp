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
#include "StdCodeFactory.h"

#include "Parser.h"
#include "Decorator.h"
#include "GraphGenerator.h"
#include "CodeGenerators/XASM/XASMGenerator.h"
#include "Assembler.h"

#include "ASTViewer.h"
#include "CFGViewer.h"
#include "Optimizer.h"


using namespace AbstractSyntaxTrees;
using namespace SyntaxAnalyzer;
using namespace ControlFlowGraph;
using namespace Platform::ConsoleManip;

void CompileCommand::Execute(StreamParser& input, Log& output)
{
    /* --- Compilation --- */

    /* Parse input stream */
    ReadArgs(input);

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

    /* Transform to CFG */
    CFGProgramPtr cfgProgram;
    if (!hasError_)
        cfgProgram = GenerateCFG(program, output);

    /* Generate assembler code */
    if (!hasError_ && cfgProgram)
        GenerateCode(*cfgProgram, output);

    /* --- Debug Output --- */

    /* Print out errors */
    errorReporter_.Flush(output);

    if (!hasError_)
        output.Success("compilation successful");

    /* Show syntax tree (AST) */
    if (options_.showAST)
        ShowAST(program, output);

    /* Show flow graph (CFG) */
    if (options_.showCFG && cfgProgram)
        ShowCFG(*cfgProgram, output);

    ErrorReporter::showWarnings = false;
}

void CompileCommand::Help(HelpPrinter& printer) const
{
    printer.Command("compile, C [OPT+]", "compiles several files");

    printer.Flags(
        {
            { "-f, --file FILE",       "specifies a file to compile to 'FILE.xasm'"                                     },
            { "-O, --optimize",        "enables all optimization passes"                                                },
            { "-W, --warn",            "enables all warnings"                                                           },
            { "-fo, --force-override", "forces the compiler to override existing filed during code generation"          },
            { "-out, --output FILE",   "specifies the output filename for the executable"                               },
            { "--show-ast",            "prints the abstract syntax tree (AST)"                                          },
            { "--show-tokens",         "prints the token stream"                                                        },
            { "--show-cfg PATH",       "writes the control flow graph (CFG) to a *.vg (DOT) file to the PATH directory" },
            { "--show-asm",            "writes the assembly to an output file"                                          },
        }
    );
}


/*
 * ======= Private: =======
 */

void CompileCommand::ReadArgs(StreamParser& input)
{
    /* Parse options */
    while (true)
    {
        if (input.Get() == "-f" || input.Get() == "--file")
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
        else if (input.Get() == "-out" || input.Get() == "--output")
        {
            input.Accept();
            outputFilename_ = input.Accept();
        }
        else if (input.Get() == "--show-cfg")
        {
            input.Accept();
            options_.cfgDumpPath    = input.Accept();
            options_.showCFG        = true;
        }
        else if ( !( input.AcceptOption(options_.showAST, "--show-ast") ||
                     input.AcceptOption(options_.showTokens, "--show-tokens") ||
                     input.AcceptOption(options_.showAsm, "--show-asm") ||
                     input.AcceptOption(options_.optimize, { "-O", "--optimize" }) ||
                     input.AcceptOption(options_.warnings, { "-W", "--warn" }) ||
                     input.AcceptOption(options_.forceOverride, { "-fo", "--force-override" }) ) )
        {
            break;
        }
    }
}

void CompileCommand::ParseProgram(Program& program, Log& output)
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

void CompileCommand::DecorateProgram(Program& program, Log& output)
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
    if (options_.showAsm)
    {
        /* Create output file */
        auto filenameAsm = OutputFilename(outputFilename_, "xasm", output);
        std::ofstream outputFile(filenameAsm);

        /* Generate code */
        if (output.verbose)
            output.Message("generate code file \"" + filenameAsm + "\" ...");

        CodeGenerator::XASMGenerator generator(outputFile);
        if (!generator.GenerateAsm(cfgProgram, errorReporter_))
            hasError_ = true;

        if (!hasError_)
        {
            /* Assemble source file */
            auto filenameByteCode = OutputFilename(outputFilename_, "xbc", output);
            if (output.verbose)
                output.Message("assemble file \"" + filenameByteCode + "\" ...");

            XieXie::Assembler assembler;
            if (!assembler.Assemble(filenameAsm, filenameByteCode, errorReporter_))
                hasError_ = true;
        }
    }
    else
    {
        /* Create I/O stream */
        std::stringstream assembly;

        /* Generate code */
        if (output.verbose)
            output.Message("generate code ...");

        CodeGenerator::XASMGenerator generator(assembly);
        if (!generator.GenerateAsm(cfgProgram, errorReporter_))
            hasError_ = true;

        if (!hasError_)
        {
            /* Assemble source file */
            auto filenameByteCode = OutputFilename(outputFilename_, "xbc", output);
            if (output.verbose)
                output.Message("assemble file \"" + filenameByteCode + "\" ...");

            XieXie::Assembler assembler;
            if (!assembler.Assemble(assembly, filenameByteCode, errorReporter_))
                hasError_ = true;
        }
    }
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
            viewer.ViewGraph(*ct, options_.cfgDumpPath);
        }
    }
}

std::string CompileCommand::OutputFilename(const std::string& inputFilename, const std::string& fileExt, Log& output) const
{
    return FileHelper::SelectOutputFilename(inputFilename, fileExt, output, options_.forceOverride);
}



// ================================================================================