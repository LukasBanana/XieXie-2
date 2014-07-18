/*
 * TAC value types header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TAC_VALUE_TYPES_H__
#define __XX_TAC_VALUE_TYPES_H__


namespace ThreeAddressCodes
{


//! TAC value types.
enum class ValueTypes
{
    Boolean,    //!< Boolean type: "true" and "false".
    Integral,   //!< Integral type: 42, 9, -4 ...
    Real,       //!< Real type: 3.14, -0.5, 123.456 ...
    String,     //!< String 
};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================