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
#include <time.h>

/* ----- Compilation configuration ----- */

//! Removes the XVM test suite (removes the "main" function)
//#define _REMOVE_XVM_TESTSUITE_

//! Enables force inlining
#define _ENABLE_INLINEING_

//! Enables runtimer debugger in virtual machine
//#define _ENABLE_RUNTIME_DEBUGGER_

//! Shows the instruction indices as hex numbers.
//#define _SHOW_RUNTIME_HEXLINES_

//! Enables opcode extraction optimization (safes one SLL instruction in x86 code)
#define _OPTIMIZE_OPCODE_EXTRACTION_

//! Enables OS specific features
#define _ENABLE_OS_FEATURES_


/* ----- OS specific includes ----- */

#ifdef _ENABLE_OS_FEATURES_

#if defined(_WIN32)
#   define NOGDICAPMASKS
#   define NOVIRTUALKEYCODES
#   define NOWINMESSAGES
#   define NOWINSTYLES
#   define NOSYSMETRICS
#   define NOMENUS
#   define NOICONS
#   define NOKEYSTATES
#   define NOSYSCOMMANDS
#   define NORASTEROPS
#   define NOSHOWWINDOW
#   define OEMRESOURCE
#   define NOATOM
#   define NOCLIPBOARD
#   define NOCOLOR
#   define NOCTLMGR
#   define NODRAWTEXT
#   define NOGDI
#   define NOKERNEL
#   define NOUSER
#   define NONLS
#   define NOMB
#   define NOMEMMGR
#   define NOMETAFILE
#   define NOMINMAX
#   define NOMSG
#   define NOOPENFILE
#   define NOSCROLL
#   define NOSERVICE
#   define NOSOUND
#   define NOTEXTMETRIC
#   define NOWH
#   define NOWINOFFSETS
#   define NOCOMM
#   define NOKANJI
#   define NOHELP
#   define NOPROFILER
#   define NODEFERWINDOWPOS
#   define NOMCX
#   include <Windows.h>
#elif defined(__linux__)
#   include <unistd.h>
#endif

#endif


#ifdef __cplusplus
extern "C" {
#endif

/* ----- Helper macros ----- */

#define XVM_MIN(a, b) ((a) < (b) ? (a) : (b))
#define XVM_MAX(a, b) ((a) > (b) ? (a) : (b))

#ifdef _ENABLE_INLINEING_
#   define INLINE __inline
#else
#   define INLINE // No inlining
#endif


/* ----- Helper functions ----- */

INLINE static int xvm_flt2int_signum(float val)
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
    REG_R0 = 0x00, // $r0  ->  General purpose register 0.
    REG_R1 = 0x01, // $r1  ->  General purpose register 1.
    REG_R2 = 0x02, // $r2  ->  General purpose register 2.
    REG_R3 = 0x03, // $r3  ->  General purpose register 3.
    REG_R4 = 0x04, // $r4  ->  General purpose register 4.
    REG_R5 = 0x05, // $r5  ->  General purpose register 5.
    REG_R6 = 0x06, // $r6  ->  General purpose register 6.
    REG_R7 = 0x07, // $r7  ->  General purpose register 7.
    REG_R8 = 0x08, // $r8  ->  General purpose register 8.
    REG_R9 = 0x09, // $r9  ->  General purpose register 9.

    /* --- Special purpose registers --- */
    REG_OP = 0x0a, // $op  ->  Object pointer: "this" POINTER to the current object in a member function.
    REG_GP = 0x0b, // $gp  ->  Global pointer: POINTER to the global variables in the stack.

    /* --- Register for internal use --- */
    REG_CF = 0x0c, // $cf  ->  Conditional flags: used for jump conditions.
    REG_LB = 0x0d, // $lb  ->  Base pointer: POINTER to the base of the current stack frame.
    REG_SP = 0x0e, // $sp  ->  Stack pointer: POINTER to the top of the stack storage.
    REG_PC = 0x0f, // $pc  ->  Program counter: POINTER to the current instruction in the byte-code.
}
register_id;

/**
Returns the name of the specified register or an empty string if the ID is invalid.
\see register_id
*/
static const char* xvm_register_get_name(register_id reg)
{
    switch (reg)
    {
        case REG_R0: return "$r0";
        case REG_R1: return "$r1";
        case REG_R2: return "$r2";
        case REG_R3: return "$r3";
        case REG_R4: return "$r4";
        case REG_R5: return "$r5";
        case REG_R6: return "$r6";
        case REG_R7: return "$r7";
        case REG_R8: return "$r8";
        case REG_R9: return "$r9";

        case REG_OP: return "$op";
        case REG_GP: return "$gp";

        case REG_CF: return "$cf";
        case REG_LB: return "$lb";
        case REG_SP: return "$sp";
        case REG_PC: return "$pc";
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
|                                                  Load Address Instruction Opcodes (1110..):                                                   |
-------------------------------------------------------------------------------------------------------------------------------------------------
| Mnemonic | Opcode      | Reg.    | Offset                                      | Description                                                  |
-------------------------------------------------------------------------------------------------------------------------------------------------
|          | 31.......26 | 25...22 | 21........................................0 |                                                              |
-------------------------------------------------------------------------------------------------------------------------------------------------
| LDA      | 1 1 1 0 0 0 | D D D D | O O O O O O O O O O O O O O O O O O O O O O | Load address from program pointer (word aligned) + offset.   |
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
    OPCODE_LDA      = GEN_OPCODE(0x38), // LDA reg0, addr  ->  *reg0 = programMemoryStartWorldAligned[addr]
    //Reserved      = GEN_OPCODE(0x39),
    //Reserved      = GEN_OPCODE(0x3a),
    //Reserved      = GEN_OPCODE(0x3b),
}
opcode_mem;

typedef enum
{
    OPCODE_LDB      = GEN_OPCODE(0x3c), // LDB reg0, (reg1) c  ->  *reg0 = dynamicMemoryByteAligned[*reg1 + c]
    OPCODE_STB      = GEN_OPCODE(0x3d), // STB reg0, (reg1) c  ->  dynamicMemoryByteAligned[*reg1 + c] = *reg0
    OPCODE_LDW      = GEN_OPCODE(0x3e), // LDW reg0, (reg1) c  ->  *reg0 = dynamicMemoryWordAligned[*reg1 + c]
    OPCODE_STW      = GEN_OPCODE(0x3f), // STW reg0, (reg1) c  ->  dynamicMemoryWordAligned[*reg1 + c] = *reg0
}
opcode_memoff;


/* ----- Virtual machine exit codes ----- */

typedef enum
{
    EXITCODE_SUCCESS                =  0,
    EXITCODE_INVALID_BYTECODE       = -1,
    EXITCODE_INVALID_STACK          = -2,
    EXITCODE_INVALID_OPCODE         = -3,
    EXITCODE_INVALID_INTRINSIC      = -4,
    EXITCODE_STACK_OVERFLOW         = -5,
    EXITCODE_STACK_UNDERFLOW        = -6,
    EXITCODE_DIVISION_BY_ZERO       = -7,
    EXITCODE_UNKNOWN_ENTRY_POINT    = -8,
    //EXITCODE_MEMORY_VIOLATION       = -9,
}
xvm_exit_codes;

static const char* xvm_exitcode_to_string(const xvm_exit_codes exit_code)
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
        case EXITCODE_UNKNOWN_ENTRY_POINT:
            return "unknown entry point";
    }
    return NULL;
}


/* ----- Debug log ----- */

INLINE static void xvm_log_print(const char* str)
{
    printf(str);
}

INLINE static void xvm_log_println(const char* str)
{
    printf(str);
    printf("\n");
}

INLINE static void xvm_log_error(const char* str)
{
    printf("error: %s\n", str);
}

INLINE static void xvm_log_readfile_error(const char* filename)
{
    printf("error: reading file \"%s\" failed\n", filename);
}

INLINE static void xvm_log_exitcode_error(const xvm_exit_codes exit_code)
{
    const char* err = xvm_exitcode_to_string(exit_code);
    if (err != NULL)
        printf("error: program terminated with error: \"%s\"\n", err);
}


/* ----- File helper ----- */

static unsigned int xvm_file_read_uint(FILE* file)
{
    unsigned int value = 0;
    if (file != NULL)
        fread(&value, sizeof(value), 1, file);
    return value;
}

