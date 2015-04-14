/*
 * AST.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ASTImport.h"
#include "MapTypeSpell.h"

#include <exception>
#include <initializer_list>
#include <algorithm>
#include <map>


namespace AbstractSyntaxTrees
{


/* --- VarName --- */

VarName::VarName(const SourceArea& area, const std::string& ident) :
    AST     { area  },
    ident   { ident }
{
}
VarName::VarName(const SourceArea& area, const std::vector<std::string>& identList) :
    AST{ area }
{
    if (!identList.empty())
    {
        ident = identList.front();
        VarName* parent = this;

        for (size_t i = 1, n = identList.size(); i < n; ++i)
        {
            parent->next = std::make_shared<VarName>(area);
            parent->next->ident = identList[i];
            parent = parent->next.get();
        }
    }
}

const TypeDenoter* VarName::GetTypeDenoter() const
{
    /* Get last variable name AST node */
    auto& last = GetLast();
    if (!last.declRef)
        return nullptr;
    
    /* Get base type denoter */
    auto varType = last.declRef->GetTypeDenoter();
    if (!varType)
        return nullptr;

    /* Get type denoter for array access */
    return varType->GetLast(last.arrayAccess.get());
}

std::string VarName::FullName(const std::string& sep) const
{
    auto name = ident;

    auto arrayNode = arrayAccess.get();
    while (arrayNode)
    {
        name += "[]";
        arrayNode = arrayNode->next.get();
    }

    if (next)
        name += sep + next->FullName(sep);

    return name;
}

VarName& VarName::GetLast()
{
    return next != nullptr ? next->GetLast() : *this;
}

const VarName& VarName::GetLast() const
{
    return next != nullptr ? next->GetLast() : *this;
}


/* --- VarDecl --- */

const TypeDenoter* VarDecl::GetTypeDenoter() const
{
    return parentRef != nullptr ? parentRef->GetTypeDenoter() : nullptr;
}

unsigned int VarDecl::MemorySize() const
{
    auto varType = GetTypeDenoter();
    return varType != nullptr ? varType->MemorySize() : 1;
}


/* --- VarDeclStmnt --- */

const TypeDenoter* VarDeclStmnt::GetTypeDenoter() const
{
    return typeDenoter.get();
}


/* --- LiteralExpr --- */

const TypeDenoter* LiteralExpr::GetTypeDenoter() const
{
    return typeDenoter.get();
}

void LiteralExpr::SetType(const Literals type)
{
    type_ = type;
    switch (type_)
    {
        case Literals::Bool:
        {
            auto builtinType = std::make_shared<BuiltinTypeDenoter>();
            builtinType->typeName = BuiltinTypeDenoter::TypeNames::Bool;
            typeDenoter = builtinType;
        }
        break;

        case Literals::Int:
        {
            auto builtinType = std::make_shared<BuiltinTypeDenoter>();
            builtinType->typeName = BuiltinTypeDenoter::TypeNames::Int;
            typeDenoter = builtinType;
        }
        break;

        case Literals::Float:
        {
            auto builtinType = std::make_shared<BuiltinTypeDenoter>();
            builtinType->typeName = BuiltinTypeDenoter::TypeNames::Float;
            typeDenoter = builtinType;
        }
        break;

        case Literals::String:
        {
            auto pointerType = std::make_shared<PointerTypeDenoter>();
            pointerType->declIdent = "String";
            typeDenoter = pointerType;
        }
        break;

        case Literals::Pointer:
            typeDenoter = std::make_shared<PointerTypeDenoter>();
            break;
    }
}


/* --- ProcCall --- */

const TypeDenoter* ProcCall::GetTypeDenoter() const
{
    return declStmntRef != nullptr ? declStmntRef->GetTypeDenoter() : nullptr;
}


/* --- ForRangeStmnt --- */

const TypeDenoter* ForRangeStmnt::GetTypeDenoter() const
{
    return &CommonTypeConstInt;
}


/* --- ForEachStmnt --- */

