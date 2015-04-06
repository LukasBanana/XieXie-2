/*
 * TypeDenoter.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_TYPE_DENOTER_H__
#define __XX_AST_TYPE_DENOTER_H__


#include "AST.h"
#include "ArrayAccess.h"


namespace AbstractSyntaxTrees
{


class TypeDenoter : public AST
{
    
    public:

        //! Returns true if this is an automatic type. By default false.
        virtual bool IsAutoType() const
        {
            return false;
        }
        //! Returns true if this is a 'void' type. By default false.
        virtual bool IsVoid() const
        {
            return false;
        }
        //! Returns true if this is a 'null' pointer type. By default false.
        virtual bool IsNull() const
        {
            return false;
        }
        //! Returns ture if this is an array type. By default fasle.
        virtual bool IsArray() const
        {
            return false;
        }
        //! Returns true if this is an integral type. By default false.
        virtual bool IsIntegral() const
        {
            return false;
        }
        //! Returns true if this is a floating-point type. By default false.
        virtual bool IsFloat() const
        {
            return false;
        }
        //! Returns true if this is a boolean type. By default false.
        virtual bool IsBoolean() const
        {
            return false;
        }
        //! Returns true if a variable of this type can be concatenated with another variable. By default false.
        virtual bool CanBeConcatenated() const
        {
            return false;
        }
        //! Returns true if this is a constant type. By default false.
        virtual bool IsConst() const
        {
            return false;
        }
        //! Returns true if this is a pointer type. By default false.
        virtual bool IsPointer() const
        {
            return false;
        }
        //! Returns true if this is a pointer type with class name 'ident' or a null pointer literal.
        virtual bool IsPointer(const std::string& ident) const
        {
            return false;
        }

        //! Returns true if this is an arithmetic type (integral or floating-point).
        bool IsArithmetic() const
        {
            return IsIntegral() || IsFloat();
        }

        //! Sets this type denoter to constant.
        virtual void SetConst(bool isConst)
        {
            // dummy
        }

        /**
        Returns the last type denoter in a hierarchy. This may also be this type denoter or null,
        if there is no distinct last type (missing pointer information for instance).
        */
        virtual const TypeDenoter* GetLast() const
        {
            return this;
        }
        /**
        Returns the last type denoter in a hierarchy in combination with the specified array access.
        THis may also be this type denoter, if 'arrayAccess' is null. If there is no distinct last type
        (missing pointer information for instance), the return value is null.
        If there are too many array accesses, the return value is null, too.
        */
        virtual const TypeDenoter* GetLast(const ArrayAccess* arrayAccess) const
        {
            return arrayAccess == nullptr ? this : nullptr;
        }

        //! Creates a 'reference copy' of this type denoter. This is not a copy of the entire AST sub tree.
        virtual TypeDenoterPtr CopyRef() const = 0;

        //! Returns a declarative string for this type.
        virtual std::string ToString() const = 0;

        //! Returns true if the two type denoters are equal.
        static bool AreEqual(const TypeDenoter& lhs, const TypeDenoter& rhs);

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