static void xvm_file_write_uint(FILE* file, unsigned int value)
{
    if (file != NULL)
        fwrite(&value, sizeof(value), 1, file);
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
    INTR_COPY_MEM       = 0x001fff02, // void CopyMem(void* dstMemAddr, const void* srcMemAddr, uint sizeInBytes).
    
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
    INTR_CMP_E          = 0x001fff30, //!< int CmpE(int x, int y)       -> x == y
    INTR_CMP_NE         = 0x001fff31, //!< int CmpNE(int x, int y)      -> x != y
    INTR_CMP_L          = 0x001fff32, //!< int CmpL(int x, int y)       -> x < y
    INTR_CMP_LE         = 0x001fff33, //!< int CmpLE(int x, int y)      -> x <= y
    INTR_CMP_G          = 0x001fff34, //!< int CmpG(int x, int y)       -> x > y
    INTR_CMP_GE         = 0x001fff35, //!< int CmpGE(int x, int y)      -> x >= y
    INTR_LOGIC_OR       = 0x001fff36, //!< int LogicOr(int x, int y)    -> x != 0 || y != 0
    INTR_LOGIC_AND      = 0x001fff37, //!< int LogicAnd(int x, int y)   -> x != 0 && y != 0
    INTR_LOGIC_NOT      = 0x001fff38, //!< int LogicNot(int x)          -> x == 0

    /* --- File intrinsics --- */
    INTR_CREATE_FILE    = 0x001fff40, // int CreateFile(const byte* stringAddress).
    INTR_DELETE_FILE    = 0x001fff41, // int DeleteFile(const byte* stringAddress).
    INTR_OPEN_FILE      = 0x001fff42, // void* OpenFile(const byte* stringAddress).
    INTR_CLOSE_FILE     = 0x001fff43, // void CloseFile(void* fileHandle).
    INTR_FILE_SIZE      = 0x001fff44, // int FileSize(const void* fileHandle).
    INTR_SET_FILE_POS   = 0x001fff45, // void FileSetPos(const void* fileHandle, int pos).
    INTR_GET_FILE_POS   = 0x001fff46, // int FileGetPos(const void* fileHandle).
    INTR_FILE_EOF       = 0x001fff47, // int FileEOF(const void* fileHandle).
    INTR_WRITE_BYTE     = 0x001fff48, // void WriteByte(const void* fileHandle, const void* memoryAddress).
    INTR_WRITE_WORD     = 0x001fff49, // void WriteWord(const void* fileHandle, const void* memoryAddress).
    INTR_READ_BYTE      = 0x001fff4a, // void ReadByte(const void* fileHandle, void* memoryAddress).
    INTR_READ_WORD      = 0x001fff4b, // void ReadWord(const void* fileHandle, void* memoryAddress).

    /* --- Math intrinsics --- */
    INTR_SIN            = 0x001fff80, // float Sin(float x).
    INTR_COS            = 0x001fff81, // float Cos(float x).
    INTR_TAN            = 0x001fff82, // float Tan(float x).
    INTR_ASIN           = 0x001fff83, // float ASin(float x).
    INTR_ACOS           = 0x001fff84, // float ACos(float x).
    INTR_ATAN           = 0x001fff85, // float ATan(float x).
    INTR_POW            = 0x001fff86, // float Pow(float base, float exp).
    INTR_SQRT           = 0x001fff87, // float Sqrt(float x).

    /* --- Other intrinsics --- */
    INTR_RAND_INT       = 0x001fff88, // int RandInt() -> In range [0 .. MAX_INT].
    INTR_RAND_FLOAT     = 0x001fff89, // float RandFloat() -> In range [0.0 .. 1.0].
    INTR_TIME           = 0x001fff8a, // int Time() -> Ellapsed time since program start (in ms.).
    INTR_SLEEP          = 0x001fff8b, // void Sleep(int duration).
}
intrinsic_addr;

//! Returns the identifier of the specified intrinsic.
static const char* xvm_intrinsic_get_ident(const intrinsic_addr addr)
{
    switch (addr)
    {
        /* --- Dynamic memory intrinsics --- */
        
        case INTR_ALLOC_MEM:    return "AllocMem";
        case INTR_FREE_MEM:     return "FreeMem";
        case INTR_COPY_MEM:     return "CopyMem";
    
        /* --- Console intrinsics --- */

        case INTR_SYS_CALL:     return "SysCall";
        case INTR_CLEAR:        return "Clear";
        case INTR_PRINT:        return "Print";
        case INTR_PRINT_LN:     return "PrintLn";
        case INTR_PRINT_INT:    return "PrintInt";
        case INTR_PRINT_FLOAT:  return "PrintFloat";
        case INTR_INPUT_INT:    return "InputInt";
        case INTR_INPUT_FLOAT:  return "InputFloat";

        /* --- Conditional intrinsics --- */

        case INTR_CMP_E:        return "CmpE";
        case INTR_CMP_NE:       return "CmpNE";
        case INTR_CMP_L:        return "CmpL";
        case INTR_CMP_LE:       return "CmpLE";
        case INTR_CMP_G:        return "CmpG";
        case INTR_CMP_GE:       return "CmpGE";
        case INTR_LOGIC_OR:     return "LogicOr";
        case INTR_LOGIC_AND:    return "LogicAnd";
        case INTR_LOGIC_NOT:    return "LogicNot";

        /* --- File intrinsics --- */

        case INTR_CREATE_FILE:  return "CreateFile";
        case INTR_DELETE_FILE:  return "DeleteFile";
        case INTR_OPEN_FILE:    return "OpenFile";
        case INTR_CLOSE_FILE:   return "CloseFile";
        case INTR_FILE_SIZE:    return "FileSize";
        case INTR_SET_FILE_POS: return "SetFilePos";
        case INTR_GET_FILE_POS: return "GetFilePos";
        case INTR_FILE_EOF:     return "FileEOF";
        case INTR_WRITE_BYTE:   return "WriteByte";
        case INTR_WRITE_WORD:   return "WriteWord";
        case INTR_READ_BYTE:    return "ReadByte";
        case INTR_READ_WORD:    return "ReadWord";

        /* --- Math intrinsics --- */

        case INTR_SIN:          return "Sin";
        case INTR_COS:          return "Cos";
        case INTR_TAN:          return "Tan";
        case INTR_ASIN:         return "ASin";
        case INTR_ACOS:         return "ACos";
        case INTR_ATAN:         return "ATan";
        case INTR_POW:          return "Pow";
        case INTR_SQRT:         return "Sqrt";

        /* --- Other intrinsics --- */

        case INTR_RAND_INT:     return "RandInt";
        case INTR_RAND_FLOAT:   return "RandFloat";
        case INTR_TIME:         return "Time";
        case INTR_SLEEP:        return "Sleep";
    }
    return "";
}


/* ----- Instruction ----- */

typedef unsigned int instr_t;

INLINE static opcode_t xvm_instr_get_opcode(const instr_t instr)
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

INLINE static unsigned int xvm_instr_get_value26(const instr_t instr)
{
    return (instr & 0x03ffffff);
}

INLINE static unsigned int xvm_instr_get_value22(const instr_t instr)
{
    return (instr & 0x003fffff);
}

INLINE static unsigned int xvm_instr_get_value18(const instr_t instr)
{
    return (instr & 0x0003ffff);
}

INLINE static int xvm_instr_get_sgn_value26(const instr_t instr)
{
    unsigned int val = xvm_instr_get_value26(instr);

    if ((val & 0x02000000) != 0)
        val |= 0xfc000000;

    return (int)val;
}

INLINE static int xvm_instr_get_sgn_value22(const instr_t instr)
{
    unsigned int val = xvm_instr_get_value22(instr);

    if ((val & 0x00200000) != 0)
        val |= 0xffc00000;

    return (int)val;
}

INLINE static int xvm_instr_get_sgn_value18(const instr_t instr)
{
    unsigned int val = xvm_instr_get_value18(instr);

    if ((val & 0x00020000) != 0)
        val |= 0xfffc0000;

    return (int)val;
}

INLINE static int xvm_instr_get_extra_value8(const instr_t instr)
{
    return (instr & 0x03fc0000) >> 18;
}

INLINE static reg_t xvm_instr_get_reg0(const instr_t instr)
{
    return (instr & 0x03c00000) >> 22;
}

INLINE static reg_t xvm_instr_get_reg1(const instr_t instr)
{
    return (instr & 0x003c0000) >> 18;
}

