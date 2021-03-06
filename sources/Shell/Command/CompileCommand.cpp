/*
 * CompileCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CompileCommand.h"
#include "FileHelper.h"
#include "StringModifier.h"

#include <xiexie/xiexie.h>
#include <fstream>


using namespace XieXie;

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

    /* Setup I/O stream for assembly */
    std::unique_ptr<std::iostream> assembly;
    if (options_.showAsm)
    {
        assembly = MakeUnique<std::fstream>(
            OutputFilename(outputFilename_, "xasm", output),
            (std::fstream::in | std::fstream::out | std::fstream::trunc)
        );
    }
    else
        assembly = MakeUnique<std::stringstream>();

    /* Setup compilation falgs */
    auto flags = GetCompilationFlags();

    if (output.verbose)
        flags << CompileFlags::Verbose;
    
    /* Setup compilation configuration */
    CompileConfig config;

    for (const auto& source : sources_)
    {
        auto file = std::make_shared<std::ifstream>(source);
        if (!file->good())
        {
            output.Error("reading source file \"" + source + "\" failed");
            return;
        }
        else
            config.sources.push_back({ source, file });
    }

    config.assembly     = assembly.get();
    config.cfgDumpPath  = options_.cfgDumpPath;
    config.flags        = flags;

    /* Compile code */
    if (Compile(config, &output.stream))
    {
        /* Assemble code */
        config.assembly->seekg(0);
        auto byteCode = Assemble(*config.assembly, &output.stream);
        if (byteCode)
        {
            /* Write byte code to file */
            if (byteCode->WriteToFile(OutputFilename(outputFilename_, "xbc", output)))
            {
                /* Print success message */
                if (output.verbose)
                    output.Success("compilation successful");
            }
        }
    }
}

void CompileCommand::Help(HelpPrinter& printer) const
{
    printer.Command("compile, C [OPT+]", "compiles several files");

    printer.Flags(
        {
            { "-f, --file FILE",       "specifies a file to compile to 'FILE.xasm'"                                     },
            { "-O, --optimize",        "enables all optimization passes"                                                },
            { "-W, --warn",            "enables all warnings"                                                           },
            { "-fo, --force-override", "forces the compiler to override existing files during code generation"          },
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

std::string CompileCommand::OutputFilename(const std::string& inputFilename, const std::string& fileExt, Log& output) const
{
    return FileHelper::SelectOutputFilename(inputFilename, fileExt, output, options_.forceOverride);
}

BitMask CompileCommand::GetCompilationFlags() const
{
    BitMask flags;

    if ( options_.optimize   ) flags << CompileFlags::Optimize;
    if ( options_.warnings   ) flags << CompileFlags::Warn;
    if ( options_.showAST    ) flags << CompileFlags::ShowAST;
    if ( options_.showCFG    ) flags << CompileFlags::ShowCFG;
    if ( options_.showTokens ) flags << CompileFlags::ShowTokens;

    return flags;
}



// ================================================================================