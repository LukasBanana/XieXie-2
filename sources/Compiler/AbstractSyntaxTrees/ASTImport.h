/*
 * ASTImport.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_IMPORT_H__
#define __XX_AST_IMPORT_H__


/* --- Common AST types --- */

#include "Program.h"
#include "CodeBlock.h"
#include "VarName.h"
#include "VarDecl.h"
#include "Param.h"
#include "Arg.h"
#include "ProcSignature.h"
#include "AttribPrefix.h"
#include "Attrib.h"
#include "ClassBodySegment.h"
#include "ArrayAccess.h"
#include "ProcCall.h"
#include "SwitchCase.h"


/* --- Statements --- */

#include "ReturnStmnt.h"
#include "CtrlTransferStmnt.h"
#include "ProcCallStmnt.h"

#include "IfStmnt.h"
#include "SwitchStmnt.h"

#include "ForStmnt.h"
#include "ForEachStmnt.h"
#include "ForEverStmnt.h"
#include "ForRangeStmnt.h"
#include "WhileStmnt.h"
#include "DoWhileStmnt.h"

#include "ClassDeclStmnt.h"
#include "VarDeclStmnt.h"
#include "ProcDeclStmnt.h"
#include "InitDeclStmnt.h"
#include "ReleaseDeclStmnt.h"

#include "CopyAssignStmnt.h"
#include "ModifyAssignStmnt.h"
#include "PostOperatorStmnt.h"


/* --- Expressions --- */

#include "BinaryExpr.h"
#include "UnaryExpr.h"
#include "LiteralExpr.h"
#include "CastExpr.h"
#include "ProcCallExpr.h"
#include "PostfixValueExpr.h"
#include "AllocExpr.h"
#include "VarAccessExpr.h"
#include "InitListExpr.h"


/* --- Type denoters --- */

#include "BuiltinTypeDenoter.h"
#include "ArrayTypeDenoter.h"
#include "PointerTypeDenoter.h"


#endif



// ================================================================================