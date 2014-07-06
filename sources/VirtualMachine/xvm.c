/*
 * XieXie 2.0 VirtualMachine (xvm) main ANSI-C file
 * 
 * Copyright (C) 2014 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <math.h>


/* ----- Compilation configuration ----- */

//! Enables force inlining
#define _ENABLE_INLINEING_

//! Enables runtimer debugger in virtual machine
//#define _ENABLE_RUNTIME_DEBUGGER_

//! Enables opcode extraction optimization (safes one SLL instruction in x86 code)
#define _OPTIMIZE_OPCODE_EXTRACTION_


/* ----- Helper macros ----- */

#define XVM_MIN(a, b) ((a) < (b) ? (a) : (b))
#define XVM_MAX(a, b) ((a) > (b) ? (a) : (b))

#ifdef _ENABLE_INLINEING_
#   define INLINE __inline
#else
#   define INLINE // No inlining
#endif


/* ----- Helper functions ----- */

INLINE static int flt2int_signum(float val)
{
    if (val > 0.0f)
        return 1;
    else if (val < 0.0f)
        return -1;
    return 0;
}


/* ----- Registers ----- */

typedef unsigned char   reg_t;
typedef int             regi_t;
typedef float           regf_t;

typedef char            byte_t;
typedef int             word_t;

#define NUM_REGISTERS               16
#define REG_TO_STACK_PTR(r)         ((stack_word_t*)(*(r)))
#define GET_PARAM_FROM_STACK(p, i)  (*((p) - (i)))

#define FLT_TO_INT_REINTERPRET(x)   (*((int*)(&x)))
#define INT_TO_FLT_REINTERPRET(x)   (*((float*)(&x)))
#define INT_TO_STR_REINTERPRET(x)   ((const char*)(x))

/*
All registers can be used for integral and floating-point data.
*/

typedef enum
{
    /* --- General purpose registers --- */
    REG_R0 = 0x00, // r0  ->  General purpose register 0.
    REG_R1 = 0x01, // r1  ->  General purpose register 1.
    REG_R2 = 0x02, // r2  ->  General purpose register 2.
    REG_R3 = 0x03, // r3  ->  General purpose register 3.
    REG_R4 = 0x04, // r4  ->  General purpose register 4.
    REG_R5 = 0x05, // r5  ->  General purpose register 5.
    REG_R6 = 0x06, // r6  ->  General purpose register 6.
    REG_R7 = 0x07, // r7  ->  General purpose register 7.
    REG_R8 = 0x08, // r8  ->  General purpose register 8.
    REG_R9 = 0x09, // r9  ->  General purpose register 9.

    /* --- Special purpose registers --- */
    REG_OP = 0x0a, // op  ->  Object pointer: "this" POINTER to the current object in a member function.
    REG_GP = 0x0b, // gp  ->  Global pointer: POINTER to the global variables in the stack.

    /* --- Register for internal use --- */
    REG_CF = 0x0c, // cf  ->  Conditional flags: used for jump conditions.
    REG_LB = 0x0d, // lb  ->  Base pointer: POINTER to the base of the current stack frame.
    REG_SP = 0x0e, // sp  ->  Stack pointer: POINTER to the top of the stack storage.
    REG_PC = 0x0f, // pc  ->  Program counter: POINTER to the current instruction in the byte-code.
}
register_id;

/**
Returns the name of the specified register or an empty string if the ID is invalid.
\see register_id
*/
static const char* register_get_name(register_id reg)
{
    switch (reg)
    {
        case REG_R0: return "r0";
        case REG_R1: return "r1";
        case REG_R2: return "r2";
        case REG_R3: return "r3";
        case REG_R4: return "r4";
        case REG_R5: return "r5";
        case REG_R6: return "r6";
        case REG_R7: return "r7";
        case REG_R8: return "r8";
        case REG_R9: return "r9";

        case REG_OP: return "op";
        case REG_GP: return "gp";

        case REG_CF: return "cf";
        case REG_LB: return "lb";
        case REG_SP: return "sp";
        case REG_PC: return "pc";
    }
    return "";
}


/* ----- OP-codes ----- */

