/*
 * ASTViewer.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ASTViewer.h"
#include "ASTImport.h"
#include "Log.h"
#include "StringModifier.h"


namespace AbstractSyntaxTrees
{


ASTViewer::ASTViewer(Log& log) :
    log_{ log }
{
}

void ASTViewer::ViewProgram(Program& ast, bool showPos)
{
    showPos_ = showPos;
    Visit(&ast);
}


/*
 * ======= Private: =======
 */

#define AST_INFO(info)  \
    Info(info, ast);    \
    SCOPED_INDENT(log_)

#define AST_VALUE(ident)    Value(#ident, ToStr(ast->ident))
#define AST_STRING(ident)   String(#ident, ast->ident)

/* --- Common AST nodes --- */

DEF_VISIT_PROC(ASTViewer, Program)
{
    AST_INFO("Program");
    Visit(ast->classDeclStmnts);
}

DEF_VISIT_PROC(ASTViewer, CodeBlock)
{
    AST_INFO("CodeBlock");
    Visit(ast->stmnts);
}

DEF_VISIT_PROC(ASTViewer, VarName)
{
    AST_INFO("VarName");
    AST_STRING(ident);
    Visit(ast->arrayAccess);
    Visit(ast->next);
}

DEF_VISIT_PROC(ASTViewer, VarDecl)
{
    AST_INFO("VarDecl");
    AST_STRING(ident);
    Visit(ast->initExpr);
}

DEF_VISIT_PROC(ASTViewer, Param)
{
    AST_INFO("Param");
    Visit(ast->typeDenoter);
    AST_STRING(ident);
    Visit(ast->defaultArgExpr);
}

DEF_VISIT_PROC(ASTViewer, Arg)
{
    AST_INFO("Arg");
    AST_STRING(paramIdent);
    Visit(ast->expr);
}

DEF_VISIT_PROC(ASTViewer, ProcSignature)
{
    AST_INFO("ProcSignature");
    //ast->modifier!!!
    Visit(ast->returnTypeDenoter);
    AST_STRING(ident);
    Visit(ast->params);
}

DEF_VISIT_PROC(ASTViewer, AttribPrefix)
{
    AST_INFO("AttribPrefix");
    Visit(ast->attribs);
}

DEF_VISIT_PROC(ASTViewer, Attrib)
{
    AST_INFO("Attrib");
    AST_STRING(ident);
    Visit(ast->exprs);
}

DEF_VISIT_PROC(ASTViewer, EnumEntry)
{
    AST_INFO("EnumEntry");
    AST_STRING(ident);
    Visit(ast->valueExpr);
}

DEF_VISIT_PROC(ASTViewer, ClassBodySegment)
{
    AST_INFO("ClassBodySegment");
    //ast->visibility!!!
    Visit(ast->declStmnts);
}

DEF_VISIT_PROC(ASTViewer, ArrayAccess)
{
    AST_INFO("ArrayAccess");
    Visit(ast->indexExpr);
    Visit(ast->next);
}

DEF_VISIT_PROC(ASTViewer, ProcCall)
{
    AST_INFO("ProcCall");
    Visit(ast->procName);
    Visit(ast->args);
}

DEF_VISIT_PROC(ASTViewer, SwitchCase)
{
    AST_INFO("SwitchCase");
    Visit(ast->items);
    Visit(ast->stmnts);
}

/* --- Statements --- */

DEF_VISIT_PROC(ASTViewer, ReturnStmnt)
{
    AST_INFO("ReturnStmnt");
    Visit(ast->expr);
}

DEF_VISIT_PROC(ASTViewer, CtrlTransferStmnt)
{
    AST_INFO("CtrlTransferStmnt");
    //ast->ctrlTransfer!!!
}

DEF_VISIT_PROC(ASTViewer, ProcCallStmnt)
{
    AST_INFO("ProcCallStmnt");
    Visit(ast->procCall);
}

DEF_VISIT_PROC(ASTViewer, IfStmnt)
{
    AST_INFO("IfStmnt");
    Visit(ast->condExpr);
    Visit(ast->codeBlock);
    Visit(ast->elseStmnt);
}

DEF_VISIT_PROC(ASTViewer, SwitchStmnt)
{
    AST_INFO("SwitchStmnt");
    Visit(ast->expr);
    Visit(ast->cases);
}

DEF_VISIT_PROC(ASTViewer, DoWhileStmnt)
{
    AST_INFO("DoWhileStmnt");
    Visit(ast->codeBlock);
    Visit(ast->condExpr);
}

DEF_VISIT_PROC(ASTViewer, WhileStmnt)
{
    AST_INFO("WhileStmnt");
    Visit(ast->condExpr);
    Visit(ast->codeBlock);
}

DEF_VISIT_PROC(ASTViewer, ForStmnt)
{
    AST_INFO("ForStmnt");
    Visit(ast->initStmnt);
    Visit(ast->condExpr);
    Visit(ast->assignStmnt);
    Visit(ast->codeBlock);
}

DEF_VISIT_PROC(ASTViewer, ForRangeStmnt)
{
    AST_INFO("ForRangeStmnt");
    AST_STRING(varIdent);
    AST_VALUE(rangeStart);
    AST_VALUE(rangeEnd);
    AST_VALUE(rangeStep);
    Visit(ast->codeBlock);
}