/**
Returns the mnemonic of the specified instruction opcode or an empty string if the opcode is invalid.
If the opcode is valid, the returned string will always consist of 4 characters plus the null terminator '\0'.
*/
static const char* xvm_instr_get_mnemonic(const opcode_t opcode)
{
    switch (opcode)
    {
        /* --- opcode_reg2 --- */

        case OPCODE_MOV1:
        case OPCODE_MOV2:   return "mov ";
        case OPCODE_NOT2:   return "not ";
        case OPCODE_AND1:
        case OPCODE_AND2:   return "and ";
        case OPCODE_OR1:
        case OPCODE_OR2:    return "or  ";
        case OPCODE_XOR1:
        case OPCODE_XOR2:   return "xor ";
        case OPCODE_ADD1:
        case OPCODE_ADD2:   return "add ";
        case OPCODE_SUB1:
        case OPCODE_SUB2:   return "sub ";
        case OPCODE_MUL1:
        case OPCODE_MUL2:   return "mul ";
        case OPCODE_DIV1:
        case OPCODE_DIV2:   return "div ";
        case OPCODE_MOD1:
        case OPCODE_MOD2:   return "mod ";
        case OPCODE_SLL1:
        case OPCODE_SLL2:   return "sll ";
        case OPCODE_SLR1:
        case OPCODE_SLR2:   return "slr ";
        case OPCODE_CMP:    return "cmp ";
        case OPCODE_FTI:    return "fti ";
        case OPCODE_ITF:    return "itf ";
            
        /* --- opcode_reg1 --- */

        case OPCODE_PUSHC:
        case OPCODE_PUSH:   return "push";
        case OPCODE_POP:    return "pop ";
        case OPCODE_INC:    return "inc ";
        case OPCODE_DEC:    return "dec ";

        /* --- opcode_jump --- */

        case OPCODE_JMP:    return "jmp ";
        case OPCODE_JE:     return "je  ";
        case OPCODE_JNE:    return "jne ";
        case OPCODE_JG:     return "jg  ";
        case OPCODE_JL:     return "jl  ";
        case OPCODE_JGE:    return "jge ";
        case OPCODE_JLE:    return "jle ";
        case OPCODE_CALL:   return "call";

        /* --- opcode_float --- */

        case OPCODE_ADDF:   return "addf";
        case OPCODE_SUBF:   return "subf";
        case OPCODE_MULF:   return "mulf";
        case OPCODE_DIVF:   return "divf";
        case OPCODE_CMPF:   return "cmpf";

        /* --- opcode_mem --- */

        case OPCODE_LDA:    return "lda ";

        /* --- opcode_memoff --- */

        case OPCODE_LDB:    return "ldb ";
        case OPCODE_STB:    return "stb ";
        case OPCODE_LDW:    return "ldw ";
        case OPCODE_STW:    return "stw ";

        /* --- opcode_special --- */

        case OPCODE_STOP:   return "stop";
        case OPCODE_RET:    return "ret ";
        case OPCODE_INVK:   return "invk";
    }
    return "";
}

/**
Prints debug information for the specified instruction
with the current state of the specified register set.
*/
static void xvm_instr_print_debug_info(const instr_t instr, regi_t instr_index, const regi_t* reg_ptr)
{
    const opcode_t opcode = xvm_instr_get_opcode(instr);

    #ifdef _SHOW_RUNTIME_HEXLINES_
    printf("0x%*.8x  %s", 8, (instr_index >> 2), xvm_instr_get_mnemonic(opcode));
    #else
    printf("%*.8i  %s  ", 8, (instr_index >> 2), xvm_instr_get_mnemonic(opcode));
    #endif

    if (opcode == OPCODE_CMP || opcode == OPCODE_CMPF)
    {
        reg_t reg0 = xvm_instr_get_reg0(instr);
        reg_t reg1 = xvm_instr_get_reg1(instr);

        const char* reg0name = xvm_register_get_name((register_id)reg0);
        const char* reg1name = xvm_register_get_name((register_id)reg1);

        printf("%s, %s    ($cf = %i)", reg0name, reg1name, reg_ptr[REG_CF]);
    }
    else if (opcode >= OPCODE_JMP && opcode <= OPCODE_JLE)
    {
        int addr_offset = xvm_instr_get_sgn_value22(instr);
        printf("%i", addr_offset);
    }
    else if (opcode == OPCODE_CALL)
    {
        int addr_offset = xvm_instr_get_sgn_value22(instr);
        if (addr_offset >= INTR_RESERVED)
            printf("%s  <intrinsic>", xvm_intrinsic_get_ident((intrinsic_addr)addr_offset));
        else
            printf("%i", addr_offset);
    }
    else if (opcode >= OPCODE_MOV2 && opcode <= OPCODE_ITF)
    {
        reg_t reg0 = xvm_instr_get_reg0(instr);
        reg_t reg1 = xvm_instr_get_reg1(instr);

        const char* reg0name = xvm_register_get_name((register_id)reg0);
        const char* reg1name = xvm_register_get_name((register_id)reg1);

        regi_t value = reg_ptr[reg0];

        printf("%s, %s    (%s = %i)", reg0name, reg1name, reg0name, value);
    }
    else if ( ( opcode >= OPCODE_MOV1 && opcode <= OPCODE_DEC  ) ||
              ( opcode >= OPCODE_LDB && opcode <= OPCODE_STW ) )
    {
        reg_t reg0 = xvm_instr_get_reg0(instr);
        regi_t value = reg_ptr[reg0];

        const char* reg0name = xvm_register_get_name((register_id)reg0);

        printf("%s         (%s = %i)", reg0name, reg0name, value);
    }
    else if (opcode >= OPCODE_ADDF && opcode <= OPCODE_DIVF)
    {
        reg_t reg0 = xvm_instr_get_reg0(instr);
        regi_t value = reg_ptr[reg0];

        const char* reg0name = xvm_register_get_name((register_id)reg0);

        printf("%s         (%s = %f)", reg0name, reg0name, INT_TO_FLT_REINTERPRET(value));
    }
    else if (opcode == OPCODE_PUSHC)
        printf("%i", xvm_instr_get_sgn_value26(instr));

    printf("\n");
}


/* ----- Instruction constructors ----- */

static instr_t xvm_instr_make_reg2(opcode_reg2 opcode, reg_t reg0, reg_t reg1)
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

static instr_t xvm_instr_make_reg1(opcode_reg1 opcode, reg_t reg, unsigned int value)
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

static instr_t xvm_instr_make_jump(opcode_jump opcode, reg_t reg, unsigned int offset)
{
    return xvm_instr_make_reg1((opcode_reg1)opcode, reg, offset);
}

static instr_t xvm_instr_make_float(opcode_float opcode, reg_t reg0, reg_t reg1)
{
    return xvm_instr_make_reg2((opcode_reg2)opcode, reg0, reg1);
}

static instr_t xvm_instr_make_mem(opcode_mem opcode, reg_t reg, unsigned int address)
{
    return xvm_instr_make_reg1((opcode_reg1)opcode, reg, address);
}

static instr_t xvm_instr_make_memoff(opcode_memoff opcode, reg_t reg0, reg_t reg1, unsigned int offset)
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

static instr_t xvm_instr_make_special1(opcode_special opcode, unsigned int value)
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

static instr_t xvm_instr_make_special2(opcode_special opcode, unsigned int result_size, unsigned int arg_size)
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


/* ----- Strings ----- */

//! XVM string structure.
typedef struct
{
    size_t  len;
    char*   str;
}
xvm_string;

//! Returns an empty string object
static xvm_string xvm_string_init()
{
    xvm_string string;
    string.len = 0;
    string.str = NULL;
    return string;
}

//! Creates a new string object with the specified length (plus the null terminator '\0').
static xvm_string xvm_string_create(size_t len)
{
    xvm_string string;

    // Allocate memory for the string and store string length
    string.len = len;
    string.str = (char*)malloc(sizeof(char)*(len + 1));

    // Initialize last string entry with the null character
    string.str[len] = '\0';

    return string;
}

//! Creates a string from the specified string literal.
static xvm_string xvm_string_create_from(const char* str)
{
    // Create string and copy data
    size_t len = strlen(str);
    xvm_string string = xvm_string_create(len);
    strcpy(string.str, str);
    return string;
}

//! Frees the memory of the specified string.
static int xvm_string_free(xvm_string* string)
{
    if (string != NULL && string->str != NULL)
    {
        // Free memory and reset data
        free(string->str);
        string->len = 0;
        string->str = NULL;
        return 1;
    }
    return 0;
}

//! Reads a string from the specified file
static xvm_string xvm_string_read_from_file(FILE* file)
{
    if (file != NULL)
    {
        // Read length and string data from file
        size_t len = (size_t)xvm_file_read_uint(file);
        xvm_string string = xvm_string_create(len);
        fread(string.str, sizeof(char), string.len, file);
        return string;
    }
    return xvm_string_init();
}

//! Reads a string from the specified file
static int xvm_string_write_to_file(xvm_string string, FILE* file)
{
    if (file != NULL && string.str != NULL)
    {
        // Write length and string data to file
        xvm_file_write_uint(file, string.len);
        fwrite(string.str, sizeof(char), string.len, file);
        return 1;
    }
    return 0;
}


/* ----- Byte code ----- */

