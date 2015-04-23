/*
 * AST.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ASTImport.h"


namespace AbstractSyntaxTrees
{


/* --- VarDeclStmnt --- */

const TypeDenoter* VarDeclStmnt::GetTypeDenoter() const
{
    return typeDenoter.get();
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


/* --- VarAccessExpr --- */

const TypeDenoter* VarAccessExpr::GetTypeDenoter() const
{
    return varName->GetTypeDenoter();
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


/* --- SwitchCase --- */

bool SwitchCase::IsDefaultCase() const
{
    return items.empty() && ranges.empty();
}


} // /namespace AbstractSyntaxTrees



// ================================================================================