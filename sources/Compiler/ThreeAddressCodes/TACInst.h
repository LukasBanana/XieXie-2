/*
 * Three address code instruction header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TAC_INST_H__
#define __XX_TAC_INST_H__


#include <string>


namespace ThreeAddressCodes
{


/**
TAC (Three Address Code) instruction base class.
This is a further IR (Intermediate Representation) of the entire program.
*/
class TACInst
{
    
    public:
        
        //! TAC instruction opcodes.
        enum class Types
        {
            Assign,         //!< x := y; x := const; x := "string";
            Arithmetic,     //!< x := y + z; x := y - z; x := y * z; ...
            Relation,       //!< x := y == z; x := y != z; x := y < z; ...
            Label,          //!< L1:
            Jump,           //!< jump to basic block
            DirectCall,     //!< jump and link
            IndirectCall,   //!< jump register and link
        };

        virtual ~TACInst()
        {
        }

        //! Returns the TAC instruction type.
        virtual Types Type() const = 0;

        //! Returns this TAC instruction as string representation (for debugging).
        virtual std::string ToString() const = 0;

    protected:

        TACInst() = default;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================