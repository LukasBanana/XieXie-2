/*
 * Program.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_PROGRAM_H__
#define __XX_AST_PROGRAM_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class Program : public AST
{
    
    public:
        
        AST_INTERFACE(Program);

        std::vector<StmntPtr> classDeclStmnts;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