//! XVM export procedure address structure.
typedef struct
{
    /**
    Export procedure address. This is the instruction
    INDEX where the procedure starts. By default 0.
    */
    unsigned int addr;
    
    /**
    Procedure ID name. This depends on the 'name mangling' of
    the high-level compiler (e.g. "main" or "_ZN@main"). By default empty.
    */
    xvm_string name;
}
xvm_export_address;

//! XVM byte code structure.
typedef struct
{
    int                 num_instructions;       //!< Number of instructions. By default 0.
    instr_t*            instructions;           //!< Instruction array. By default NULL.
    unsigned int        num_export_addresses;   //!< Number of export addresses. By default 0.
    xvm_export_address* export_addresses;       //!< Export addresses array. By default NULL.
}
xvm_bytecode;

//! Initializes the export address with its default values.
static int xvm_export_address_init(xvm_export_address* export_address)
{
    if (export_address != NULL)
    {
        export_address->addr = 0;
        export_address->name = xvm_string_init();
        return 1;
    }
    return 0;
}

//! Initializes the export address with the specified startup values.
static int xvm_export_address_setup(xvm_export_address* export_address, unsigned int addr, xvm_string name)
{
    if (export_address != NULL)
    {
        export_address->addr = addr;
        export_address->name = name;
        return 1;
    }
    return 0;
}

/**
Initializes the specified byte code structure.
\code
xvm_bytecode byte_code;
xvm_bytecode_init(&byte_code);
xvm_bytecode_create_instructions(&byte_code, num_instructions);
xvm_bytecode_create_export_addresses(&byte_code, num_export_addresses);
// ...
xvm_bytecode_free(&byte_code);
\endcode
*/
static int xvm_bytecode_init(xvm_bytecode* byte_code)
{
    if (byte_code != NULL)
    {
        // Initialize byte code data
        byte_code->num_instructions     = 0;
        byte_code->instructions         = NULL;
        byte_code->num_export_addresses = 0;
        byte_code->export_addresses     = NULL;
        return 1;
    }
    return 0;
}

/**
Allocates memory for the specified amount of byte code instructions.
\param[in,out] byte_code Pointer to the byte code object.
\param[in] num_instructions Specifies the number of instruction to allocate for the byte code.
\see xvm_bytecode_init
\note All instructions be uninitialized!
*/
static int xvm_bytecode_create_instructions(xvm_bytecode* byte_code, int num_instructions)
{
    if (byte_code != NULL && byte_code->instructions == NULL && num_instructions > 0)
    {
        byte_code->num_instructions = num_instructions;
        byte_code->instructions     = (instr_t*)malloc(sizeof(instr_t)*num_instructions);
        return 1;
    }
    return 0;
}

/**
Allocates memory for the specified amount of byte code export addresses.
\param[in,out] byte_code Pointer to the byte code object.
\param[in] num_instructions Specifies the number of instruction to allocate for the byte code.
\see xvm_bytecode_init
\note All export addresses be uninitialized!
*/
static int xvm_bytecode_create_export_addresses(xvm_bytecode* byte_code, unsigned int num_export_addresses)
{
    if (byte_code != NULL && byte_code->export_addresses == NULL && num_export_addresses > 0)
    {
        byte_code->num_export_addresses = num_export_addresses;
        byte_code->export_addresses     = (xvm_export_address*)malloc(sizeof(xvm_export_address)*num_export_addresses);
        return 1;
    }
    return 0;
}

//! Frees the memory for the specified byte code object.
static int xvm_bytecode_free(xvm_bytecode* byte_code)
{
    if (byte_code != NULL)
    {
        if (byte_code->instructions != NULL)
        {
            // Free instruction list
            free(byte_code->instructions);
            byte_code->instructions     = NULL;
            byte_code->num_instructions = 0;
        }
        if (byte_code->export_addresses != NULL)
        {
            // Free string of each export address
            for (unsigned int i = 0; i < byte_code->num_export_addresses; ++i)
            {
                xvm_export_address* export_addr = &(byte_code->export_addresses[i]);
                xvm_string_free(&(export_addr->name));
            }

            // Free export address list
            free(byte_code->export_addresses);
            byte_code->export_addresses     = NULL;
            byte_code->num_export_addresses = 0;
        }
        return 1;
    }
    return 0;
}

/**
Inserts an ASCII data field into the byte code instructions.
In XASM this could look like this:
\code
str0: .ascii "Hello, World!\n"
\endcode
\param[in,out] byte_code Pointer to the byte code whose instructions will be filled with the data field.
\param[in] instr_offset Index offset of the first instruction where the data field is to be inserted.
\param[in] text Constant pointer to the string which is to be filled into the data field.
\param[out] num_instructions Optional pointer to the number of instruction which are used for this data field.
This may also be NULL.
\return Non zero on success otherwise 0 when the byte code is NULL or it has not enough instructions or 'text' is NULL.
If the byte code has not enough instruction, the output parameter 'num_instructions' will still
contain the number of instruction which are required to fill the data field.
\remarks This function can also be used to only determine the number of required instructions for the data field.
This could look like this:
\code
size_t num_instr = 0;
xvm_bytecode_datafield_ascii(NULL, 0, "Hello, World\n", &num_instr);
// num_instr == 4 { "Hell", "o, W", "orld", "\n\0\0\0" }
\endcode
*/
static int xvm_bytecode_datafield_ascii(
    xvm_bytecode* byte_code, size_t instr_offset, const char* text, size_t* num_instructions)
{
    instr_t* instr;
    char* instr_byte;
    size_t num_instr, remainder;

    if (text == NULL)
        return 0;

    // Determine number of required instructions
    num_instr = strlen(text);
    remainder = num_instr % 4;
    num_instr = (num_instr + (4 - remainder)) / 4;

    if (num_instructions != NULL)
        *num_instructions = num_instr;

    if (byte_code == NULL || byte_code->instructions == NULL || instr_offset + num_instr >= (size_t)(byte_code->num_instructions))
        return 0;

    // Fill data field
    instr = (byte_code->instructions + instr_offset);

    for (; num_instr > 1; --num_instr)
    {
        *instr = *((instr_t*)text);
        ++instr;
        text += 4;
    }

    // Fill last data field
    *instr = 0;
    instr_byte = (char*)instr;

    for (; remainder > 0; --remainder)
    {
        *instr_byte = *text;
        ++instr_byte;
        ++text;
    }

    return 1;
}

/*

WORD: 32-bit unsigned integer
STR:
    WORD:           length
    Byte[length]:   data

--- XBC file format spec (Version 1.01): ---

WORD: magic number (Must be *(int*)"XBCF")
WORD: version number (Must be 101 for "1.01")
WORD: number of instructions (n)
n times:
    WORD: instruction

--- XBC file format spec (Version 1.02): ---

WORD: magic number (Must be *(int*)"XBCF")
WORD: version number (Must be 102 for "1.02")
WORD: number of instructions (n)
n times:
    WORD: instruction
WORD: number of export addresses (m)
m times:
    WORD: address
    STR: name

*/

#define XBC_FORMAT_MAGIC        (*((int*)("XBCF")))
#define XBC_FORMAT_VERSION_1_01 101
#define XBC_FORMAT_VERSION_1_02 102

static int xvm_bytecode_read_from_file(xvm_bytecode* byte_code, const char* filename)
{
    // Check arguments
    if (byte_code == NULL || filename == NULL)
    {
        xvm_log_error("invalid arguments to read byte code");
        return 0;
    }

    // Open file for reading
    FILE* file = fopen(filename, "rb");
    if (file == NULL)
    {
        xvm_log_error("unable to open file for reading");
        return 0;
    }

    // Read magic number
    unsigned int magic = xvm_file_read_uint(file);
    if (magic != XBC_FORMAT_MAGIC)
    {
        xvm_log_error("invalid magic number (must be \"XBCF\")");
        fclose(file);
        return 0;
    }

    // Read version number
    unsigned int version = xvm_file_read_uint(file);
    if (version != XBC_FORMAT_VERSION_1_01 && version != XBC_FORMAT_VERSION_1_02)
    {
        xvm_log_error("invalid version number (must be 1.01 or 1.02)");
        fclose(file);
        return 0;
    }

    // Read instructions
    unsigned int num_instr = xvm_file_read_uint(file);

    if (xvm_bytecode_create_instructions(byte_code, (int)num_instr) == 0)
    {
        xvm_log_error("creating byte code instructions failed");
        fclose(file);
        return 0;
    }

    fread(byte_code->instructions, sizeof(instr_t), num_instr, file);

    // Read export addresses
    if (version >= XBC_FORMAT_VERSION_1_02)
    {
        unsigned int num_export_addr = xvm_file_read_uint(file);

        if (num_export_addr > 0)
        {
            if (xvm_bytecode_create_export_addresses(byte_code, num_export_addr) == 0)
            {
                xvm_log_error("creating byte code export addresses failed");
                fclose(file);
                return 0;
            }

            for (unsigned int i = 0; i < num_export_addr; ++i)
            {
                // Read address and name and store it into the export address
                xvm_export_address* export_addr = &(byte_code->export_addresses[i]);

                unsigned int addr = xvm_file_read_uint(file);
                xvm_string string = xvm_string_read_from_file(file);

                xvm_export_address_setup(export_addr, addr, string);
            }
        }
    }

    // Close file and return with success
    fclose(file);

    return 1;
}

