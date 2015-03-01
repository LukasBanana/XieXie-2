/*
 * Shell file
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Shell.h"
#include "Version.h"
#include "StringModifier.h"

#include "Assembler.h"
#include "Scanner.h"
#include "SourceFile.h"
#include "Parser.h"
#include "ASTViewer.h"

#include <iostream>
#include <exception>
#include <sstream>


/*
!!!!!!! TODO -> Refactor the shell completely !!!!!!!
*/

using namespace SyntaxAnalyzer;

bool Shell::ExecuteCommandLine(const ArgList& args)
{
    if (args.empty())
        return false;
    
    /* Temporaries */
    std::vector<std::string> filenames;
    bool pause = false;
    bool scan = false;
    bool showPos = false;
    
    /* Parse all arguments */
    try
    {
        for (auto it = args.begin(), end = args.end(); it != end; ++it)
        {
            const auto& arg = *it;

            if (arg.empty())
                continue;

            if (arg.front() == '-')
            {
                if (arg == "--version")
                    CmdVersion();
                else if (arg == "--help" || arg == "-h")
                    CmdHelp();
                else if (arg == "--pause")
                    pause = true;
                else if (arg == "--scan")
                    scan = true;
                else if (arg == "--show-pos")
                    showPos = true;
                else
                    log_.Error("unknown command flag \"" + arg + "\"");
            }
            else if (arg == "input")
                WaitForUserInput();
            else if (arg == "help")
                CmdHelp();
            else
            {
                std::ifstream file(arg);
                if (file.good())
                    filenames.push_back(arg);
                else
                    log_.Error("file not found: \"" + arg + "\"");
            }
        }
    }
    catch (const std::exception& err)
    {
        log_.Error(err.what());
    }

    /* Process input files */
    if (scan)
    {
        for (const auto& file : filenames)
            ScanAndPrintTokens(file, showPos);
    }
    else
    {
        for (const auto& file : filenames)
            ProcessFile(file);
    }

    if (pause)
        Pause();

    return true;
}

void Shell::WaitForUserInput()
{
    std::string line, in;
    ArgList args;

    while (true)
    {
        /* Get command line input */
        std::getline(std::cin, line);
        std::istringstream stream(line);

        while (stream >> in)
        {
            if (in == "exit" || in == "q" || in == "quit" || in == "esc" || in == "escape")
                return;
            args.push_back(in);
        }

        /* Execute command line */
        ExecuteCommandLine(args);
        args.clear();
    }
}


/*
 * ======= Private: =======
 */

void Shell::CmdVersion()
{
    log_.Messages(
        {
            "XieXie Compiler " + Version::AsString(),
            "Copyright (c) 2014  Lukas Hermanns"
        }
    );
}

void Shell::CmdHelp()
{
    log_.Message("no help available yet");
    //...
}

void Shell::Pause()
{
    system("pause");
}

void Shell::ProcessFile(const std::string& filename)
{
    auto fileExt = ToLower(ExtractFileExtension(filename));
    if (fileExt == "xx")
        CompileFile(filename);
    else if (fileExt == "xasm")
        AssembleFile(filename);
    else
        log_.Error("unknown file extension \"" + fileExt + "\"");
}

void Shell::CompileFile(const std::string& filename)
{
    #if 1//!!!
    
    log_.Message("parse file \"" + filename + "\"");

    SyntaxAnalyzer::Parser parser;
    ErrorReporter errorReporter;

    auto program = parser.ParseSource(filename, errorReporter);

    errorReporter.Flush(log_);

    if (program)
    {
        ASTViewer viewer(log_);
        viewer.VisitProgram(program.get());
    }

    #endif
}

void Shell::AssembleFile(const std::string& filename)
{
    /* Assemble code */
    log_.Message("assemble file \"" + filename + "\"");

    XieXie::Assembler assembler(log_);

    if (assembler.AssembleFile(filename, filename + ".xbc"))
        log_.Success("assembling XASM file succeeded");
    else
        log_.Error("assembling XASM file failed");
}

void Shell::ScanAndPrintTokens(const std::string& filename, bool showPos)
{
    Scanner scanner;
    ErrorReporter errorReporter;

    auto source = std::make_shared<SourceFile>();
    if (source->ReadFile(filename))
    {
        scanner.ScanSource(source, errorReporter);

        TokenPtr tkn;

        while (true)
        {
            tkn = scanner.Next();
            if (tkn != nullptr && tkn->Type() != Token::Types::EndOfFile)
            {
                if (showPos)
                    log_.PrintLn("(" + tkn->Area().ToString() + ") '" + tkn->Spell() + "'");
                else
                    log_.PrintLn(tkn->Spell());
            }
            else
                break;
        }

        errorReporter.Flush(log_);
    }
}



// ================================================================================