/**

This is full documentation of the 32-bit instruction set of the XieXie 2.0 VirtualMachine.
Currently only single precision floats are supported. There are also no instructions that can handle 64 bit integers.

BYTES are 8 bits wide
WORDS are 32 bits wide.
FLOATS are 32 bits wide.

-------------------------------------------------------------------------------------------------------------------------------------------------
|                                                   2 Register Instruction Opcodes (00....):                                                    |
-------------------------------------------------------------------------------------------------------------------------------------------------
| Mnemonic | Opcode      | Dest.   | Source  | Unused                              | Description                                                |
-------------------------------------------------------------------------------------------------------------------------------------------------
|          | 31.......26 | 25...22 | 21...18 | 17................................0 |                                                            |
-------------------------------------------------------------------------------------------------------------------------------------------------
| MOV      | 0 0 0 0 0 1 | D D D D | S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Move register source to destination.                       |
-------------------------------------------------------------------------------------------------------------------------------------------------
| NOT      | 0 0 0 0 1 0 | D D D D | S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Bitwise NOT.                                               |
-------------------------------------------------------------------------------------------------------------------------------------------------
| AND      | 0 0 0 0 1 1 | D D D D | S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Bitwise AND.                                               |
-------------------------------------------------------------------------------------------------------------------------------------------------
| OR       | 0 0 0 1 0 0 | D D D D | S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Bitwise OR.                                                |
-------------------------------------------------------------------------------------------------------------------------------------------------
| XOR      | 0 0 0 1 0 1 | D D D D | S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Bitwise XOR.                                               |
-------------------------------------------------------------------------------------------------------------------------------------------------
| ADD      | 0 0 0 1 1 0 | D D D D | S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Arithmetic addition.                                       |
-------------------------------------------------------------------------------------------------------------------------------------------------
| SUB      | 0 0 0 1 1 1 | D D D D | S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Arithmetic subtraction.                                    |
-------------------------------------------------------------------------------------------------------------------------------------------------
| MUL      | 0 0 1 0 0 0 | D D D D | S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Arithmetic multiplication.                                 |
-------------------------------------------------------------------------------------------------------------------------------------------------
| DIV      | 0 0 1 0 0 1 | D D D D | S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Arithmetic division.                                       |
-------------------------------------------------------------------------------------------------------------------------------------------------
| MOD      | 0 0 1 0 1 0 | D D D D | S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Arithmetic modulo.                                         |
-------------------------------------------------------------------------------------------------------------------------------------------------
| SLL      | 0 0 1 0 1 1 | D D D D | S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Shift locial left.                                         |
-------------------------------------------------------------------------------------------------------------------------------------------------
| SLR      | 0 0 1 1 0 0 | D D D D | S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Shift locial right.                                        |
-------------------------------------------------------------------------------------------------------------------------------------------------
| CMP      | 0 0 1 1 0 1 | X X X X | Y Y Y Y | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Compares the two registers and stores the result in "cf".  |
-------------------------------------------------------------------------------------------------------------------------------------------------
| FTI      | 0 0 1 1 1 0 | D D D D | S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Float to integer.                                          |
-------------------------------------------------------------------------------------------------------------------------------------------------
| ITF      | 0 0 1 1 1 1 | D D D D | S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Integer to float (Flags like FTI).                         |
-------------------------------------------------------------------------------------------------------------------------------------------------


-------------------------------------------------------------------------------------------------------------------------------------------------
|                                                   1 Register Instruction Opcodes (01....):                                                    |
-------------------------------------------------------------------------------------------------------------------------------------------------
| Mnemonic | Opcode      | Reg.    | Value                                       | Description                                                  |
-------------------------------------------------------------------------------------------------------------------------------------------------
|          | 31.......26 | 25...22 | 21........................................0 |                                                              |
-------------------------------------------------------------------------------------------------------------------------------------------------
| MOV      | 0 1 0 0 0 0 | D D D D | V V V V V V V V V V V V V V V V V V V V V V | Move value to register.                                      |
-------------------------------------------------------------------------------------------------------------------------------------------------
| AND      | 0 1 0 0 0 1 | D D D D | V V V V V V V V V V V V V V V V V V V V V V | Bitwise AND.                                                 |
-------------------------------------------------------------------------------------------------------------------------------------------------
| OR       | 0 1 0 0 1 0 | D D D D | V V V V V V V V V V V V V V V V V V V V V V | Bitwise OR.                                                  |
-------------------------------------------------------------------------------------------------------------------------------------------------
| XOR      | 0 1 0 0 1 1 | D D D D | V V V V V V V V V V V V V V V V V V V V V V | Bitwise XOR.                                                 |
-------------------------------------------------------------------------------------------------------------------------------------------------
| ADD      | 0 1 0 1 0 0 | D D D D | V V V V V V V V V V V V V V V V V V V V V V | Arithmetic addition.                                         |
-------------------------------------------------------------------------------------------------------------------------------------------------
| SUB      | 0 1 0 1 0 1 | D D D D | V V V V V V V V V V V V V V V V V V V V V V | Arithmetic subtraction.                                      |
-------------------------------------------------------------------------------------------------------------------------------------------------
| MUL      | 0 1 0 1 1 0 | D D D D | V V V V V V V V V V V V V V V V V V V V V V | Arithmetic multiplication.                                   |
-------------------------------------------------------------------------------------------------------------------------------------------------
| DIV      | 0 1 0 1 1 1 | D D D D | V V V V V V V V V V V V V V V V V V V V V V | Arithmetic division.                                         |
-------------------------------------------------------------------------------------------------------------------------------------------------
| MOD      | 0 1 1 0 0 0 | D D D D | V V V V V V V V V V V V V V V V V V V V V V | Arithmetic modulo.                                           |
-------------------------------------------------------------------------------------------------------------------------------------------------
| SLL      | 0 1 1 0 0 1 | D D D D | V V V V V V V V V V V V V V V V V V V V V V | Shift locial left.                                           |
-------------------------------------------------------------------------------------------------------------------------------------------------
| SLR      | 0 1 1 0 1 0 | D D D D | V V V V V V V V V V V V V V V V V V V V V V | Shift locial right.                                          |
-------------------------------------------------------------------------------------------------------------------------------------------------
| PUSH     | 0 1 1 0 1 1 | S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Push source register onto stack.                             |
-------------------------------------------------------------------------------------------------------------------------------------------------
| POP      | 0 1 1 1 0 0 | D D D D | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Pop destination register from stack.                         |
-------------------------------------------------------------------------------------------------------------------------------------------------
| INC      | 0 1 1 1 0 1 | D D D D | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Increment integral register.                                 |
-------------------------------------------------------------------------------------------------------------------------------------------------
| DEC      | 0 1 1 1 1 0 | D D D D | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Decrement integral register.                                 |
-------------------------------------------------------------------------------------------------------------------------------------------------


-------------------------------------------------------------------------------------------------------------------------------------------------
|                                                      Jump Instruction Opcodes (100...):                                                       |
-------------------------------------------------------------------------------------------------------------------------------------------------
| Mnemonic | Opcode      | Reg.    | Offset                                      | Description                                                  |
-------------------------------------------------------------------------------------------------------------------------------------------------
|          | 31.......26 | 25...22 | 21........................................0 |                                                              |
-------------------------------------------------------------------------------------------------------------------------------------------------
| JMP      | 1 0 0 0 0 0 | S S S S | O O O O O O O O O O O O O O O O O O O O O O | Jump to address, stored in register 'S' + offset 'O'.        |
-------------------------------------------------------------------------------------------------------------------------------------------------
| JE       | 1 0 0 0 0 1 | S S S S | O O O O O O O O O O O O O O O O O O O O O O | Jump if greater.                                             |
-------------------------------------------------------------------------------------------------------------------------------------------------
| JNE      | 1 0 0 0 1 0 | S S S S | O O O O O O O O O O O O O O O O O O O O O O | Jump if not-equal.                                           |
-------------------------------------------------------------------------------------------------------------------------------------------------
| JG       | 1 0 0 0 1 1 | S S S S | O O O O O O O O O O O O O O O O O O O O O O | Jump if greater.                                             |
-------------------------------------------------------------------------------------------------------------------------------------------------
| JL       | 1 0 0 1 0 0 | S S S S | O O O O O O O O O O O O O O O O O O O O O O | Jump if less.                                                |
-------------------------------------------------------------------------------------------------------------------------------------------------
| JGE      | 1 0 0 1 0 1 | S S S S | O O O O O O O O O O O O O O O O O O O O O O | Jump if greater or equal.                                    |
-------------------------------------------------------------------------------------------------------------------------------------------------
| JLE      | 1 0 0 1 1 0 | S S S S | O O O O O O O O O O O O O O O O O O O O O O | Jump if less or equal.                                       |
-------------------------------------------------------------------------------------------------------------------------------------------------
|          |             |         |                                             | Push the dynamic link ('lb' and 'pc' register) onto stack.   |
| CALL     | 1 0 0 1 1 1 | S S S S | O O O O O O O O O O O O O O O O O O O O O O | Set 'lb' register to new stack frame.                        |
|          |             |         |                                             | Jump to address, stored in register 'S' + offset 'O'.        |
-------------------------------------------------------------------------------------------------------------------------------------------------


-------------------------------------------------------------------------------------------------------------------------------------------------
|                                                     Float Instruction Opcodes (101...):                                                       |
-------------------------------------------------------------------------------------------------------------------------------------------------
| Mnemonic | Opcode      | Dest.   | Source  | Unused                              | Description                                                |
-------------------------------------------------------------------------------------------------------------------------------------------------
|          | 31.......26 | 25...22 | 21...18 | 17................................0 |                                                            |
-------------------------------------------------------------------------------------------------------------------------------------------------
| ADDF     | 1 0 1 0 1 1 | D D D D | S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Arithmetic addition.                                       |
-------------------------------------------------------------------------------------------------------------------------------------------------
| SUBF     | 1 0 1 1 0 0 | D D D D | S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Arithmetic subtraction.                                    |
-------------------------------------------------------------------------------------------------------------------------------------------------
| MULF     | 1 0 1 1 0 1 | D D D D | S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Arithmetic multiplication.                                 |
-------------------------------------------------------------------------------------------------------------------------------------------------
| DIVF     | 1 0 1 1 1 0 | D D D D | S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Arithmetic division.                                       |
-------------------------------------------------------------------------------------------------------------------------------------------------
| CMPF     | 1 0 1 1 1 1 | X X X X | Y Y Y Y | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Compares the two registers and stores the result in "cf".  |
-------------------------------------------------------------------------------------------------------------------------------------------------


-------------------------------------------------------------------------------------------------------------------------------------------------
|                                                   Load/Store Instruction Opcodes (1110..):                                                    |
-------------------------------------------------------------------------------------------------------------------------------------------------
| Mnemonic | Opcode      | Reg.    | Address                                     | Description                                                  |
-------------------------------------------------------------------------------------------------------------------------------------------------
|          | 31.......26 | 25...22 | 21........................................0 |                                                              |
-------------------------------------------------------------------------------------------------------------------------------------------------
| LDB      | 1 1 1 0 0 0 | D D D D | A A A A A A A A A A A A A A A A A A A A A A | Load byte from program data section to register.             |
-------------------------------------------------------------------------------------------------------------------------------------------------
| LDW      | 1 1 1 0 0 1 | D D D D | A A A A A A A A A A A A A A A A A A A A A A | Load word from program data section to register.             |
-------------------------------------------------------------------------------------------------------------------------------------------------


-------------------------------------------------------------------------------------------------------------------------------------------------
|                                              Load/Store (Offset) Instruction Opcodes (1111..):                                                |
-------------------------------------------------------------------------------------------------------------------------------------------------
| Mnemonic | Opcode      | Dest.   | AddrReg | Offset                              | Description                                                |
-------------------------------------------------------------------------------------------------------------------------------------------------
|          | 31.......26 | 25...22 | 21...18 | 17................................0 |                                                            |
-------------------------------------------------------------------------------------------------------------------------------------------------
| LDB      | 1 1 1 1 0 0 | D D D D | A A A A | O O O O O O O O O O O O O O O O O O | Load byte from memory to register.                         |
-------------------------------------------------------------------------------------------------------------------------------------------------
| STB      | 1 1 1 1 0 1 | S S S S | A A A A | O O O O O O O O O O O O O O O O O O | Store byte from register to memory.                        |
-------------------------------------------------------------------------------------------------------------------------------------------------
| LDW      | 1 1 1 1 1 0 | D D D D | A A A A | O O O O O O O O O O O O O O O O O O | Load word from memory to register.                         |
-------------------------------------------------------------------------------------------------------------------------------------------------
| STW      | 1 1 1 1 1 1 | S S S S | A A A A | O O O O O O O O O O O O O O O O O O | Store word from register to memory.                        |
-------------------------------------------------------------------------------------------------------------------------------------------------


-------------------------------------------------------------------------------------------------------------------------------------------------
|                                                    Special Instruction Opcodes (110...):                                                      |
-------------------------------------------------------------------------------------------------------------------------------------------------
| Mnemonic | Opcode      | Unused or Value                                     | Description                                                    |
-------------------------------------------------------------------------------------------------------------------------------------------------
|          | 31.......26 | 25................................................0 |                                                                |
-------------------------------------------------------------------------------------------------------------------------------------------------
| STOP     | 0 0 0 0 0 0 | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Stop program execution.                                        |
-------------------------------------------------------------------------------------------------------------------------------------------------
| PUSH     | 1 1 0 0 0 1 | V V V V V V V V V V V V V V V V V V V V V V V V V V | Push value 'V' onto stack.                                     |
-------------------------------------------------------------------------------------------------------------------------------------------------
| INVK     | 1 1 0 0 1 0 | V V V V V V V V V V V V V V V V V V V V V V V V V V | Invoke external procedure with ID 'V'.                         |
-------------------------------------------------------------------------------------------------------------------------------------------------
| Mnemonic | Opcode      | Result Size     | Arguments Size                      | Description                                                  |
-------------------------------------------------------------------------------------------------------------------------------------------------
|          | 31.......26 | 25...........18 | 17................................0 |                                                              |
-------------------------------------------------------------------------------------------------------------------------------------------------
|          |             |                 |                                     | Pop R words from the stack and buffers them.                 |
|          |             |                 |                                     | Pop the current stack frame.                                 |
| RET      | 1 1 0 0 0 0 | R R R R R R R R | A A A A A A A A A A A A A A A A A A | Pop A words from the stack.                                  |
|          |             |                 |                                     | Push the R words back onto the stack.                        |
|          |             |                 |                                     | Restore the 'lb' and 'pc' registers.                         |
-------------------------------------------------------------------------------------------------------------------------------------------------

*/

#ifdef _OPTIMIZE_OPCODE_EXTRACTION_
typedef unsigned int opcode_t;
#else
typedef unsigned char opcode_t;
#endif

#ifdef _OPTIMIZE_OPCODE_EXTRACTION_
#   define GEN_OPCODE(op) ((op) << 26)
#else
#   define GEN_OPCODE(op) (op)
#endif

typedef enum
{
    OPCODE_MOV2     = GEN_OPCODE(0x01), // MOV  reg0, reg1  ->  *reg0 = *reg1
    OPCODE_NOT2     = GEN_OPCODE(0x02), // NOT  reg0, reg1  ->  *reg0 = ~*reg1
    OPCODE_AND2     = GEN_OPCODE(0x03), // AND  reg0, reg1  ->  *reg0 &= *reg1
    OPCODE_OR2      = GEN_OPCODE(0x04), // OR   reg0, reg1  ->  *reg0 |= *reg1
    OPCODE_XOR2     = GEN_OPCODE(0x05), // XOR  reg0, reg1  ->  *reg0 ^= *reg1
    OPCODE_ADD2     = GEN_OPCODE(0x06), // ADD  reg0, reg1  ->  *reg0 += *reg1
    OPCODE_SUB2     = GEN_OPCODE(0x07), // SUB  reg0, reg1  ->  *reg0 -= *reg1
    OPCODE_MUL2     = GEN_OPCODE(0x08), // MUL  reg0, reg1  ->  *reg0 *= *reg1
    OPCODE_DIV2     = GEN_OPCODE(0x09), // DIV  reg0, reg1  ->  *reg0 /= *reg1
    OPCODE_MOD2     = GEN_OPCODE(0x0a), // MOD  reg0, reg1  ->  *reg0 %= *reg1
    OPCODE_SLL2     = GEN_OPCODE(0x0b), // SLL  reg0, reg1  ->  *reg0 <<= *reg1
    OPCODE_SLR2     = GEN_OPCODE(0x0c), // SLR  reg0, reg1  ->  *reg0 >>= *reg1
    OPCODE_CMP      = GEN_OPCODE(0x0d), // CMP  reg0, reg1  ->  REG_CF = (*reg0 - *reg1)
    OPCODE_FTI      = GEN_OPCODE(0x0e), // FTI  reg0, reg1  ->  *reg0 = (int)*reg1
    OPCODE_ITF      = GEN_OPCODE(0x0f), // ITF  reg0, reg1  ->  *reg0 = (float)*reg1
}
opcode_reg2;

typedef enum
{
    OPCODE_MOV1     = GEN_OPCODE(0x10), // MOV  reg, c  ->  *reg = c
    OPCODE_AND1     = GEN_OPCODE(0x11), // AND  reg, c  ->  *reg &= c
    OPCODE_OR1      = GEN_OPCODE(0x12), // OR   reg, c  ->  *reg |= c
    OPCODE_XOR1     = GEN_OPCODE(0x13), // XOR  reg, c  ->  *reg ^= c
    OPCODE_ADD1     = GEN_OPCODE(0x14), // ADD  reg, c  ->  *reg += c
    OPCODE_SUB1     = GEN_OPCODE(0x15), // SUB  reg, c  ->  *reg -= c
    OPCODE_MUL1     = GEN_OPCODE(0x16), // MUL  reg, c  ->  *reg *= c
    OPCODE_DIV1     = GEN_OPCODE(0x17), // DIV  reg, c  ->  *reg /= c
    OPCODE_MOD1     = GEN_OPCODE(0x18), // MOD  reg, c  ->  *reg %= c
    OPCODE_SLL1     = GEN_OPCODE(0x19), // SLL  reg, c  ->  *reg <<= c
    OPCODE_SLR1     = GEN_OPCODE(0x1a), // SLR  reg, c  ->  *reg >>= c
    OPCODE_PUSH     = GEN_OPCODE(0x1b), // PUSH reg     ->  stack.push(*reg)
    OPCODE_POP      = GEN_OPCODE(0x1c), // POP  reg     ->  *reg = stack.pop()
    OPCODE_INC      = GEN_OPCODE(0x1d), // INC  reg     ->  ++*reg
    OPCODE_DEC      = GEN_OPCODE(0x1e), // DEV  reg     ->  --*reg
    //Reserved      = GEN_OPCODE(0x1f),
}
opcode_reg1;

