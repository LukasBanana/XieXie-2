/*
 * ASTViewer.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ASTViewer.h"
#include "ASTImport.h"
#include "Log.h"


namespace AbstractSyntaxTrees
{


ASTViewer::ASTViewer(Log& log) :
    log_{ log }
{
}

#define AST_INFO(info)  \
    Info(info, ast);    \
    SCOPED_INDENT(log_)

#define AST_VALUE(ident)    Value(#ident, ast->ident)
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
    //visibility!!!
    Visit(ast->declStmnts);
}

DEF_VISIT_PROC(ASTViewer, ArrayAccess)
{
    AST_INFO("ArrayAccess");
}

DEF_VISIT_PROC(ASTViewer, ProcCall)
{
    AST_INFO("ProcCall");
}

DEF_VISIT_PROC(ASTViewer, SwitchCase)
{
    AST_INFO("SwitchCase");
}

/* --- Statements --- */

DEF_VISIT_PROC(ASTViewer, ReturnStmnt)
{
    AST_INFO("ReturnStmnt");
}

DEF_VISIT_PROC(ASTViewer, CtrlTransferStmnt)
{
    AST_INFO("CtrlTransferStmnt");
}

DEF_VISIT_PROC(ASTViewer, ProcCallStmnt)
{
    AST_INFO("ProcCallStmnt");
}

DEF_VISIT_PROC(ASTViewer, IfStmnt)
{
    AST_INFO("IfStmnt");
}

DEF_VISIT_PROC(ASTViewer, SwitchStmnt)
{
    AST_INFO("SwitchStmnt");
}

DEF_VISIT_PROC(ASTViewer, DoWhileStmnt)
{
    AST_INFO("DoWhileStmnt");
}

DEF_VISIT_PROC(ASTViewer, WhileStmnt)
{
    AST_INFO("WhileStmnt");
}

DEF_VISIT_PROC(ASTViewer, ForStmnt)
{
    AST_INFO("ForStmnt");
}

DEF_VISIT_PROC(ASTViewer, ForRangeStmnt)
{
    AST_INFO("ForRangeStmnt");
}

DEF_VISIT_PROC(ASTViewer, ForEachStmnt)
{
    AST_INFO("ForEachStmnt");
}

DEF_VISIT_PROC(ASTViewer, ForEverStmnt)
{
    AST_INFO("ForEverStmnt");
}

DEF_VISIT_PROC(ASTViewer, ClassDeclStmnt)
{
    AST_INFO("ClassDeclStmnt");
}

DEF_VISIT_PROC(ASTViewer, ExternClassDeclStmnt)
{
    AST_INFO("ExternClassDeclStmnt");
}

DEF_VISIT_PROC(ASTViewer, VarDeclStmnt)
{
    AST_INFO("VarDeclStmnt");
}

DEF_VISIT_PROC(ASTViewer, EnumDeclStmnt)
{
    AST_INFO("EnumDeclStmnt");
}

DEF_VISIT_PROC(ASTViewer, FlagsDeclStmnt)
{
    AST_INFO("FlagsDeclStmnt");
}

DEF_VISIT_PROC(ASTViewer, ProcDeclStmnt)
{
    AST_INFO("ProcDeclStmnt");
}

DEF_VISIT_PROC(ASTViewer, InitDeclStmnt)
{
    AST_INFO("InitDeclStmnt");
}

DEF_VISIT_PROC(ASTViewer, CopyAssignStmnt)
{
    AST_INFO("CopyAssignStmnt");
}

DEF_VISIT_PROC(ASTViewer, ModifyAssignStmnt)
{
    AST_INFO("ModifyAssignStmnt");
}

DEF_VISIT_PROC(ASTViewer, PostOperatorStmnt)
{
    AST_INFO("PostOperatorStmnt");
}

/* --- Expressions --- */

DEF_VISIT_PROC(ASTViewer, BinaryExpr)
{
    AST_INFO("BinaryExpr");
}

DEF_VISIT_PROC(ASTViewer, UnaryExpr)
{
    AST_INFO("UnaryExpr");
}

DEF_VISIT_PROC(ASTViewer, LiteralExpr)
{
    AST_INFO("LiteralExpr");
}

DEF_VISIT_PROC(ASTViewer, CastExpr)
{
    AST_INFO("CastExpr");
}

DEF_VISIT_PROC(ASTViewer, ProcCallExpr)
{
    AST_INFO("ProcCallExpr");
}

DEF_VISIT_PROC(ASTViewer, MemberCallExpr)
{
    AST_INFO("MemberCallExpr");
}

DEF_VISIT_PROC(ASTViewer, AllocExpr)
{
    AST_INFO("AllocExpr");
}

DEF_VISIT_PROC(ASTViewer, VarAccessExpr)
{
    AST_INFO("VarAccessExpr");
}

DEF_VISIT_PROC(ASTViewer, InitListExpr)
{
    AST_INFO("InitListExpr");
}

/* --- Type denoters --- */

DEF_VISIT_PROC(ASTViewer, BuiltinTypeDenoter)
{
    AST_INFO("BuiltinTypeDenoter");
}

DEF_VISIT_PROC(ASTViewer, ArrayTypeDenoter)
{
    AST_INFO("ArrayTypeDenoter");
}

DEF_VISIT_PROC(ASTViewer, PointerTypeDenoter)
{
    AST_INFO("PointerTypeDenoter");
}

#undef AST_INFO
#undef AST_VALUE
#undef AST_STRING


/*
 * ======= Private: =======
 */

void ASTViewer::Info(const std::string& info, const AST* ast)
{
    if (ast)
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


} // /namespace AbstractSyntaxTrees



// ================================================================================