const TypeDenoter* ForEachStmnt::GetTypeDenoter() const
{
    /* Return lower type of array */
    auto listType = listExpr->GetTypeDenoter();
    if (listType && listType->Type() == AST::Types::ArrayTypeDenoter)
    {
        auto arrayType = static_cast<const ArrayTypeDenoter*>(listType);
        return arrayType->lowerTypeDenoter.get();
    }
    return nullptr;
}


/* --- ProcCallExpr --- */

const TypeDenoter* ProcCallExpr::GetTypeDenoter() const
{
    return procCall->GetTypeDenoter();
}


/* --- PostfixValueExpr --- */

const TypeDenoter* PostfixValueExpr::GetTypeDenoter() const
{
    if (procCall)
        return procCall->GetTypeDenoter();
    if (varName)
        return varName->GetTypeDenoter();
    return primaryValueExpr->GetTypeDenoter()->GetLast(arrayAccess.get());
}


/* --- UnaryExpr --- */

const TypeDenoter* UnaryExpr::GetTypeDenoter() const
{
    return expr->GetTypeDenoter();
}


/* --- CastExpr --- */

const TypeDenoter* CastExpr::GetTypeDenoter() const
{
    return castTypeDenoter.get();
}


/* --- AllocExpr --- */

const TypeDenoter* AllocExpr::GetTypeDenoter() const
{
    return typeDenoter.get();
}


/* --- Arg --- */

const TypeDenoter* Arg::GetTypeDenoter() const
{
    return expr->GetTypeDenoter();
}


/* --- Param --- */

const TypeDenoter* Param::GetTypeDenoter() const
{
    return typeDenoter.get();
}


/* --- InitListExpr --- */

const TypeDenoter* InitListExpr::GetTypeDenoter() const
{
    return &typeDenoter;
}

void InitListExpr::DeduceTypeDenoter()
{
    /* Find non-null pointer type in sub expressions */
    auto itRef = std::find_if(
        exprs.begin(), exprs.end(),
        [](const ExprPtr& expr)
        {
            auto exprType = expr->GetTypeDenoter();
            return exprType != nullptr && !exprType->IsNull();
        }
    );
    if (itRef != exprs.end())
        typeDenoter.lowerTypeDenoter = (*itRef)->GetTypeDenoter()->CopyRef();
}

const TypeDenoter* InitListExpr::GetDeducedTypeDenoter() const
{
    return typeDenoter.lowerTypeDenoter.get();
}

void InitListExpr::EstablishArrayType(const TypeDenoterPtr& lowerTypeDenoter)
{
    typeDenoter.lowerTypeDenoter = lowerTypeDenoter;
}


/* --- VarAccessExpr --- */

const TypeDenoter* VarAccessExpr::GetTypeDenoter() const
{
    return varName->GetTypeDenoter();
}


/* --- BuiltinTypeDenoter --- */

BuiltinTypeDenoter::BuiltinTypeDenoter(const TypeNames initTypeName, bool isConst) :
    typeName{ initTypeName },
    isConst { isConst      }
{
}

BuiltinTypeDenoter::BuiltinTypeDenoter(const SourceArea& area, const TypeNames initTypeName, bool isConst) :
    TypeDenoter { area         },
    typeName    { initTypeName },
    isConst     { isConst      }
{
}

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

TypeDenoterPtr BuiltinTypeDenoter::CopyRef() const
{
    auto copy = std::make_shared<BuiltinTypeDenoter>();
    copy->typeName  = typeName;
    copy->isConst   = isConst;
    return copy;
}

std::string BuiltinTypeDenoter::ToString() const
{
    switch (typeName)
    {
        case TypeNames::Void:
            return "void";
        case TypeNames::Bool:
            return "bool";
        case TypeNames::Int:
            return "int";
        case TypeNames::Float:
            return "float";
    }
    return "<unknown>";
}

unsigned int BuiltinTypeDenoter::MemorySize() const
{
    switch (typeName)
    {
        case TypeNames::Void:
            return 0;
        case TypeNames::Bool:
            return 1;
        case TypeNames::Int:
            return 4;
        case TypeNames::Float:
            return 4;
    }
    return 0;
}


/* --- CtrlTransferStmnt --- */