DEF_VISIT_PROC(ASTViewer, ForEachStmnt)
{
    AST_INFO("ForEachStmnt");
    Visit(ast->varDeclStmnt);
    Visit(ast->listExpr);
    Visit(ast->codeBlock);
}

DEF_VISIT_PROC(ASTViewer, ForEverStmnt)
{
    AST_INFO("ForEverStmnt");
    Visit(ast->codeBlock);
}

DEF_VISIT_PROC(ASTViewer, ClassDeclStmnt)
{
    AST_INFO("ClassDeclStmnt");
    Visit(ast->attribPrefix);
    AST_STRING(ident);
    Visit(ast->inheritanceTypeName);
    Visit(ast->bodySegments);
}

DEF_VISIT_PROC(ASTViewer, VarDeclStmnt)
{
    AST_INFO("VarDeclStmnt");
    Visit(ast->typeDenoter);
    Visit(ast->varDecls);
}

DEF_VISIT_PROC(ASTViewer, EnumDeclStmnt)
{
    AST_INFO("EnumDeclStmnt");
    AST_STRING(ident);
    Visit(ast->entries);
}

DEF_VISIT_PROC(ASTViewer, FlagsDeclStmnt)
{
    AST_INFO("FlagsDeclStmnt");
    AST_STRING(ident);
    for (const auto& entry : ast->entries)
        String("entry", entry);
}

DEF_VISIT_PROC(ASTViewer, ProcDeclStmnt)
{
    AST_INFO("ProcDeclStmnt");
    Visit(ast->attribPrefix);
    Visit(ast->procSignature);
    Visit(ast->codeBlock);
}

DEF_VISIT_PROC(ASTViewer, InitDeclStmnt)
{
    AST_INFO("InitDeclStmnt");
    Visit(ast->attribPrefix);
    Visit(ast->params);
    Visit(ast->codeBlock);
}

DEF_VISIT_PROC(ASTViewer, CopyAssignStmnt)
{
    AST_INFO("CopyAssignStmnt");
    Visit(ast->varNames);
    Visit(ast->expr);
}

DEF_VISIT_PROC(ASTViewer, ModifyAssignStmnt)
{
    AST_INFO("ModifyAssignStmnt");
    Visit(ast->varName);
    //ast->modifyOperator!!!
    Visit(ast->expr);
}

DEF_VISIT_PROC(ASTViewer, PostOperatorStmnt)
{
    AST_INFO("PostOperatorStmnt");
    Visit(ast->varName);
    //ast->postOperator!!!
}

/* --- Expressions --- */

DEF_VISIT_PROC(ASTViewer, BinaryExpr)
{
    AST_INFO("BinaryExpr");
    Visit(ast->lhsExpr);
    String("binaryOperator", BinaryExpr::GetOperatorSpell(ast->binaryOperator));
    Visit(ast->rhsExpr);
}

DEF_VISIT_PROC(ASTViewer, UnaryExpr)
{
    AST_INFO("UnaryExpr");
    String("unaryOperator", UnaryExpr::GetOperatorSpell(ast->unaryOperator));
    Visit(ast->expr);
}

DEF_VISIT_PROC(ASTViewer, LiteralExpr)
{
    AST_INFO("LiteralExpr");
    AST_STRING(value);
}

DEF_VISIT_PROC(ASTViewer, CastExpr)
{
    AST_INFO("CastExpr");
    Visit(ast->typeDenoter);
    Visit(ast->expr);
}

DEF_VISIT_PROC(ASTViewer, ProcCallExpr)
{
    AST_INFO("ProcCallExpr");
    Visit(ast->procCall);
}

DEF_VISIT_PROC(ASTViewer, MemberCallExpr)
{
    AST_INFO("MemberCallExpr");
    Visit(ast->objectExpr);
    Visit(ast->procCall);
}

DEF_VISIT_PROC(ASTViewer, AllocExpr)
{
    AST_INFO("AllocExpr");
    Visit(ast->typeDenoter);
    Visit(ast->ctorArgs);
}

DEF_VISIT_PROC(ASTViewer, VarAccessExpr)
{
    AST_INFO("VarAccessExpr");
    Visit(ast->varName);
}

DEF_VISIT_PROC(ASTViewer, InitListExpr)
{
    AST_INFO("InitListExpr");
    Visit(ast->exprs);
}

/* --- Type denoters --- */

DEF_VISIT_PROC(ASTViewer, BuiltinTypeDenoter)
{
    AST_INFO("BuiltinTypeDenoter");
    //ast->typeName!!!
}

DEF_VISIT_PROC(ASTViewer, ArrayTypeDenoter)
{
    AST_INFO("ArrayTypeDenoter");
    Visit(ast->lowerTypeDenoter);
}

DEF_VISIT_PROC(ASTViewer, PointerTypeDenoter)
{
    AST_INFO("PointerTypeDenoter");
    AST_STRING(declIdent);
}

/* --- Misc --- */

void ASTViewer::Info(const std::string& info, const AST* ast)
{
    if (ast && showPos_)
        log_.Message("(" + ast->sourceArea.ToString() + ") -- " + info);
    else
        log_.Message(info);
}

void ASTViewer::Value(const std::string& ident, const std::string& value)
{
    Info(ident + " = " + value);
}

void ASTViewer::String(const std::string& ident, const std::string& value)
{
    Info(ident + " = \"" + value + "\"");
}

#undef AST_INFO
#undef AST_VALUE
#undef AST_STRING


} // /namespace AbstractSyntaxTrees



// ================================================================================