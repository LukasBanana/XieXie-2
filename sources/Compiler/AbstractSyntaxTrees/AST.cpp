/*
 * AST.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "BuiltinTypeDenoter.h"
#include "CodeBlock.h"
#include "Stmnt.h"

#include <exception>
#include <initializer_list>
#include <algorithm>


namespace AbstractSyntaxTrees
{


bool ContainsString(const std::string& str, const std::initializer_list<std::string>& list)
{
    return std::find(list.begin(), list.end(), str) != list.end();
}


/* --- BuiltinTypeDenoter --- */

BuiltinTypeDenoter::BasicTypes BuiltinTypeDenoter::BasicType() const
{
    /* Check for valid type denoters */
    if (typeName == "void")
        return BasicTypes::Void;
    if (typeName == "bool")
        return BasicTypes::Boolean;
    if (typeName == "int")
        return BasicTypes::Integer;
    if (typeName == "float")
        return BasicTypes::Float;
    
    /* Check for reserved type denoters */
    if (ContainsString(typeName, { "short", "ushort", "uint", "long", "ulong", "double" }))
        throw std::invalid_argument("reserved built-in type denoter \"" + typeName + "\" for future use");

    /* Unknown built-in type denoter */
    throw std::invalid_argument("unknown built-in type denoter \"" + typeName + "\"");
    return BasicTypes::Void;
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