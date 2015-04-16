/*
 * TACInst.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TAC_INST_H__
#define __XX_TAC_INST_H__


#include "TACVar.h"
#include "BitMask.h"

#include <string>
#include <set>
#include <memory>


namespace ThreeAddressCodes
{


class TACVar;

/**
TAC (Three Address Code) instruction base class.
This is a further IR (Intermediate Representation) of the entire program.
*/
class TACInst
{
    
    public:
        
        /* === Enumerations === */

        //! TAC instruction class types.
        enum class Types
        {
            Copy,           //!< Set: x := y; x := const; x := "string";
            Modify,         //!< Compute: x := y + z; x := y - z; x := y * z; ...
            Relation,       //!< Compute relation: y = z, y != z, y < z, ...
            Switch,         //!< Switch to basic block
            DirectCall,     //!< Jump and link
            IndirectCall,   //!< Jump register and link
            Return,         //!< Procedure return
            Param,          //!< Fetch parameter. // <-- !TODO! REPLACE BY "Heap" TAC instruction !!!!!!!!!!!!!
            Stack,          //!< Stack operation (Push or Pop).
            Heap,           //!< Heap operation (Load/Store Byte/Half/Word).
        };

        //! TAC instruction opcodes.
        enum class OpCodes
        {
            NOP,

            /* Copy */
            MOV,
            NOT,
            FTI,        // float-to-int static cast
            ITF,        // int-to-float static cast
            DYNCAST,    // dynamic (down) cast

            /* Modify */
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
            FMOD,

            /* Relation */
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

            /* Switch */
            SWITCH,

            /* Return */
            RETURN,

            /* DirectCall */
            DIRCALL,

            /* IndirectCall */
            INDCALL,

            /* Result */
            RESULT,

            /* Param */
            PARAM,

            /* Stack */
            PUSH,
            POP,

            /* Heap */
            LDB,
            STB,
            LDH,
            STH,
            LDW,
            STW,
        };

        /* === Structures === */

        struct VarFlags
        {
            enum
            {
                Dest        = (1 << 0),
                Source      = (1 << 1),
                TempOnly    = (1 << 2),
            };
        };

        /* === Functions === */

        virtual ~TACInst()
        {
        }

        //! Returns the TAC instruction type.
        virtual Types Type() const = 0;

        //! Returns this TAC instruction as string representation (for debugging).
        virtual std::string ToString() const = 0;

        //! Returns true if this instruction writes the specified variable.
        virtual bool WritesVar(const TACVar& var) const;
        //! Returns true if this instruction reads the specified variable.
        virtual bool ReadsVar(const TACVar& var) const;

        //! Inserts the destination variable of this instruction (if it has one) to the specified set.
        virtual void InsertDestVar(std::set<TACVar>& vars, const BitMask& flags = 0) const;
        //! Replaces the variable 'varToReplace' by 'replacedVar'.
        virtual void ReplaceVar(const TACVar& varToReplace, const TACVar& replacedVar, const BitMask& flags = (VarFlags::Dest | VarFlags::Source));

        //! Returns a string for the specified op-code.
        static std::string OpCodeToString(const OpCodes opcode);
        /**
        Returns a string for the op-code of this instruction,
        plus a space offset to give all op-code strings a unique length.
        \see OpCodeToString
        */
        std::string OpCodePrefix() const;

        //! Returns true if this is a floating-point operation.
        bool IsFloatOp() const;

        /* === Members === */

        //! TAC instruction op-code.
        OpCodes opcode = OpCodes::NOP;

    protected:

        TACInst() = default;
        TACInst(const OpCodes opcode) :
            opcode{ opcode }
        {
        }

};

//! Unique pointer to TACInst.
using TACInstPtr = std::unique_ptr<TACInst>;


} // /namespace ThreeAddressCodes


#endif



// ================================================================================