typedef enum
{
    OPCODE_JMP      = GEN_OPCODE(0x20), // JMP  addr    -> goto addr
    OPCODE_JE       = GEN_OPCODE(0x21), // JE   addr    -> if (REG_CF == 0) then goto addr
    OPCODE_JNE      = GEN_OPCODE(0x22), // JNE  addr    -> if (REG_CF != 0) then goto addr
    OPCODE_JG       = GEN_OPCODE(0x23), // JG   addr    -> if (REG_CF  > 0) then goto addr
    OPCODE_JL       = GEN_OPCODE(0x24), // JL   addr    -> if (REG_CF  < 0) then goto addr
    OPCODE_JGE      = GEN_OPCODE(0x25), // JGE  addr    -> if (REG_CF >= 0) then goto addr
    OPCODE_JLE      = GEN_OPCODE(0x26), // JLE  addr    -> if (REG_CF <= 0) then goto addr
    OPCODE_CALL     = GEN_OPCODE(0x27), // CALL addr    -> PUSH pc ; PUSH lb ; MOV lb, sp ; JMP addr
    //Reserved      = GEN_OPCODE(0x28),
    //Reserved      = GEN_OPCODE(0x29),
    //Reserved      = GEN_OPCODE(0x2a),
}
opcode_jump;

typedef enum
{
    OPCODE_ADDF     = GEN_OPCODE(0x2b), // ADDF reg0, reg1  ->  *reg0 += *reg1
    OPCODE_SUBF     = GEN_OPCODE(0x2c), // SUBF reg0, reg1  ->  *reg0 -= *reg1
    OPCODE_MULF     = GEN_OPCODE(0x2d), // MULF reg0, reg1  ->  *reg0 *= *reg1
    OPCODE_DIVF     = GEN_OPCODE(0x2e), // DIVF reg0, reg1  ->  *reg0 /= *reg1
    OPCODE_CMPF     = GEN_OPCODE(0x2f), // CMPF reg0, reg1  ->  REG_CF = signum(*reg0 - *reg1)
}
opcode_float;

typedef enum
{
    OPCODE_STOP     = GEN_OPCODE(0x00), // STOP          ->  exit(0)
    OPCODE_RET      = GEN_OPCODE(0x30), // RET  (c0) c1  ->  return
    OPCODE_PUSHC    = GEN_OPCODE(0x31), // PUSH value    ->  stack.push(value)
    OPCODE_INVK     = GEN_OPCODE(0x32), // INVK addr     ->  invoke external procedure (no jump, no stack change)
    //Reserved      = GEN_OPCODE(0x33),
    //Reserved      = GEN_OPCODE(0x34),
    //Reserved      = GEN_OPCODE(0x35),
    //Reserved      = GEN_OPCODE(0x36),
    //Reserved      = GEN_OPCODE(0x37),
}
opcode_special;

typedef enum
{
    OPCODE_LDB      = GEN_OPCODE(0x38), // LDB reg0, addr  ->  *reg0 = programDataSectionByteAligned[addr]
    OPCODE_LDW      = GEN_OPCODE(0x39), // STB reg0, addr  ->  *reg0 = programDataSectionWorldAligned[addr]
    //Reserved      = GEN_OPCODE(0x3a),
    //Reserved      = GEN_OPCODE(0x3b),
}
opcode_mem;

typedef enum
{
    OPCODE_LDBO     = GEN_OPCODE(0x3c), // LDB reg0, (reg1) c  ->  *reg0 = dynamicMemoryByteAligned[*reg1 + c]
    OPCODE_STBO     = GEN_OPCODE(0x3d), // STB reg0, (reg1) c  ->  dynamicMemoryByteAligned[*reg1 + c] = *reg0
    OPCODE_LDWO     = GEN_OPCODE(0x3e), // LDW reg0, (reg1) c  ->  *reg0 = dynamicMemoryWordAligned[*reg1 + c]
    OPCODE_STWO     = GEN_OPCODE(0x3f), // STW reg0, (reg1) c  ->  dynamicMemoryWordAligned[*reg1 + c] = *reg0
}
opcode_memoff;


/* ----- Virtual machine exit codes ----- */

typedef enum
{
    EXITCODE_SUCCESS            = 0,
    EXITCODE_INVALID_BYTECODE   = -1,
    EXITCODE_INVALID_STACK      = -2,
    EXITCODE_INVALID_OPCODE     = -3,
    EXITCODE_INVALID_INTRINSIC  = -4,
    EXITCODE_STACK_OVERFLOW     = -5,
    EXITCODE_STACK_UNDERFLOW    = -6,
    EXITCODE_DIVISION_BY_ZERO   = -7,
}
xvm_exit_codes;

static const char* exitcode_to_string(const xvm_exit_codes exit_code)
{
    switch (exit_code)
    {
        case EXITCODE_SUCCESS:
            return "success";
        case EXITCODE_INVALID_BYTECODE:
            return "invalid byte code";
        case EXITCODE_INVALID_STACK:
            return "invalid stack";
        case EXITCODE_INVALID_OPCODE:
            return "invalid opcode";
        case EXITCODE_INVALID_INTRINSIC:
            return "invalid intrinsic";
        case EXITCODE_STACK_OVERFLOW:
            return "stack overflow";
        case EXITCODE_STACK_UNDERFLOW:
            return "stack underflow";
        case EXITCODE_DIVISION_BY_ZERO:
            return "division by zero";
    }
    return NULL;
}


/* ----- Debug log ----- */

INLINE static void log_print(const char* str)
{
    printf(str);
}

INLINE static void log_println(const char* str)
{
    printf(str);
    printf("\n");
}

INLINE static void log_error(const char* str)
{
    printf("ERROR: %s!\n", str);
}

INLINE static void log_readfile_error(const char* filename)
{
    printf("ERROR: Reading file \"%s\" failed!\n", filename);
}

INLINE static void log_exitcode_error(const xvm_exit_codes exit_code)
{
    const char* err = exitcode_to_string(exit_code);
    if (err != NULL)
        printf("ERROR: Program terminated with error: \"%s\"!\n", err);
}


/* ----- File helper ----- */

static unsigned int file_read_uint(FILE* file)
{
    unsigned int value;
    fread(&value, sizeof(value), 1, file);
    return value;
}

static void file_write_uint(FILE* file, unsigned int value)
{
    fwrite(&value, sizeof(value), 1, file);
}


/* ----- Instruction ----- */

typedef unsigned int instr_t;

INLINE static opcode_t instr_get_opcode(const instr_t instr)
{
    #ifdef _OPTIMIZE_OPCODE_EXTRACTION_
    /*
    This is the optimization for '_OPTIMIZE_OPCODE_EXTRACTION_'.
    This removes one SLL instruction in x86 for every executed virtual instruction.
    */
    return (instr & 0xfc000000);
    #else
    return (instr & 0xfc000000) >> 26;
    #endif
}

INLINE static unsigned int instr_get_value26(const instr_t instr)
{
    return (instr & 0x03ffffff);
}

INLINE static unsigned int instr_get_value22(const instr_t instr)
{
    return (instr & 0x003fffff);
}

INLINE static unsigned int instr_get_value18(const instr_t instr)
{
    return (instr & 0x0003ffff);
}

INLINE static int instr_get_sgn_value26(const instr_t instr)
{
    unsigned int val = instr_get_value26(instr);

    if ((val & 0x02000000) != 0)
        val |= 0xfc000000;

    return (int)val;
}

INLINE static int instr_get_sgn_value22(const instr_t instr)
{
    unsigned int val = instr_get_value22(instr);

    if ((val & 0x00200000) != 0)
        val |= 0xffc00000;

    return (int)val;
}

INLINE static int instr_get_sgn_value18(const instr_t instr)
{
    unsigned int val = instr_get_value18(instr);

    if ((val & 0x00020000) != 0)
        val |= 0xfffc0000;

    return (int)val;
}

INLINE static int instr_get_extra_value8(const instr_t instr)
{
    return (instr & 0x03fc0000) >> 18;
}

INLINE static reg_t instr_get_reg0(const instr_t instr)
{
    return (instr & 0x03c00000) >> 22;
}

INLINE static reg_t instr_get_reg1(const instr_t instr)
{
    return (instr & 0x003c0000) >> 18;
}

/**
Returns the mnemonic of the specified instruction opcode or an empty string if the opcode is invalid.
*/
static const char* intr_get_mnemonic(const opcode_t opcode)
{
    switch (opcode)
    {
        /* --- opcode_reg2 --- */

        case OPCODE_MOV1:
        case OPCODE_MOV2:   return "mov";
        case OPCODE_NOT2:   return "not";
        case OPCODE_AND1:
        case OPCODE_AND2:   return "and";
        case OPCODE_OR1:
        case OPCODE_OR2:    return "or";
        case OPCODE_XOR1:
        case OPCODE_XOR2:   return "xor";
        case OPCODE_ADD1:
        case OPCODE_ADD2:   return "add";
        case OPCODE_SUB1:
        case OPCODE_SUB2:   return "sub";
        case OPCODE_MUL1:
        case OPCODE_MUL2:   return "mul";
        case OPCODE_DIV1:
        case OPCODE_DIV2:   return "div";
        case OPCODE_MOD1:
        case OPCODE_MOD2:   return "mod";
        case OPCODE_SLL1:
        case OPCODE_SLL2:   return "sll";
        case OPCODE_SLR1:
        case OPCODE_SLR2:   return "slr";
        case OPCODE_CMP:    return "cmp";
        case OPCODE_FTI:    return "fti";
        case OPCODE_ITF:    return "itf";
            
        /* --- opcode_reg1 --- */

        case OPCODE_PUSH:   return "push";
        case OPCODE_POP:    return "pop";
        case OPCODE_INC:    return "inc";
        case OPCODE_DEC:    return "dec";

        /* --- opcode_jump --- */

        case OPCODE_JMP:    return "jmp";
        case OPCODE_JE:     return "je";
        case OPCODE_JNE:    return "jne";
        case OPCODE_JG:     return "jg";
        case OPCODE_JL:     return "jl";
        case OPCODE_JGE:    return "jge";
        case OPCODE_JLE:    return "jle";
        case OPCODE_CALL:   return "call";

        /* --- opcode_float --- */

        case OPCODE_ADDF:   return "addf";
        case OPCODE_SUBF:   return "subf";
        case OPCODE_MULF:   return "mulf";
        case OPCODE_DIVF:   return "divf";
        case OPCODE_CMPF:   return "cmpf";

        /* --- opcode_mem --- */

        case OPCODE_LDBO:
        case OPCODE_LDB:    return "ldb";
        case OPCODE_LDWO:
        case OPCODE_LDW:    return "ldw";

        /* --- opcode_memoff --- */

        case OPCODE_STBO:   return "stb";
        case OPCODE_STWO:   return "stw";

        /* --- opcode_special --- */

        case OPCODE_STOP:   return "stop";
        case OPCODE_RET:    return "ret";
        case OPCODE_PUSHC:  return "push";
        case OPCODE_INVK:   return "invk";
    }
    return "";
}

