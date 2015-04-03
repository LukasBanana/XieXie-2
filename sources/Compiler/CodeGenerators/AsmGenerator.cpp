/*
 * AsmGenerator.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "AsmGenerator.h"
#include "CompilerMessage.h"


namespace CodeGenerator
{


AsmGenerator::AsmGenerator(std::ostream& outputStream, const std::string& indentStr) :
    stream_     ( outputStream ),
    indentStr_  ( indentStr    )
{
}
AsmGenerator::~AsmGenerator()
{
}

void AsmGenerator::Error(const std::string& msg)
{
    throw CodeGenError(msg);
}

void AsmGenerator::Line(const std::string& line)
{
    stream_ << indent_ << line << std::endl;
}

void AsmGenerator::Blank()
{
    if (config.blanks)
        stream_ << indent_ << std::endl;
}

void AsmGenerator::Blanks(size_t num)
{
    while (num-- > 0)
        Blank();
}

void AsmGenerator::IncIndent()
{
    if (config.indentation)
        indent_ += indentStr_;
}

void AsmGenerator::DecIndent()
{
    if (config.indentation)
    {
        if (indent_.size() > indentStr_.size())
            indent_.resize(indent_.size() - indentStr_.size());
        else
            indent_.clear();
    }
}


} // /namespace CodeGenerator



// ================================================================================