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

        AST_INTERFACE_EXT(BuiltinTypeDenoter, TypeDenoter);

        BuiltinTypeDenoter(const TypeNames initTypeName);

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
        bool IsIntegral() const override
        {
            return typeName == TypeNames::Int;
        }
        bool IsBoolean() const override
        {
            return typeName == TypeNames::Bool;
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


static const BuiltinTypeDenoter CommonVoidType(BuiltinTypeDenoter::TypeNames::Void);
static const BuiltinTypeDenoter CommonBoolType(BuiltinTypeDenoter::TypeNames::Bool);
static const BuiltinTypeDenoter CommonIntType(BuiltinTypeDenoter::TypeNames::Int);
static const BuiltinTypeDenoter CommonFloatType(BuiltinTypeDenoter::TypeNames::Float);


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