/**
Prints debug information for the specified instruction
with the current state of the specified register set.
*/
static void instr_print_debug_info(const instr_t instr, regi_t instr_index, const regi_t* reg_ptr)
{
    const opcode_t opcode = instr_get_opcode(instr);

    printf("0x%*.8x  %s", 8, (instr_index >> 2), intr_get_mnemonic(opcode));

    if (opcode == OPCODE_CMP)
    {
        reg_t reg0 = instr_get_reg0(instr);
        reg_t reg1 = instr_get_reg1(instr);
        printf(" %s, %s", register_get_name(reg0), register_get_name(reg1));
    }
    else if ( ( opcode >= OPCODE_MOV2 && opcode <= OPCODE_DEC  ) ||
              ( opcode >= OPCODE_LDBO && opcode <= OPCODE_STWO ) )
    {
        reg_t reg0 = instr_get_reg0(instr);
        regi_t value = reg_ptr[reg0];
        printf(" %s (val: %i)", register_get_name(reg0), value);
    }
    else if (opcode >= OPCODE_ADDF && opcode <= OPCODE_DIVF)
    {
        reg_t reg0 = instr_get_reg0(instr);
        regi_t value = reg_ptr[reg0];
        printf(" %s (val: %f)", register_get_name(reg0), INT_TO_FLT_REINTERPRET(value));
    }
    else if (opcode == OPCODE_PUSHC)
        printf(" %i", instr_get_sgn_value26(instr));

    printf("\n");
}


/* ----- Instruction constructors ----- */

static instr_t instr_make_reg2(opcode_reg2 opcode, reg_t reg0, reg_t reg1)
{
    return (instr_t)(
        #ifdef _OPTIMIZE_OPCODE_EXTRACTION_
        opcode                        |
        #else
        ((opcode & 0x3f      ) << 26) |
        #endif
        ((reg0   & 0x0f      ) << 22) |
        ((reg1   & 0x0f      ) << 18)
    );
}

static instr_t instr_make_reg1(opcode_reg1 opcode, reg_t reg, unsigned int value)
{
    return (instr_t)(
        #ifdef _OPTIMIZE_OPCODE_EXTRACTION_
        opcode                        |
        #else
        ((opcode & 0x3f      ) << 26) |
        #endif
        ((reg    & 0x0f      ) << 22) |
         (value  & 0x003fffff)
    );
}

static instr_t instr_make_jump(opcode_jump opcode, reg_t reg, unsigned int offset)
{
    return instr_make_reg1(opcode, reg, offset);
}

static instr_t instr_make_float(opcode_float opcode, reg_t reg0, reg_t reg1)
{
    return instr_make_reg2(opcode, reg0, reg1);
}

static instr_t instr_make_mem(opcode_mem opcode, reg_t reg, unsigned int address)
{
    return instr_make_reg1(opcode, reg, address);
}

static instr_t instr_make_memoff(opcode_memoff opcode, reg_t reg0, reg_t reg1, unsigned int offset)
{
    return (instr_t)(
        #ifdef _OPTIMIZE_OPCODE_EXTRACTION_
        opcode                        |
        #else
        ((opcode & 0x3f      ) << 26) |
        #endif
        ((reg0   & 0x0f      ) << 22) |
        ((reg1   & 0x0f      ) << 18) |
         (offset & 0x0003ffff)
    );
}

static instr_t instr_make_special1(opcode_special opcode, unsigned int value)
{
    return (instr_t)(
        #ifdef _OPTIMIZE_OPCODE_EXTRACTION_
        opcode                        |
        #else
        ((opcode & 0x3f      ) << 26) |
        #endif
         (value  & 0x03ffffff)
    );
}

static instr_t instr_make_special2(opcode_special opcode, unsigned int result_size, unsigned int arg_size)
{
    return (instr_t)(
        #ifdef _OPTIMIZE_OPCODE_EXTRACTION_
        opcode                             |
        #else
        ((opcode      & 0x3f      ) << 26) |
        #endif
        ((result_size & 0x000000ff) << 18) |
         (arg_size    & 0x0003ffff)
    );
}


/* ----- Byte code ----- */

typedef struct
{
    int         num_instructions;
    instr_t*    instructions;
}
xvm_bytecode;

static int xvm_bytecode_init(xvm_bytecode* byte_code)
{
    if (byte_code != NULL)
    {
        // Initialize byte code data
        byte_code->num_instructions = 0;
        byte_code->instructions     = NULL;
        return 1;
    }
    return 0;
}

static int xvm_bytecode_create(xvm_bytecode* byte_code, int num_instructions)
{
    if (byte_code != NULL && byte_code->instructions == NULL && num_instructions > 0)
    {
        byte_code->num_instructions = num_instructions;
        byte_code->instructions     = (instr_t*)malloc(sizeof(instr_t)*num_instructions);
        return 1;
    }
    return 0;
}

static int xvm_bytecode_free(xvm_bytecode* byte_code)
{
    if (byte_code != NULL)
    {
        if (byte_code->instructions != NULL)
            free(byte_code->instructions);

        // Reset byte code data
        byte_code->num_instructions = 0;
        byte_code->instructions     = NULL;

        return 1;
    }
    return 0;
}

/*

XBC file format spec:

WORD: magic number (Must be *(int*)"XBCF")
WORD: version number (Must be 101 for "1.01")
WORD: number of instructions
WORD[n]: instructions

*/

#define XBC_FORMAT_MAGIC    (*((int*)("XBCF")))
#define XBC_FORMAT_VERSION  101

static int xvm_bytecode_read_from_file(xvm_bytecode* byte_code, const char* filename)
{
    // Check arguments
    if (byte_code == NULL || filename == NULL)
    {
        log_error("Invalid arguments");
        return 0;
    }

    // Open file for reading
    FILE* file = fopen(filename, "rb");
    if (file == NULL)
    {
        log_error("Unable to open file for reading");
        return 0;
    }

    // Read magic number
    unsigned int magic = file_read_uint(file);
    if (magic != XBC_FORMAT_MAGIC)
    {
        log_error("Invalid magic number (Must be \"XBCF\")");
        fclose(file);
        return 0;
    }

    // Read version number
    unsigned int version = file_read_uint(file);
    if (version != XBC_FORMAT_VERSION)
    {
        log_error("Invalid version number (Must be 101)");
        fclose(file);
        return 0;
    }

    // Read instructions
    unsigned int num_instr = file_read_uint(file);

    if (xvm_bytecode_create(byte_code, (int)num_instr) == 0)
    {
        log_error("Creating byte code failed");
        fclose(file);
        return 0;
    }

    fread(byte_code->instructions, sizeof(instr_t), num_instr, file);

    // Close file and return with success
    fclose(file);

    return 1;
}

static int xvm_bytecode_write_to_file(const xvm_bytecode* byte_code, const char* filename)
{
    // Check arguments
    if (byte_code == NULL || byte_code->num_instructions <= 0 || byte_code->instructions == NULL || filename == NULL)
    {
        log_error("Invalid arguments");
        return 0;
    }

    // Open file for writing
    FILE* file = fopen(filename, "wb");
    if (file == NULL)
    {
        log_error("Unable to open file for writing");
        return 0;
    }

    // Write magic number
    file_write_uint(file, XBC_FORMAT_MAGIC);

    // Write version number
    file_write_uint(file, XBC_FORMAT_VERSION);

    // Write instructions
    unsigned int num_instr = (unsigned int)byte_code->num_instructions;
    file_write_uint(file, num_instr);

    fwrite(byte_code->instructions, sizeof(instr_t), num_instr, file);

    // Close file and return with success
    fclose(file);

    return 1;
}


/* ----- Exception handling ----- */

jmp_buf xvm_exception_envbuf;
const char* xvm_exception_err = "";

static void xvm_exception_throw(const char* error_message, int error_code)
{
    // Setup exception error message and make a long jump
    xvm_exception_err = error_message;
    longjmp(xvm_exception_envbuf, error_code);
}


/* ----- Stack ----- */

typedef int stack_word_t;

typedef struct
{
    size_t          stack_size; // Number of entries in the stack.
    stack_word_t*   storage;    // Stack data storage.
}
xvm_stack;

/**
Initializes the specified stack object.
\see xvm_stack_create
*/
static int xvm_stack_init(xvm_stack* stack)
{
    if (stack != NULL)
    {
        // Initialize stack data
        stack->stack_size   = 0;
        stack->storage      = NULL;
        return 1;
    }
    return 0;
}

/**
Clears all entries in the stack with the specified value
\see xvm_stack_create
*/
static int xvm_stack_clear(xvm_stack* stack, stack_word_t value)
{
    if (stack != NULL && stack->stack_size > 0 && stack->storage != NULL)
    {
        for (size_t i = 0; i < stack->stack_size; ++i)
            stack->storage[i] = value;
        return 1;
    }
    return 0;
}

/**
Creats a new stack for the virtual machine.
\param[out] stack Pointer to the resulting stack object.
\param[in] stack_size Specifies the stack size or rather the number of WORD (32-bit) entries.
\remarks Example:
\code
xvm_stack stack;
xvm_stack_init(&stack);
xvm_stack_create(&stack, 256);
// ...
xvm_stack_free(&stack);
\endcode
*/
static int xvm_stack_create(xvm_stack* stack, size_t stack_size)
{
    if (stack != NULL && stack->storage == NULL && stack_size != 0)
    {
        stack->stack_size   = stack_size;
        stack->storage      = (stack_word_t*)malloc(sizeof(stack_word_t)*stack_size);
        return 1;
    }
    return 0;
}

static int xvm_stack_free(xvm_stack* stack)
{
    if (stack != NULL)
    {
        if (stack->storage != NULL)
            free(stack->storage);

        // Reset stack data
        stack->stack_size   = 0;
        stack->storage      = NULL;

        return 1;
    }
    return 0;
}

INLINE static void xvm_stack_push(xvm_stack* stack, regi_t* reg_sp, stack_word_t value)
{
    stack_word_t* stack_ptr = REG_TO_STACK_PTR(reg_sp);
    if (stack_ptr < stack->storage + stack->stack_size)
    {
        *stack_ptr = value;
        (*reg_sp) += sizeof(stack_word_t);
    }
    else
        xvm_exception_throw("Stack overflow", EXITCODE_STACK_OVERFLOW);
}

INLINE static stack_word_t xvm_stack_pop(xvm_stack* stack, regi_t* reg_sp)
{
    stack_word_t* stack_ptr = REG_TO_STACK_PTR(reg_sp);
    if (stack_ptr > stack->storage)
        (*reg_sp) -= sizeof(stack_word_t);
    else
        xvm_exception_throw("Stack underflow", EXITCODE_STACK_OVERFLOW);
    return *REG_TO_STACK_PTR(reg_sp);
}

