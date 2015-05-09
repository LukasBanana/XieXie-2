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
            AutoType,   //!< Invalid variable type; actual type must be automatically deduced.
            Void,       //!< Procedure only return type.
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
        BuiltinTypeDenoter(const SourceArea& area, const TypeNames initTypeName, bool isConst = false);

        //! Returns the type name for the specified type spelling.
        static TypeNames GetTypeName(const std::string& spell);

        TypeDenoterPtr CopyRef() const override;
        std::string ToString() const override;
        unsigned int MemorySize() const override;

        bool IsAutoType() const override
        {
            return typeName == TypeNames::AutoType;
        }
        bool IsVoid() const override
        {
            return typeName == TypeNames::Void;
        }
        bool IsIntegral() const override
        {
            return typeName == TypeNames::Int;
        }
        bool IsFloat() const override
        {
            return typeName == TypeNames::Float;
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

        void SetConst(bool isConst) override
        {
            this->isConst = isConst;
        }

        TypeNames   typeName    = TypeNames::Void;  // e.g. "int", "float", "bool"
        bool        isConst     = false;            // is constant or mutable?

};


namespace CommonTypes
{

static const BuiltinTypeDenoter Void    ( BuiltinTypeDenoter::TypeNames::Void      );
static const BuiltinTypeDenoter Bool    ( BuiltinTypeDenoter::TypeNames::Bool      );
static const BuiltinTypeDenoter Int     ( BuiltinTypeDenoter::TypeNames::Int       );
static const BuiltinTypeDenoter ConstInt( BuiltinTypeDenoter::TypeNames::Int, true );
static const BuiltinTypeDenoter Float   ( BuiltinTypeDenoter::TypeNames::Float     );

} // /namepsace CommonTypes

} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
