/*
 * AST.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ASTImport.h"

#include <exception>
#include <initializer_list>
#include <algorithm>
#include <map>


namespace AbstractSyntaxTrees
{


/* --- <Internal> --- */

template <typename T> T MapSpellToType(
    const std::string& spell, const std::map<std::string, T>& list, const std::string& error)
{
    auto it = list.find(spell);
    if (it != list.end())
        return it->second;
    throw std::invalid_argument(error);
}

template <typename T> std::string MapTypeToSpell(
    const T& type, const std::map<std::string, T>& list)
{
    for (const auto& it : list)
    {
        if (it.second == type)
            return it.first;
    }
    return "";
}

/*bool ContainsString(const std::string& str, const std::initializer_list<std::string>& list)
{
    return std::find(list.begin(), list.end(), str) != list.end();
}*/


/* --- VarName --- */

const TypeDenoter* VarName::GetTypeDenoter() const
{
    /* Return type denoter of the last variable name AST node */
    auto& last = GetLast();
    return last.declRef != nullptr ? last.declRef->GetTypeDenoter() : nullptr;
}

std::string VarName::FullName(const std::string& sep) const
{
    return next != nullptr ? (ident + sep + next->FullName(sep)) : ident;
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


/* --- VarDeclStmnt --- */

const TypeDenoter* VarDeclStmnt::GetTypeDenoter() const
{
    return typeDenoter.get();
}


/* --- CodeBlock --- */

void CodeBlock::UpdateSourceArea()
{
    if (!stmnts.empty())
    {
        /* Refresh source area of statements first */
        for (auto& ast : stmnts)
            ast->UpdateSourceArea();

        /* Set source area to the begin and end of the statement list */
        sourceArea.start    = stmnts.front()->sourceArea.start;
        sourceArea.end      = stmnts.back()->sourceArea.end;
    }
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


/* --- ProcSignature --- */

const TypeDenoter* ProcSignature::GetTypeDenoter() const
{
    return returnTypeDenoter.get();
}


/* --- ProcCall --- */

const TypeDenoter* ProcCall::GetTypeDenoter() const
{
    return declStmntRef != nullptr ? declStmntRef->GetTypeDenoter() : nullptr;
}


/* --- ProcDeclStmnt --- */

const TypeDenoter* ProcDeclStmnt::GetTypeDenoter() const
{
    return procSignature->GetTypeDenoter();
}


/* --- ForRangeStmnt --- */

const TypeDenoter* ForRangeStmnt::GetTypeDenoter() const
{
    return &CommonTypeConstInt;
}


/* --- ProcCallExpr --- */

const TypeDenoter* ProcCallExpr::GetTypeDenoter() const
{
    return procCall->GetTypeDenoter();
}


/* --- MemberCallExpr --- */

const TypeDenoter* MemberCallExpr::GetTypeDenoter() const
{
    return procCall->GetTypeDenoter();
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
    const auto& lastIdent = varName->GetLast();
    auto varType = lastIdent.GetTypeDenoter();
    
    if (varType)
    {
        /* Reduce type for each array access */
        auto arrayAccess = lastIdent.arrayAccess.get();
        while (arrayAccess)
        {
            if (varType->Type() == AST::Types::ArrayTypeDenoter)
            {
                /* Get next lower type */
                auto arrayType = static_cast<const ArrayTypeDenoter*>(varType);
                varType = arrayType->lowerTypeDenoter.get();
            }
            else
            {
                /* Array access but no further lower type */
                return nullptr;
            }
            arrayAccess = arrayAccess->next.get();
        }
    }

    return varType;
}


/* --- ClassDeclStmnt --- */

const TypeDenoter* ClassDeclStmnt::GetTypeDenoter() const
{
    return &thisTypeDenoter_;
}

void ClassDeclStmnt::BindBaseClassRef(ClassDeclStmnt* classDeclStmnt)
{
    baseClassRef_ = classDeclStmnt;
    if (baseClassRef_)
        symTab.fallbackSymTab = &(baseClassRef_->symTab);
    else
        symTab.fallbackSymTab = nullptr;
}

ClassDeclStmnt* ClassDeclStmnt::GetBaseClassRef() const
{
    return baseClassRef_;
}

bool ClassDeclStmnt::IsSuperClassOf(const ClassDeclStmnt& classDeclStmnt) const
{
    return classDeclStmnt.IsSubClassOf(*this);
}

bool ClassDeclStmnt::IsSubClassOf(const ClassDeclStmnt& classDeclStmnt) const
{
    /* Check for cycles */
    if (&classDeclStmnt == this)
        return false;

    if (baseClassRef_)
    {
        /* Check if specified class is the direct base of this class */
        if (baseClassRef_ == &classDeclStmnt)
            return true;

        /* Continue search in direct base class */
        return baseClassRef_->IsSubClassOf(classDeclStmnt);
    }

    return false;
}


/* --- BuiltinTypeDenoter --- */

BuiltinTypeDenoter::BuiltinTypeDenoter(const TypeNames initTypeName, bool isConst) :
    typeName{ initTypeName },
    isConst { isConst      }
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


/* --- ClassBodySegment --- */

ClassBodySegment::Visibilities ClassBodySegment::GetVisibility(const std::string& spell)
{
    return MapSpellToType<Visibilities>(
        spell,
        {
            { "private", Visibilities::Private },
            { "public",  Visibilities::Public  },
        },
        "invalid class visibility \"" + spell + "\""
    );
}


} // /namespace AbstractSyntaxTrees



// ================================================================================