INLINE static stack_word_t xvm_stack_read(regi_t reg_sp, int word_offset)
{
    stack_word_t* stack_ptr = (stack_word_t*)reg_sp;
    return stack_ptr[word_offset];
}

INLINE static void xvm_stack_write(regi_t reg_sp, int word_offset, stack_word_t value)
{
    stack_word_t* stack_ptr = (stack_word_t*)reg_sp;
    stack_ptr[word_offset] = value;
}

static void xvm_stack_debug(xvm_stack* stack, size_t first_entry, size_t num_entries)
{
    if (stack != NULL)
    {
        // Print all n-th stack entries
        const size_t n = XVM_MIN(first_entry + num_entries, stack->stack_size);
        for (size_t i = first_entry; i < n; ++i)
            printf("stack[%i] = %i\n", i, stack->storage[i]);
    }
}

static void xvm_stack_debug_float(xvm_stack* stack, size_t first_entry, size_t num_entries)
{
    if (stack != NULL)
    {
        // Print all n-th stack entries
        const size_t n = XVM_MIN(first_entry + num_entries, stack->stack_size);
        for (size_t i = first_entry; i < n; ++i)
        {
            stack_word_t* entry = (stack->storage + i);
            printf("stack[%i] = %f\n", i, *((float*)entry));
        }
    }
}


/* ----- Intrinsics ----- */

/*

Calling convention:
- CALLER pushes all arguments from RIGHT-to-LEFT onto stack.
- CALLEE pops all arguments from the stack.
- CALLEE pushes result onto stack.

*/

typedef enum
{
    INTR_RESERVED       = 0x001fff00, // Start address of reserved primitive procedures.

    /* --- Dynamic memory intrinsics --- */
    INTR_ALLOC_MEM      = 0x001fff00, // void* AllocMem(uint sizeInBytes).
    INTR_FREE_MEM       = 0x001fff01, // void FreeMem(void* memoryAddress).
    INTR_COPY_MEM       = 0x001fff02, // void CopyMem(const void* srcMemAddr, void* dstMemAddr, uint sizeInBytes);
    
    /* --- Console intrinsics --- */
    INTR_SYS_CALL       = 0x001fff20, // void SysCall(const byte* stringAddress).
    INTR_CLEAR          = 0x001fff21, // void ClearTerm().
    INTR_PRINT          = 0x001fff22, // void Print(const byte* stringAddress).
    INTR_PRINT_LN       = 0x001fff23, // void PrintLn(const byte* stringAddress).
    INTR_PRINT_INT      = 0x001fff24, // void PrintInt(int value);
    INTR_PRINT_FLOAT    = 0x001fff25, // void PrintFloat(float value);
    INTR_INPUT_INT      = 0x001fff26, // int InputInt();
    INTR_INPUT_FLOAT    = 0x001fff27, // float InputFloat();

    /* --- Conditional intrinsics --- */
    // For easier expression evaluation.
    // For better performance, make use of conditional jump instructions instead.
    INTR_CMP_E          = 0x001fff30,
    INTR_CMP_NE         = 0x001fff31,
    INTR_CMP_L          = 0x001fff32,
    INTR_CMP_LE         = 0x001fff33,
    INTR_CMP_G          = 0x001fff34,
    INTR_CMP_GE         = 0x001fff35,
    INTR_LOGIC_OR       = 0x001fff36,
    INTR_LOGIC_AND      = 0x001fff37,
    INTR_LOGIC_NOT      = 0x001fff38,

    /* --- File intrinsics --- */
    INTR_CREATE_FILE    = 0x001fff40, // int CreateFile(const byte* stringAddress).
    INTR_DELETE_FILE    = 0x001fff41, // int DeleteFile(const byte* stringAddress).
    INTR_OPEN_FILE      = 0x001fff42, // void* OpenFile(const byte* stringAddress).
    INTR_CLOSE_FILE     = 0x001fff43, // void CloseFile(void* fileHandle).
    INTR_FILE_SIZE      = 0x001fff44, // int FileSize(const void* fileHandle).
    INTR_SET_FILE_POS   = 0x001fff45, // void FileSetPos(const void* fileHandle, int pos).
    INTR_GET_FILE_POS   = 0x001fff46, // int FileGetPos(const void* fileHandle).
    INTR_FileEOF        = 0x001fff47, // int FileEOF(const void* fileHandle).
    INTR_WRITE_BYTE     = 0x001fff48, // void WriteByte(const void* fileHandle, const void* memoryAddress).
    INTR_WRITE_INT      = 0x001fff49, // void WriteWord(const void* fileHandle, const void* memoryAddress).
    INTR_WRITE_FLOAT    = 0x001fff4a, // void WriteFloat(const void* fileHandle, const void* memoryAddress).
    INTR_READ_BYTE      = 0x001fff4b, // void ReadByte(const void* fileHandle, void* memoryAddress).
    INTR_READ_INT       = 0x001fff4c, // void ReadWord(const void* fileHandle, void* memoryAddress).
    INTR_READ_FLOAT     = 0x001fff4d, // void ReadFloat(const void* fileHandle, void* memoryAddress).

    /* --- Math intrinsics --- */
    INTR_SIN            = 0x001fff80, // float Sin(float x).
    INTR_COS            = 0x001fff81, // float Cos(float x).
    INTR_TAN            = 0x001fff82, // float Tan(float x).
    INTR_ASIN           = 0x001fff83, // float ASin(float x).
    INTR_ACOS           = 0x001fff84, // float ACos(float x).
    INTR_ATAN           = 0x001fff85, // float ATan(float x).
    INTR_POW            = 0x001fff86, // float Pow(float base, float exp).
    INTR_SQRT           = 0x001fff87, // float Sqrt(float x).
}
intrinsic_addr;


/* ----- Virtual machine ----- */

typedef struct
{
    int pc_reset;
    //int max_num_cycles;
}
xvm_execution_state;

static void xvm_call_intrinsic(int intrinsic_addr, xvm_stack* const stack, regi_t* reg_sp)
{
    switch (intrinsic_addr)
    {
        /* --- Dynamic memory intrinsics --- */

        case INTR_ALLOC_MEM:
        case INTR_FREE_MEM:
        case INTR_COPY_MEM:
        break;

        /* --- Console intrinsics --- */

        case INTR_SYS_CALL:
        {
            int arg0 = xvm_stack_read(*reg_sp, -1);
            system(INT_TO_STR_REINTERPRET(arg0));
        }
        break;

        case INTR_CLEAR:
        break;

        case INTR_PRINT:
        {
            int arg0 = xvm_stack_read(*reg_sp, -1);
            printf("%s", INT_TO_STR_REINTERPRET(arg0));
        }
        break;

        case INTR_PRINT_LN:
        {
            //int arg0 = xvm_stack_read(*reg_sp, -1);
            //printf("%s\n", INT_TO_STR_REINTERPRET(arg0));
            printf("\n");
        }
        break;

        case INTR_PRINT_INT:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            printf("%i", arg0);
        }
        break;

        case INTR_PRINT_FLOAT:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            printf("%f", INT_TO_FLT_REINTERPRET(arg0));
        }
        break;

        case INTR_INPUT_INT:
        {
            int result;
            scanf("%i", &result);
            xvm_stack_push(stack, reg_sp, result);
        }
        break;

        case INTR_INPUT_FLOAT:
        {
            float result;
            scanf("%f", &result);
            xvm_stack_push(stack, reg_sp, FLT_TO_INT_REINTERPRET(result));
        }
        break;

        /* --- Conditional intrinsics --- */

        case INTR_CMP_E:
        case INTR_CMP_NE:
        case INTR_CMP_L:
        case INTR_CMP_LE:
        case INTR_CMP_G:
        case INTR_CMP_GE:
        case INTR_LOGIC_OR:
        case INTR_LOGIC_AND:
        case INTR_LOGIC_NOT:
        break;

        /* --- File intrinsics --- */

        case INTR_CREATE_FILE:
        case INTR_DELETE_FILE:
        case INTR_OPEN_FILE:
        case INTR_CLOSE_FILE:
        case INTR_FILE_SIZE:
        case INTR_SET_FILE_POS:
        case INTR_GET_FILE_POS:
        case INTR_FileEOF:
        case INTR_WRITE_BYTE:
        case INTR_WRITE_INT:
        case INTR_WRITE_FLOAT:
        case INTR_READ_BYTE:
        case INTR_READ_INT:
        case INTR_READ_FLOAT:
        break;

        /* --- Math intrinsics --- */

        case INTR_SIN:
        {
            int arg0 = xvm_stack_read(*reg_sp, -1);
            float result = sinf(INT_TO_FLT_REINTERPRET(arg0));
            xvm_stack_write(*reg_sp, -1, FLT_TO_INT_REINTERPRET(result));
        }
        break;

        case INTR_COS:
        {
            int arg0 = xvm_stack_read(*reg_sp, -1);
            float result = cosf(INT_TO_FLT_REINTERPRET(arg0));
            xvm_stack_write(*reg_sp, -1, FLT_TO_INT_REINTERPRET(result));
        }
        break;

        case INTR_TAN:
        {
            int arg0 = xvm_stack_read(*reg_sp, -1);
            float result = tanf(INT_TO_FLT_REINTERPRET(arg0));
            xvm_stack_write(*reg_sp, -1, FLT_TO_INT_REINTERPRET(result));
        }
        break;

        case INTR_ASIN:
        {
            int arg0 = xvm_stack_read(*reg_sp, -1);
            float result = asinf(INT_TO_FLT_REINTERPRET(arg0));
            xvm_stack_write(*reg_sp, -1, FLT_TO_INT_REINTERPRET(result));
        }
        break;

        case INTR_ACOS:
        {
            int arg0 = xvm_stack_read(*reg_sp, -1);
            float result = acosf(INT_TO_FLT_REINTERPRET(arg0));
            xvm_stack_write(*reg_sp, -1, FLT_TO_INT_REINTERPRET(result));
        }
        break;

        case INTR_ATAN:
        {
            int arg0 = xvm_stack_read(*reg_sp, -1);
            float result = atanf(INT_TO_FLT_REINTERPRET(arg0));
            xvm_stack_write(*reg_sp, -1, FLT_TO_INT_REINTERPRET(result));
        }
        break;

        case INTR_POW:
        {
            int arg0 = xvm_stack_read(*reg_sp, -1);
            int arg1 = xvm_stack_read(*reg_sp, -2);
            float result = powf(INT_TO_FLT_REINTERPRET(arg0), INT_TO_FLT_REINTERPRET(arg1));
            xvm_stack_pop(stack, reg_sp);
            xvm_stack_write(*reg_sp, -1, FLT_TO_INT_REINTERPRET(result));
        }
        break;

        case INTR_SQRT:
        {
            int arg0 = xvm_stack_read(*reg_sp, -1);
            float result = sqrtf(INT_TO_FLT_REINTERPRET(arg0));
            xvm_stack_write(*reg_sp, -1, FLT_TO_INT_REINTERPRET(result));
        }
        break;

        default:
            xvm_exception_throw("Invalid intrinsic", EXITCODE_INVALID_INTRINSIC);
            break;
    }
}

