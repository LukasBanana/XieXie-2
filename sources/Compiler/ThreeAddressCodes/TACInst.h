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
        
        //! TAC instruction class types.
        enum class Types
        {
            Copy,           //!< x := y; x := const; x := "string";
            Modify,         //!< x := y + z; x := y - z; x := y * z; ...
            Jump,           //!< jump to basic block
            CondJump,       //!< conditional jump to basic block, if y = z, y != z, y < z, ...
            DirectCall,     //!< jump and link
            IndirectCall,   //!< jump register and link
            CallReturn,     //!< return from call
        };

        //! TAC instruction opcodes.
        enum class OpCodes
        {
            NOP,

            NOT,
            AND,
            OR,
            XOR,

            ADD,
            SUB,
            MUL,
            DIV,
            MOD,
            SLL,
            SLR,
            
            FADD,
            FSUB,
            FMUL,
            FDIV,
            
            CMPE,
            CMPNE,
            CMPL,
            CMPLE,
            CMPG,
            CMPGE,

            FCMPE,
            FCMPNE,
            FCMPL,
            FCMPLE,
            FCMPG,
            FCMPGE,
        };

        virtual ~TACInst()
        {
        }

        //! Returns the TAC instruction type.
        virtual Types Type() const = 0;

        //! Returns this TAC instruction as string representation (for debugging).
        virtual std::string ToString() const = 0;

        OpCodes opcode = OpCodes::NOP;

    protected:

        TACInst() = default;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================