/*
 * SwitchCase.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_SWITCH_CASE_H__
#define __XX_AST_SWITCH_CASE_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class SwitchCase : public AST
{
    
    public:
        
        AST_INTERFACE(SwitchCase);

        //! Returns true if this is a default case.
        bool IsDefaultCase() const;

        std::vector<ExprPtr>    items;  // if empty -> default case
        std::vector<StmntPtr>   stmnts;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