std::string CtrlTransferStmnt::GetTransferSpell(const Transfers transfer)
{
    switch (transfer)
    {
        case Transfers::Break:
            return "break";
        case Transfers::Continue:
            return "continue";
    }
    return "";
}


/* --- PostOperatorStmnt --- */

PostOperatorStmnt::Operators PostOperatorStmnt::GetOperator(const std::string& spell)
{
    return MapSpellToType<Operators>(
        spell,
        {
            { "++", Operators::Inc },
            { "--", Operators::Dec },
        },
        "invalid post-operator \"" + spell + "\""
    );
}


/* --- ModifyAssignStmnt --- */

ModifyAssignStmnt::Operators ModifyAssignStmnt::GetOperator(const std::string& spell)
{
    return MapSpellToType<Operators>(
        spell,
        {
            { "+=",  Operators::Add    },
            { "-=",  Operators::Sub    },
            { "*=",  Operators::Mul    },
            { "/=",  Operators::Div    },
            { "%=",  Operators::Mod    },
            { "<<=", Operators::LShift },
            { ">>=", Operators::RShift },
            { "|=",  Operators::Or     },
            { "&=",  Operators::And    },
            { "^=",  Operators::XOr    },
        },
        "invalid modify-assign-operator \"" + spell + "\""
    );
}


/* --- UnaryExpr --- */

static std::map<std::string, UnaryExpr::Operators> MapUnaryExpr()
{
    using Ty = UnaryExpr::Operators;
    return std::map<std::string, Ty>
    {
        { "not", Ty::LogicNot   },
        { "~",   Ty::BitwiseNot },
        { "-",   Ty::Negate     },
    };
}

UnaryExpr::Operators UnaryExpr::GetOperator(const std::string& spell)
{
    return MapSpellToType<Operators>(
        spell, MapUnaryExpr(), "invalid unary operator \"" + spell + "\""
    );
}

std::string UnaryExpr::GetOperatorSpell(const Operators op)
{
    return MapTypeToSpell<Operators>(op, MapUnaryExpr());
}


/* --- BinaryExpr --- */

static std::map<std::string, BinaryExpr::Operators> MapBinaryExpr()
{
    using Ty = BinaryExpr::Operators;
    return std::map<std::string, Ty>
    {
        { "or",  Ty::LogicOr      },
        { "and", Ty::LogicAnd     },
        { "|",   Ty::BitwiseOr    },
        { "^",   Ty::BitwiseXOr   },
        { "&",   Ty::BitwiseAnd   },
        { "=",   Ty::Equal        },
        { "!=",  Ty::Inequal      },
        { "<",   Ty::Less         },
        { "<=",  Ty::LessEqual    },
        { ">",   Ty::Greater      },
        { ">=",  Ty::GreaterEqual },
        { "+",   Ty::Add          },
        { "-",   Ty::Sub          },
        { "*",   Ty::Mul          },
        { "/",   Ty::Div          },
        { "%",   Ty::Mod          },
        { "<<",  Ty::LShift       },
        { ">>",  Ty::RShift       },
    };
}

BinaryExpr::Operators BinaryExpr::GetOperator(const std::string& spell)
{
    return MapSpellToType<Operators>(
        spell, MapBinaryExpr(), "invalid binary operator \"" + spell + "\""
    );
}

std::string BinaryExpr::GetOperatorSpell(const Operators op)
{
    return MapTypeToSpell<Operators>(op, MapBinaryExpr());
}

const TypeDenoter* BinaryExpr::GetTypeDenoter() const
{
    return HasBooleanOperator() ? (&CommonTypeBool) : lhsExpr->GetTypeDenoter();
}

bool BinaryExpr::HasBooleanOperator() const
{
    return
        binaryOperator == Operators::LogicOr  ||
        binaryOperator == Operators::LogicAnd ||
        ( binaryOperator >= Operators::Equal && binaryOperator <= Operators::GreaterEqual );
}

bool BinaryExpr::HasBoolCompatibleOperator() const
{
    return
        binaryOperator == Operators::LogicOr  ||
        binaryOperator == Operators::LogicAnd ||
        binaryOperator == Operators::Equal    ||
        binaryOperator == Operators::Inequal;
}


} // /namespace AbstractSyntaxTrees



// ================================================================================