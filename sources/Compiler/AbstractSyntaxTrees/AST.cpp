/*
 * AST.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "BuiltinTypeDenoter.h"
#include "PostOperatorStmnt.h"
#include "ModifyAssignStmnt.h"
#include "CodeBlock.h"
#include "Stmnt.h"
#include "VarName.h"
#include "ProcSignature.h"

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


/* --- BuiltinTypeDenoter --- */

BuiltinTypeDenoter::TypeNames BuiltinTypeDenoter::GetTypeName(const std::string& spell)
{
    if (spell == "bool")
        return TypeNames::Bool;
    if (spell == "int")
        return TypeNames::Int;
    if (spell == "float")
        return TypeNames::Float;
    return TypeNames::Void;
}


/* --- PostOperatorStmnt --- */

PostOperatorStmnt::Operators PostOperatorStmnt::GetOperator(const std::string& spell)
{
    if (spell == "++")
        return Operators::Inc;
    if (spell == "--")
        return Operators::Dec;
    throw std::invalid_argument("invalid post-operator \"" + spell + "\"");
}


/* --- ModifyAssignStmnt --- */

ModifyAssignStmnt::Operators ModifyAssignStmnt::GetOperator(const std::string& spell)
{
    if (spell == "+=")
        return Operators::Add;
    if (spell == "-=")
        return Operators::Sub;
    if (spell == "*=")
        return Operators::Mul;
    if (spell == "/=")
        return Operators::Div;
    if (spell == "%=")
        return Operators::Mod;
    if (spell == "<<=")
        return Operators::LShift;
    if (spell == ">>=")
        return Operators::RShift;
    if (spell == "|=")
        return Operators::Or;
    if (spell == "&=")
        return Operators::And;
    if (spell == "^=")
        return Operators::XOr;
    throw std::invalid_argument("invalid modify-assign-operator \"" + spell + "\"");
}


/* --- ProcSignature --- */

ProcSignature::Modifiers ProcSignature::GetModifier(const std::string& spell)
{
    if (spell == "static")
        return Modifiers::Static;
    //if (spell == "abstract")
    //    return Modifiers::Abstract;
    throw std::invalid_argument("invalid procedure modifier \"" + spell + "\"");
}


} // /namespace AbstractSyntaxTrees



// ================================================================================