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

        BuiltinTypeDenoter(const TypeNames initTypeName, bool isConst = false);

        //! Returns the type name for the specified type spelling.
        static TypeNames GetTypeName(const std::string& spell);

        TypeDenoterPtr CopyRef() const override;
        std::string ToString() const override;

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
        bool IsConst() const override
        {
            return isConst;
        }

        TypeNames   typeName    = TypeNames::Void;  // e.g. "int", "float", "bool"
        bool        isConst     = false;            // is constant or mutable?

};


static const BuiltinTypeDenoter CommonTypeVoid(BuiltinTypeDenoter::TypeNames::Void);
static const BuiltinTypeDenoter CommonTypeBool(BuiltinTypeDenoter::TypeNames::Bool);
static const BuiltinTypeDenoter CommonTypeInt(BuiltinTypeDenoter::TypeNames::Int);
static const BuiltinTypeDenoter CommonTypeConstInt(BuiltinTypeDenoter::TypeNames::Int, true);
static const BuiltinTypeDenoter CommonTypeFloat(BuiltinTypeDenoter::TypeNames::Float);


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