static int xvm_bytecode_write_to_file(const xvm_bytecode* byte_code, const char* filename)
{
    // Check arguments
    if (byte_code == NULL || byte_code->num_instructions <= 0 || byte_code->instructions == NULL || filename == NULL)
    {
        xvm_log_error("Invalid arguments to write byte code");
        return 0;
    }

    // Open file for writing
    FILE* file = fopen(filename, "wb");
    if (file == NULL)
    {
        xvm_log_error("Unable to open file for writing");
        return 0;
    }

    // Write magic number
    xvm_file_write_uint(file, XBC_FORMAT_MAGIC);

    // Write version number
    const int has_export_addr = (byte_code->export_addresses != NULL && byte_code->num_export_addresses > 0 ? 1 : 0);

    xvm_file_write_uint(
        file,
        has_export_addr != 0 ?
            XBC_FORMAT_VERSION_1_02 :
            XBC_FORMAT_VERSION_1_01
    );

    // Write instructions
    unsigned int num_instr = (unsigned int)byte_code->num_instructions;
    xvm_file_write_uint(file, num_instr);

    fwrite(byte_code->instructions, sizeof(instr_t), num_instr, file);

    // Write export addresses
    if (has_export_addr != 0)
    {
        unsigned int num_export_addr = byte_code->num_export_addresses;
        xvm_file_write_uint(file, num_export_addr);

        for (unsigned int i = 0; i < num_export_addr; ++i)
        {
            xvm_export_address* export_addr = &(byte_code->export_addresses[i]);

            xvm_file_write_uint(file, export_addr->addr);
            xvm_string_write_to_file(export_addr->name, file);
        }
    }

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
        xvm_exception_throw("stack overflow", EXITCODE_STACK_OVERFLOW);
}

