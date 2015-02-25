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


DECL_SHR_PTR( Visitor       );
DECL_SHR_PTR( AST           );

DECL_SHR_PTR( CodeBlock     );

DECL_SHR_PTR( Stmnt         );
DECL_SHR_PTR( StmntList     );
DECL_SHR_PTR( DoWhileStmnt  );
DECL_SHR_PTR( WhileStmnt    );
DECL_SHR_PTR( ForStmnt      );
DECL_SHR_PTR( ForRangeStmnt );
DECL_SHR_PTR( ForEachStmnt  );
DECL_SHR_PTR( ForEverStmnt  );


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================