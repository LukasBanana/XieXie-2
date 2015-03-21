/*
 * Source area file
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "SourceArea.h"
#include "StringModifier.h"


namespace SyntaxAnalyzer
{


const SourceArea SourceArea::ignore;

SourceArea::SourceArea(const SourcePosition& pos) :
    // bracket initializer required for GCC
    start( pos                             ),
    end  ( { pos.Row(), pos.Column() + 1 } )
{
}
SourceArea::SourceArea(const SourcePosition& startPos, const SourcePosition& endPos) :
    start{ startPos },
    end  { endPos   }
{
}

std::string SourceArea::ToString() const
{
    return start == end ? start.ToString() : start.ToString() + " - " + end.ToString();
}

void SourceArea::IncRow()
{
    start.IncRow();
    end.IncRow();
}

void SourceArea::IncColumn()
{
    start.IncColumn();
    end.IncColumn();
}

bool SourceArea::IsValid() const
{
    return start < end;
}

void SourceArea::Reset()
{
    start.Reset();
    end.Reset();
}

bool SourceArea::IsMultiLine() const
{
    return start.Row() != end.Row();
}


} // /namespace SyntaxAnalyzer



// ================================================================================
