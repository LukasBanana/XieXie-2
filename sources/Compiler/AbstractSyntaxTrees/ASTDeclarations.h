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


DeclPtr( Visitor       );
DeclPtr( AST           );

DeclPtr( CodeBlock     );

DeclPtr( Stmnt         );
DeclPtr( StmntList     );
DeclPtr( DoWhileStmnt  );
DeclPtr( WhileStmnt    );
DeclPtr( ForStmnt      );
DeclPtr( ForRangeStmnt );
DeclPtr( ForEachStmnt  );
DeclPtr( ForEverStmnt  );


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================