INLINE static stack_word_t xvm_stack_pop(xvm_stack* stack, regi_t* reg_sp)
{
    stack_word_t* stack_ptr = REG_TO_STACK_PTR(reg_sp);
    if (stack_ptr > stack->storage)
        (*reg_sp) -= sizeof(stack_word_t);
    else
        xvm_exception_throw("stack underflow", EXITCODE_STACK_OVERFLOW);
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


/* ----- Virtual machine ----- */

static void xvm_call_intrinsic(int intrinsic_addr, xvm_stack* const stack, regi_t* reg_sp)
{
    switch (intrinsic_addr)
    {
        /* --- Dynamic memory intrinsics --- */

        // void* AllocMem(uint sizeInBytes)
        case INTR_ALLOC_MEM:
        {
            size_t arg0 = (size_t)(xvm_stack_read(*reg_sp, -1));
            void* result = malloc(arg0);
            xvm_stack_write(*reg_sp, -1, (stack_word_t)result);
        }
        break;

        // void FreeMem(void* memoryAddress)
        case INTR_FREE_MEM:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            if (arg0 != 0)
                free((void*)arg0);
        }
        break;

        // void CopyMem(void* dstMemAddr, const void* srcMemAddr, uint sizeInBytes)
        case INTR_COPY_MEM:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            int arg1 = xvm_stack_pop(stack, reg_sp);
            int arg2 = xvm_stack_pop(stack, reg_sp);
            memcpy((void*)arg0, (const void*)arg1, (size_t)arg2);
        }
        break;

        /* --- Console intrinsics --- */

        case INTR_SYS_CALL:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            system(INT_TO_STR_REINTERPRET(arg0));
        }
        break;

        case INTR_CLEAR:
        {
            //todo...
        }
        break;

        case INTR_PRINT:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            printf("%s", INT_TO_STR_REINTERPRET(arg0));
        }
        break;

        case INTR_PRINT_LN:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            printf("%s\n", INT_TO_STR_REINTERPRET(arg0));
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
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            int arg1 = xvm_stack_pop(stack, reg_sp);
            int result = (arg0 == arg1 ? 1 : 0);
            xvm_stack_push(stack, reg_sp, result);
        }
        break;

        case INTR_CMP_NE:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            int arg1 = xvm_stack_pop(stack, reg_sp);
            int result = (arg0 != arg1 ? 1 : 0);
            xvm_stack_push(stack, reg_sp, result);
        }
        break;

        case INTR_CMP_L:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            int arg1 = xvm_stack_pop(stack, reg_sp);
            int result = (arg0 < arg1 ? 1 : 0);
            xvm_stack_push(stack, reg_sp, result);
        }
        break;

        case INTR_CMP_LE:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            int arg1 = xvm_stack_pop(stack, reg_sp);
            int result = (arg0 <= arg1 ? 1 : 0);
            xvm_stack_push(stack, reg_sp, result);
        }
        break;

        case INTR_CMP_G:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            int arg1 = xvm_stack_pop(stack, reg_sp);
            int result = (arg0 > arg1 ? 1 : 0);
            xvm_stack_push(stack, reg_sp, result);
        }
        break;

        case INTR_CMP_GE:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            int arg1 = xvm_stack_pop(stack, reg_sp);
            int result = (arg0 >= arg1 ? 1 : 0);
            xvm_stack_push(stack, reg_sp, result);
        }
        break;

        case INTR_LOGIC_OR:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            int arg1 = xvm_stack_pop(stack, reg_sp);
            int result = (arg0 != 0 || arg1 != 0 ? 1 : 0);
            xvm_stack_push(stack, reg_sp, result);
        }
        break;

        case INTR_LOGIC_AND:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            int arg1 = xvm_stack_pop(stack, reg_sp);
            int result = (arg0 != 0 && arg1 != 0 ? 1 : 0);
            xvm_stack_push(stack, reg_sp, result);
        }
        break;

        case INTR_LOGIC_NOT:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            int result = (arg0 == 0 ? 1 : 0);
            xvm_stack_push(stack, reg_sp, result);
        }
        break;

        /* --- File intrinsics --- */

        case INTR_CREATE_FILE:
        case INTR_DELETE_FILE:
        case INTR_OPEN_FILE:
        case INTR_CLOSE_FILE:
        case INTR_FILE_SIZE:
        case INTR_SET_FILE_POS:
        case INTR_GET_FILE_POS:
        case INTR_FILE_EOF:
        case INTR_WRITE_BYTE:
        case INTR_WRITE_WORD:
        case INTR_READ_BYTE:
        case INTR_READ_WORD:
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

        /* --- Other intrinsics --- */
        
        case INTR_RAND_INT:
        {
            int result = rand();
            xvm_stack_push(stack, reg_sp, result);
        }
        break;

        case INTR_RAND_FLOAT:
        {
            float result = ((float)rand()) / RAND_MAX;
            xvm_stack_push(stack, reg_sp, FLT_TO_INT_REINTERPRET(result));
        }
        break;

        case INTR_TIME:
        {
            clock_t ticks = clock() / (CLOCKS_PER_SEC / 1000);
            xvm_stack_push(stack, reg_sp, (int)ticks);
        }
        break;

        #ifdef _ENABLE_OS_FEATURES_

        case INTR_SLEEP:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            #if defined(_WIN32)
            Sleep((DWORD)arg0);
            #elif defined(__linux__)
            usleep(((useconds_t)arg0) * 1000);
            #endif
        }
        break;

        #endif

        default:
            xvm_exception_throw("invalid intrinsic", EXITCODE_INVALID_INTRINSIC);
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
\param[in] entry_point Optional pointer to the entry point address.
This may also be NULL to start the program at the top.
\remarks This is the main function for the entire virtual machine.
All instructions are implemented inside this function and its large switch-case statement.
\see xvm_bytecode_create_instructions
\see xvm_stack_create
*/
static xvm_exit_codes xvm_execute_program_ext(
    const xvm_bytecode* const byte_code,
    xvm_stack* const stack,
    const xvm_export_address* entry_point)
{
    #define JUMP_ADDRESS(r, a) (reg.i[r] + ((a) << 2))

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
    *reg_pc = instr_ptr_begin;

    if (entry_point != NULL)
        *reg_pc += (entry_point->addr << 2);

    /* --- Catch exceptions --- */
    int exception_val = setjmp(xvm_exception_envbuf);
    if (exception_val != 0)
    {
        xvm_log_error(xvm_exception_err);
        return (xvm_exit_codes)exception_val;
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
            xvm_instr_print_debug_info(instr, prev_pc_index, reg.i);
        #endif

        /* Load next instruction */
        instr = *((instr_t*)(*reg_pc));

        opcode = xvm_instr_get_opcode(instr);

        #ifdef _ENABLE_RUNTIME_DEBUGGER_
        prev_pc_index = *reg_pc - instr_ptr_begin;
        #endif

        /* Execute current instruction */
        switch (opcode)
        {
            /* --- opcode_reg2 --- */

            case OPCODE_MOV2:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                reg.i[reg0] = reg.i[reg1];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_NOT2:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                reg.i[reg0] = ~reg.i[reg1];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_AND2:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                reg.i[reg0] &= reg.i[reg1];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_OR2:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                reg.i[reg0] |= reg.i[reg1];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_XOR2:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                reg.i[reg0] ^= reg.i[reg1];
            }
            break;

            case OPCODE_ADD2:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                reg.i[reg0] += reg.i[reg1];
            }
            break;

            case OPCODE_SUB2:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                reg.i[reg0] -= reg.i[reg1];
            }
            break;

            case OPCODE_MUL2:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                reg.i[reg0] *= reg.i[reg1];
            }
            break;

            case OPCODE_DIV2:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                reg.i[reg0] /= reg.i[reg1];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_MOD2:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                reg.i[reg0] %= reg.i[reg1];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_SLL2:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                reg.i[reg0] <<= reg.i[reg1];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_SLR2:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                reg.i[reg0] >>= reg.i[reg1];
            }
            break;

            case OPCODE_CMP:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                *reg_cf = (reg.i[reg0] - reg.i[reg1]);
            }
            break;

            case OPCODE_FTI:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                reg.i[reg0] = (regi_t)reg.f[reg1];
            }
            break;

            case OPCODE_ITF:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                reg.f[reg0] = (regf_t)reg.i[reg1];
            }
            break;
            
            /* --- opcode_reg1 --- */

            case OPCODE_MOV1:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg.i[reg0] = xvm_instr_get_sgn_value22(instr);
            }
            break;

            // Undefined behavior for floats
            case OPCODE_AND1:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg.i[reg0] &= xvm_instr_get_sgn_value22(instr);
            }
            break;

            // Undefined behavior for floats
            case OPCODE_OR1:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg.i[reg0] |= xvm_instr_get_sgn_value22(instr);
            }
            break;

            // Undefined behavior for floats
            case OPCODE_XOR1:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg.i[reg0] ^= xvm_instr_get_sgn_value22(instr);
            }
            break;

            // Undefined behavior for floats
            case OPCODE_ADD1:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg.i[reg0] += xvm_instr_get_sgn_value22(instr);
            }
            break;

            // Undefined behavior for floats
            case OPCODE_SUB1:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg.i[reg0] -= xvm_instr_get_sgn_value22(instr);
            }
            break;

            // Undefined behavior for floats
            case OPCODE_MUL1:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg.i[reg0] *= xvm_instr_get_sgn_value22(instr);
            }
            break;

            // Undefined behavior for floats
            case OPCODE_DIV1:
            {
                reg0 = xvm_instr_get_reg0(instr);
                sgn_value = xvm_instr_get_sgn_value22(instr);
                if (sgn_value == 0)
                    xvm_exception_throw("division by zero (DIV instruction)", EXITCODE_DIVISION_BY_ZERO);
                reg.i[reg0] /= sgn_value;
            }
            break;

            // Undefined behavior for floats
            case OPCODE_MOD1:
            {
                reg0 = xvm_instr_get_reg0(instr);
                sgn_value = xvm_instr_get_sgn_value22(instr);
                if (sgn_value == 0)
                    xvm_exception_throw("division by zero (MOD instruction)", EXITCODE_DIVISION_BY_ZERO);
                reg.i[reg0] %= sgn_value;
            }
            break;

            // Undefined behavior for floats
            case OPCODE_SLL1:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg.i[reg0] <<= xvm_instr_get_sgn_value22(instr);
            }
            break;

            // Undefined behavior for floats
            case OPCODE_SLR1:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg.i[reg0] >>= xvm_instr_get_sgn_value22(instr);
            }
            break;

            case OPCODE_PUSH:
            {
                reg0 = xvm_instr_get_reg0(instr);
                xvm_stack_push(stack, reg_sp, reg.i[reg0]);
            }
            break;

            case OPCODE_POP:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg.i[reg0] = xvm_stack_pop(stack, reg_sp);
            }
            break;

            case OPCODE_INC:
            {
                reg0 = xvm_instr_get_reg0(instr);
                ++reg.i[reg0];
            }
            break;

            case OPCODE_DEC:
            {
                reg0 = xvm_instr_get_reg0(instr);
                --reg.i[reg0];
            }
            break;

            /* --- opcode_jump --- */

            // Undefined behavior for floats
            case OPCODE_JMP:
            {
                // Set program counter to (address + offset)
                reg0 = xvm_instr_get_reg0(instr);
                sgn_value = xvm_instr_get_sgn_value22(instr);
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
                    reg0 = xvm_instr_get_reg0(instr);
                    sgn_value = xvm_instr_get_sgn_value22(instr);
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
                    reg0 = xvm_instr_get_reg0(instr);
                    sgn_value = xvm_instr_get_sgn_value22(instr);
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
                    reg0 = xvm_instr_get_reg0(instr);
                    sgn_value = xvm_instr_get_sgn_value22(instr);
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
                    reg0 = xvm_instr_get_reg0(instr);
                    sgn_value = xvm_instr_get_sgn_value22(instr);
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
                    reg0 = xvm_instr_get_reg0(instr);
                    sgn_value = xvm_instr_get_sgn_value22(instr);
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
                    reg0 = xvm_instr_get_reg0(instr);
                    sgn_value = xvm_instr_get_sgn_value22(instr);
                    *reg_pc = JUMP_ADDRESS(reg0, sgn_value);
                    continue;
                }
            }
            break;

            // Undefined behavior for floats
            case OPCODE_CALL:
            {
                sgn_value = xvm_instr_get_sgn_value22(instr);

                if (sgn_value < INTR_RESERVED)
                {
                    // Push dynamic link (lb and pc registers)
                    extra_value = *reg_lb;
                    *reg_lb = *reg_sp;
                    xvm_stack_push(stack, reg_sp, extra_value);
                    xvm_stack_push(stack, reg_sp, *reg_pc);

                    // Jump to procedure address
                    reg0 = xvm_instr_get_reg0(instr);
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
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                reg.f[reg0] += reg.f[reg1];
            }
            break;

            case OPCODE_SUBF:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                reg.f[reg0] -= reg.f[reg1];
            }
            break;

            case OPCODE_MULF:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                reg.f[reg0] *= reg.f[reg1];
            }
            break;

            case OPCODE_DIVF:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                reg.f[reg0] /= reg.f[reg1];
            }
            break;

            case OPCODE_CMPF:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                *reg_cf = xvm_flt2int_signum(reg.f[reg0] - reg.f[reg1]);
            }
            break;

            /* --- opcode_mem --- */

            case OPCODE_LDA:
            {
                reg0 = xvm_instr_get_reg0(instr);
                unsgn_value = xvm_instr_get_value22(instr);

                cbyte_mem_addr = program_start_ptr + (unsgn_value << 2);

                reg.i[reg0] = (regi_t)(cbyte_mem_addr);
            }
            break;

            /* --- opcode_memoff --- */

            case OPCODE_LDB:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                sgn_value = xvm_instr_get_sgn_value18(instr);

                cbyte_mem_addr = (const byte_t*)reg.i[reg1];
                cbyte_mem_addr += sgn_value;

                reg.i[reg0] = (regi_t)(*cbyte_mem_addr);
            }
            break;

            case OPCODE_STB:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                sgn_value = xvm_instr_get_sgn_value18(instr);

                byte_mem_addr = (byte_t*)reg.i[reg1];
                byte_mem_addr += sgn_value;

                *byte_mem_addr = (byte_t)(reg.i[reg0]);
            }
            break;

            case OPCODE_LDW:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                sgn_value = xvm_instr_get_sgn_value18(instr);

                cbyte_mem_addr = (const byte_t*)reg.i[reg1];
                cbyte_mem_addr += sgn_value;

                cword_mem_addr = (const word_t*)cbyte_mem_addr;

                reg.i[reg0] = (regi_t)(*cword_mem_addr);
            }
            break;

            case OPCODE_STW:
            {
                reg0 = xvm_instr_get_reg0(instr);
                reg1 = xvm_instr_get_reg1(instr);
                sgn_value = xvm_instr_get_sgn_value18(instr);

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
                extra_value = xvm_instr_get_extra_value8(instr);
                unsgn_value = xvm_instr_get_value18(instr);

                stack_word_t* stack_frame_ptr = REG_TO_STACK_PTR(reg_lb);

                // Pop stack frame
                stack_word_t* stack_result_ptr = REG_TO_STACK_PTR(reg_sp) - extra_value;
                stack_word_t* stack_args_ptr = REG_TO_STACK_PTR(reg_lb) - unsgn_value;
                *reg_sp = (regi_t)(stack_args_ptr + extra_value);

                // Pop dynamic link (pc and lb registers)
                *reg_lb = (regi_t)(stack_frame_ptr[0]);
                *reg_pc = (regi_t)(stack_frame_ptr[1]);

                // Move result memory
                if (extra_value > 0)
                    memcpy(stack_args_ptr, stack_result_ptr, sizeof(stack_word_t) * extra_value);
            }
            // Don't 'continue' -> pc has old value and must be increased for the next instruction!
            break;

            case OPCODE_PUSHC:
            {
                sgn_value = xvm_instr_get_sgn_value26(instr);
                xvm_stack_push(stack, reg_sp, sgn_value);
            }
            break;

            case OPCODE_INVK:
            {
                unsgn_value = xvm_instr_get_value26(instr);
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

    return EXITCODE_SUCCESS;

    #undef JUMP_ADDRESS
}

