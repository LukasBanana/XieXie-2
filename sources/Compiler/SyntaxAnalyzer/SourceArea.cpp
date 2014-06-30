/*
 * Source area file
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2013 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "SourceArea.h"
#include "StringMod.h"


namespace SyntacticAnalyzer
{


const SourceArea SourceArea::ignore;

SourceArea::SourceArea(const SourcePosition& pos) :
    start( pos ),
    end  ( pos )
{
}
SourceArea::SourceArea(const SourcePosition& startPos, const SourcePosition& endPos) :
    start( startPos ),
    end  ( endPops  )
{
}

std::string SourceArea::ToString() const
{
    return start == end ? start.ToString() : start.ToString() + " - " + end.ToString();
}

bool SourceArea::IsValid() const
{
    return start <= end;
}

bool SourceArea::IsMultiLine() const
{
    return start.GetRow() != end.GetRow();
}


} // /namespace SyntacticAnalyzer



// ================================================================================