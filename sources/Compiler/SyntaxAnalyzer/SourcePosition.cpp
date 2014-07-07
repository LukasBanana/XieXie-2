/*
 * Source position file
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2013 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "SourcePosition.h"
#include "StringModifier.h"


namespace SyntaxAnalyzer
{


const SourcePosition SourcePosition::ignore;

SourcePosition::SourcePosition(unsigned int row, unsigned int column) :
    row_   ( row    ),
    column_( column )
{
}

std::string SourcePosition::ToString() const
{
    return ToStr(row_) + ":" + ToStr(column_);
}

void SourcePosition::IncRow()
{
    ++row_;
    column_ = 0;
}
void SourcePosition::IncColumn()
{
    ++column_;
}

bool SourcePosition::IsValid() const
{
    return row_ > 0 && column_ > 0;
}

void SourcePosition::Reset()
{
    row_ = column_ = 0;
}


} // /namespace SyntaxAnalyzer



// ================================================================================