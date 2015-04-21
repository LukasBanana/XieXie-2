/*
 * TAC variable header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TAC_VAR_H__
#define __XX_TAC_VAR_H__


#include "ValueTypes.h"
//#include "Variant.h"

#include <string>


namespace ThreeAddressCodes
{


//! TAC variable class.
class TACVar
{
    
    public:
        
        /* === Types and Enumerations === */

        using IDType = unsigned int;
        static const IDType invalidID = 0;

        //! TAC variable classes.
        enum class Types
        {
            Literal,    //!< Literal constant.
            Label,      //!< Address label.

            Temp,       //!< Compiler temporary variable.
            Local,      //!< Local variable.
            Global,     //!< Global variable.
            Member,     //!< Member variable.

            Result,     //!< Argument return '$ar'.
            ThisPtr,    //!< This pointer '$xr'.
            StackPtr,   //!< Stack pointer '$sp'.
            FramePtr,   //!< Frame pointer '$lb'.
        };

        /* === Functions === */

        TACVar() = default;
        TACVar(IDType id, const Types type = Types::Temp);
        TACVar(const std::string& value);
        TACVar(const char* value);

        static TACVar Int(int value);
        static TACVar Int(unsigned int value);
        static TACVar Float(float value);

        TACVar& operator = (const TACVar&) = default;

        //! Returns this variable as string representation (for debugging).
        std::string ToString() const;

        /**
        Returns true if the ID of this variable is valid.
        \see id
        \see invalidID
        */
        bool IsValid() const;

        //! Returns true if this variable is a constant literal.
        bool IsConst() const;
        //! Returns true if this variable is an address label.
        bool IsLabel() const;

        //! Returns ture if this is a temporary variable.
        bool IsTemp() const;
        //! Returns ture if this is a local variable.
        bool IsLocal() const;
        //! Returns ture if this is a global variable.
        bool IsGlobal() const;
        //! Returns ture if this is a member variable.
        bool IsMember() const;

        //! Returns true if this is a frame pointer variable.
        bool IsThisPtr() const;
        //! Returns true if this is a frame pointer variable.
        bool IsStackPtr() const;
        //! Returns true if this is a frame pointer variable.
        bool IsFramePtr() const;

        //! Returns the value as integer.
        int Int() const;
        //! Returns the value as floating-point.
        float Float() const;

        //! Replaces this variable by 'replacedVar' if this variable is equal to 'varToReplace'.
        bool Replace(const TACVar& varToReplace, const TACVar& replacedVar);

        //! Appends the specified offset to the ID number, if this is a temporary or local variable.
        void IDOffset(const IDType offset);

        /* === Members === */

        IDType          id      = TACVar::invalidID;
        Types           type    = Types::Temp;
        std::string     value;

        unsigned int    offset  = 0;                    //!< Memory offset (in bytes) from the respective scope (global or class scope).
        unsigned int    size    = 4;                    //!< Size (in bytes) of this variables). By default 4.

        static const TACVar varResult;
        static const TACVar varThisPtr;
        static const TACVar varStackPtr;
        static const TACVar varFramePtr;

};


bool operator == (const TACVar& lhs, const TACVar& rhs);
bool operator != (const TACVar& lhs, const TACVar& rhs);
bool operator < (const TACVar& lhs, const TACVar& rhs);


} // /namespace ThreeAddressCodes


#endif



// ================================================================================