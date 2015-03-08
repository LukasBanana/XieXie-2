/*
 * GraphGenerator.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GraphGenerator.h"
#include "ASTImport.h"


namespace ControlFlowGraph
{


std::vector<std::unique_ptr<ClassTree>> GraphGenerator::GenerateCFG(const Program& program)
{
    /* Cast program AST node to non-const; we don't modify the AST here at all! */
    programClassTrees_.clear();
    Visit(const_cast<Program*>(&program));
    return std::move(programClassTrees_);
}


/*
 * ======= Private: =======
 */

/* --- Common AST nodes --- */

DEF_VISIT_PROC(GraphGenerator, Program)
{
    Visit(ast->classDeclStmnts);
}

DEF_VISIT_PROC(GraphGenerator, CodeBlock)
{
    Visit(ast->stmnts);
}

DEF_VISIT_PROC(GraphGenerator, VarName)
{
}

DEF_VISIT_PROC(GraphGenerator, VarDecl)
{
}

DEF_VISIT_PROC(GraphGenerator, Param)
{
}

DEF_VISIT_PROC(GraphGenerator, Arg)
{
}

DEF_VISIT_PROC(GraphGenerator, ProcSignature)
{
}

DEF_VISIT_PROC(GraphGenerator, AttribPrefix)
{
    // do nothing
}

DEF_VISIT_PROC(GraphGenerator, Attrib)
{
    // do nothing
}

DEF_VISIT_PROC(GraphGenerator, ClassBodySegment)
{
    Visit(ast->declStmnts);
}

DEF_VISIT_PROC(GraphGenerator, ArrayAccess)
{
}

DEF_VISIT_PROC(GraphGenerator, ProcCall)
{
}

DEF_VISIT_PROC(GraphGenerator, SwitchCase)
{
}

/* --- Statements --- */

DEF_VISIT_PROC(GraphGenerator, ReturnStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, CtrlTransferStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, ProcCallStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, IfStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, SwitchStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, DoWhileStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, WhileStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, ForStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, ForRangeStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, ForEachStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, ForEverStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, ClassDeclStmnt)
{
    CreateClassTree();
    Visit(ast->bodySegments);
}

DEF_VISIT_PROC(GraphGenerator, VarDeclStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, ProcDeclStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, InitDeclStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, CopyAssignStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, ModifyAssignStmnt)
{
}

DEF_VISIT_PROC(GraphGenerator, PostOperatorStmnt)
{
}

/* --- Expressions --- */

DEF_VISIT_PROC(GraphGenerator, BinaryExpr)
{
}

DEF_VISIT_PROC(GraphGenerator, UnaryExpr)
{
}

DEF_VISIT_PROC(GraphGenerator, LiteralExpr)
{
}

DEF_VISIT_PROC(GraphGenerator, CastExpr)
{
}

DEF_VISIT_PROC(GraphGenerator, ProcCallExpr)
{
}

DEF_VISIT_PROC(GraphGenerator, MemberCallExpr)
{
}

DEF_VISIT_PROC(GraphGenerator, AllocExpr)
{
}

DEF_VISIT_PROC(GraphGenerator, VarAccessExpr)
{
}

DEF_VISIT_PROC(GraphGenerator, InitListExpr)
{
}

/* --- Type denoters --- */

DEF_VISIT_PROC(GraphGenerator, BuiltinTypeDenoter)
{
}

DEF_VISIT_PROC(GraphGenerator, ArrayTypeDenoter)
{
}

DEF_VISIT_PROC(GraphGenerator, PointerTypeDenoter)
{
}

/* --- Conversion --- */

void GraphGenerator::CreateClassTree()
{
    programClassTrees_.emplace_back(std::make_unique<ClassTree>());
    classTree_ = programClassTrees_.back().get();
}


} // /namespace ControlFlowGraph



// ================================================================================