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

        std::string ToString() const override
        {
            switch (typeName)
            {
                case TypeNames::Void:
                    return "void";
                case TypeNames::Bool:
                    return "bool";
                case TypeNames::Int:
                    return "int";
                case TypeNames::Float:
                    return "float";
            }
            return "<unknown>";
        }

        bool IsVoid() const override
        {
            return typeName == TypeNames::Void;
        }

        bool CanBeConcatenated() const override
        {
            return typeName != TypeNames::Bool;
        }

        TypeDenoterPtr CopyRef() const override
        {
            auto copy = std::make_shared<BuiltinTypeDenoter>();
            copy->typeName = typeName;
            return copy;
        }

        TypeNames typeName = TypeNames::Void; // e.g. "int", "float", "bool"

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
