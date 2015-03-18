/*
 * AST pointer declarations header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_PTRDECL_H__
#define __XX_AST_PTRDECL_H__


#include "DeclPtr.h"


namespace AbstractSyntaxTrees
{


/* --- Abstract AST nodes --- */

DECL_SHR_PTR( Visitor              );
DECL_SHR_PTR( AST                  );
DECL_SHR_PTR( Stmnt                );
DECL_SHR_PTR( Expr                 );
DECL_SHR_PTR( TypeDenoter          );

/* --- Common AST nodes --- */

DECL_SHR_PTR( Program              );
DECL_SHR_PTR( CodeBlock            );
DECL_SHR_PTR( VarName              );
DECL_SHR_PTR( VarDecl              );
DECL_SHR_PTR( Param                );
DECL_SHR_PTR( Arg                  );
DECL_SHR_PTR( ProcSignature        );
DECL_SHR_PTR( AttribPrefix         );
DECL_SHR_PTR( Attrib               );
DECL_SHR_PTR( ClassBodySegment     );
DECL_SHR_PTR( ArrayAccess          );
DECL_SHR_PTR( ProcCall             );
DECL_SHR_PTR( SwitchCase           );

/* --- Statements --- */

DECL_SHR_PTR( ReturnStmnt          );
DECL_SHR_PTR( CtrlTransferStmnt    );
DECL_SHR_PTR( ProcCallStmnt        );

DECL_SHR_PTR( IfStmnt              );
DECL_SHR_PTR( SwitchStmnt          );

DECL_SHR_PTR( DoWhileStmnt         );
DECL_SHR_PTR( WhileStmnt           );
DECL_SHR_PTR( ForStmnt             );
DECL_SHR_PTR( ForRangeStmnt        );
DECL_SHR_PTR( ForEachStmnt         );
DECL_SHR_PTR( ForEverStmnt         );

DECL_SHR_PTR( ClassDeclStmnt       );
DECL_SHR_PTR( VarDeclStmnt         );
DECL_SHR_PTR( ProcDeclStmnt        );
DECL_SHR_PTR( InitDeclStmnt        );

DECL_SHR_PTR( CopyAssignStmnt      );
DECL_SHR_PTR( ModifyAssignStmnt    );
DECL_SHR_PTR( PostOperatorStmnt    );

/* --- Expressions --- */

DECL_SHR_PTR( BinaryExpr           );
DECL_SHR_PTR( UnaryExpr            );
DECL_SHR_PTR( LiteralExpr          );
DECL_SHR_PTR( CastExpr             );
DECL_SHR_PTR( ProcCallExpr         );
DECL_SHR_PTR( PostfixValueExpr     );
DECL_SHR_PTR( AllocExpr            );
DECL_SHR_PTR( VarAccessExpr        );
DECL_SHR_PTR( InitListExpr         );

/* --- Type denoters --- */

DECL_SHR_PTR( BuiltinTypeDenoter   );
DECL_SHR_PTR( ArrayTypeDenoter     );
DECL_SHR_PTR( PointerTypeDenoter   );


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================