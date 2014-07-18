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
        enum class OpCodes
        {
            AssignInst,     //!< x := y; x := const; x := "string";
            ArithmeticInst, //!< x := y + z; x := y - z; x := y * z; x := y / z; x := y % z;
            RelationInst,   //!< x := y == z; x := y != z; x := y < z; x := y <= z; x := y > z; x := y >= z;
            LabelInst,      //!< L1:
            JumpInst,       //!< goto L1; ifz x then goto L1;
            //...
        };

        virtual ~TACInst()
        {
        }

        //! Returns the TAC instruction opcode (or rather type).
        virtual OpCodes OpCode() const = 0;

        //! Returns this TAC instruction as string representation (for debugging).
        virtual std::string ToString() const = 0;

    protected:

        TACInst() = default;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================