/**
Executes the specified XBC (XieXie Byte Code) program within the XVM (XieXie Virtual Machine) from the beginning.
\see xvm_execute_program_ext
*/
static xvm_exit_codes xvm_execute_program(
    const xvm_bytecode* const byte_code,
    xvm_stack* const stack)
{
    return xvm_execute_program_ext(byte_code, stack, NULL);
}

/**
Executes the specified XBC (XieXie Byte Code) program within the XVM (XieXie Virtual Machine) from the specified entry point.
\param[in] entry_point Specifies the entry point. If this is an unkown entry point, the return value is EXITCODE_UNKNOWN_ENTRY_POINT.
\see xvm_execute_program_ext
*/
static xvm_exit_codes xvm_execute_program_entry_point(
    const xvm_bytecode* const byte_code,
    xvm_stack* const stack,
    const char* entry_point)
{
    const xvm_export_address* export_addr = NULL;
    const xvm_export_address* entry_point_addr = NULL;

    if (byte_code == NULL)
        return EXITCODE_INVALID_BYTECODE;
    if (byte_code->export_addresses == NULL)
        return EXITCODE_UNKNOWN_ENTRY_POINT;

    // Find entry point in export addresses
    for (unsigned int i = 0; i < byte_code->num_export_addresses; ++i)
    {
        export_addr = &(byte_code->export_addresses[i]);
        if (strcmp(export_addr->name.str, entry_point) == 0)
        {
            entry_point_addr = export_addr;
            break;
        }
    }

    if (entry_point_addr == NULL)
        return EXITCODE_UNKNOWN_ENTRY_POINT;

    // Execute program from entry point
    return xvm_execute_program_ext(byte_code, stack, entry_point_addr);
}


/* ----- Shell ----- */

static void shell_print_help()
{
    xvm_log_println("Usage: xvm [options] file");
    xvm_log_println("Options:");
    xvm_log_println("  -h --help help           Prints the help information");
    xvm_log_println("  --version                Prints the version and license note");
    xvm_log_println("  --verbose                Prints additional output before and after program execution");
    xvm_log_println("  -st --stack-size <arg>   Sets the stack size (by default 256)");
}

static void shell_print_version()
{
    #ifdef _ENABLE_RUNTIME_DEBUGGER_
    xvm_log_println("XieXie 2.0 VirtualMachine (XVM) with RuntimeDebugger (RTD)");
    #else
    xvm_log_println("XieXie 2.0 VirtualMachine (XVM)");
    #endif
    xvm_log_println("");
    xvm_log_println("Copyright (C) 2014 Lukas Hermanns");
    xvm_log_println("All rights reserved.");
    xvm_log_println("");
    xvm_log_println("This software may be modified and distributed under the terms");
    xvm_log_println("of the BSD license.  See the LICENSE file for details.");
}

static int shell_parse_args(int argc, char* argv[])
{
    // Configuration memory
    int verbose = 0;
    const char* filename = NULL;
    size_t stack_size = 256;

    // Check if there are any arguments
    const char* arg;

    if (argc <= 0)
    {
        xvm_log_println("no input: enter \"help\" for information");
        return 0;
    }

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
                    xvm_log_error("Stack size must be greater than zero");
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
                xvm_log_error("Expected argument after \"-st\" and \"--stack-size\" flag");
                return 0;
            }
        }
        else if (strlen(arg) > 0)
        {
            if (filename != NULL)
            {
                xvm_log_error("Only a single program can be executed at a time");
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
            xvm_log_readfile_error(filename);
            return 0;
        }

        // Create stack
        xvm_stack stack;
        xvm_stack_init(&stack);
        xvm_stack_create(&stack, stack_size);

        // Execute program
        const xvm_exit_codes exit_code = xvm_execute_program(&byte_code, &stack);

        if (exit_code != EXITCODE_SUCCESS)
            xvm_log_exitcode_error(exit_code);
        else if (verbose != 0)
            xvm_log_println("Program terminated successful");

        // Clean up
        xvm_bytecode_free(&byte_code);
        xvm_stack_free(&stack);
    }

    return 1;
}


