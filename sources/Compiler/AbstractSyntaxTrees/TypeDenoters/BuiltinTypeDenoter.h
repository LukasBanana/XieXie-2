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

        enum class TypeNames
        {
            Void,
            Bool,
            //Byte,
            //UByte,
            //Short,
            //UShort,
            Int,
            //UInt,
            //Long,
            //ULong,
            Float,
            //Double,
        };

        //! Returns the type name for the specified type spelling.
        static TypeNames GetTypeName(const std::string& spell);

        TypeNames typeName = TypeNames::Void; // e.g. "int", "float", "bool"

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
