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


/* ----- Compilation configuration ----- */

#define _ENABLE_INLINEING_
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
#define IS_REG_FLOAT(r)             ((r) >= REG_F0)
#define REG_TO_STACK_PTR(r)         ((stack_word_t*)(*(r)))
#define GET_PARAM_FROM_STACK(p, i)  (*((p) - (i)))

typedef enum
{
    REG_I0 = 0x00, // i0  ->  Integer register 0.
    REG_I1 = 0x01, // i1  ->  Integer register 1.
    REG_I2 = 0x02, // i2  ->  Integer register 2.
    REG_I3 = 0x03, // i3  ->  Integer register 3.
    REG_I4 = 0x04, // i4  ->  Integer register 4.
    REG_I5 = 0x05, // i5  ->  Integer register 5.

    REG_CF = 0x06, // cf  ->  Conditional flags: used for jump conditions.
    REG_LB = 0x07, // lb  ->  Local base pointer: POINTER to the current stack frame base.
    REG_SP = 0x08, // sp  ->  Stack-pointer: POINTER to the top of the stack storage.
    REG_PC = 0x09, // pc  ->  Program-counter: INDEX (beginning with 0) to the byte-code instruction list.

    REG_F0 = 0x0a, // f0  ->  Floating-point register 0.
    REG_F1 = 0x0b, // f1  ->  Floating-point register 1.
    REG_F2 = 0x0c, // f2  ->  Floating-point register 2.
    REG_F3 = 0x0d, // f3  ->  Floating-point register 3.
    REG_F4 = 0x0e, // f4  ->  Floating-point register 4.
    REG_F5 = 0x0f, // f5  ->  Floating-point register 5.
}
register_id;


/* ----- OP-codes ----- */

/**

This is full documentation of the 32-bit instruction set of the XieXie 2.0 VirtualMachine.
Currently only single precision floats are supported. There are also no instructions that can handle 64 bit integers.

BYTES are 8 bits wide
WORDS are 32 bits wide.
FLOATS are 32 bits wide.

-------------------------------------------------------------------------------------------------------------------------------------------------
|                                                        2 Register Instruction Opcodes:                                                        |
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
|                                                        1 Register Instruction Opcodes:                                                        |
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
|                                                           Jump Instruction Opcodes:                                                           |
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
|                                                        Load/Store Instruction Opcodes:                                                        |
-------------------------------------------------------------------------------------------------------------------------------------------------
| Mnemonic | Opcode      | Reg.    | Address                                     | Description                                                  |
-------------------------------------------------------------------------------------------------------------------------------------------------
|          | 31.......26 | 25...22 | 21........................................0 |                                                              |
-------------------------------------------------------------------------------------------------------------------------------------------------
| LDB      | 1 1 1 0 0 0 | D D D D | A A A A A A A A A A A A A A A A A A A A A A | Load byte from memory to register.                           |
-------------------------------------------------------------------------------------------------------------------------------------------------
| STB      | 1 1 1 0 0 1 | S S S S | A A A A A A A A A A A A A A A A A A A A A A | Store byte from register to memory.                          |
-------------------------------------------------------------------------------------------------------------------------------------------------
| LDW      | 1 1 1 0 1 0 | D D D D | A A A A A A A A A A A A A A A A A A A A A A | Load word from memory to register.                           |
-------------------------------------------------------------------------------------------------------------------------------------------------
| STW      | 1 1 1 0 1 1 | S S S S | A A A A A A A A A A A A A A A A A A A A A A | Store word from register to memory.                          |
-------------------------------------------------------------------------------------------------------------------------------------------------


-------------------------------------------------------------------------------------------------------------------------------------------------
|                                                   Load/Store (Offset) Instruction Opcodes:                                                    |
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
|                                                         Special Instruction Opcodes:                                                          |
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
    /*
    Reserved        = GEN_OPCODE(0x28),
    Reserved        = GEN_OPCODE(0x29),
    Reserved        = GEN_OPCODE(0x2a),
    Reserved        = GEN_OPCODE(0x2b),
    Reserved        = GEN_OPCODE(0x2c),
    Reserved        = GEN_OPCODE(0x2d),
    Reserved        = GEN_OPCODE(0x2e),
    Reserved        = GEN_OPCODE(0x2f),
    */
}
opcode_jump;

typedef enum
{
    OPCODE_STOP     = GEN_OPCODE(0x00), // STOP          ->  exit(0)
    OPCODE_RET      = GEN_OPCODE(0x30), // RET  (c0) c1  ->  return
    OPCODE_PUSHC    = GEN_OPCODE(0x31), // PUSH value    ->  stack.push(value)
    OPCODE_INVK     = GEN_OPCODE(0x32), // INVK addr     ->  invoke external procedure (no jump, no stack change)
    /*
    Reserved        = GEN_OPCODE(0x33),
    Reserved        = GEN_OPCODE(0x34),
    Reserved        = GEN_OPCODE(0x35),
    Reserved        = GEN_OPCODE(0x36),
    Reserved        = GEN_OPCODE(0x37),
    */
}
opcode_special;

