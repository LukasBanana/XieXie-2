/*
 * TACInst.h
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
            CopyAssign,     //!< x := y; x := const; x := "string";
            ModifyAssign,   //!< x := y + z; x := y - z; x := y * z; ...
            Jump,           //!< jump to basic block
            CondJump,       //!< conditional jump to basic block, if y = z, y != z, y < z, ...
            DirectCall,     //!< jump and link
            IndirectCall,   //!< jump register and link
            CallReturn,     //!< return from call
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