/**
"Invoke extern" procedure signature.
\param[in] proc_id Specifies the external procedure which is to be invoked.
\param[in] stack_ptr Pointer to the current stack frame.
THe first parameter can be accessed with (stack_ptr - 1), the second with (stack_ptr - 2) etc. (from left-to-right).
*/
typedef void (*XVM_INVOKE_EXTERN_PROC)(unsigned int proc_id, stack_word_t* stack_ptr);

void xvm_invoke_extern_dummy(unsigned int proc_id, stack_word_t* stack_ptr)
{
    /* Dummy */
}

XVM_INVOKE_EXTERN_PROC xvm_invoke_extern = (&xvm_invoke_extern_dummy);

/**
Executes the specified XBC (XieXie Byte Code) program within the XVM (XieXie Virtual Machine).
\param[in] byte_code Pointer to the byte code to execute.
\param[in] stack Pointer to the stack which is to be used during execution.
\param[in,out] exe_state Optional pointer to the execution state. This may also be NULL.
\remakrs This is the main function for the entire virtual machine.
All instructions are implemented inside this function and its large switch-case statement.
\see xvm_bytecode_create
\see xvm_stack_create
*/
static xvm_exit_codes xvm_execute_program(
    const xvm_bytecode* const byte_code,
    xvm_stack* const stack,
    xvm_execution_state* const exe_state)
{
    #define JUMP_ADDRESS(r, v) (reg.i[r] + ((v) << 2))

    if (byte_code == NULL)
        return EXITCODE_INVALID_BYTECODE;
    if (stack == NULL)
        return EXITCODE_INVALID_STACK;

    /* --- VM internals --- */
    union
    {
        regi_t i[NUM_REGISTERS];
        regf_t f[NUM_REGISTERS];
    }
    reg;

    const int num_instr = byte_code->num_instructions;

    const regi_t instr_ptr_begin    = (regi_t)byte_code->instructions;
    const regi_t instr_ptr_end      = instr_ptr_begin + num_instr*4;

    regi_t* const reg_cf = (reg.i + REG_CF);
    regi_t* const reg_lb = (reg.i + REG_LB);
    regi_t* const reg_sp = (reg.i + REG_SP);
    regi_t* const reg_pc = (reg.i + REG_PC);

    // Program start pointer is used to load memory from program "DATA" section
    const byte_t* const program_start_ptr = (const byte_t*)(byte_code->instructions);

    /* --- Temporary memory --- */
    instr_t         instr;
    opcode_t        opcode;
    reg_t           reg0;
    reg_t           reg1;

    byte_t*         byte_mem_addr;
    word_t*         word_mem_addr;

    const byte_t*   cbyte_mem_addr;
    const word_t*   cword_mem_addr;

    int             sgn_value;
    unsigned int    unsgn_value;
    int             extra_value;

    /* --- Initialize VM (only reset reserved registers) --- */
    *reg_lb = (regi_t)stack->storage;
    *reg_sp = (regi_t)stack->storage;

    if (exe_state != NULL)
        *reg_pc = exe_state->pc_reset;
    else
        *reg_pc = instr_ptr_begin;

    /* --- Catch exceptions --- */
    int exception_val = setjmp(xvm_exception_envbuf);
    if (exception_val != 0)
    {
        log_error(xvm_exception_err);
        return exception_val;
    }

    #ifdef _ENABLE_RUNTIME_DEBUGGER_
    instr = 0;
    regi_t prev_pc_index = 0;
    #endif

    /* --- Start with program execution --- */
    while (*reg_pc < instr_ptr_end)
    {
        #ifdef _ENABLE_RUNTIME_DEBUGGER_
        if (instr != 0)
            instr_print_debug_info(instr, prev_pc_index, reg.i);
        #endif

        /* Load next instruction */
        instr = *((instr_t*)(*reg_pc));

        opcode = instr_get_opcode(instr);

        #ifdef _ENABLE_RUNTIME_DEBUGGER_
        prev_pc_index = *reg_pc - instr_ptr_begin;
        #endif

        /* Execute current instruction */
        switch (opcode)
        {
            /* --- opcode_reg2 --- */

            case OPCODE_MOV2:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                reg.i[reg0] = reg.i[reg1];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_NOT2:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                reg.i[reg0] = ~reg.i[reg1];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_AND2:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                reg.i[reg0] &= reg.i[reg1];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_OR2:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                reg.i[reg0] |= reg.i[reg1];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_XOR2:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                reg.i[reg0] ^= reg.i[reg1];
            }
            break;

            case OPCODE_ADD2:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                reg.i[reg0] += reg.i[reg1];
            }
            break;

            case OPCODE_SUB2:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                reg.i[reg0] -= reg.i[reg1];
            }
            break;

            case OPCODE_MUL2:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                reg.i[reg0] *= reg.i[reg1];
            }
            break;

            case OPCODE_DIV2:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                reg.i[reg0] /= reg.i[reg1];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_MOD2:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                reg.i[reg0] %= reg.i[reg1];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_SLL2:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                reg.i[reg0] <<= reg.i[reg1];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_SLR2:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                reg.i[reg0] >>= reg.i[reg1];
            }
            break;

            case OPCODE_CMP:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                *reg_cf = (reg.i[reg0] - reg.i[reg1]);
            }
            break;

            case OPCODE_FTI:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                reg.i[reg0] = (regi_t)reg.f[reg1];
            }
            break;

            case OPCODE_ITF:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                reg.f[reg0] = (regf_t)reg.i[reg1];
            }
            break;
            
            /* --- opcode_reg1 --- */

            case OPCODE_MOV1:
            {
                reg0 = instr_get_reg0(instr);
                reg.i[reg0] = instr_get_sgn_value22(instr);
            }
            break;

            // Undefined behavior for floats
            case OPCODE_AND1:
            {
                reg0 = instr_get_reg0(instr);
                reg.i[reg0] &= instr_get_sgn_value22(instr);
            }
            break;

            // Undefined behavior for floats
            case OPCODE_OR1:
            {
                reg0 = instr_get_reg0(instr);
                reg.i[reg0] |= instr_get_sgn_value22(instr);
            }
            break;

            // Undefined behavior for floats
            case OPCODE_XOR1:
            {
                reg0 = instr_get_reg0(instr);
                reg.i[reg0] ^= instr_get_sgn_value22(instr);
            }
            break;

            // Undefined behavior for floats
            case OPCODE_ADD1:
            {
                reg0 = instr_get_reg0(instr);
                reg.i[reg0] += instr_get_sgn_value22(instr);
            }
            break;

            // Undefined behavior for floats
            case OPCODE_SUB1:
            {
                reg0 = instr_get_reg0(instr);
                reg.i[reg0] -= instr_get_sgn_value22(instr);
            }
            break;

            // Undefined behavior for floats
            case OPCODE_MUL1:
            {
                reg0 = instr_get_reg0(instr);
                reg.i[reg0] *= instr_get_sgn_value22(instr);
            }
            break;

            // Undefined behavior for floats
            case OPCODE_DIV1:
            {
                reg0 = instr_get_reg0(instr);
                sgn_value = instr_get_sgn_value22(instr);
                if (sgn_value == 0)
                    xvm_exception_throw("Division by zero", EXITCODE_DIVISION_BY_ZERO);
                reg.i[reg0] /= sgn_value;
            }
            break;

            // Undefined behavior for floats
            case OPCODE_MOD1:
            {
                reg0 = instr_get_reg0(instr);
                sgn_value = instr_get_sgn_value22(instr);
                if (sgn_value == 0)
                    xvm_exception_throw("Division by zero", EXITCODE_DIVISION_BY_ZERO);
                reg.i[reg0] %= sgn_value;
            }
            break;

            // Undefined behavior for floats
            case OPCODE_SLL1:
            {
                reg0 = instr_get_reg0(instr);
                reg.i[reg0] <<= instr_get_sgn_value22(instr);
            }
            break;

            // Undefined behavior for floats
            case OPCODE_SLR1:
            {
                reg0 = instr_get_reg0(instr);
                reg.i[reg0] >>= instr_get_sgn_value22(instr);
            }
            break;

            case OPCODE_PUSH:
            {
                reg0 = instr_get_reg0(instr);
                xvm_stack_push(stack, reg_sp, reg.i[reg0]);
            }
            break;

            case OPCODE_POP:
            {
                reg0 = instr_get_reg0(instr);
                reg.i[reg0] = xvm_stack_pop(stack, reg_sp);
            }
            break;

            case OPCODE_INC:
            {
                reg0 = instr_get_reg0(instr);
                ++reg.i[reg0];
            }
            break;

            case OPCODE_DEC:
            {
                reg0 = instr_get_reg0(instr);
                --reg.i[reg0];
            }
            break;

            /* --- opcode_jump --- */

            // Undefined behavior for floats
            case OPCODE_JMP:
            {
                // Set program counter to (address + offset)
                reg0 = instr_get_reg0(instr);
                sgn_value = instr_get_sgn_value22(instr);
                *reg_pc = JUMP_ADDRESS(reg0, sgn_value);
                continue;
            }
            break;

            // Undefined behavior for floats
            case OPCODE_JE:
            {
                if (*reg_cf == 0)
                {
                    // Set program counter to (address + offset)
                    reg0 = instr_get_reg0(instr);
                    sgn_value = instr_get_sgn_value22(instr);
                    *reg_pc = JUMP_ADDRESS(reg0, sgn_value);
                    continue;
                }
            }
            break;

            // Undefined behavior for floats
            case OPCODE_JNE:
            {
                if (*reg_cf != 0)
                {
                    // Set program counter to (address + offset)
                    reg0 = instr_get_reg0(instr);
                    sgn_value = instr_get_sgn_value22(instr);
                    *reg_pc = JUMP_ADDRESS(reg0, sgn_value);
                    continue;
                }
            }
            break;

            // Undefined behavior for floats
            case OPCODE_JG:
            {
                if (*reg_cf > 0)
                {
                    // Set program counter to (address + offset)
                    reg0 = instr_get_reg0(instr);
                    sgn_value = instr_get_sgn_value22(instr);
                    *reg_pc = JUMP_ADDRESS(reg0, sgn_value);
                    continue;
                }
            }
            break;

            // Undefined behavior for floats
            case OPCODE_JL:
            {
                if (*reg_cf < 0)
                {
                    // Set program counter to (address + offset)
                    reg0 = instr_get_reg0(instr);
                    sgn_value = instr_get_sgn_value22(instr);
                    *reg_pc = JUMP_ADDRESS(reg0, sgn_value);
                    continue;
                }
            }
            break;

            // Undefined behavior for floats
            case OPCODE_JGE:
            {
                if (*reg_cf >= 0)
                {
                    // Set program counter to (address + offset)
                    reg0 = instr_get_reg0(instr);
                    sgn_value = instr_get_sgn_value22(instr);
                    *reg_pc = JUMP_ADDRESS(reg0, sgn_value);
                    continue;
                }
            }
            break;

            // Undefined behavior for floats
            case OPCODE_JLE:
            {
                if (*reg_cf <= 0)
                {
                    // Set program counter to (address + offset)
                    reg0 = instr_get_reg0(instr);
                    sgn_value = instr_get_sgn_value22(instr);
                    *reg_pc = JUMP_ADDRESS(reg0, sgn_value);
                    continue;
                }
            }
            break;

            // Undefined behavior for floats
            case OPCODE_CALL:
            {
                sgn_value = instr_get_sgn_value22(instr);

                if (sgn_value < INTR_RESERVED)
                {
                    // Push dynamic link (lb and pc registers)
                    extra_value = *reg_lb;
                    *reg_lb = *reg_sp;
                    xvm_stack_push(stack, reg_sp, extra_value);
                    xvm_stack_push(stack, reg_sp, *reg_pc);

                    // Jump to procedure address
                    reg0 = instr_get_reg0(instr);
                    *reg_pc = JUMP_ADDRESS(reg0, sgn_value);
                    continue;
                }
                else
                {
                    // Call intrinsic procedure
                    xvm_call_intrinsic(sgn_value, stack, reg_sp);
                }
            }
            break;

            /* --- opcode_float --- */

            case OPCODE_ADDF:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                reg.f[reg0] += reg.f[reg1];
            }
            break;

            case OPCODE_SUBF:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                reg.f[reg0] -= reg.f[reg1];
            }
            break;

            case OPCODE_MULF:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                reg.f[reg0] *= reg.f[reg1];
            }
            break;

            case OPCODE_DIVF:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                reg.f[reg0] /= reg.f[reg1];
            }
            break;

            case OPCODE_CMPF:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                *reg_cf = flt2int_signum(reg.f[reg0] - reg.f[reg1]);
            }
            break;

            /* --- opcode_mem --- */

            case OPCODE_LDB:
            {
                reg0 = instr_get_reg0(instr);
                unsgn_value = instr_get_value22(instr);

                cbyte_mem_addr = program_start_ptr + unsgn_value;

                reg.i[reg0] = (regi_t)(*cbyte_mem_addr);
            }
            break;

            case OPCODE_LDW:
            {
                reg0 = instr_get_reg0(instr);
                unsgn_value = instr_get_value22(instr);

                cbyte_mem_addr = program_start_ptr + unsgn_value;
                cword_mem_addr = (const word_t*)cbyte_mem_addr;

                reg.i[reg0] = (regi_t)(*cword_mem_addr);
            }
            break;

            /* --- opcode_memoff --- */

            case OPCODE_LDBO:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                sgn_value = instr_get_sgn_value18(instr);

                cbyte_mem_addr = (const byte_t*)reg.i[reg1];
                cbyte_mem_addr += sgn_value;

                reg.i[reg0] = (regi_t)(*cbyte_mem_addr);
            }
            break;

            case OPCODE_STBO:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                sgn_value = instr_get_sgn_value18(instr);

                byte_mem_addr = (byte_t*)reg.i[reg1];
                byte_mem_addr += sgn_value;

                *byte_mem_addr = (byte_t)(reg.i[reg0]);
            }
            break;

            case OPCODE_LDWO:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                sgn_value = instr_get_sgn_value18(instr);

                cbyte_mem_addr = (const byte_t*)reg.i[reg1];
                cbyte_mem_addr += sgn_value;

                cword_mem_addr = (const word_t*)cbyte_mem_addr;

                reg.i[reg0] = (regi_t)(*cword_mem_addr);
            }
            break;

            case OPCODE_STWO:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                sgn_value = instr_get_sgn_value18(instr);

                byte_mem_addr = (byte_t*)reg.i[reg1];
                byte_mem_addr += sgn_value;

                word_mem_addr = (word_t*)byte_mem_addr;

                *word_mem_addr = (word_t)(reg.i[reg0]);
            }
            break;

            /* --- opcode_special --- */

            case OPCODE_STOP:
                return EXITCODE_SUCCESS;

            case OPCODE_RET:
            {
                // Get result size (extra_value) and argument size (unsgn_value)
                // These sizes are in words (i.e. 4 bytes)
                extra_value = instr_get_extra_value8(instr);
                unsgn_value = instr_get_value18(instr);

                stack_word_t* stack_frame_ptr = REG_TO_STACK_PTR(reg_lb);

                // Pop stack frame
                stack_word_t* stack_result_ptr = REG_TO_STACK_PTR(reg_sp) - extra_value;
                stack_word_t* stack_args_ptr = REG_TO_STACK_PTR(reg_lb) - unsgn_value;
                *reg_sp = (regi_t)(stack_args_ptr + extra_value);

                // Pop dynamic link (pc and lb registers)
                *reg_lb = (regi_t)(stack_frame_ptr[0]);
                *reg_pc = (regi_t)(stack_frame_ptr[1]);

                // Move result memory
                if (extra_value > 0 && unsgn_value > 0)
                    memcpy(stack_args_ptr, stack_result_ptr, sizeof(stack_word_t) * extra_value);
            }
            // Don't 'continue' -> pc has old value and must be increased for the next instruction!
            break;

            case OPCODE_PUSHC:
            {
                sgn_value = instr_get_sgn_value26(instr);
                xvm_stack_push(stack, reg_sp, sgn_value);
            }
            break;

            case OPCODE_INVK:
            {
                unsgn_value = instr_get_value26(instr);
                xvm_invoke_extern(unsgn_value, REG_TO_STACK_PTR(reg_sp));
            }
            break;

            default:
                // Unknown opcode -> return with error
                return EXITCODE_INVALID_OPCODE;
        }

        // Increase program-counter register
        *reg_pc += 4;
    }

    // Return program counter if set
    if (exe_state != NULL)
        exe_state->pc_reset = *reg_pc;

    return EXITCODE_SUCCESS;

    #undef JUMP_ADDRESS
}


