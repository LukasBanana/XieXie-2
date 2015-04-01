/*
 * XASMGenerator.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "XASMGenerator.h"
#include "Version.h"
#include "Timer.h"


namespace CodeGenerator
{


XASMGenerator::XASMGenerator(std::ostream& outputStream, const std::string& indentStr) :
    AsmGenerator{ outputStream, indentStr }
{
}

bool XASMGenerator::GenerateAsm(const std::vector<std::unique_ptr<ClassTree>>& classTrees, ErrorReporter& errorReporter)
{
    try
    {
        WriteHeader();

        //...

        return true;
    }
    catch (const CompilerMessage& err)
    {
        errorReporter.Add(err);
    }
    return false;
}


/*
 * ======= Private: =======
 */

/* --- Writing --- */

void XASMGenerator::Comment(const std::string& line)
{
    if (config.comments)
        Line("; " + line);
}

void XASMGenerator::GlobalLabel(const std::string& label)
{
    Line(label + ":");
}

void XASMGenerator::LocalLabel(const std::string& label)
{
    Line("." + label + ":");
}

void XASMGenerator::WORDAddress(const std::string& label)
{
    Line(".word @" + label);
}

void XASMGenerator::ASCII(const std::string& text)
{
    Line(".ascii \"" + text + "\"");
}

void XASMGenerator::WriteHeader()
{
    Comment("XieXie Assembler File (XASM)");
    Comment("XieXie Compiler Version " + Version::AsString());
    Comment("Generated at " + Timer::CurrentTime());
    Blank();
}

/* --- Code Generation --- */

//...


} // /namespace CodeGenerator



// ================================================================================