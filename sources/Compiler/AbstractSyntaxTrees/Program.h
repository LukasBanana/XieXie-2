/*
 * Program.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_PROGRAM_H__
#define __XX_AST_PROGRAM_H__


#include "ScopedStmnt.h"


namespace AbstractSyntaxTrees
{


//! \note The "Program" AST node must be a scoped statement, to provide the "FetchSymbol" function!
class Program : public ScopedStmnt
{
    
    public:
        
        AST_INTERFACE_EXT(Program, ScopedStmnt);

        std::vector<std::string>    importFilenames;
        std::vector<StmntPtr>       classDeclStmnts;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