/* ----- Shell ----- */

static void shell_print_help()
{
    log_println("Usage: xvm [options] file");
    log_println("Options:");
    log_println("  -h --help help           Prints the help information");
    log_println("  --version                Prints the version and license note");
    log_println("  --verbose                Prints additional output before and after program execution");
    log_println("  -st --stack-size <arg>   Sets the stack size (by default 256)");
}

static void shell_print_version()
{
    log_println("XieXie 2.0 VirtualMachine (XVM)");
    log_println("");
    log_println("Copyright (C) 2014 Lukas Hermanns");
    log_println("All rights reserved.");
    log_println("");
    log_println("This software may be modified and distributed under the terms");
    log_println("of the BSD license.  See the LICENSE file for details.");
}

static int shell_parse_args(int argc, char* argv[])
{
    const char* arg;

    if (argc <= 0)
    {
        log_println("No input: enter \"help\" for information");
        return 0;
    }

    // Configuration memory
    int verbose = 0;
    const char* filename = NULL;
    size_t stack_size = 256;

    // Parse all arguments
    while (argc > 0)
    {
        // Store current argument
        arg = *argv;

        // Get next argument
        ++argv;
        --argc;

        // Parse current argument
        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0 || strcmp(arg, "help") == 0)
            shell_print_help();
        else if (strcmp(arg, "--version") == 0)
            shell_print_version();
        else if (strcmp(arg, "--verbose") == 0)
            verbose = 1;
        else if (strcmp(arg, "-st") == 0 || strcmp(arg, "--stack-size") == 0)
        {
            if (argc > 0)
            {
                // Get parameter from next argument
                arg = *argv;
                int param = atoi(arg);

                if (param <= 0)
                {
                    log_error("Stack size must be greater than zero");
                    return 0;
                }
                else
                    stack_size = (size_t)param;

                // Get next argument
                ++argv;
                --argc;
            }
            else
            {
                log_error("Expected argument after \"-st\" and \"--stack-size\" flag");
                return 0;
            }
        }
        else if (strlen(arg) > 0)
        {
            if (filename != NULL)
            {
                log_error("Only a single program can be executed at a time");
                return 0;
            }
            else
                filename = arg;
        }
    }

    // Execute shell command
    if (filename != NULL)
    {
        // Read byte code from file
        xvm_bytecode byte_code;
        xvm_bytecode_init(&byte_code);

        if (xvm_bytecode_read_from_file(&byte_code, filename) == 0)
        {
            log_readfile_error(filename);
            return 0;
        }

        // Create stack
        xvm_stack stack;
        xvm_stack_init(&stack);
        xvm_stack_create(&stack, stack_size);

        // Execute program
        const xvm_exit_codes exit_code = xvm_execute_program(&byte_code, &stack, NULL);

        if (exit_code != EXITCODE_SUCCESS)
            log_exitcode_error(exit_code);
        else if (verbose != 0)
            log_println("Program terminated successful");

        // Clean up
        xvm_bytecode_free(&byte_code);
        xvm_stack_free(&stack);
    }

    return 1;
}


/* ----- Main ----- */

typedef enum
{
    TESTPROCID_HELLO_WORLD,
}
TestInvokeProcIDs;

void TestInvokeExtern(unsigned int proc_id, stack_word_t* stack_ptr)
{
    switch (proc_id)
    {
        case TESTPROCID_HELLO_WORLD:
            printf("\nHello, World\n");
            break;
    }
}

