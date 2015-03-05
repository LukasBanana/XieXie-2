/*
 * TypeDenoter.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_TYPE_DENOTER_H__
#define __XX_AST_TYPE_DENOTER_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class TypeDenoter : public AST
{
    
    public:

        //! Returns true if this is a 'void' type. By default false.
        virtual bool IsVoid() const
        {
            return false;
        }

        //! Returns true if a variable fo this type can be concatenated with another variable. By default false.
        virtual bool CanBeConcatenated() const
        {
            return false;
        }

        //! Returns a declarative string for this type.
        virtual std::string ToString() const = 0;

    protected:
        
        TypeDenoter() = default;
        TypeDenoter(const SourceArea& area) :
            AST{ area }
        {
        }

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
