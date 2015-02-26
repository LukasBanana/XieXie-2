/*
 * BuiltinTypeDenoter.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_BUILTIN_TYPE_DENOTER_H__
#define __XX_AST_BUILTIN_TYPE_DENOTER_H__


#include "TypeDenoter.h"


namespace AbstractSyntaxTrees
{


class BuiltinTypeDenoter : public TypeDenoter
{
    
    public:
        
        AST_INTERFACE_EXT(BuiltinTypeDenoter, TypeDenoter);

        enum class BasicTypes
        {
            Void,       // void
            Boolean,    // bool
            Integer,    // short, ushort, int, uint, long, ulong
            Float,      // float, double
        };

        //! \throws std::invalid_argument
        BasicTypes BasicType() const;

        std::string typeName; // e.g. "int", "float", "bool"

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
