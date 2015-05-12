/*
 * xiexie.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "SearchPaths.h"
#include "Log.h"
#include "BitMask.h"
#include "SourceStream.h"
#include "StdCodeFactory.h"
#include "Platform/ConsoleManip.h"
#include "FileHelper.h"

#include "Parser.h"
#include "Decorator.h"
#include "GraphGenerator.h"
#include "CodeGenerators/XASM/XASMGenerator.h"
#include "Assembler.h"

#include "ASTViewer.h"
#include "CFGViewer.h"
#include "Optimizer.h"

#include <xiexie/xiexie.h>
#include <sstream>
#include <fstream>


namespace XieXie
{


using namespace VirtualMachine;
using namespace AbstractSyntaxTrees;
using namespace SyntaxAnalyzer;
using namespace ControlFlowGraph;
using namespace Platform::ConsoleManip;

/* --- Common --- */

void SetupPaths(const std::string& libraryPath, const std::string& modulesPath)
{
    SearchPaths::LibraryPath() = libraryPath;
    SearchPaths::ModulesPath() = modulesPath;
}

static std::unique_ptr<Log> MakeLogger(std::ostream* log)
{
    return log != nullptr ? MakeUnique<Log>(*log) : nullptr;
}

static void FlushErrors(ErrorReporter& errorReporter, const std::unique_ptr<Log>& log)
{
    if (log)
        errorReporter.Flush(*log);
}


/* --- Compilation Functions --- */

struct CompileOptions
{
    bool verbose    = false;
    bool optimize   = false;
    bool warnings   = false;
    bool showAST    = false;
    bool showTokens = false;
    bool showCFG    = false;
    bool showAsm    = false;
};

struct CompileState
{
    CompileState(Log* output, ErrorReporter&  errorReporter) :
        output          ( output        ),
        errorReporter   ( errorReporter )
    {
    }

    Log*            output;
    ErrorReporter&  errorReporter;
    CompileOptions  options;
    Program         astProgram;
    CFGProgramPtr   cfgProgram;
};

static void GetCompileOptions(CompileOptions& options, const BitMask& flags)
{
    if (flags( CompileFlags::Verbose    )) options.verbose      = true;
    if (flags( CompileFlags::Warn       )) options.warnings     = true;
    if (flags( CompileFlags::Optimize   )) options.optimize     = true;
    if (flags( CompileFlags::ShowAST    )) options.showAST      = true;
    if (flags( CompileFlags::ShowCFG    )) options.showCFG      = true;
    if (flags( CompileFlags::ShowTokens )) options.showTokens   = true;
}

static VirtualMachine::ByteCodePtr AssembleExt(std::istream& assembly, ErrorReporter& errorReporter)
{
    Assembler assembler;
    return assembler.Assemble(assembly, errorReporter);
}

static void LogMessage(CompileState& state, const std::string& msg)
{
    if (state.options.verbose && state.output)
        state.output->Message(msg);
}

static bool ParseProgram(const CompileConfig& config, CompileState& state)
{
    /* Initialize parser */
    Parser parser;

    std::stringstream tokenStream;
    if (state.options.showTokens)
        parser.tokenStream = &tokenStream;

    /* Generate built-in class declarations */
    ContextAnalyzer::StdCodeFactory::GenerateBuiltinClasses(state.astProgram);
    
    /* Parse input streams */
    std::set<std::string> passedSources;
    bool hasError = false;
    size_t numStreams = 0;

    for (auto& source : config.sources)
    {
        /* Keep track of files which have already been parsed */
        if (!source.filename.empty())
        {
            LogMessage(state, "parse file \"" + ExtractFilename(source.filename) + "\" ...");
            passedSources.insert(ToLower(source.filename));
        }
        else
            LogMessage(state, "parse stream (" + std::to_string(++numStreams) + ") ...");

        /* Parse source stream */
        if (!parser.ParseSource(state.astProgram, std::make_shared<SourceStream>(source.stream, source.filename), state.errorReporter))
            hasError = true;
    }

    /* Parse input filenames */
    std::vector<std::string> nextSources;

    while (true)
    {
        /* Collect import filenames */
        for (const auto& import : state.astProgram.importFilenames)
        {
            /* Check if filename has already been parsed */
            auto importLower = ToLower(import);
            if (passedSources.find(importLower) == passedSources.end())
            {
                nextSources.push_back(import);
                passedSources.insert(importLower);
            }
        }
        state.astProgram.importFilenames.clear();

        if (nextSources.empty())
            break;

        /* Process next sources */
        for (const auto& filename : nextSources)
        {
            LogMessage(state, "parse file \"" + ExtractFilename(filename) + "\" ...");

            /* Parse source file */
            if (!parser.ParseSource(state.astProgram, std::make_shared<SourceStream>(filename), state.errorReporter))
                hasError = true;
        }

        nextSources.clear();
    }

    /* Show debug output */
    if (state.options.showTokens && state.output)
    {
        LogMessage(state, "token stream dump:");
        ScopedColor scopedColor(state.output->stream, Color::Cyan);
        LogMessage(state, tokenStream.str());
    }

    return !hasError;
}

static bool DecorateProgram(CompileState& state)
{
    /* Decoreate AST */
    LogMessage(state, "context analysis ...");
    ContextAnalyzer::Decorator decorator;
    return decorator.DecorateProgram(state.astProgram, state.errorReporter);
}

static bool GenerateCFG(CompileState& state)
{
    /* Generate CFG */
    LogMessage(state, "graph generation ...");
    GraphGenerator converter;
    state.cfgProgram = converter.GenerateCFG(state.astProgram, state.errorReporter);

    if (state.errorReporter.HasErrors())
        return false;

    if (state.options.optimize)
    {
        /* Optimize CFG */
        LogMessage(state, "optimization ...");
        Optimization::Optimizer::OptimizeProgram(*state.cfgProgram);
    }

    return true;
}

