/*
 * Source area file
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "SourceArea.h"
#include "StringModifier.h"
#include "SourceCode.h"


namespace SyntaxAnalyzer
{


const SourceArea SourceArea::ignore { SourcePosition{ 1, 0 }, SourcePosition{ 0, 0 } };

SourceArea::SourceArea(const SourcePosition& pos, const std::shared_ptr<const SourceCode>& source) :
    start   { pos                                         },
    end     { SourcePosition(pos.Row(), pos.Column() + 1) },
    source_ { source                                      }
{
}
SourceArea::SourceArea(const SourcePosition& startPos, const SourcePosition& endPos, const std::shared_ptr<const SourceCode>& source) :
    start   { startPos },
    end     { endPos   },
    source_ { source   }
{
}
SourceArea::SourceArea(const SourceArea& rhs, const std::shared_ptr<const SourceCode>& source) :
    start   { rhs.start                                },
    end     { rhs.end                                  },
    source_ { source != nullptr ? source : rhs.source_ }
{
}

std::string SourceArea::ToString() const
{
    std::string str;

    if (source_ && !source_->Name().empty())
        str += source_->Name() + ": ";

    if (start == end)
        str += start.ToString();
    else
        str += start.ToString() + " - " + end.ToString();

    return str;
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
    return start <= end;
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

const SourceCode* SourceArea::GetSource() const
{
    return source_.get();
}


} // /namespace SyntaxAnalyzer



// ================================================================================