typedef enum
{
    OPCODE_LDB      = GEN_OPCODE(0x38), // LDB reg0, addr  ->  *reg0 = programDataSectionByteAligned[addr]
    //Reserved      = GEN_OPCODE(0x39),
    OPCODE_LDW      = GEN_OPCODE(0x3a), // STB reg0, addr  ->  *reg0 = programDataSectionWorldAligned[addr]
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


/* ----- Instruction constructors ----- */

static instr_t instr_make_reg2(opcode_reg2 opcode, reg_t reg0, reg_t reg1, unsigned int flags)
{
    return (instr_t)(
        #ifdef _OPTIMIZE_OPCODE_EXTRACTION_
        opcode                        |
        #else
        ((opcode & 0x3f      ) << 26) |
        #endif
        ((reg0   & 0x0f      ) << 22) |
        ((reg1   & 0x0f      ) << 18) |
         (flags  & 0x0003ffff)
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

static instr_t instr_make_mem(opcode_mem opcode, reg_t reg, unsigned int address)
{
    return instr_make_reg1(opcode, reg, address);
}

static instr_t instr_make_memoff(opcode_memoff opcode, reg_t reg0, reg_t reg1, unsigned int offset)
{
    return instr_make_reg2(opcode, reg0, reg1, offset);
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

static int xvm_bytecode_read_from_file(xvm_bytecode* byte_code, const char* filename)
{
    
    //...

    return 0;
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


/* ----- Intrinsics ----- */

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
    // Only for XieXie 1.0 compatibility, don't use them for performance seasons.
    // Make use of CMP instruction and conditional jump instructions (see 'opcode_jump' enumeration).
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
    // Use every 2nd address to allow extension when
    // double precision floating points are supported in future.
    INTR_SIN            = 0x001fff80, // float Sin(float x).
    INTR_COS            = 0x001fff82, // float Cos(float x).
    INTR_TAN            = 0x001fff84, // float Tan(float x).
    INTR_ASIN           = 0x001fff86, // float ASin(float x).
    INTR_ACOS           = 0x001fff88, // float ACos(float x).
    INTR_ATAN           = 0x001fff8a, // float ATan(float x).
    INTR_POW            = 0x001fff8c, // float Pow(float base, float exp).
    INTR_SQRT           = 0x001fff8e, // float Sqrt(float x).
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
    //xvm_stack_read(*reg_sp, -1);

    switch (intrinsic_addr)
    {
        /* --- Dynamic memory intrinsics --- */

        case INTR_ALLOC_MEM:
        case INTR_FREE_MEM:
        case INTR_COPY_MEM:
        break;

        /* --- Console intrinsics --- */

        case INTR_SYS_CALL:
        case INTR_CLEAR:
        case INTR_PRINT:
        case INTR_PRINT_LN:
        case INTR_PRINT_INT:
        case INTR_PRINT_FLOAT:
        case INTR_INPUT_INT:
        case INTR_INPUT_FLOAT:
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
        case INTR_COS:
        case INTR_TAN:
        case INTR_ASIN:
        case INTR_ACOS:
        case INTR_ATAN:
        case INTR_POW:
        case INTR_SQRT:
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

    const instr_t* const instr_ptr = byte_code->instructions;
    const int num_instr = byte_code->num_instructions;

    regi_t* const reg_cf = (reg.i + REG_CF);
    regi_t* const reg_lb = (reg.i + REG_LB);
    regi_t* const reg_sp = (reg.i + REG_SP);
    regi_t* const reg_pc = (reg.i + REG_PC);

    // Program start pointer is used to load memory from program "DATA" section
    const byte_t* const program_start_ptr = (const byte_t*)instr_ptr;

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
        *reg_pc = 0;

    /* --- Catch exceptions --- */
    int exception_val = setjmp(xvm_exception_envbuf);
    if (exception_val != 0)
    {
        log_error(xvm_exception_err);
        return exception_val;
    }

    /* --- Start with program execution --- */
    while (*reg_pc < num_instr)
    {
        /* Load next instruction */
        instr = instr_ptr[*reg_pc];

        opcode = instr_get_opcode(instr);

        /* Execute current instruction */
        switch (opcode)
        {
            /* --- opcode_reg2 --- */

            case OPCODE_MOV2:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                if (IS_REG_FLOAT(reg0))
                    reg.f[reg0] = reg.f[reg1];
                else
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
                if (IS_REG_FLOAT(reg0))
                    reg.f[reg0] += reg.f[reg1];
                else
                    reg.i[reg0] += reg.i[reg1];
            }
            break;

            case OPCODE_SUB2:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                if (IS_REG_FLOAT(reg0))
                    reg.f[reg0] -= reg.f[reg1];
                else
                    reg.i[reg0] -= reg.i[reg1];
            }
            break;

            case OPCODE_MUL2:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                if (IS_REG_FLOAT(reg0))
                    reg.f[reg0] *= reg.f[reg1];
                else
                    reg.i[reg0] *= reg.i[reg1];
            }
            break;

            case OPCODE_DIV2:
            {
                reg0 = instr_get_reg0(instr);
                reg1 = instr_get_reg1(instr);
                if (IS_REG_FLOAT(reg0))
                    reg.f[reg0] /= reg.f[reg1];
                else
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
                if (IS_REG_FLOAT(reg0))
                    *reg_cf = flt2int_signum(reg.f[reg0] - reg.f[reg1]);
                else
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
                if (IS_REG_FLOAT(reg0))
                    ++reg.f[reg0];
                else
                    ++reg.i[reg0];
            }
            break;

            case OPCODE_DEC:
            {
                reg0 = instr_get_reg0(instr);
                if (IS_REG_FLOAT(reg0))
                    --reg.f[reg0];
                else
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
                *reg_pc = reg.i[reg0] + sgn_value;
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
                    *reg_pc = reg.i[reg0] + sgn_value;
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
                    *reg_pc = reg.i[reg0] + sgn_value;
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
                    *reg_pc = reg.i[reg0] + sgn_value;
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
                    *reg_pc = reg.i[reg0] + sgn_value;
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
                    *reg_pc = reg.i[reg0] + sgn_value;
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
                    *reg_pc = reg.i[reg0] + sgn_value;
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
                    *reg_pc = reg.i[reg0] + sgn_value;
                    continue;
                }
                else
                {
                    // Call intrinsic procedure
                    xvm_call_intrinsic(sgn_value, stack, reg_sp);
                }
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
        ++(*reg_pc);
    }

    // Return program counter if set
    if (exe_state != NULL)
        exe_state->pc_reset = *reg_pc;

    return EXITCODE_SUCCESS;
}


/* ----- Shell ----- */

static void shell_print_help()
{
    log_println("Usage: xvm [options] file");
    log_println("Options:");
    log_println("  -h --help                Prints the help information");
    log_println("  -v --version             Prints the version and license note");
    log_println("  -st --stack-size <arg>   Sets the stack size (by default 256)");
}

static void shell_print_version()
{
    log_println("XieXie 2.0 VirtualMachine (XVM)");
    log_println("GNU LESSER GENERAL PUBLIC LICENSE Version 3 (LGPLv3)");
    log_println("Copyright (c) 2014  Lukas Hermanns");
}

static int shell_parse_args(int argc, char* argv[])
{
    const char* arg;

    if (argc <= 0)
    {
        shell_print_help();
        return 0;
    }

    // Configuration memory
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
        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0)
            shell_print_help();
        else if (strcmp(arg, "-v") == 0 || strcmp(arg, "--version") == 0)
            shell_print_version();
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
        xvm_execution_state exe_state;
        const xvm_exit_codes exit_code = xvm_execute_program(&byte_code, &stack, &exe_state);

        if (exit_code != EXITCODE_SUCCESS)
            log_exitcode_error(exit_code);

        // Clean up
        xvm_bytecode_free(&byte_code);
        xvm_stack_free(&stack);
    }

    return 1;
}


/* ----- Main ----- */

typedef enum
{
    TESTPROCID_PRINT,
    TESTPROCID_PRINTLN,
    TESTPROCID_PRINTINT,
}
TestInvokeProcIDs;

void TestInvokeExtern(unsigned int proc_id, stack_word_t* stack_ptr)
{
    switch (proc_id)
    {
        case TESTPROCID_PRINTINT:
            printf("%i", GET_PARAM_FROM_STACK(stack_ptr, 1));
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

    #if 0 //TEST1

    /*
    // Counts from 0 to n
    for int i := 0 ; i < 10 ; i++ {
        Stack.Push(i)
    }
    */

    /*
    00          mov i0, 0
    01          mov i1, 10
    02  l_for:  cmp i0, i1
    03          jge l_end   ; jge (pc) 4
    04          push i0
    05          inc i0
    06          jmp l_for   ; jmp (pc) -4
    07  l_end:  stop
    */

    ADD_INSTR(instr_make_reg1       (OPCODE_MOV1, REG_I0, 0                 ))
    ADD_INSTR(instr_make_reg1       (OPCODE_MOV1, REG_I1, 10                ))
    ADD_INSTR(instr_make_reg2       (OPCODE_CMP,  REG_I0, REG_I1, 0         ))
    ADD_INSTR(instr_make_jump       (OPCODE_JGE,  REG_PC, 4                 ))
    ADD_INSTR(instr_make_reg1       (OPCODE_PUSH, REG_I0, 0                 ))
    ADD_INSTR(instr_make_reg1       (OPCODE_INC,  REG_I0, 0                 ))
    ADD_INSTR(instr_make_reg1       (OPCODE_JMP,  REG_PC, (unsigned int)(-4)))
    ADD_INSTR(instr_make_special1   (OPCODE_STOP, 0                         ))

    #else //TEST2

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
    01          xor i0, i0      ; i = 0
    02          mov i1, 10      ; n = 10
    03  .for0:  cmp i0, i1      ; compare i >= n
    04          jge .end0       ; if i >= n then goto l_end ; jge (pc) 7
    05          stw i0 (lb) 0   ; store i
    06          add i0, 2       ; i += 2
    07          push i0         ; push i
    08          call func       ; Stack.Push(func(i)) (actually 'pop i0' after call, but we keep it on stack) ; call (pc) 5
    09          ldw i0, (lb) 0  ; load i
    10          inc i0          ; i++
    11          jmp .for0       ; jmp (pc) -7
    12  .end0:  stop            ; exit
    13  func:   ldw i0, (lb) -4 ; load x
    14          mul i0, i0      ; x *= x
    15          push i0         ; push x
    16          ret (1) 1       ; return result ((x*x) = 1 word) and pop arguments (x = 1 word)
    */

    /*ADD_INSTR(instr_make_special1   (OPCODE_PUSHC,25                                ))
    ADD_INSTR(instr_make_special1   (OPCODE_INVK, TESTPROCID_PRINTINT               ))
    ADD_INSTR(instr_make_reg1       (OPCODE_SUB1, REG_SP, 4                         ))
    ADD_INSTR(instr_make_jump       (OPCODE_JMP,  REG_PC, -3                        ))*/

    ADD_INSTR(instr_make_reg1       (OPCODE_ADD1, REG_SP, 8                         ))
    ADD_INSTR(instr_make_reg2       (OPCODE_XOR2, REG_I0, REG_I0, 0                 ))
    ADD_INSTR(instr_make_reg1       (OPCODE_MOV1, REG_I1, 20                        ))
    ADD_INSTR(instr_make_reg2       (OPCODE_CMP,  REG_I0, REG_I1, 0                 ))
    ADD_INSTR(instr_make_jump       (OPCODE_JGE,  REG_PC, 8                         ))
    ADD_INSTR(instr_make_memoff     (OPCODE_STWO, REG_I0, REG_LB, 0                 ))
    ADD_INSTR(instr_make_reg1       (OPCODE_ADD1, REG_I0, 2                         ))
    ADD_INSTR(instr_make_reg1       (OPCODE_PUSH, REG_I0, 0                         ))
    ADD_INSTR(instr_make_jump       (OPCODE_CALL, REG_PC, 5                         ))
    ADD_INSTR(instr_make_memoff     (OPCODE_LDWO, REG_I0, REG_LB, 0                 ))
    ADD_INSTR(instr_make_reg1       (OPCODE_INC,  REG_I0, 0                         ))
    ADD_INSTR(instr_make_jump       (OPCODE_JMP,  REG_PC, -8                        ))
    ADD_INSTR(instr_make_special1   (OPCODE_STOP, 0                                 ))
    ADD_INSTR(instr_make_memoff     (OPCODE_LDWO, REG_I0, REG_LB, (unsigned int)(-4)))
    ADD_INSTR(instr_make_reg2       (OPCODE_MUL2, REG_I0, REG_I0, 0                 ))
    ADD_INSTR(instr_make_reg1       (OPCODE_PUSH, REG_I0, 0                         ))
    ADD_INSTR(instr_make_special2   (OPCODE_RET,  1, 1                              ))

    #endif
    
    #undef ADD_INSTR

    // Execute the virtual program
    xvm_invoke_extern = (&TestInvokeExtern);

    const xvm_exit_codes exitCode = xvm_execute_program(&byte_code, &stack, NULL);

    if (exitCode != EXITCODE_SUCCESS)
        printf("\nProgram terminated with error code: %i\n\n", exitCode);

    // Show stack output for the 20th first values
    log_println("-- Stack content: --");
    xvm_stack_debug(&stack, 2, 20);

    xvm_stack_free(&stack);
    xvm_bytecode_free(&byte_code);

    system("pause");

    #endif

    return 0;
}
