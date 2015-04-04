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
#include "ConsoleManip.h"


namespace AbstractSyntaxTrees
{


using namespace Platform::ConsoleManip;

ASTViewer::ASTViewer(Log& log) :
    log_(log)
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

#define AST_INFO_COLOR(info, color) \
    PushColor(log_.stream, color);  \
    Info(info, ast);                \
    PopColor(log_.stream);          \
    SCOPED_INDENT(log_)

#define AST_VALUE(ident)    Value(#ident, ToStr(ast->ident))
#define AST_STRING(ident)   String(#ident, ast->ident)

static const Color::ValueType colorStmnt    = Color::Yellow;
static const Color::ValueType colorExpr     = Color::Red | Color::Intens;

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

DEF_VISIT_PROC(ASTViewer, ClassBodySegment)
{
    AST_INFO("ClassBodySegment");
    String("visibility", ClassBodySegment::GetVisibilitySpell(ast->visibility));
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
    AST_INFO_COLOR("ReturnStmnt", colorStmnt);
    Visit(ast->expr);
}

DEF_VISIT_PROC(ASTViewer, CtrlTransferStmnt)
{
    AST_INFO_COLOR("CtrlTransferStmnt", colorStmnt);
    String("ctrlTransfer", CtrlTransferStmnt::GetTransferSpell(ast->ctrlTransfer));
}

DEF_VISIT_PROC(ASTViewer, ProcCallStmnt)
{
    AST_INFO_COLOR("ProcCallStmnt", colorStmnt);
    Visit(ast->procCall);
}

DEF_VISIT_PROC(ASTViewer, IfStmnt)
{
    AST_INFO_COLOR("IfStmnt", colorStmnt);
    Visit(ast->condExpr);
    Visit(ast->codeBlock);
    Visit(ast->elseStmnt);
}

DEF_VISIT_PROC(ASTViewer, SwitchStmnt)
{
    AST_INFO_COLOR("SwitchStmnt", colorStmnt);
    Visit(ast->expr);
    Visit(ast->cases);
}

DEF_VISIT_PROC(ASTViewer, DoWhileStmnt)
{
    AST_INFO_COLOR("DoWhileStmnt", colorStmnt);
    Visit(ast->codeBlock);
    Visit(ast->condExpr);
}

DEF_VISIT_PROC(ASTViewer, WhileStmnt)
{
    AST_INFO_COLOR("WhileStmnt", colorStmnt);
    Visit(ast->condExpr);
    Visit(ast->codeBlock);
}

DEF_VISIT_PROC(ASTViewer, ForStmnt)
{
    AST_INFO_COLOR("ForStmnt", colorStmnt);
    Visit(ast->initStmnt);
    Visit(ast->condExpr);
    Visit(ast->assignStmnt);
    Visit(ast->codeBlock);
}

DEF_VISIT_PROC(ASTViewer, ForRangeStmnt)
{
    AST_INFO_COLOR("ForRangeStmnt", colorStmnt);
    AST_STRING(varIdent);
    AST_VALUE(rangeStart);
    AST_VALUE(rangeEnd);
    AST_VALUE(rangeStep);
    Visit(ast->codeBlock);
}

DEF_VISIT_PROC(ASTViewer, ForEachStmnt)
{
    AST_INFO_COLOR("ForEachStmnt", colorStmnt);
    AST_STRING(varIdent);
    Visit(ast->listExpr);
    Visit(ast->codeBlock);
}

DEF_VISIT_PROC(ASTViewer, RepeatStmnt)
{
    AST_INFO_COLOR("RepeatStmnt", colorStmnt);
    Visit(ast->codeBlock);
}

DEF_VISIT_PROC(ASTViewer, ClassDeclStmnt)
{
    AST_INFO_COLOR("ClassDeclStmnt", colorStmnt);
    Visit(ast->attribPrefix);
    AST_STRING(ident);
    AST_STRING(baseClassIdent);
    Visit(&(ast->publicSegment));
    Visit(&(ast->privateSegment));
}

DEF_VISIT_PROC(ASTViewer, VarDeclStmnt)
{
    AST_INFO_COLOR("VarDeclStmnt", colorStmnt);
    Visit(ast->typeDenoter);
    Visit(ast->varDecls);
}

DEF_VISIT_PROC(ASTViewer, ProcDeclStmnt)
{
    AST_INFO_COLOR("ProcDeclStmnt", colorStmnt);
    Visit(ast->attribPrefix);
    Visit(ast->procSignature);
    Visit(ast->codeBlock);
}

DEF_VISIT_PROC(ASTViewer, InitDeclStmnt)
{
    AST_INFO_COLOR("InitDeclStmnt", colorStmnt);
    Visit(ast->attribPrefix);
    Visit(ast->params);
    Visit(ast->codeBlock);
}

DEF_VISIT_PROC(ASTViewer, ReleaseDeclStmnt)
{
    AST_INFO_COLOR("ReleaseDeclStmnt", colorStmnt);
    Visit(ast->codeBlock);
}

DEF_VISIT_PROC(ASTViewer, CopyAssignStmnt)
{
    AST_INFO_COLOR("CopyAssignStmnt", colorStmnt);
    Visit(ast->varNames);
    Visit(ast->expr);
}

DEF_VISIT_PROC(ASTViewer, ModifyAssignStmnt)
{
    AST_INFO_COLOR("ModifyAssignStmnt", colorStmnt);
    Visit(ast->varName);
    //ast->modifyOperator!!!
    Visit(ast->expr);
}

DEF_VISIT_PROC(ASTViewer, PostOperatorStmnt)
{
    AST_INFO_COLOR("PostOperatorStmnt", colorStmnt);
    Visit(ast->varName);
    //ast->postOperator!!!
}

/* --- Expressions --- */

DEF_VISIT_PROC(ASTViewer, BinaryExpr)
{
    AST_INFO_COLOR("BinaryExpr", colorExpr);
    Visit(ast->lhsExpr);
    String("binaryOperator", BinaryExpr::GetOperatorSpell(ast->binaryOperator));
    Visit(ast->rhsExpr);
}

DEF_VISIT_PROC(ASTViewer, UnaryExpr)
{
    AST_INFO_COLOR("UnaryExpr", colorExpr);
    String("unaryOperator", UnaryExpr::GetOperatorSpell(ast->unaryOperator));
    Visit(ast->expr);
}

DEF_VISIT_PROC(ASTViewer, LiteralExpr)
{
    AST_INFO_COLOR("LiteralExpr", colorExpr);
    AST_STRING(value);
}

DEF_VISIT_PROC(ASTViewer, CastExpr)
{
    AST_INFO_COLOR("CastExpr", colorExpr);
    Visit(ast->castTypeDenoter);
    Visit(ast->expr);
}

DEF_VISIT_PROC(ASTViewer, ProcCallExpr)
{
    AST_INFO_COLOR("ProcCallExpr", colorExpr);
    Visit(ast->procCall);
}

DEF_VISIT_PROC(ASTViewer, PostfixValueExpr)
{
    AST_INFO_COLOR("PostfixValueExpr", colorExpr);
    Visit(ast->primaryValueExpr);
    Visit(ast->arrayAccess);
    Visit(ast->procCall);
    Visit(ast->varName);
}

DEF_VISIT_PROC(ASTViewer, AllocExpr)
{
    AST_INFO_COLOR("AllocExpr", colorExpr);
    Visit(ast->typeDenoter);
    Visit(ast->ctorArgs);
}

DEF_VISIT_PROC(ASTViewer, VarAccessExpr)
{
    AST_INFO_COLOR("VarAccessExpr", colorExpr);
    Visit(ast->varName);
}

DEF_VISIT_PROC(ASTViewer, InitListExpr)
{
    AST_INFO_COLOR("InitListExpr", colorExpr);
    Visit(ast->exprs);
}

/* --- Type denoters --- */

DEF_VISIT_PROC(ASTViewer, BuiltinTypeDenoter)
{
    AST_INFO("BuiltinTypeDenoter");
    String("typeName", ast->ToString());
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
    log_.StartLn();
    log_.Print(ident + " = ");
    PushColor(log_.stream, Color::Pink);
    log_.Print(value);
    PopColor(log_.stream);
    log_.EndLn();
}

void ASTViewer::String(const std::string& ident, const std::string& value)
{
    Value(ident, "\"" + value + "\"");
}

#undef AST_INFO
#undef AST_VALUE
#undef AST_STRING


} // /namespace AbstractSyntaxTrees



// ================================================================================
