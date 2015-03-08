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
        
        using IDType = unsigned int;
        static const IDType invalidID = 0;

        //! TAC variable classes.
        enum class Types
        {
            Literal,    //!< Literal constant.
            Global,     //!< Global variable
            Local,      //!< Local variable.
            Temp,       //!< Compiler temporary variable.
            ArgIn,      //!< Input argument for a procedure call.
            ArgOut,     //!< Output argument from a procedure call.
        };

        TACVar() = default;
        TACVar(IDType id, const Types type = Types::Temp);
        TACVar(const std::string& value);
        TACVar(const char* value);

        //! Returns this variable as string representation (for debugging).
        std::string ToString() const;

        IDType      id      = TACVar::invalidID;
        Types       type    = Types::Temp;
        std::string value;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================