int main(int argc, char* argv[])
{
    // Ignore program path argument, then parse all other arguments
    shell_parse_args(--argc, ++argv);

    #ifdef _DEBUG

    // Create a virtual stack
    xvm_stack stack;
    xvm_stack_init(&stack);
    xvm_stack_create(&stack, 256);
    //xvm_stack_clear(&stack, 0xdeadbeef);

    // Create the byte code
    xvm_bytecode byte_code;
    xvm_bytecode_init(&byte_code);
    xvm_bytecode_create(&byte_code, 50);

    size_t i = 0;
    #define ADD_INSTR(INSTR) byte_code.instructions[i++] = INSTR;
    #define FINISH_INSTR byte_code.num_instructions = i;

    #if 0 //TEST1 (loop)

    /*
    // Counts from 0 to n
    for int i := 0 ; i < 10 ; i++ {
        Stack.Push(i)
    }
    */

    /*
    00          mov r0, 0
    01          mov r1, 10
    02  l_for:  cmp r0, r1
    03          jge l_end   ; jge (pc) 4
    04          push r0
    05          inc r0
    06          jmp l_for   ; jmp (pc) -4
    07  l_end:  stop
    */

    ADD_INSTR(instr_make_reg1       (OPCODE_MOV1, REG_R0, 0                 ))
    ADD_INSTR(instr_make_reg1       (OPCODE_MOV1, REG_R1, 10                ))
    ADD_INSTR(instr_make_reg2       (OPCODE_CMP,  REG_R0, REG_R1            ))
    ADD_INSTR(instr_make_jump       (OPCODE_JGE,  REG_PC, 4                 ))
    ADD_INSTR(instr_make_reg1       (OPCODE_PUSH, REG_R0, 0                 ))
    ADD_INSTR(instr_make_reg1       (OPCODE_INC,  REG_R0, 0                 ))
    ADD_INSTR(instr_make_reg1       (OPCODE_JMP,  REG_PC, (unsigned int)(-4)))
    ADD_INSTR(instr_make_special1   (OPCODE_STOP, 0                         ))

    #elif 0 //TEST2 (function)

    /*
    // Computes the first n-th squares: 0, 1, 4, 9, 16 ...
    int Func(int x) {
        return x*x
    }
    for int i := 0 ; i < 10 ; i++ {
        Stack.Push(Func(i))
    }
    */

    /*
    00  main:   add sp, 8       ; int i, n
    01          xor r0, r0      ; i = 0
    02          mov r1, r0      ; n = 10
    03  .for0:  cmp r0, r1      ; compare i >= n
    04          jge .end0       ; if i >= n then goto l_end ; jge (pc) 7
    05          stw r0 (lb) 0   ; store i
    06          add r0, 2       ; i += 2
    07          push r0         ; push i
    08          call func       ; Stack.Push(func(i)) (actually 'pop i0' after call, but we keep it on stack) ; call (pc) 5
    09          ldw r0, (lb) 0  ; load i
    10          inc r0          ; i++
    11          jmp .for0       ; jmp (pc) -7
    12  .end0:  stop            ; exit
    13  func:   ldw r0, (lb) -4 ; load x
    14          mul r0, r0      ; x *= x
    15          push r0         ; push x
    16          ret (1) 1       ; return result ((x*x) = 1 word) and pop arguments (x = 1 word)
    */

    /*ADD_INSTR(instr_make_special1   (OPCODE_PUSHC,25                                ))
    ADD_INSTR(instr_make_special1   (OPCODE_INVK, TESTPROCID_PRINTINT               ))
    ADD_INSTR(instr_make_reg1       (OPCODE_SUB1, REG_SP, 4                         ))
    ADD_INSTR(instr_make_jump       (OPCODE_JMP,  REG_PC, -3                        ))*/

    ADD_INSTR(instr_make_reg1       (OPCODE_ADD1, REG_SP, 8                         ))
    ADD_INSTR(instr_make_reg2       (OPCODE_XOR2, REG_R0, REG_R0                    ))
    ADD_INSTR(instr_make_reg1       (OPCODE_MOV1, REG_R1, 20                        ))
    ADD_INSTR(instr_make_reg2       (OPCODE_CMP,  REG_R0, REG_R1                    ))
    ADD_INSTR(instr_make_jump       (OPCODE_JGE,  REG_PC, 8                         ))
    ADD_INSTR(instr_make_memoff     (OPCODE_STWO, REG_R0, REG_LB, 0                 ))
    ADD_INSTR(instr_make_reg1       (OPCODE_ADD1, REG_R0, 2                         ))
    ADD_INSTR(instr_make_reg1       (OPCODE_PUSH, REG_R0, 0                         ))
    ADD_INSTR(instr_make_jump       (OPCODE_CALL, REG_PC, 5                         ))
    ADD_INSTR(instr_make_memoff     (OPCODE_LDWO, REG_R0, REG_LB, 0                 ))
    ADD_INSTR(instr_make_reg1       (OPCODE_INC,  REG_R0, 0                         ))
    ADD_INSTR(instr_make_jump       (OPCODE_JMP,  REG_PC, -8                        ))
    ADD_INSTR(instr_make_special1   (OPCODE_STOP, 0                                 ))
    ADD_INSTR(instr_make_memoff     (OPCODE_LDWO, REG_R0, REG_LB, (unsigned int)(-4)))
    ADD_INSTR(instr_make_reg2       (OPCODE_MUL2, REG_R0, REG_R0                    ))
    ADD_INSTR(instr_make_reg1       (OPCODE_PUSH, REG_R0, 0                         ))
    ADD_INSTR(instr_make_special2   (OPCODE_RET,  1, 1                              ))

    #elif 0 //TEST3 (floats)

    /*
    Print(Pow(3, 5))
    */

    /*
    00  ldw r0, flt_lit_0
    01  ldw r1, flt_lit_1
    02  push r1
    03  push r0
    04  call Intr.Pow
    05  call Intr.PrintFloat
    06  stop
    07  flt_lit_0: DATA.float 3.0
    08  flt_lit_1: DATA.float 5.0
    */

    float flt_lit0 = 3.0f;
    float flt_lit1 = 5.0f;
    
    ADD_INSTR(instr_make_mem        (OPCODE_LDW, REG_R0, 8*4                ))
    ADD_INSTR(instr_make_mem        (OPCODE_LDW, REG_R1, 9*4                ))
    ADD_INSTR(instr_make_reg1       (OPCODE_PUSH, REG_R1, 0                 ))
    ADD_INSTR(instr_make_reg1       (OPCODE_PUSH, REG_R0, 0                 ))
    ADD_INSTR(instr_make_jump       (OPCODE_CALL, REG_PC, INTR_POW          ))
    ADD_INSTR(instr_make_jump       (OPCODE_CALL, REG_PC, INTR_PRINT_FLOAT  ))
    ADD_INSTR(instr_make_special1   (OPCODE_INVK, TESTPROCID_HELLO_WORLD    ))
    ADD_INSTR(instr_make_special1   (OPCODE_STOP, 0                         ))
    ADD_INSTR(FLT_TO_INT_REINTERPRET(flt_lit0))
    ADD_INSTR(FLT_TO_INT_REINTERPRET(flt_lit1))

    #elif 1 //TEST4 (fibonacci)

    /*
    int fib(int n) {
        if n <= 2 {
            return 1
        }
        return fib(n - 1) + fib(n - 2)
    }
    int n := Intr.InputInt()
    while n > 0 {
        Intr.PrintInt(fib(n))
        Intr.PrintLn("")
        n--
    }
    */

    /*
    00          call Intr.InputInt
    01          pop r0
    02  .loop:  xor r1, r1
    03          cmp r0, r1
    04          jle .end0           ; while begin
    05          push r0
    06          push r0             ; fib argument
    07          call fib
    08          call Intr.PrintInt  ; print result
    09          call Intr.PrintLn
    0a          pop r0
    0b          dec r0
    0c          jmp .loop           ; while end
    0d  .end:   stop
    0e  fib:    ldw r0, (lb) -4     ; get argument
    0f          mov r1, 2
    10          cmp r0, r1
    11          jg .else
    12          push 1
    13          ret (1) 1
    14  .else:  dec r0
    15          push r0             ; push t0 = (n-1)
    16          push r0             ; fib argument (n-1)
    17          call fib
    18          pop r0              ; fib result
    19          pop r1              ; pop t0
    1a          dec r1
    1b          push r0             ; push t1 = result
    1c          push r1             ; fib argument (n-2)
    1d          call fib
    1e          pop r0              ; fib result
    1f          pop r1              ; pop t1
    20          add r0, r1
    21          push r0             ; push result
    22          ret (1) 1
    */

    ADD_INSTR(instr_make_jump       (OPCODE_CALL, REG_PC, INTR_INPUT_INT            ))
    ADD_INSTR(instr_make_reg1       (OPCODE_POP,  REG_R0, 0                         ))
    ADD_INSTR(instr_make_reg2       (OPCODE_XOR2, REG_R1, REG_R1                    ))
    ADD_INSTR(instr_make_reg2       (OPCODE_CMP,  REG_R0, REG_R1                    ))
    ADD_INSTR(instr_make_jump       (OPCODE_JLE,  REG_PC, 9                         ))
    ADD_INSTR(instr_make_reg1       (OPCODE_PUSH, REG_R0, 0                         ))
    ADD_INSTR(instr_make_reg1       (OPCODE_PUSH, REG_R0, 0                         ))
    ADD_INSTR(instr_make_jump       (OPCODE_CALL, REG_PC, 7                         )) // call fib
    ADD_INSTR(instr_make_jump       (OPCODE_CALL, REG_PC, INTR_PRINT_INT            )) // call Intr.PrintInt
    ADD_INSTR(instr_make_jump       (OPCODE_CALL, REG_PC, INTR_PRINT_LN             )) // call Intr.PrintLn
    ADD_INSTR(instr_make_reg1       (OPCODE_POP,  REG_R0, 0                         ))
    ADD_INSTR(instr_make_reg1       (OPCODE_DEC,  REG_R0, 0                         ))
    ADD_INSTR(instr_make_jump       (OPCODE_JMP,  REG_PC, (unsigned int)(-10)       ))
    ADD_INSTR(instr_make_special1   (OPCODE_STOP, 0                                 ))
    ADD_INSTR(instr_make_memoff     (OPCODE_LDWO, REG_R0, REG_LB, (unsigned int)(-4))) // fib:
    ADD_INSTR(instr_make_reg1       (OPCODE_MOV1, REG_R1, 2                         ))
    ADD_INSTR(instr_make_reg2       (OPCODE_CMP,  REG_R0, REG_R1                    ))
    ADD_INSTR(instr_make_jump       (OPCODE_JG,   REG_PC, 3                         ))
    ADD_INSTR(instr_make_special1   (OPCODE_PUSHC,1                                 ))
    ADD_INSTR(instr_make_special2   (OPCODE_RET,  1, 1                              )) // ret (1) 1
    ADD_INSTR(instr_make_reg1       (OPCODE_DEC,  REG_R0, 0                         ))
    ADD_INSTR(instr_make_reg1       (OPCODE_PUSH, REG_R0, 0                         ))
    ADD_INSTR(instr_make_reg1       (OPCODE_PUSH, REG_R0, 0                         ))
    ADD_INSTR(instr_make_jump       (OPCODE_CALL, REG_PC, (unsigned int)(-9)        )) // call fib
    ADD_INSTR(instr_make_reg1       (OPCODE_POP,  REG_R0, 0                         ))
    ADD_INSTR(instr_make_reg1       (OPCODE_POP,  REG_R1, 0                         ))
    ADD_INSTR(instr_make_reg1       (OPCODE_DEC,  REG_R1, 0                         ))
    ADD_INSTR(instr_make_reg1       (OPCODE_PUSH, REG_R0, 0                         ))
    ADD_INSTR(instr_make_reg1       (OPCODE_PUSH, REG_R1, 0                         ))
    ADD_INSTR(instr_make_jump       (OPCODE_CALL, REG_PC, (unsigned int)(-15)       )) // call fib
    ADD_INSTR(instr_make_reg1       (OPCODE_POP,  REG_R0, 0                         ))
    ADD_INSTR(instr_make_reg1       (OPCODE_POP,  REG_R1, 0                         ))
    ADD_INSTR(instr_make_reg2       (OPCODE_ADD2, REG_R0, REG_R1                    ))
    ADD_INSTR(instr_make_reg1       (OPCODE_PUSH, REG_R0, 0                         ))
    ADD_INSTR(instr_make_special2   (OPCODE_RET,  1, 1                              )) // ret (1) 1

    #endif
    
    FINISH_INSTR

    #undef ADD_INSTR
    #undef FINISH_INSTR

    // Execute the virtual program
    xvm_invoke_extern = (&TestInvokeExtern);

    const xvm_exit_codes exitCode = xvm_execute_program(&byte_code, &stack, NULL);

    if (exitCode != EXITCODE_SUCCESS)
        printf("\nProgram terminated with error code: %i\n\n", exitCode);

    // Show stack output for the 20th first values
    printf("\n\n");

    #if 1
    log_println("-- Stack content: --");
    //xvm_stack_debug(&stack, 2, 20);
    xvm_stack_debug_float(&stack, 0, 10);
    #endif

    xvm_bytecode_write_to_file(&byte_code, "fibonacci.xbc");

    xvm_stack_free(&stack);
    xvm_bytecode_free(&byte_code);

    system("pause");

    #endif

    return 0;
}
