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

        //! Returns true if this variable is a constant literal.
        bool IsConst() const;
        //! Returns ture if this is a temporary variable.
        bool IsTemp() const;
        /**
        Returns true if the ID of this variable is valid.
        \see id
        \see invalidID
        */
        bool IsValid() const;

        //! Returns the value as integer.
        int Int() const;
        //! Returns the value as floating-point.
        float Float() const;

        //! Replaces this variable by 'replacedVar' if this variable is equal to 'varToReplace'.
        bool Replace(const TACVar& varToReplace, const TACVar& replacedVar);

        IDType      id      = TACVar::invalidID;
        Types       type    = Types::Temp;
        std::string value;

};


bool operator == (const TACVar& lhs, const TACVar& rhs);
bool operator < (const TACVar& lhs, const TACVar& rhs);


} // /namespace ThreeAddressCodes


#endif



// ================================================================================