static bool GenerateCode(const CompileConfig& config, CompileState& state)
{
    /* Generate code */
    LogMessage(state, "code generate ...");
    CodeGenerator::XASMGenerator generator(*config.assembly);
    return generator.GenerateAsm(*state.cfgProgram, state.errorReporter);
}

static void ShowAST(CompileState& state)
{
    if (state.output)
    {
        ASTViewer viewer(*state.output);
        viewer.ViewProgram(state.astProgram);
    }
}

static void ShowCFG(const CompileConfig& config, CompileState& state)
{
    if (!state.cfgProgram)
        return;

    CFGViewer viewer;

    size_t i = 0;
    for (const auto& ct : state.cfgProgram->classTrees)
    {
        const auto classDecl = ct->GetClassDeclAST();
        if (!classDecl->isBuiltin && !classDecl->isExtern && !classDecl->isModule)
        {
            LogMessage(state, "dump CFG class tree \"" + classDecl->ident + "\"");
            viewer.ViewGraph(*ct, config.cfgDumpPath);
        }
    }
}

static bool CompileExt(const CompileConfig& config, Log* log, ErrorReporter& errorReporter)
{
    /* Determine compilation options */
    CompileState state(log, errorReporter);
    GetCompileOptions(state.options, config.flags);

    auto showWarningsPrev = ErrorReporter::showWarnings;
    ErrorReporter::showWarnings = state.options.warnings;

    bool result = false;

    /* Parse program */
    if (ParseProgram(config, state))
    {
        /* Decorate program */
        if (DecorateProgram(state))
        {
            if (config.assembly)
            {
                /* Transform to CFG */
                auto cfgProgram = GenerateCFG(state);
                if (cfgProgram)
                {
                    /* Generate assembler code */
                    if (GenerateCode(config, state))
                        result = true;
                }
            }
            else
            {
                /*
                If code generation is to be ignored,
                set result to 'true' for success here
                */
                result = true;
            }
        }
    }

    /* Show syntax tree (AST) */
    if (state.options.showAST)
        ShowAST(state);

    /* Show flow graph (CFG) */
    if (state.options.showCFG)
        ShowCFG(config, state);

    ErrorReporter::showWarnings = showWarningsPrev;

    return result;
}

bool Compile(const CompileConfig& config, std::ostream* log)
{
    /* Initialize error reporter and logger */
    ErrorReporter errorReporter;
    auto logger = MakeLogger(log);

    /* Compile */
    auto result = CompileExt(config, logger.get(), errorReporter);

    /* Flush errors and return byte code */
    FlushErrors(errorReporter, logger);
    return result;
}

VirtualMachine::ByteCodePtr Assemble(std::istream& assembly, std::ostream* log)
{
    /* Initialize error reporter and logger */
    ErrorReporter errorReporter;
    auto logger = MakeLogger(log);

    /* Assemble */
    auto byteCode = AssembleExt(assembly, errorReporter);

    /* Flush errors and return byte code */
    FlushErrors(errorReporter, logger);
    return std::move(byteCode);
}


/* --- Very High-Level Functions --- */

static VirtualMachine::ByteCodePtr CompileFromStream(
    const std::shared_ptr<std::istream>& inputStream, const std::string& filename, int compileFlags, std::ostream* log)
{
    /* Initialize error reporter and logger */
    ErrorReporter errorReporter;
    auto logger = MakeLogger(log);

    /* Setup compilation configuration */
    std::stringstream assembly;

    CompileConfig config;
    config.sources  = { { filename, std::move(inputStream) } };
    config.assembly = &assembly;
    config.flags    = compileFlags;

    /* Compile */
    ByteCodePtr byteCode;
    if (CompileExt(config, logger.get(), errorReporter))
    {
        /* Assemble */
        assembly.seekg(0);
        byteCode = AssembleExt(assembly, errorReporter);
        if (byteCode)
            byteCode->BindAutomaticModules(SearchPaths::ModulesPath());
    }

    /* Flush errors and return byte code */
    FlushErrors(errorReporter, logger);
    return std::move(byteCode);
}

VirtualMachine::ByteCodePtr CompileFromString(const std::string& codeString, int compileFlags, std::ostream* log)
{
    auto inputStream = std::make_shared<std::stringstream>();
    *inputStream << codeString;
    return CompileFromStream(inputStream, "", compileFlags, log);
}

VirtualMachine::ByteCodePtr CompileFromFile(const std::string& codeFilename, int compileFlags, std::ostream* log)
{
    auto inputStream = std::make_shared<std::ifstream>(codeFilename);
    if (!inputStream->good())
        return nullptr;
    return CompileFromStream(inputStream, codeFilename, compileFlags, log);
}

ExitCodes RunFromString(const std::string& codeString, int compileFlags, const size_t stackSize, std::ostream* log)
{
    auto byteCode = CompileFromString(codeString, compileFlags, log);
    if (byteCode)
    {
        Stack stack(stackSize);
        return ExecuteProgram(*byteCode, stack);
    }
    return ExitCodes::InvalidByteCode;
}

ExitCodes RunFromFile(const std::string& codeFilename, int compileFlags, const size_t stackSize, std::ostream* log)
{
    auto byteCode = CompileFromFile(codeFilename, compileFlags, log);
    if (byteCode)
    {
        Stack stack(stackSize);
        return ExecuteProgram(*byteCode, stack);
    }
    return ExitCodes::InvalidByteCode;
}


} // /namespace XieXie



// ================================================================================