#ifndef _REMOVE_XVM_TESTSUITE_

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

    #if defined(_DEBUG) || 0

    // Create a virtual stack
    xvm_stack stack;
    xvm_stack_init(&stack);
    xvm_stack_create(&stack, 256);
    //xvm_stack_clear(&stack, 0xdeadbeef);

    // Create the byte code
    xvm_bytecode byte_code;
    xvm_bytecode_init(&byte_code);
    xvm_bytecode_create_instructions(&byte_code, 50);

    size_t i = 0;
    #define ADD_INSTR(INSTR) byte_code.instructions[i++] = INSTR;
    #define FINISH_INSTR byte_code.num_instructions = i;

    #define TEST 1

    #if TEST == 1 //TEST1 (loop)

    /*
    // Counts from 0 to n
    for int i := 0 ; i < 10 ; i++ {
        Stack.Push(i)
    }
    */

    /*
    00          mov     $r0, 0
    01          mov     $r1, 10
    02  l_for:  cmp     $r0, $r1
    03          jge     l_end           ; jge ($pc) 10
    04          call    Intr.Time
    05          call    Intr.PrintInt
    06          lda     $r2, @str0
    07          push    $r2
    08          call    Intr.PrintLn
    09          push    1000
    10          call    Intr.Sleep
    11          inc     $r0
    12          jmp     l_for           ; jmp ($pc) -10
    13  l_end:  stop
    14  str0:   .ascii  "\nHi"
    */

    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_MOV1, REG_R0, 0                     ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_MOV1, REG_R1, 10                    ))
    ADD_INSTR(xvm_instr_make_reg2       (OPCODE_CMP,  REG_R0, REG_R1                ))
    ADD_INSTR(xvm_instr_make_jump       (OPCODE_JGE,  REG_PC, 10                    ))
    ADD_INSTR(xvm_instr_make_jump       (OPCODE_CALL, REG_PC, INTR_TIME             ))
    ADD_INSTR(xvm_instr_make_jump       (OPCODE_CALL, REG_PC, INTR_PRINT_INT        ))
    ADD_INSTR(xvm_instr_make_mem        (OPCODE_LDA,  REG_R2, 14                    ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_PUSH, REG_R2, 0                     ))
    ADD_INSTR(xvm_instr_make_jump       (OPCODE_CALL, REG_PC, INTR_PRINT_LN         ))
    ADD_INSTR(xvm_instr_make_special1   (OPCODE_PUSHC,100                           ))
    ADD_INSTR(xvm_instr_make_jump       (OPCODE_CALL, REG_PC, INTR_SLEEP            ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_INC,  REG_R0, 0                     ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_JMP,  REG_PC, (unsigned int)(-10)   ))
    ADD_INSTR(xvm_instr_make_special1   (OPCODE_STOP, 0                             ))

    size_t tmp = 0;
    xvm_bytecode_datafield_ascii(&byte_code, i, "\nHello, World!", &tmp);
    i += tmp;

    #elif TEST == 2 //TEST2 (function)

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

    /*ADD_INSTR(xvm_instr_make_special1   (OPCODE_PUSHC,25                                ))
    ADD_INSTR(xvm_instr_make_special1   (OPCODE_INVK, TESTPROCID_PRINTINT               ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_SUB1, REG_SP, 4                         ))
    ADD_INSTR(xvm_instr_make_jump       (OPCODE_JMP,  REG_PC, -3                        ))*/

    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_ADD1, REG_SP, 8                         ))
    ADD_INSTR(xvm_instr_make_reg2       (OPCODE_XOR2, REG_R0, REG_R0                    ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_MOV1, REG_R1, 20                        ))
    ADD_INSTR(xvm_instr_make_reg2       (OPCODE_CMP,  REG_R0, REG_R1                    ))
    ADD_INSTR(xvm_instr_make_jump       (OPCODE_JGE,  REG_PC, 8                         ))
    ADD_INSTR(xvm_instr_make_memoff     (OPCODE_STW,  REG_R0, REG_LB, 0                 ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_ADD1, REG_R0, 2                         ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_PUSH, REG_R0, 0                         ))
    ADD_INSTR(xvm_instr_make_jump       (OPCODE_CALL, REG_PC, 5                         ))
    ADD_INSTR(xvm_instr_make_memoff     (OPCODE_LDW,  REG_R0, REG_LB, 0                 ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_INC,  REG_R0, 0                         ))
    ADD_INSTR(xvm_instr_make_jump       (OPCODE_JMP,  REG_PC, -8                        ))
    ADD_INSTR(xvm_instr_make_special1   (OPCODE_STOP, 0                                 ))
    ADD_INSTR(xvm_instr_make_memoff     (OPCODE_LDW,  REG_R0, REG_LB, (unsigned int)(-4)))
    ADD_INSTR(xvm_instr_make_reg2       (OPCODE_MUL2, REG_R0, REG_R0                    ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_PUSH, REG_R0, 0                         ))
    ADD_INSTR(xvm_instr_make_special2   (OPCODE_RET,  1, 1                              ))

    #elif TEST == 3 //TEST3 (addresses)

    /*
    Print(Pow(3, 5))
    */

    /*
    00  lda r0, flt_lit_0
    01  ldw r1, (r0) 0
    02  ldw r2, (r0) 4
    03  push r2
    04  push r1
    05  call Intr.Pow
    06  call Intr.PrintFloat
    07  invk TESTPROCID_HELLO_WORLD
    08  stop
    09  flt_lit_0: DATA.float 3.0
    10  flt_lit_1: DATA.float 5.0
    */

    float flt_lit0 = 3.0f;
    float flt_lit1 = 5.0f;
    
    ADD_INSTR(xvm_instr_make_mem        (OPCODE_LDA,  REG_R0, 9                 ))
    ADD_INSTR(xvm_instr_make_memoff     (OPCODE_LDW,  REG_R1, REG_R0, 0         ))
    ADD_INSTR(xvm_instr_make_memoff     (OPCODE_LDW,  REG_R2, REG_R0, 4         ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_PUSH, REG_R2, 0                 ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_PUSH, REG_R1, 0                 ))
    ADD_INSTR(xvm_instr_make_jump       (OPCODE_CALL, REG_PC, INTR_POW          ))
    ADD_INSTR(xvm_instr_make_jump       (OPCODE_CALL, REG_PC, INTR_PRINT_FLOAT  ))
    ADD_INSTR(xvm_instr_make_special1   (OPCODE_INVK, TESTPROCID_HELLO_WORLD    ))
    ADD_INSTR(xvm_instr_make_special1   (OPCODE_STOP, 0                         ))
    ADD_INSTR(FLT_TO_INT_REINTERPRET(flt_lit0))
    ADD_INSTR(FLT_TO_INT_REINTERPRET(flt_lit1))

    #elif TEST == 4 //TEST4 (fibonacci)

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
    00          call    Intr.InputInt
    01          pop     $r0
    02  .loop:  xor     $r1, $r1
    03          cmp     $r0, $r1
    04          jle     .end0           ; while begin
    05          push    $r0
    06          push    $r0             ; fib argument
    07          call    fib
    08          call    Intr.PrintInt   ; print result
    09          call    Intr.PrintLn
    0a          pop     $r0
    0b          dec     $r0
    0c          jmp     .loop           ; while end
    0d  .end:   stop
    0e  fib:    ldw     $r0, ($lb) -4   ; get argument
    0f          mov     $r1, 2
    10          cmp     $r0, $r1
    11          jg      .else
    12          push    1
    13          ret     (1) 1
    14  .else:  dec     $r0
    15          push    $r0             ; push t0 = (n-1)
    16          push    $r0             ; fib argument (n-1)
    17          call    fib
    18          pop     $r0             ; fib result
    19          pop     $r1             ; pop t0
    1a          dec     $r1
    1b          push    $r0             ; push t1 = result
    1c          push    $r1             ; fib argument (n-2)
    1d          call    fib
    1e          pop     $r0             ; fib result
    1f          pop     $r1             ; pop t1
    20          add     $r0, $r1
    21          push    $r0             ; push result
    22          ret     (1) 1
    */

    ADD_INSTR(xvm_instr_make_jump       (OPCODE_CALL, REG_PC, INTR_INPUT_INT            ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_POP,  REG_R0, 0                         ))
    ADD_INSTR(xvm_instr_make_reg2       (OPCODE_XOR2, REG_R1, REG_R1                    ))
    ADD_INSTR(xvm_instr_make_reg2       (OPCODE_CMP,  REG_R0, REG_R1                    ))
    ADD_INSTR(xvm_instr_make_jump       (OPCODE_JLE,  REG_PC, 9                         ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_PUSH, REG_R0, 0                         ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_PUSH, REG_R0, 0                         ))
    ADD_INSTR(xvm_instr_make_jump       (OPCODE_CALL, REG_PC, 7                         )) // call fib
    ADD_INSTR(xvm_instr_make_jump       (OPCODE_CALL, REG_PC, INTR_PRINT_INT            )) // call Intr.PrintInt
    ADD_INSTR(xvm_instr_make_jump       (OPCODE_CALL, REG_PC, INTR_PRINT_LN             )) // call Intr.PrintLn
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_POP,  REG_R0, 0                         ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_DEC,  REG_R0, 0                         ))
    ADD_INSTR(xvm_instr_make_jump       (OPCODE_JMP,  REG_PC, (unsigned int)(-10)       ))
    ADD_INSTR(xvm_instr_make_special1   (OPCODE_STOP, 0                                 ))
    ADD_INSTR(xvm_instr_make_memoff     (OPCODE_LDW,  REG_R0, REG_LB, (unsigned int)(-4))) // fib:
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_MOV1, REG_R1, 2                         ))
    ADD_INSTR(xvm_instr_make_reg2       (OPCODE_CMP,  REG_R0, REG_R1                    ))
    ADD_INSTR(xvm_instr_make_jump       (OPCODE_JG,   REG_PC, 3                         ))
    ADD_INSTR(xvm_instr_make_special1   (OPCODE_PUSHC,1                                 ))
    ADD_INSTR(xvm_instr_make_special2   (OPCODE_RET,  1, 1                              )) // ret (1) 1
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_DEC,  REG_R0, 0                         ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_PUSH, REG_R0, 0                         ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_PUSH, REG_R0, 0                         ))
    ADD_INSTR(xvm_instr_make_jump       (OPCODE_CALL, REG_PC, (unsigned int)(-9)        )) // call fib
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_POP,  REG_R0, 0                         ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_POP,  REG_R1, 0                         ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_DEC,  REG_R1, 0                         ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_PUSH, REG_R0, 0                         ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_PUSH, REG_R1, 0                         ))
    ADD_INSTR(xvm_instr_make_jump       (OPCODE_CALL, REG_PC, (unsigned int)(-15)       )) // call fib
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_POP,  REG_R0, 0                         ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_POP,  REG_R1, 0                         ))
    ADD_INSTR(xvm_instr_make_reg2       (OPCODE_ADD2, REG_R0, REG_R1                    ))
    ADD_INSTR(xvm_instr_make_reg1       (OPCODE_PUSH, REG_R0, 0                         ))
    ADD_INSTR(xvm_instr_make_special2   (OPCODE_RET,  1, 1                              )) // ret (1) 1

    #endif
    
    FINISH_INSTR

    #undef ADD_INSTR
    #undef FINISH_INSTR

    // Execute the virtual program
    xvm_invoke_extern = (&TestInvokeExtern);

    const xvm_exit_codes exitCode = xvm_execute_program(&byte_code, &stack);

    if (exitCode != EXITCODE_SUCCESS)
        printf("\nProgram terminated with error code: %i\n\n", exitCode);

    // Show stack output for the 20th first values
    printf("\n\n");

    #if 1
    xvm_log_println("-- Stack content: --");
    //xvm_stack_debug(&stack, 2, 20);
    xvm_stack_debug_float(&stack, 0, 10);
    #endif

    xvm_bytecode_write_to_file(&byte_code, "test_byte_code.xbc");

    xvm_stack_free(&stack);
    xvm_bytecode_free(&byte_code);

    system("pause");

    #endif

    return 0;
}

#endif // /_REMOVE_XVM_TESTSUITE_

#ifdef __cplusplus
}
#endif

