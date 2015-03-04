/*
 * TAC variable header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TAC_VAR_H__
#define __XX_TAC_VAR_H__


#include "ValueTypes.h"

#include <string>


namespace ThreeAddressCodes
{


//! TAC variable class.
class TACVar
{
    
    public:
        
        typedef unsigned int IdType;

        //! TAC variable classes.
        enum class VarClasses
        {
            Global, //!< Global variable
            Local,  //!< Local variable.
            Temp,   //!< Compiler temporary variable.
            ArgIn,  //!< Input argument for a procedure call.
            ArgOut, //!< Output argument from a procedure call.
        };

        TACVar();

        //! Returns this variable as string representation (for debugging).
        std::string ToString() const;

        VarClasses  varClass    = VarClasses::Temp;
        ValueTypes  valueType   = ValueTypes::Integral;
        
        IdType      id          = 0;

    private:
        
        static IdType idCounter_;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================