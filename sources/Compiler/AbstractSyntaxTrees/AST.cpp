/*
 * AST.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "BuiltinTypeDenoter.h"
#include "CodeBlock.h"
#include "Stmnt.h"
#include "VarName.h"

#include <exception>
#include <initializer_list>
#include <algorithm>


namespace AbstractSyntaxTrees
{


/*bool ContainsString(const std::string& str, const std::initializer_list<std::string>& list)
{
    return std::find(list.begin(), list.end(), str) != list.end();
}*/


/* --- VarName --- */

std::string VarName::FullName(const std::string& sep) const
{
    return (next != nullptr) ? (ident + sep + next->FullName(sep)) : ident;
}


/* --- CodeBlock --- */

void CodeBlock::RefreshSourceArea()
{
    if (!stmnts.empty())
    {
        /* Refresh source area of statements first */
        for (auto& ast : stmnts)
            ast->RefreshSourceArea();

        /* Set source area to the begin and end of the statement list */
        sourceArea.start    = stmnts.front()->sourceArea.start;
        sourceArea.end      = stmnts.back()->sourceArea.end;
    }
}


} // /namespace AbstractSyntaxTrees



// ================================================================================