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

#include <sstream>
#include <xiexie/xiexie.h>


using namespace Platform::ConsoleManip;
using namespace XieXie;

void Shell::Execute(StreamParser parser)
{
    try
    {
        while (parser)
        {
            /* Read next command */
            auto cmdName = parser.Get();
            auto cmd = CommandFactory::Instance()->FindCommand(cmdName);

            /* Process command line */
            if (cmd)
            {
                parser.Accept();

                if (parser.Get() == "--help")
                {
                    /* Print help for command */
                    parser.Accept();
                    HelpPrinter printer;
                    cmd->Help(printer);
                    printer.Flush(log_);
                }
                else
                {
                    /* Execute command */
                    cmd->Execute(parser, log_);
                }
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
        << "class Script { " << std::endl
        << "static void print(int val) { Intrinsics.printInt(val) }" << std::endl
        << "static void print(float val) { Intrinsics.printFloat(val) }" << std::endl
        << "static void print(String val) { Intrinsics.print(val.pointer()) }" << std::endl
        //<< "void print(int[] val) { foreach v : val { print(v) } }" << std::endl
        //<< "void print(float[] val) { foreach v : val { print(v) } }" << std::endl
        //<< "void print(String[] val) { foreach v : val { print(v) } }" << std::endl
        << "static void main() {" << std::endl
        /*<< "    var script := new Script()" << std::endl
        << "}" << std::endl
        << "init() {" << std::endl*/
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

    /* Run script immediately */
    auto exitCode = RunFromString(
        source->str(),
        CompileFlags::Warn,
        VirtualMachine::Stack::defaultSize,
        &log_.stream
    );

    if (exitCode != VirtualMachine::ExitCodes::Success)
        log_.Error(VirtualMachine::ExitCodeString(exitCode));
}



// ================================================================================