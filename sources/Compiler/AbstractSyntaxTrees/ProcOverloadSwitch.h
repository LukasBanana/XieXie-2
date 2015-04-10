/*
 * ProcOverloadSwitch.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_PROC_OVERLOAD_SWITCH_H__
#define __XX_AST_PROC_OVERLOAD_SWITCH_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


/**
This is a special case AST class. It is used to register procedure symbols only once in each scope.
This AST class switches between overloaded procedure names.
*/
class ProcOverloadSwitch : public AST
{
    
    public:
        
        ProcOverloadSwitch() = default;

        static const Types astType = Types::ProcOverloadSwitch;

        Types Type() const override
        {
            return Types::ProcOverloadSwitch;
        }
        void Visit(Visitor* visitor, void* args = nullptr) override
        {
            // do nothing
        }

        std::vector<ProcDeclStmnt*> procDeclRefs; // reference to all procedure declarations with the same identifier

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
