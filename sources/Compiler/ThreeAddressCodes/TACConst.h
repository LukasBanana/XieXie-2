/*
 * TAC constant header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TAC_CONST_H__
#define __XX_TAC_CONST_H__


#include "DeclPtr.h"
#include "ValueTypes.h"

#include <string>


namespace ThreeAddressCodes
{


DECL_SHR_PTR(TACConst);

//! TAC constant class.
class TACConst
{
    
    public:
        
        TACConst(bool boolean);
        TACConst(int integral);
        TACConst(float real);
        TACConst(const std::string& str);

        //! Returns the constant value type.
        ValueTypes Type() const
        {
            return type_;
        }

        bool ToBoolean() const
        {
            return boolean_;
        }
        int ToIntegral() const
        {
            return integral_;
        }
        float ToReal() const
        {
            return real_;
        }
        std::string ToString() const
        {
            return string_;
        }

    private:

        ValueTypes  type_       = ValueTypes::Integral;

        bool        boolean_    = false;
        int         integral_   = 0;
        float       real_       = 0.0f;
        std::string string_;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================