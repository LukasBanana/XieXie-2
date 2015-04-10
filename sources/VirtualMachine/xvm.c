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

//! Disables the shell (and the "main" function)
//#define _DISABLE_SHELL_

//! Makes all functions non-static
//#define _NON_STATIC_FUNCTIONS_

//! Enables force inlining
#define _ENABLE_INLINEING_

//! Enables runtimer debugger in virtual machine
//#define _ENABLE_RUNTIME_DEBUGGER_

//! Shows the instruction indices as hex numbers.
//#define _SHOW_RUNTIME_HEXLINES_

//! Enables opcode extraction optimization (safes one SLL instruction in x86 code)
#define _OPTIMIZE_OPCODE_EXTRACTION_


/* ----- OS specific includes ----- */

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
#   include <dlfcn.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

/* ----- Helper macros ----- */

#define XVM_MIN(a, b) ((a) < (b) ? (a) : (b))
#define XVM_MAX(a, b) ((a) > (b) ? (a) : (b))

#ifdef _NON_STATIC_FUNCTIONS_
#   define STATIC // No static functions
#else
#   define STATIC static
#endif

#ifdef _ENABLE_INLINEING_
#   define INLINE __inline
#else
#   define INLINE // No inlining
#endif


/* ----- Helper functions ----- */

INLINE STATIC int xvm_flt2int_signum(float val)
{
    if (val > 0.0f)
        return 1;
    else if (val < 0.0f)
        return -1;
    return 0;
}


/* ----- Registers ----- */

typedef unsigned char   reg_t;  //!< Register index type (8-bit unsigned integral type).
typedef int             regi_t; //!< Integer register type (32-bit signed integral type).
typedef float           regf_t; //!< Float register type (32-bit IEEE 754 floating-point type).

typedef char            byte_t; //!< Byte type (8-bit).
typedef short           half_t; //!< Half-word type (16-bit).
typedef int             word_t; //!< Word type (32-bit).

#define XVM_NUM_REGISTERS           32
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
    REG_R0  = 0x00, // $r0   ->  General purpose register 0.
    REG_R1  = 0x01, // $r1   ->  General purpose register 1.
    REG_R2  = 0x02, // $r2   ->  General purpose register 2.
    REG_R3  = 0x03, // $r3   ->  General purpose register 3.
    REG_R4  = 0x04, // $r4   ->  General purpose register 4.
    REG_R5  = 0x05, // $r5   ->  General purpose register 5.
    REG_R6  = 0x06, // $r6   ->  General purpose register 6.
    REG_R7  = 0x07, // $r7   ->  General purpose register 7.
    REG_R8  = 0x08, // $r8   ->  General purpose register 8.
    REG_R9  = 0x09, // $r9   ->  General purpose register 9.
    REG_R10 = 0x0a, // $r10  ->  General purpose register 10.
    REG_R11 = 0x0b, // $r11  ->  General purpose register 11.
    REG_R12 = 0x0c, // $r12  ->  General purpose register 12.
    REG_R13 = 0x0d, // $r13  ->  General purpose register 13.
    REG_R14 = 0x0e, // $r14  ->  General purpose register 14.
    REG_R15 = 0x0f, // $r15  ->  General purpose register 15.
    REG_R16 = 0x10, // $r16  ->  General purpose register 16.
    REG_R17 = 0x11, // $r17  ->  General purpose register 17.
    REG_R18 = 0x12, // $r18  ->  General purpose register 18.
    REG_R19 = 0x13, // $r19  ->  General purpose register 19.
    REG_R20 = 0x14, // $r20  ->  General purpose register 20.
    REG_R21 = 0x15, // $r21  ->  General purpose register 21.
    REG_R22 = 0x16, // $r22  ->  General purpose register 22.
    REG_R23 = 0x17, // $r23  ->  General purpose register 23.
    REG_R24 = 0x18, // $r24  ->  General purpose register 24.

    /* --- Special purpose registers --- */
    REG_AR  = 0x19, // $ar   ->  Argument return: can be used as 32-bit return value.
    REG_XR  = 0x1a, // $xr   ->  Extended register: register for special purposes (e.g. assembler temporary or 'this' pointer).
    REG_GP  = 0x1b, // $gp   ->  Global pointer: POINTER to the global variables in the stack.

    /* --- Register for internal use --- */
    REG_CF  = 0x1c, // $cf   ->  Conditional flags: used for jump conditions.
    REG_LB  = 0x1d, // $lb   ->  Local base pointer: POINTER to the base of the current stack frame.
    REG_SP  = 0x1e, // $sp   ->  Stack pointer: POINTER to the top of the stack storage.
    REG_PC  = 0x1f, // $pc   ->  Program counter: POINTER to the current instruction in the byte-code.
}
register_id;

/**
Returns the name of the specified register or an empty string if the ID is invalid.
\see register_id
*/
STATIC const char* xvm_register_get_name(reg_t reg)
{
    switch (reg)
    {
        case REG_R0:  return "$r0";
        case REG_R1:  return "$r1";
        case REG_R2:  return "$r2";
        case REG_R3:  return "$r3";
        case REG_R4:  return "$r4";
        case REG_R5:  return "$r5";
        case REG_R6:  return "$r6";
        case REG_R7:  return "$r7";
        case REG_R8:  return "$r8";
        case REG_R9:  return "$r9";
        case REG_R10: return "$r10";
        case REG_R11: return "$r11";
        case REG_R12: return "$r12";
        case REG_R13: return "$r13";
        case REG_R14: return "$r14";
        case REG_R15: return "$r15";
        case REG_R16: return "$r16";
        case REG_R17: return "$r17";
        case REG_R18: return "$r18";
        case REG_R19: return "$r19";
        case REG_R20: return "$r20";
        case REG_R21: return "$r21";
        case REG_R22: return "$r22";
        case REG_R23: return "$r23";
        case REG_R24: return "$r24";

        case REG_AR:  return "$ar";
        case REG_XR:  return "$xr";
        case REG_GP:  return "$gp";

        case REG_CF:  return "$cf";
        case REG_LB:  return "$lb";
        case REG_SP:  return "$sp";
        case REG_PC:  return "$pc";
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

---------------------------------------------------------------------------------------------------------------------------------------------------
|                                                3 Register Instruction Opcodes (00....)                                                          |
|-------------------------------------------------------------------------------------------------------------------------------------------------|
|          | Opcode      | Dest.     | LSource   | RSource   | Unused                | Description                                                |
| Mnemonic |-------------|-----------|-----------|-----------|-----------------------|------------------------------------------------------------|
|          | 31.......26 | 25.....21 | 20.....16 | 15.....11 | 10..................0 |                                                            |
|----------|-------------|-----------|-----------|-----------|-----------------------|------------------------------------------------------------|
| AND      | 0 0 0 0 0 1 | D D D D D | L L L L L | R R R R R | 0 0 0 0 0 0 0 0 0 0 0 | Bitwise AND.                                               |
|----------|-------------|-----------|-----------|-----------|-----------------------|------------------------------------------------------------|
| OR       | 0 0 0 0 1 0 | D D D D D | L L L L L | R R R R R | 0 0 0 0 0 0 0 0 0 0 0 | Bitwise OR.                                                |
|----------|-------------|-----------|-----------|-----------|-----------------------|------------------------------------------------------------|
| XOR      | 0 0 0 0 1 1 | D D D D D | L L L L L | R R R R R | 0 0 0 0 0 0 0 0 0 0 0 | Bitwise XOR.                                               |
|----------|-------------|-----------|-----------|-----------|-----------------------|------------------------------------------------------------|
| ADD      | 0 0 0 1 0 0 | D D D D D | L L L L L | R R R R R | 0 0 0 0 0 0 0 0 0 0 0 | Arithmetic addition.                                       |
|----------|-------------|-----------|-----------|-----------|-----------------------|------------------------------------------------------------|
| SUB      | 0 0 0 1 0 1 | D D D D D | L L L L L | R R R R R | 0 0 0 0 0 0 0 0 0 0 0 | Arithmetic subtraction.                                    |
|----------|-------------|-----------|-----------|-----------|-----------------------|------------------------------------------------------------|
| MUL      | 0 0 0 1 1 0 | D D D D D | L L L L L | R R R R R | 0 0 0 0 0 0 0 0 0 0 0 | Arithmetic multiplication.                                 |
|----------|-------------|-----------|-----------|-----------|-----------------------|------------------------------------------------------------|
| DIV      | 0 0 0 1 1 1 | D D D D D | L L L L L | R R R R R | 0 0 0 0 0 0 0 0 0 0 0 | Arithmetic division.                                       |
|----------|-------------|-----------|-----------|-----------|-----------------------|------------------------------------------------------------|
| MOD      | 0 0 1 0 0 0 | D D D D D | L L L L L | R R R R R | 0 0 0 0 0 0 0 0 0 0 0 | Arithmetic remainder (modulo).                             |
|----------|-------------|-----------|-----------|-----------|-----------------------|------------------------------------------------------------|
| SLL      | 0 0 1 0 0 1 | D D D D D | L L L L L | R R R R R | 0 0 0 0 0 0 0 0 0 0 0 | Shift logical left.                                        |
|----------|-------------|-----------|-----------|-----------|-----------------------|------------------------------------------------------------|
| SLR      | 0 0 1 0 1 0 | D D D D D | L L L L L | R R R R R | 0 0 0 0 0 0 0 0 0 0 0 | Shift logical right.                                       |
|----------|-------------|-----------|-----------|-----------|-----------------------|------------------------------------------------------------|
| ADDF     | 0 0 1 0 1 1 | D D D D D | L L L L L | R R R R R | 0 0 0 0 0 0 0 0 0 0 0 | Arithmetic float addition.                                 |
|----------|-------------|-----------|-----------|-----------|-----------------------|------------------------------------------------------------|
| SUBF     | 0 0 1 1 0 0 | D D D D D | L L L L L | R R R R R | 0 0 0 0 0 0 0 0 0 0 0 | Arithmetic float subtraction.                              |
|----------|-------------|-----------|-----------|-----------|-----------------------|------------------------------------------------------------|
| MULF     | 0 0 1 1 0 1 | D D D D D | L L L L L | R R R R R | 0 0 0 0 0 0 0 0 0 0 0 | Arithmetic float multiplication.                           |
|----------|-------------|-----------|-----------|-----------|-----------------------|------------------------------------------------------------|
| DIVF     | 0 0 1 1 1 0 | D D D D D | L L L L L | R R R R R | 0 0 0 0 0 0 0 0 0 0 0 | Arithmetic float division.                                 |
|----------|-------------|-----------|-----------|-----------|-----------------------|------------------------------------------------------------|
| MODF     | 0 0 1 1 1 1 | D D D D D | L L L L L | R R R R R | 0 0 0 0 0 0 0 0 0 0 0 | Arithmetic float remainder (modulo).                       |
---------------------------------------------------------------------------------------------------------------------------------------------------


---------------------------------------------------------------------------------------------------------------------------------------------------
|                                                    2 Register Instruction Opcodes (01....)                                                      |
|-------------------------------------------------------------------------------------------------------------------------------------------------|
|          | Opcode      | Dest.     | Source    | Unused or Value                 | Description                                                  |
| Mnemonic |-------------|-----------|-----------|---------------------------------|--------------------------------------------------------------|
|          | 31.......26 | 25.....21 | 20.....16 | 15............................0 |                                                              |
|----------|-------------|-----------|-----------|---------------------------------|--------------------------------------------------------------|
| MOV      | 0 1 0 0 0 0 | D D D D D | S S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Move register source to destination.                         |
|----------|-------------|-----------|-----------|---------------------------------|--------------------------------------------------------------|
| NOT      | 0 1 0 0 0 1 | D D D D D | S S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Bitwise NOT.                                                 |
|----------|-------------|-----------|-----------|---------------------------------|--------------------------------------------------------------|
| FTI      | 0 1 0 0 1 0 | D D D D D | S S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Float to integer.                                            |
|----------|-------------|-----------|-----------|---------------------------------|--------------------------------------------------------------|
| ITF      | 0 1 0 0 1 1 | D D D D D | S S S S S | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Integer to float.                                            |
|----------|-------------|-----------|-----------|---------------------------------|--------------------------------------------------------------|
| AND      | 0 1 0 1 0 0 | D D D D D | S S S S S | V V V V V V V V V V V V V V V V | Bitwise AND.                                                 |
|----------|-------------|-----------|-----------|---------------------------------|--------------------------------------------------------------|
| OR       | 0 1 0 1 0 1 | D D D D D | S S S S S | V V V V V V V V V V V V V V V V | Bitwise OR.                                                  |
|----------|-------------|-----------|-----------|---------------------------------|--------------------------------------------------------------|
| XOR      | 0 1 0 1 1 0 | D D D D D | S S S S S | V V V V V V V V V V V V V V V V | Bitwise XOR.                                                 |
|----------|-------------|-----------|-----------|---------------------------------|--------------------------------------------------------------|
| ADD      | 0 1 0 1 1 1 | D D D D D | S S S S S | V V V V V V V V V V V V V V V V | Arithmetic addition.                                         |
|----------|-------------|-----------|-----------|---------------------------------|--------------------------------------------------------------|
| SUB      | 0 1 1 0 0 0 | D D D D D | S S S S S | V V V V V V V V V V V V V V V V | Arithmetic subtraction.                                      |
|----------|-------------|-----------|-----------|---------------------------------|--------------------------------------------------------------|
| MUL      | 0 1 1 0 0 1 | D D D D D | S S S S S | V V V V V V V V V V V V V V V V | Arithmetic multiplication.                                   |
|----------|-------------|-----------|-----------|---------------------------------|--------------------------------------------------------------|
| DIV      | 0 1 1 0 1 0 | D D D D D | S S S S S | V V V V V V V V V V V V V V V V | Arithmetic division.                                         |
|----------|-------------|-----------|-----------|---------------------------------|--------------------------------------------------------------|
| MOD      | 0 1 1 0 1 1 | D D D D D | S S S S S | V V V V V V V V V V V V V V V V | Arithmetic modulo.                                           |
|----------|-------------|-----------|-----------|---------------------------------|--------------------------------------------------------------|
| SLL      | 0 1 1 1 0 0 | D D D D D | S S S S S | V V V V V V V V V V V V V V V V | Shift locial left.                                           |
|----------|-------------|-----------|-----------|---------------------------------|--------------------------------------------------------------|
| SLR      | 0 1 1 1 0 1 | D D D D D | S S S S S | V V V V V V V V V V V V V V V V | Shift locial right.                                          |
|----------|-------------|-----------|-----------|---------------------------------|--------------------------------------------------------------|
| CMP      | 0 1 1 1 1 0 | X X X X X | Y Y Y Y Y | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Compares the two registers and stores the result in "cf".    |
|----------|-------------|-----------|-----------|---------------------------------|--------------------------------------------------------------|
| CMPF     | 0 1 1 1 1 1 | X X X X X | Y Y Y Y Y | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Compares the two float and stores the result in "cf".        |
---------------------------------------------------------------------------------------------------------------------------------------------------


---------------------------------------------------------------------------------------------------------------------------------------------------
|                                                    1 Register Instruction Opcodes (100...)                                                      |
|-------------------------------------------------------------------------------------------------------------------------------------------------|
|          | Opcode      | Register  | Unused, Value, or Offset                  | Description                                                    |
| Mnemonic |-------------|-----------|-------------------------------------------|----------------------------------------------------------------|
|          | 31.......26 | 25.....21 | 20......................................0 |                                                                |
|----------|-------------|-----------|-------------------------------------------|----------------------------------------------------------------|
| PUSH     | 1 0 0 0 0 0 | R R R R R | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Push source register onto stack.                               |
|----------|-------------|-----------|-------------------------------------------|----------------------------------------------------------------|
| POP      | 1 0 0 0 0 1 | R R R R R | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Pop destination register from stack.                           |
|----------|-------------|-----------|-------------------------------------------|----------------------------------------------------------------|
| INC      | 1 0 0 0 1 0 | R R R R R | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Increment integral register.                                   |
|----------|-------------|-----------|-------------------------------------------|----------------------------------------------------------------|
| DEC      | 1 0 0 0 1 1 | R R R R R | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Decrement integral register.                                   |
|----------|-------------|-----------|-------------------------------------------|----------------------------------------------------------------|
| MOV      | 1 0 0 1 0 0 | R R R R R | V V V V V V V V V V V V V V V V V V V V V | Move value to register.                                        |
|----------|-------------|-----------|-------------------------------------------|----------------------------------------------------------------|
| LDA      | 1 0 0 1 0 1 | R R R R R | O O O O O O O O O O O O O O O O O O O O O | Load address from program pointer (word aligned) + offset 'V'. |
|----------|-------------|-----------|-------------------------------------------|----------------------------------------------------------------|
| MOV      | 1 0 0 1 1 0 | R R R R R | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Move next instruction as word to register. Increment $pc.      |
---------------------------------------------------------------------------------------------------------------------------------------------------


-------------------------------------------------------------------------------------------------------------------------------------------------
|                                                      Jump Instruction Opcodes (101...)                                                        |
|-----------------------------------------------------------------------------------------------------------------------------------------------|
|          | Opcode      | Register  | Offset                                    | Description                                                  |
| Mnemonic |-------------|-----------|-------------------------------------------|--------------------------------------------------------------|
|          | 31.......26 | 25.....21 | 20......................................0 |                                                              |
|----------|-------------|-----------|-------------------------------------------|--------------------------------------------------------------|
| JMP      | 1 0 1 0 0 0 | R R R R R | O O O O O O O O O O O O O O O O O O O O O | Jump to address, stored in register 'R' + offset 'O'.        |
|----------|-------------|-----------|-------------------------------------------|--------------------------------------------------------------|
| JE       | 1 0 1 0 0 1 | R R R R R | O O O O O O O O O O O O O O O O O O O O O | Jump if equal.                                               |
|----------|-------------|-----------|-------------------------------------------|--------------------------------------------------------------|
| JNE      | 1 0 1 0 1 0 | R R R R R | O O O O O O O O O O O O O O O O O O O O O | Jump if not-equal.                                           |
|----------|-------------|-----------|-------------------------------------------|--------------------------------------------------------------|
| JG       | 1 0 1 0 1 1 | R R R R R | O O O O O O O O O O O O O O O O O O O O O | Jump if greater.                                             |
|----------|-------------|-----------|-------------------------------------------|--------------------------------------------------------------|
| JL       | 1 0 1 1 0 0 | R R R R R | O O O O O O O O O O O O O O O O O O O O O | Jump if less.                                                |
|----------|-------------|-----------|-------------------------------------------|--------------------------------------------------------------|
| JGE      | 1 0 1 1 0 1 | R R R R R | O O O O O O O O O O O O O O O O O O O O O | Jump if greater or equal.                                    |
|----------|-------------|-----------|-------------------------------------------|--------------------------------------------------------------|
| JLE      | 1 0 1 1 1 0 | R R R R R | O O O O O O O O O O O O O O O O O O O O O | Jump if less or equal.                                       |
|----------|-------------|-----------|-------------------------------------------|--------------------------------------------------------------|
|          |             |           |                                           | Push the dynamic link ('lb' and 'pc' register) onto stack.   |
| CALL     | 1 0 1 1 1 1 | R R R R R | O O O O O O O O O O O O O O O O O O O O O | Set 'lb' register to new stack frame.                        |
|          |             |           |                                           | Jump to address, stored in register 'R' + offset 'O'.        |
-------------------------------------------------------------------------------------------------------------------------------------------------


-------------------------------------------------------------------------------------------------------------------------------------------------
|                                              Load/Store (Offset) Instruction Opcodes (110...)                                                 |
|-----------------------------------------------------------------------------------------------------------------------------------------------|
|          | Opcode      | Register  | Address   | Offset                          | Description                                                |
| Mnemonic |-------------|-----------|-----------|---------------------------------|------------------------------------------------------------|
|          | 31.......26 | 25.....21 | 20.....16 | 15............................0 |                                                            |
|----------|-------------|-----------|-----------|---------------------------------|------------------------------------------------------------|
| LDB      | 1 1 0 0 0 0 | R R R R R | A A A A A | O O O O O O O O O O O O O O O O | Load byte from memory to register (destination).           |
|----------|-------------|-----------|-----------|---------------------------------|------------------------------------------------------------|
| STB      | 1 1 0 0 0 1 | R R R R R | A A A A A | O O O O O O O O O O O O O O O O | Store byte from register (source) to memory.               |
|----------|-------------|-----------|-----------|---------------------------------|------------------------------------------------------------|
| LDH      | 1 1 0 0 1 0 | R R R R R | A A A A A | O O O O O O O O O O O O O O O O | Load half-word from memory to register (destination).      |
|----------|-------------|-----------|-----------|---------------------------------|------------------------------------------------------------|
| STH      | 1 1 0 0 1 1 | R R R R R | A A A A A | O O O O O O O O O O O O O O O O | Store half-word from register (source) to memory.          |
|----------|-------------|-----------|-----------|---------------------------------|------------------------------------------------------------|
| LDW      | 1 1 0 1 0 0 | R R R R R | A A A A A | O O O O O O O O O O O O O O O O | Load word from memory to register (destination).           |
|----------|-------------|-----------|-----------|---------------------------------|------------------------------------------------------------|
| STW      | 1 1 0 1 0 1 | R R R R R | A A A A A | O O O O O O O O O O O O O O O O | Store word from register (source) to memory.               |
-------------------------------------------------------------------------------------------------------------------------------------------------


-------------------------------------------------------------------------------------------------------------------------------------------------
|                                                       Special Instruction Opcodes 1:                                                          |
|-----------------------------------------------------------------------------------------------------------------------------------------------|
| Mnemonic | Opcode      | Unused or Value                                     | Description                                                    |
|----------|-------------|-----------------------------------------------------|----------------------------------------------------------------|
|          | 31.......26 | 25................................................0 |                                                                |
|----------|-------------|-----------------------------------------------------|----------------------------------------------------------------|
| STOP     | 0 0 0 0 0 0 | 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | Stop program execution.                                        |
|----------|-------------|-----------------------------------------------------|----------------------------------------------------------------|
| PUSH     | 1 1 1 0 0 0 | V V V V V V V V V V V V V V V V V V V V V V V V V V | Push value 'V' onto stack.                                     |
|----------|-------------|-----------------------------------------------------|----------------------------------------------------------------|
| INVK     | 1 1 1 0 0 1 | V V V V V V V V V V V V V V V V V V V V V V V V V V | Invoke external procedure with ID 'V'.                         |
|----------|-------------|-----------------------------------------------------|----------------------------------------------------------------|
| INSC     | 1 1 1 0 1 0 | V V V V V V V V V V V V V V V V V V V V V V V V V V | Call intrinsic with ID 'V'.                                    |
-------------------------------------------------------------------------------------------------------------------------------------------------


-------------------------------------------------------------------------------------------------------------------------------------------------
|                                                       Special Instruction Opcodes 2:                                                          |
|-----------------------------------------------------------------------------------------------------------------------------------------------|
| Mnemonic | Opcode      | Result Size         | Arguments Size                  | Description                                                  |
|----------|-------------|---------------------|---------------------------------|--------------------------------------------------------------|
|          | 31.......26 | 25...............16 | 15............................0 |                                                              |
|----------|-------------|---------------------|---------------------------------|--------------------------------------------------------------|
|          |             |                     |                                 | Pop R words from the stack and buffer them.                  |
|          |             |                     |                                 | Pop the current stack frame.                                 |
| RET      | 1 1 1 0 1 1 | R R R R R R R R R R | A A A A A A A A A A A A A A A A | Pop A words from the stack.                                  |
|          |             |                     |                                 | Push the R words back onto the stack.                        |
|          |             |                     |                                 | Restore the 'lb' and 'pc' registers.                         |
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

//! XASM instruction opcodes.
typedef enum
{
    /* --- 3 register opcodes --- */
    OPCODE_AND3     = GEN_OPCODE(0x01), // AND  reg0, reg1, reg2  ->  reg0 = reg1 & reg2
    OPCODE_OR3      = GEN_OPCODE(0x02), // OR   reg0, reg1, reg2  ->  reg0 = reg1 | reg2
    OPCODE_XOR3     = GEN_OPCODE(0x03), // XOR  reg0, reg1, reg2  ->  reg0 = reg1 ^ reg2
    OPCODE_ADD3     = GEN_OPCODE(0x04), // ADD  reg0, reg1, reg2  ->  reg0 = reg1 + reg2
    OPCODE_SUB3     = GEN_OPCODE(0x05), // SUB  reg0, reg1, reg2  ->  reg0 = reg1 - reg2
    OPCODE_MUL3     = GEN_OPCODE(0x06), // MUL  reg0, reg1, reg2  ->  reg0 = reg1 * reg2
    OPCODE_DIV3     = GEN_OPCODE(0x07), // DIV  reg0, reg1, reg2  ->  reg0 = reg1 / reg2
    OPCODE_MOD3     = GEN_OPCODE(0x08), // MOD  reg0, reg1, reg2  ->  reg0 = reg1 % reg2
    OPCODE_SLL3     = GEN_OPCODE(0x09), // SLL  reg0, reg1, reg2  ->  reg0 = reg1 << reg2
    OPCODE_SLR3     = GEN_OPCODE(0x0a), // SLR  reg0, reg1, reg2  ->  reg0 = reg1 >> reg2
    OPCODE_ADDF     = GEN_OPCODE(0x0b), // ADDF reg0, reg1, reg2  ->  reg0 = reg1 + reg2
    OPCODE_SUBF     = GEN_OPCODE(0x0c), // SUBF reg0, reg1, reg2  ->  reg0 = reg1 - reg2
    OPCODE_MULF     = GEN_OPCODE(0x0d), // MULF reg0, reg1, reg2  ->  reg0 = reg1 * reg2
    OPCODE_DIVF     = GEN_OPCODE(0x0e), // DIVF reg0, reg1, reg2  ->  reg0 = reg1 / reg2
    OPCODE_MODF     = GEN_OPCODE(0x0f), // MODF reg0, reg1, reg2  ->  reg0 = fmodf(reg1, reg2)

    /* --- 2 register opcodes --- */
    OPCODE_MOV2     = GEN_OPCODE(0x10), // MOV  reg0, reg1     ->  reg0 = reg1
    OPCODE_NOT      = GEN_OPCODE(0x11), // NOT  reg0, reg1     ->  reg0 = ~reg1
    OPCODE_FTI      = GEN_OPCODE(0x12), // FTI  reg0, reg1     ->  reg0 = (int)reg1
    OPCODE_ITF      = GEN_OPCODE(0x13), // ITF  reg0, reg1     ->  reg0 = (float)reg1
    OPCODE_AND2     = GEN_OPCODE(0x14), // AND  reg0, reg1, c  ->  reg0 = reg1 & c
    OPCODE_OR2      = GEN_OPCODE(0x15), // OR   reg0, reg1, c  ->  reg0 = reg1 | c
    OPCODE_XOR2     = GEN_OPCODE(0x16), // XOR  reg0, reg1, c  ->  reg0 = reg1 ^ c
    OPCODE_ADD2     = GEN_OPCODE(0x17), // ADD  reg0, reg1, c  ->  reg0 = reg1 + c
    OPCODE_SUB2     = GEN_OPCODE(0x18), // SUB  reg0, reg1, c  ->  reg0 = reg1 - c
    OPCODE_MUL2     = GEN_OPCODE(0x19), // MUL  reg0, reg1, c  ->  reg0 = reg1 * c
    OPCODE_DIV2     = GEN_OPCODE(0x1a), // DIV  reg0, reg1, c  ->  reg0 = reg1 / c
    OPCODE_MOD2     = GEN_OPCODE(0x1b), // MOD  reg0, reg1, c  ->  reg0 = reg1 % c
    OPCODE_SLL2     = GEN_OPCODE(0x1c), // SLL  reg0, reg1, c  ->  reg0 = reg1 << c
    OPCODE_SLR2     = GEN_OPCODE(0x1d), // SLR  reg0, reg1, c  ->  reg0 = reg1 >> c
    OPCODE_CMP      = GEN_OPCODE(0x1e), // CMP  reg0, reg1     ->  REG_CF = (reg0 - reg1)
    OPCODE_CMPF     = GEN_OPCODE(0x1f), // CMPF reg0, reg1     ->  REG_CF = signum(reg0 - reg1)

    /* --- 1 register opcodes --- */
    OPCODE_PUSH     = GEN_OPCODE(0x20), // PUSH reg        ->  stack.push(reg)
    OPCODE_POP      = GEN_OPCODE(0x21), // POP  reg        ->  reg = stack.pop()
    OPCODE_INC      = GEN_OPCODE(0x22), // INC  reg        ->  ++reg
    OPCODE_DEC      = GEN_OPCODE(0x23), // DEC  reg        ->  --reg
    OPCODE_MOV1     = GEN_OPCODE(0x24), // MOV  reg, c     ->  reg = c
    OPCODE_LDA      = GEN_OPCODE(0x25), // LDA  reg, addr  ->  reg = programMemoryStartWorldAligned[addr]
    OPCODE_MOVI     = GEN_OPCODE(0x26), // MOVI reg        ->  reg = programMemoryStartWorldAligned[pc + 1]; ++pc
    //Reserved      = GEN_OPCODE(0x27),

    /* --- jump opcodes --- */
    OPCODE_JMP      = GEN_OPCODE(0x28), // JMP  addr  ->  goto addr
    OPCODE_JE       = GEN_OPCODE(0x29), // JE   addr  ->  if (REG_CF == 0) then goto addr
    OPCODE_JNE      = GEN_OPCODE(0x2a), // JNE  addr  ->  if (REG_CF != 0) then goto addr
    OPCODE_JG       = GEN_OPCODE(0x2b), // JG   addr  ->  if (REG_CF  > 0) then goto addr
    OPCODE_JL       = GEN_OPCODE(0x2c), // JL   addr  ->  if (REG_CF  < 0) then goto addr
    OPCODE_JGE      = GEN_OPCODE(0x2d), // JGE  addr  ->  if (REG_CF >= 0) then goto addr
    OPCODE_JLE      = GEN_OPCODE(0x2e), // JLE  addr  ->  if (REG_CF <= 0) then goto addr
    OPCODE_CALL     = GEN_OPCODE(0x2f), // CALL addr  ->  PUSH $pc; PUSH $lb; MOV $lb, $sp; JMP addr;

    /* --- load/store opcodes --- */
    OPCODE_LDB      = GEN_OPCODE(0x30), // LDB reg0, (reg1) c  ->  reg0 = globalMemoryByteAligned[reg1 + c] (load byte)
    OPCODE_STB      = GEN_OPCODE(0x31), // STB reg0, (reg1) c  ->  globalMemoryByteAligned[reg1 + c] = reg0 (store byte)
    OPCODE_LDH      = GEN_OPCODE(0x32), // LDH reg0, (reg1) c  ->  reg0 = globalMemoryByteAligned[reg1 + c] (load half-word)
    OPCODE_STH      = GEN_OPCODE(0x33), // STH reg0, (reg1) c  ->  globalMemoryByteAligned[reg1 + c] = reg0 (store half-word)
    OPCODE_LDW      = GEN_OPCODE(0x34), // LDW reg0, (reg1) c  ->  reg0 = globalMemoryByteAligned[reg1 + c] (load word)
    OPCODE_STW      = GEN_OPCODE(0x35), // STW reg0, (reg1) c  ->  globalMemoryByteAligned[reg1 + c] = reg0 (store word)
    //Reserved      = GEN_OPCODE(0x36),
    //Reserved      = GEN_OPCODE(0x37),

    /* --- special opcodes --- */
    OPCODE_STOP     = GEN_OPCODE(0x00), // STOP          ->  exit(0)
    OPCODE_PUSHC    = GEN_OPCODE(0x38), // PUSH value    ->  stack.push(value)
    OPCODE_INVK     = GEN_OPCODE(0x39), // INVK addr     ->  invoke external procedure
    OPCODE_INSC     = GEN_OPCODE(0x3a), // INSC value    ->  call intrinsic
    OPCODE_RET      = GEN_OPCODE(0x3b), // RET  (c0) c1  ->  return
    //Reserved      = GEN_OPCODE(0x3c),
    //Reserved      = GEN_OPCODE(0x3d),
    //Reserved      = GEN_OPCODE(0x3e),
    //Reserved      = GEN_OPCODE(0x3f),
}
xvm_opcode;


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
    EXITCODE_INVOCATION_VIOLATION   = -9,
  //EXITCODE_MEMORY_VIOLATION       = -10,
}
xvm_exit_codes;

STATIC const char* xvm_exitcode_to_string(const xvm_exit_codes exit_code)
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
        case EXITCODE_INVOCATION_VIOLATION:
            return "invocation violation";
    }
    return NULL;
}


/* ----- Debug log ----- */

INLINE STATIC void xvm_log_print(const char* str)
{
    printf("%s", str);
}

INLINE STATIC void xvm_log_println(const char* str)
{
    puts(str);
}

INLINE STATIC void xvm_log_error(const char* str)
{
    printf("error: %s\n", str);
}

INLINE STATIC void xvm_log_readfile_error(const char* filename)
{
    printf("error: reading file \"%s\" failed\n", filename);
}

INLINE STATIC void xvm_log_exitcode_error(const xvm_exit_codes exit_code)
{
    const char* err = xvm_exitcode_to_string(exit_code);
    if (err != NULL)
        printf("error: program terminated with error: \"%s\"\n", err);
}


/* ----- File helper ----- */

STATIC unsigned int xvm_file_read_uint(FILE* file)
{
    unsigned int value = 0;
    if (file != NULL)
        fread(&value, sizeof(value), 1, file);
    return value;
}

STATIC void xvm_file_write_uint(FILE* file, unsigned int value)
{
    if (file != NULL)
        fwrite(&value, sizeof(value), 1, file);
}


/* ----- Intrinsics ----- */

/*
Calling convention:
- CALLER pushes all arguments from RIGHT-to-LEFT onto stack.
- CALLEE pops all arguments from the stack.
- CALLEE stores result in '$ar' register.
*/

typedef enum
{
    /* --- Dynamic memory intrinsics --- */
    INSC_ALLOC_MEM = 0, // void* AllocMem(uint sizeInBytes)
    INSC_FREE_MEM,      // void FreeMem(void* memoryAddr)
    INSC_COPY_MEM,      // void CopyMem(void* dstMemAddr, const void* srcMemAddr, uint sizeInBytes)
    INSC_FILL_MEM,      // void FillMem(void* memoryAddr, uint sizeInBytes, int value)

    /* --- Console intrinsics --- */
    INSC_SYS_CALL,      // void SysCall(const byte* stringAddr)
    INSC_PRINT,         // void Print(const byte* stringAddr)
    INSC_PRINT_LN,      // void PrintLn(const byte* stringAddr)
    INSC_PRINT_INT,     // void PrintInt(int value)
    INSC_PRINT_FLOAT,   // void PrintFloat(float value)
    INSC_INPUT,         // void Input(byte* stringAddr, int maxLen)
    INSC_INPUT_INT,     // int InputInt()
    INSC_INPUT_FLOAT,   // float InputFloat()

    /* --- File intrinsics --- */
    INSC_CREATE_FILE,   // int CreateFile(const byte* stringAddress)
    INSC_DELETE_FILE,   // int DeleteFile(const byte* stringAddress)
    INSC_OPEN_FILE,     // void* OpenFile(const byte* stringAddress)
    INSC_CLOSE_FILE,    // void CloseFile(void* fileHandle)
    INSC_FILE_SIZE,     // int FileSize(const void* fileHandle)
    INSC_SET_FILE_POS,  // void FileSetPos(const void* fileHandle, int pos)
    INSC_GET_FILE_POS,  // int FileGetPos(const void* fileHandle)
    INSC_FILE_EOF,      // int FileEOF(const void* fileHandle)
    INSC_WRITE_BYTE,    // void WriteByte(const void* fileHandle, const void* memoryAddress)
    INSC_WRITE_WORD,    // void WriteWord(const void* fileHandle, const void* memoryAddress)
    INSC_READ_BYTE,     // void ReadByte(const void* fileHandle, void* memoryAddress)
    INSC_READ_WORD,     // void ReadWord(const void* fileHandle, void* memoryAddress)

    /* --- Math intrinsics --- */
    INSC_SIN,           // float Sin(float x)
    INSC_COS,           // float Cos(float x)
    INSC_TAN,           // float Tan(float x)
    INSC_ASIN,          // float ASin(float x)
    INSC_ACOS,          // float ACos(float x)
    INSC_ATAN,          // float ATan(float x)
    INSC_POW,           // float Pow(float base, float exp)
    INSC_SQRT,          // float Sqrt(float x)

    /* --- Other intrinsics --- */
    INSC_RAND_INT,      // int RandInt() -> In range [0 .. MAX_INT]
    INSC_RAND_FLOAT,    // float RandFloat() -> In range [0.0 .. 1.0]
    INSC_TIME,          // int Time() -> Ellapsed time since program start (in ms.)
    INSC_SLEEP,         // void Sleep(int duration)
}
xvm_intrinsic_id;

//! Returns the identifier of the specified intrinsic.
STATIC const char* xvm_intrinsic_get_ident(const xvm_intrinsic_id addr)
{
    switch (addr)
    {
        /* --- Dynamic memory intrinsics --- */
        
        case INSC_ALLOC_MEM:    return "AllocMem";
        case INSC_FREE_MEM:     return "FreeMem";
        case INSC_COPY_MEM:     return "CopyMem";
        case INSC_FILL_MEM:     return "FillMem";
    
        /* --- Console intrinsics --- */

        case INSC_SYS_CALL:     return "SysCall";
        case INSC_PRINT:        return "Print";
        case INSC_PRINT_LN:     return "PrintLn";
        case INSC_PRINT_INT:    return "PrintInt";
        case INSC_PRINT_FLOAT:  return "PrintFloat";
        case INSC_INPUT:        return "Input";
        case INSC_INPUT_INT:    return "InputInt";
        case INSC_INPUT_FLOAT:  return "InputFloat";

        /* --- File intrinsics --- */

        case INSC_CREATE_FILE:  return "CreateFile";
        case INSC_DELETE_FILE:  return "DeleteFile";
        case INSC_OPEN_FILE:    return "OpenFile";
        case INSC_CLOSE_FILE:   return "CloseFile";
        case INSC_FILE_SIZE:    return "FileSize";
        case INSC_SET_FILE_POS: return "SetFilePos";
        case INSC_GET_FILE_POS: return "GetFilePos";
        case INSC_FILE_EOF:     return "FileEOF";
        case INSC_WRITE_BYTE:   return "WriteByte";
        case INSC_WRITE_WORD:   return "WriteWord";
        case INSC_READ_BYTE:    return "ReadByte";
        case INSC_READ_WORD:    return "ReadWord";

        /* --- Math intrinsics --- */

        case INSC_SIN:          return "Sin";
        case INSC_COS:          return "Cos";
        case INSC_TAN:          return "Tan";
        case INSC_ASIN:         return "ASin";
        case INSC_ACOS:         return "ACos";
        case INSC_ATAN:         return "ATan";
        case INSC_POW:          return "Pow";
        case INSC_SQRT:         return "Sqrt";

        /* --- Other intrinsics --- */

        case INSC_RAND_INT:     return "RandInt";
        case INSC_RAND_FLOAT:   return "RandFloat";
        case INSC_TIME:         return "Time";
        case INSC_SLEEP:        return "Sleep";
    }
    return "";
}


/* ----- Instruction ----- */

#define XVM_VALUE26_MAX     (0x03ffffff)
#define XVM_VALUE21_MAX     (0x001fffff)
#define XVM_VALUE16_MAX     (0x0000ffff)

#define XVM_VALUE26_MIN     (0)
#define XVM_VALUE21_MIN     (0)
#define XVM_VALUE16_MIN     (0)

#define XVM_SGN_VALUE26_MAX (0x01ffffff)
#define XVM_SGN_VALUE21_MAX (0x000fffff)
#define XVM_SGN_VALUE16_MAX (0x0007ffff)

#define XVM_SGN_VALUE26_MIN (-0x02000000)
#define XVM_SGN_VALUE21_MIN (-0x00100000)
#define XVM_SGN_VALUE16_MIN (-0x00008000)

typedef unsigned int instr_t;

//! Returns the instruction opcode.
INLINE STATIC opcode_t _xvm_instr_get_opcode(const instr_t instr)
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

//! Returns the 26-bit unsigned value of the specified instruction.
INLINE STATIC unsigned int _xvm_instr_get_value26(const instr_t instr)
{
    return (instr & XVM_VALUE26_MAX);
}

//! Returns the 21-bit unsigned value of the specified instruction.
INLINE STATIC unsigned int _xvm_instr_get_value21(const instr_t instr)
{
    return (instr & XVM_VALUE21_MAX);
}

//! Returns the 16-bit unsigned value of the specified instruction.
INLINE STATIC unsigned int _xvm_instr_get_value16(const instr_t instr)
{
    return (instr & XVM_VALUE16_MAX);
}

//! Returns the 26-bit signed value of the specified instruction.
INLINE STATIC int _xvm_instr_get_sgn_value26(const instr_t instr)
{
    unsigned int val = _xvm_instr_get_value26(instr);

    // Sign extend
    if ((val & 0x02000000) != 0)
        val |= 0xfc000000;

    return (int)val;
}

//! Returns the 21-bit signed value of the specified instruction.
INLINE STATIC int _xvm_instr_get_sgn_value21(const instr_t instr)
{
    unsigned int val = _xvm_instr_get_value21(instr);

    // Sign extend
    if ((val & 0x00100000) != 0)
        val |= 0xffe00000;

    return (int)val;
}

//! Returns the 16-bit signed value of the specified instruction.
INLINE STATIC int _xvm_instr_get_sgn_value16(const instr_t instr)
{
    unsigned int val = _xvm_instr_get_value16(instr);

    // Sign extend
    if ((val & 0x00008000) != 0)
        val |= 0xffff0000;

    return (int)val;
}

//! Returns the extra value of the special instruction RET.
INLINE STATIC int _xvm_instr_get_extra_value10(const instr_t instr)
{
    return (instr & 0x03ff0000) >> 16;
}

//! Returns the index of the register 0.
INLINE STATIC reg_t _xvm_instr_get_reg0(const instr_t instr)
{
    return (instr & 0x03e00000) >> 21;
}

//! Returns the index of the register 1.
INLINE STATIC reg_t _xvm_instr_get_reg1(const instr_t instr)
{
    return (instr & 0x001f0000) >> 16;
}

//! Returns the index of the register 2.
INLINE STATIC reg_t _xvm_instr_get_reg2(const instr_t instr)
{
    return (instr & 0x0000f800) >> 11;
}

/**
Returns the mnemonic of the specified instruction opcode or an empty string if the opcode is invalid.
If the opcode is valid, the returned string will always consist of 4 characters plus the null terminator '\0'.
*/
STATIC const char* xvm_instr_get_mnemonic(const opcode_t opcode)
{
    switch (opcode)
    {
        case OPCODE_MOVI:
        case OPCODE_MOV1:
        case OPCODE_MOV2:   return "mov ";
        case OPCODE_NOT:    return "not ";
        case OPCODE_AND2:
        case OPCODE_AND3:   return "and ";
        case OPCODE_OR2:
        case OPCODE_OR3:    return "or  ";
        case OPCODE_XOR2:
        case OPCODE_XOR3:   return "xor ";
        case OPCODE_ADD2:
        case OPCODE_ADD3:   return "add ";
        case OPCODE_SUB2:
        case OPCODE_SUB3:   return "sub ";
        case OPCODE_MUL2:
        case OPCODE_MUL3:   return "mul ";
        case OPCODE_DIV2:
        case OPCODE_DIV3:   return "div ";
        case OPCODE_MOD2:
        case OPCODE_MOD3:   return "mod ";
        case OPCODE_SLL2:
        case OPCODE_SLL3:   return "sll ";
        case OPCODE_SLR2:
        case OPCODE_SLR3:   return "slr ";
        case OPCODE_CMP:    return "cmp ";
        case OPCODE_FTI:    return "fti ";
        case OPCODE_ITF:    return "itf ";
        case OPCODE_PUSHC:
        case OPCODE_PUSH:   return "push";
        case OPCODE_POP:    return "pop ";
        case OPCODE_INC:    return "inc ";
        case OPCODE_DEC:    return "dec ";
        case OPCODE_JMP:    return "jmp ";
        case OPCODE_JE:     return "je  ";
        case OPCODE_JNE:    return "jne ";
        case OPCODE_JG:     return "jg  ";
        case OPCODE_JL:     return "jl  ";
        case OPCODE_JGE:    return "jge ";
        case OPCODE_JLE:    return "jle ";
        case OPCODE_CALL:   return "call";
        case OPCODE_ADDF:   return "addf";
        case OPCODE_SUBF:   return "subf";
        case OPCODE_MULF:   return "mulf";
        case OPCODE_DIVF:   return "divf";
        case OPCODE_MODF:   return "modf";
        case OPCODE_CMPF:   return "cmpf";
        case OPCODE_LDA:    return "lda ";
        case OPCODE_LDB:    return "ldb ";
        case OPCODE_STB:    return "stb ";
        case OPCODE_LDH:    return "ldh ";
        case OPCODE_STH:    return "sth ";
        case OPCODE_LDW:    return "ldw ";
        case OPCODE_STW:    return "stw ";
        case OPCODE_STOP:   return "stop";
        case OPCODE_INVK:   return "invk";
        case OPCODE_INSC:   return "insc";
        case OPCODE_RET:    return "ret ";
    }
    return "";
}

#ifdef _ENABLE_RUNTIME_DEBUGGER_

/**
Prints debug information for the specified instruction
with the current state of the specified register set.
*/
STATIC void _xvm_instr_print_debug_info(const instr_t instr, regi_t instr_index, const regi_t* reg_ptr)
{
    const opcode_t opcode = _xvm_instr_get_opcode(instr);

    #ifdef _SHOW_RUNTIME_HEXLINES_
    printf("0x%*.8x  %s", 8, (instr_index >> 2), xvm_instr_get_mnemonic(opcode));
    #else
    printf("%*.8i  %s  ", 8, (instr_index >> 2), xvm_instr_get_mnemonic(opcode));
    #endif

    if (opcode >= OPCODE_AND3 && opcode <= OPCODE_MODF)
    {
        reg_t reg0 = _xvm_instr_get_reg0(instr);
        reg_t reg1 = _xvm_instr_get_reg1(instr);
        reg_t reg2 = _xvm_instr_get_reg2(instr);

        const char* reg0name = xvm_register_get_name(reg0);
        const char* reg1name = xvm_register_get_name(reg1);
        const char* reg2name = xvm_register_get_name(reg2);

        if (opcode >= OPCODE_ADDF)
        {
            printf(
                "%s, %s, %s  (%s = %f)",
                reg0name, reg1name, reg2name, reg0name, INT_TO_FLT_REINTERPRET(reg_ptr[reg0])
            );
        }
        else
        {
            printf(
                "%s, %s, %s  (%s = %i)",
                reg0name, reg1name, reg2name, reg0name, reg_ptr[reg0]
            );
        }
    }
    else if (opcode >= OPCODE_MOV2 && opcode <= OPCODE_ITF)
    {
        reg_t reg0 = _xvm_instr_get_reg0(instr);
        reg_t reg1 = _xvm_instr_get_reg1(instr);

        const char* reg0name = xvm_register_get_name(reg0);
        const char* reg1name = xvm_register_get_name(reg1);

        printf("%s, %s       (%s = %i)", reg0name, reg1name, reg0name, reg_ptr[reg0]);
    }
    else if (opcode >= OPCODE_AND2 && opcode <= OPCODE_SLR2)
    {
        reg_t reg0 = _xvm_instr_get_reg0(instr);
        reg_t reg1 = _xvm_instr_get_reg1(instr);

        const char* reg0name = xvm_register_get_name(reg0);
        const char* reg1name = xvm_register_get_name(reg1);

        int value = _xvm_instr_get_sgn_value16(instr);

        printf("%s, %s, %i    (%s = %i)", reg0name, reg1name, value, reg0name, reg_ptr[reg0]);
    }
    if (opcode == OPCODE_CMP)
    {
        reg_t reg0 = _xvm_instr_get_reg0(instr);
        reg_t reg1 = _xvm_instr_get_reg1(instr);

        const char* reg0name = xvm_register_get_name(reg0);
        const char* reg1name = xvm_register_get_name(reg1);

        printf("%s, %s       ($cf = %i)", reg0name, reg1name, reg_ptr[reg0] - reg_ptr[reg1]);
    }
    else if (opcode == OPCODE_CMPF)
    {
        reg_t reg0 = _xvm_instr_get_reg0(instr);
        reg_t reg1 = _xvm_instr_get_reg1(instr);

        const char* reg0name = xvm_register_get_name(reg0);
        const char* reg1name = xvm_register_get_name(reg1);

        float lhs = INT_TO_FLT_REINTERPRET(reg_ptr[reg0]);
        float rhs = INT_TO_FLT_REINTERPRET(reg_ptr[reg1]);

        printf("%s, %s       ($cf = %i)", reg0name, reg1name, xvm_flt2int_signum(lhs - rhs));
    }
    else if (opcode >= OPCODE_JMP && opcode <= OPCODE_JLE)
    {
        reg_t reg0 = _xvm_instr_get_reg0(instr);
        const char* reg0name = xvm_register_get_name(reg0);

        int addr_offset = _xvm_instr_get_sgn_value21(instr);

        printf("(%s), %i", reg0name, addr_offset);
    }
    else if (opcode >= OPCODE_LDB && opcode <= OPCODE_STW)
    {
        reg_t reg0 = _xvm_instr_get_reg0(instr);
        reg_t reg1 = _xvm_instr_get_reg1(instr);

        const char* reg0name = xvm_register_get_name(reg0);
        const char* reg1name = xvm_register_get_name(reg1);

        int offset = _xvm_instr_get_sgn_value16(instr);

        printf("%s, (%s), %i  (%s = %i)", reg0name, reg1name, offset, reg0name, reg_ptr[reg0]);
    }
    else if (opcode == OPCODE_CALL)
    {
        int addr_offset = _xvm_instr_get_sgn_value21(instr);
        if (addr_offset == XVM_SGN_VALUE21_MAX)
        {
            reg_t reg0 = _xvm_instr_get_reg0(instr);
            const char* reg0name = xvm_register_get_name(reg0);

            printf("%s", reg0name);
        }
        else
        {
            reg_t reg0 = _xvm_instr_get_reg0(instr);
            const char* reg0name = xvm_register_get_name(reg0);
            
            printf("(%s), %i", reg0name, addr_offset);
        }
    }
    else if (opcode >= OPCODE_PUSH && opcode <= OPCODE_DEC)
    {
        reg_t reg0 = _xvm_instr_get_reg0(instr);
        const char* reg0name = xvm_register_get_name(reg0);

        printf("%s            (%s = %i)", reg0name, reg0name, reg_ptr[reg0]);
    }
    else if (opcode == OPCODE_MOV1)
    {
        reg_t reg0 = _xvm_instr_get_reg0(instr);

        const char* reg0name = xvm_register_get_name(reg0);
        int value = _xvm_instr_get_sgn_value21(instr);

        printf("%s, %i         (%s = %i)", reg0name, value, reg0name, reg_ptr[reg0]);
    }
    else if (opcode == OPCODE_MOVI)
    {
        reg_t reg0 = _xvm_instr_get_reg0(instr);
        const char* reg0name = xvm_register_get_name(reg0);

        printf("%s, %i         (%s = %i)", reg0name, reg_ptr[reg0], reg0name, reg_ptr[reg0]);
    }
    else if (opcode == OPCODE_LDA)
    {
        reg_t reg0 = _xvm_instr_get_reg0(instr);
        const char* reg0name = xvm_register_get_name(reg0);

        printf("%s            (%s = %i)", reg0name, reg0name, reg_ptr[reg0]);
    }
    else if (opcode == OPCODE_RET)
    {
        unsigned int extra_value = _xvm_instr_get_extra_value10(instr);
        unsigned int value = _xvm_instr_get_value16(instr);

        printf("(%u) %u", extra_value, value);
    }
    else if (opcode == OPCODE_INSC)
    {
        unsigned int value = _xvm_instr_get_value26(instr);
        printf("%s  <intrinsic>", xvm_intrinsic_get_ident((xvm_intrinsic_id)value));
    }
    else if (opcode == OPCODE_PUSHC)
        printf("%i", _xvm_instr_get_sgn_value26(instr));

    printf("\n");
}

#endif


/* ----- Instruction constructors ----- */

//! Makes a 3-register instruction.
STATIC instr_t xvm_instr_make_reg3(xvm_opcode opcode, reg_t reg0, reg_t reg1, reg_t reg2)
{
    return (instr_t)(
        #ifdef _OPTIMIZE_OPCODE_EXTRACTION_
        opcode                        |
        #else
        ((opcode & 0x3f      ) << 26) |
        #endif
        ((reg0   & 0x1f      ) << 21) |
        ((reg1   & 0x1f      ) << 16) |
        ((reg2   & 0x1f      ) << 11)
    );
}

//! Makes a 2-register instruction.
STATIC instr_t xvm_instr_make_reg2(xvm_opcode opcode, reg_t reg0, reg_t reg1, unsigned int value)
{
    return (instr_t)(
        #ifdef _OPTIMIZE_OPCODE_EXTRACTION_
        opcode                             |
        #else
        ((opcode & 0x3f           ) << 26) |
        #endif
        ((reg0   & 0x1f           ) << 21) |
        ((reg1   & 0x1f           ) << 16) |
         (value  & XVM_VALUE16_MAX)
    );
}

//! Makes a 1-register instruction.
STATIC instr_t xvm_instr_make_reg1(xvm_opcode opcode, reg_t reg, unsigned int value)
{
    return (instr_t)(
        #ifdef _OPTIMIZE_OPCODE_EXTRACTION_
        opcode                             |
        #else
        ((opcode & 0x3f           ) << 26) |
        #endif
        ((reg    & 0x1f           ) << 21) |
         (value  & XVM_VALUE21_MAX)
    );
}

//! Makes a jump instruction.
STATIC instr_t xvm_instr_make_jump(xvm_opcode opcode, reg_t reg, unsigned int offset)
{
    return xvm_instr_make_reg1(opcode, reg, offset);
}

//! Makes a load/store instruction.
STATIC instr_t xvm_instr_make_loadstore(xvm_opcode opcode, reg_t reg0, reg_t reg1, unsigned int offset)
{
    return xvm_instr_make_reg2(opcode, reg0, reg1, offset);
}

//! Makes a special-1 instruction.
STATIC instr_t xvm_instr_make_special1(xvm_opcode opcode, unsigned int value)
{
    return (instr_t)(
        #ifdef _OPTIMIZE_OPCODE_EXTRACTION_
        opcode                             |
        #else
        ((opcode & 0x3f           ) << 26) |
        #endif
         (value  & XVM_VALUE26_MAX)
    );
}

//! Makes a special-2 instruction.
STATIC instr_t xvm_instr_make_special2(xvm_opcode opcode, unsigned int result_size, unsigned int arg_size)
{
    return (instr_t)(
        #ifdef _OPTIMIZE_OPCODE_EXTRACTION_
        opcode                                  |
        #else
        ((opcode      & 0x3f           ) << 26) |
        #endif
        ((result_size & 0x000003ff     ) << 16) |
         (arg_size    & XVM_VALUE16_MAX)
    );
}

STATIC instr_t _xvm_instr_patch_value26(instr_t instr, unsigned int value)
{
    return (instr & 0xfc000000) | (value & XVM_VALUE26_MAX);
}

STATIC instr_t _xvm_instr_patch_value21(instr_t instr, unsigned int value)
{
    return (instr & 0xffe00000) | (value & XVM_VALUE21_MAX);
}

STATIC instr_t _xvm_instr_patch_value16(instr_t instr, unsigned int value)
{
    return (instr & 0xffff0000) | (value & XVM_VALUE16_MAX);
}

STATIC instr_t xvm_instr_patch_value(instr_t instr, unsigned int value)
{
    opcode_t opcode = _xvm_instr_get_opcode(instr);

    if ( ( opcode >= OPCODE_LDB && opcode <= OPCODE_STW ) || ( opcode >= OPCODE_FTI && opcode <= OPCODE_SLR2 ) )
        return _xvm_instr_patch_value16(instr, value);
    if ( ( opcode >= OPCODE_JMP && opcode <= OPCODE_CALL ) || opcode == OPCODE_LDA || opcode == OPCODE_MOV1 )
        return _xvm_instr_patch_value21(instr, value);
    if (opcode == OPCODE_PUSHC || opcode == OPCODE_INVK)
        return _xvm_instr_patch_value26(instr, value);

    return instr;
}


/* ----- String ----- */

//! XVM string structure.
typedef struct
{
    size_t  len;
    char*   str;
}
xvm_string;

//! Returns an empty string object
STATIC xvm_string xvm_string_init()
{
    xvm_string string;
    string.len = 0;
    string.str = NULL;
    return string;
}

//! Creates a new string object with the specified length (plus the null terminator '\0').
STATIC xvm_string xvm_string_create(size_t len)
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
STATIC xvm_string xvm_string_create_from(const char* str)
{
    // Create string and copy data
    size_t len = strlen(str);
    xvm_string string = xvm_string_create(len);
    strcpy(string.str, str);
    return string;
}

//! Frees the memory of the specified string.
STATIC int xvm_string_free(xvm_string* string)
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
STATIC xvm_string xvm_string_read_from_file(FILE* file)
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
STATIC int xvm_string_write_to_file(xvm_string string, FILE* file)
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


/* ----- Exception handling ----- */

static jmp_buf _xvm_exception_envbuf;
static const char* _xvm_exception_err = "";

STATIC void _xvm_exception_throw(const char* error_message, int error_code)
{
    // Setup exception error message and make a long jump
    _xvm_exception_err = error_message;
    longjmp(_xvm_exception_envbuf, error_code);
}

STATIC void _xvm_exception_stack_overflow()
{
    _xvm_exception_throw("stack overflow", EXITCODE_STACK_OVERFLOW);
}

STATIC void _xvm_exception_stack_underflow()
{
    _xvm_exception_throw("stack underflow", EXITCODE_STACK_UNDERFLOW);
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
STATIC int xvm_stack_init(xvm_stack* stack)
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
STATIC int xvm_stack_clear(xvm_stack* stack, stack_word_t value)
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
STATIC int xvm_stack_create(xvm_stack* stack, size_t stack_size)
{
    if (stack != NULL && stack->storage == NULL && stack_size != 0)
    {
        stack->stack_size   = stack_size;
        stack->storage      = (stack_word_t*)malloc(sizeof(stack_word_t)*stack_size);
        return 1;
    }
    return 0;
}

STATIC int xvm_stack_free(xvm_stack* stack)
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

INLINE STATIC void xvm_stack_push(xvm_stack* stack, regi_t* reg_sp, stack_word_t value)
{
    stack_word_t* stack_ptr = REG_TO_STACK_PTR(reg_sp);
    if (stack_ptr < stack->storage + stack->stack_size)
    {
        *stack_ptr = value;
        (*reg_sp) += sizeof(stack_word_t);
    }
    else
        _xvm_exception_stack_overflow();
}

INLINE STATIC stack_word_t xvm_stack_pop(xvm_stack* stack, regi_t* reg_sp)
{
    stack_word_t* stack_ptr = REG_TO_STACK_PTR(reg_sp);
    if (stack_ptr > stack->storage)
        (*reg_sp) -= sizeof(stack_word_t);
    else
        _xvm_exception_stack_underflow();
    return *REG_TO_STACK_PTR(reg_sp);
}

INLINE STATIC stack_word_t xvm_stack_read(regi_t reg_sp, int word_offset)
{
    stack_word_t* stack_ptr = (stack_word_t*)reg_sp;
    return stack_ptr[word_offset];
}

INLINE STATIC void xvm_stack_write(regi_t reg_sp, int word_offset, stack_word_t value)
{
    stack_word_t* stack_ptr = (stack_word_t*)reg_sp;
    stack_ptr[word_offset] = value;
}

STATIC void xvm_stack_debug(xvm_stack* stack, size_t first_entry, size_t num_entries)
{
    if (stack != NULL)
    {
        // Print all n-th stack entries
        const size_t n = XVM_MIN(first_entry + num_entries, stack->stack_size);
        for (size_t i = first_entry; i < n; ++i)
            printf("stack[%i] = %i\n", i, stack->storage[i]);
    }
}

STATIC void xvm_stack_debug_float(xvm_stack* stack, size_t first_entry, size_t num_entries)
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


/* ----- Stack Environment ----- */

//! XVM environment state type.
typedef void* xvm_env;

typedef struct
{
    stack_word_t**  ptr_ref;    // reference to stack pointer (register $sp)
    stack_word_t*   begin;      // stack begin
    stack_word_t*   end;        // stack end
}
_xvm_env_internal;

/**
Invocation procedure signature. This is the signature for external procedure invocations in ANSI C.
\param[in] env Environment handle to access the program state (e.g. the virtual stack).
*/
typedef void (*xvm_invocation_proc)(xvm_env env);

//! Null invocation procedure -> throws EXITCODE_INVOCATION_VIOLATION.
STATIC void _xvm_null_invocation(xvm_env env)
{
    _xvm_exception_throw("invocation of unbound external procedure", EXITCODE_INVOCATION_VIOLATION);
}

#if 0

//! Returns the argument as integer, specified by the parameter index (beginning with 1).
STATIC int xvm_env_param_int(xvm_env env, unsigned int param_index)
{
    stack_word_t* ptr = *((_xvm_env_internal*)env)->ptr_ref;
    return *(int*)(ptr - param_index);
}

//! Returns the argument as float, specified by the parameter index (beginning with 1).
STATIC float xvm_env_param_float(xvm_env env, unsigned int param_index)
{
    stack_word_t* ptr = *((_xvm_env_internal*)env)->ptr_ref;
    return *(float*)(ptr - param_index);
}

//! Returns the argument as null-terminated C string, specified by the parameter index (beginning with 1).
STATIC char* xvm_env_param_string(xvm_env env, unsigned int param_index)
{
    stack_word_t* ptr = *((_xvm_env_internal*)env)->ptr_ref;
    return (char*)*(ptr - param_index);
}

//! Returns the argument as raw pointer, specified by the parameter index (beginning with 1).
STATIC void* xvm_env_param_pointer(xvm_env env, unsigned int param_index)
{
    stack_word_t* ptr = *((_xvm_env_internal*)env)->ptr_ref;
    return (void*)*(ptr - param_index);
}

// Pop 'arg_size' words from the stack.
STATIC void xvm_env_return_void(xvm_env env, unsigned int arg_size)
{
    _xvm_env_internal* stack_env = (_xvm_env_internal*)env;

    // Pop arguments from stack
    *stack_env->ptr_ref -= arg_size;
    if (*stack_env->ptr_ref < stack_env->begin)
        _xvm_exception_stack_underflow();
}

// Pop 'arg_size' words from the stack and push 'value' onto the stack.
STATIC void xvm_env_return_int(xvm_env env, unsigned int arg_size, int value)
{
    _xvm_env_internal* stack_env = (_xvm_env_internal*)env;

    // Pop arguments from stack
    *stack_env->ptr_ref -= arg_size;
    if (*stack_env->ptr_ref < stack_env->begin)
        _xvm_exception_stack_underflow();

    // Push result onto stack
    **stack_env->ptr_ref = value;
    ++*stack_env->ptr_ref;
    if (*stack_env->ptr_ref > stack_env->end)
        _xvm_exception_stack_overflow();
}

// Pop 'arg_size' words from the stack and push 'value' onto the stack.
STATIC void xvm_env_return_float(xvm_env env, unsigned int arg_size, float value)
{
    xvm_env_return_int(env, arg_size, FLT_TO_INT_REINTERPRET(value));
}

#define XVM_PARAM_INT(ident, index) int ident = xvm_env_param_int(env, index)
#define XVM_PARAM_FLOAT(ident, index) float ident = xvm_env_param_float(env, index)
#define XVM_PARAM_STRING(ident, index) char* ident = xvm_env_param_string(env, index)

#define XVM_RETURN_INT(value) xvm_env_push_int(env, value)
#define XVM_RETURN_FLOAT(value) xvm_env_push_float(env, value)

#endif


/* ----- Export Address ----- */

//! XVM export procedure address structure.
typedef struct
{
    /**
    Export procedure address. This is the instruction
    INDEX where the procedure starts. By default 0.
    */
    unsigned int    addr;
    
    /**
    Procedure ID name. This depends on the 'name mangling' of
    the high-level compiler (e.g. "main" or "_ZN@main"). By default empty.
    */
    xvm_string      label;
}
xvm_export_address;

//! Initializes the export address with its default values.
STATIC int xvm_export_address_init(xvm_export_address* export_address)
{
    if (export_address != NULL)
    {
        export_address->addr    = 0;
        export_address->label   = xvm_string_init();
        return 1;
    }
    return 0;
}

//! Initializes the export address with the specified startup values.
STATIC int xvm_export_address_setup(xvm_export_address* export_address, unsigned int addr, xvm_string label)
{
    if (export_address != NULL)
    {
        export_address->addr    = addr;
        export_address->label   = label;
        return 1;
    }
    return 0;
}

//! Frees the memory for the specified export address object.
STATIC int xvm_export_address_free(xvm_export_address* export_address)
{
    if (export_address != NULL)
    {
        export_address->addr = 0;
        xvm_string_free(&(export_address->label));
        return 1;
    }
    return 0;
}


/* ----- Import Address ----- */

//! XVM import procedure address structure.
typedef struct
{
    unsigned int    num_indices;    //!< Number of instructions which must be back-patched.
    unsigned int*   indices;        //!< Instruction indices which require to be back-patched.
    xvm_string      label;          //!< Address label to import. This must match any of the export addresses.
}
xvm_import_address;

//! Initializes the import address with its default values.
STATIC int xvm_import_address_init(xvm_import_address* import_address)
{
    if (import_address != NULL)
    {
        import_address->num_indices = 0;
        import_address->indices     = 0;
        import_address->label       = xvm_string_init();
        return 1;
    }
    return 0;
}

/**
Initializes the import address with the specified startup values.
\note All indices are uninitialized!
*/
STATIC int xvm_import_address_setup(xvm_import_address* import_address, unsigned int num_indices)
{
    if (import_address != NULL)
    {
        import_address->num_indices = num_indices;
        import_address->indices     = (unsigned int*)malloc(sizeof(unsigned int) * num_indices);
        import_address->label       = xvm_string_init();
        return 1;
    }
    return 0;
}

//! Frees the memory for the specified import address object.
STATIC int xvm_import_address_free(xvm_import_address* import_address)
{
    if (import_address != NULL)
    {
        if (import_address->indices != NULL)
        {
            free(import_address->indices);
            import_address->indices = NULL;
        }
        import_address->num_indices = 0;
        xvm_string_free(&(import_address->label));
        return 1;
    }
    return 0;
}


/* ----- Byte Code ----- */

//! XVM byte code structure.
typedef struct
{
    unsigned int            num_instructions;       //!< Number of instructions. By default 0.
    instr_t*                instructions;           //!< Instruction array. By default NULL.

    unsigned int            num_export_addresses;   //!< Number of export addresses. By default 0.
    xvm_export_address*     export_addresses;       //!< Export address array. By default NULL.

    unsigned int            num_import_addresses;   //!< Number of import addresses. By default 0.
    xvm_import_address*     import_addresses;       //!< Import address array. By default NULL.

    unsigned int            num_invoke_idents;      //!< Number of invocation identifiers. By default 0.
    xvm_string*             invoke_idents;          //!< Invocation identifiers. By default NULL.
    xvm_invocation_proc*    invoke_bindings;        //!< Invocation procedure bindings. By default NULL.
}
xvm_bytecode;

/**
Initializes the specified byte code structure.
\code
xvm_bytecode byte_code;
xvm_bytecode_init(&byte_code);
xvm_bytecode_create_instructions(&byte_code, num_instructions);
xvm_bytecode_create_export_addresses(&byte_code, num_export_addresses);
xvm_bytecode_create_import_addresses(&byte_code, num_import_addresses);
// ...
xvm_bytecode_free(&byte_code);
\endcode
*/
STATIC int xvm_bytecode_init(xvm_bytecode* byte_code)
{
    if (byte_code != NULL)
    {
        // Initialize byte code data
        byte_code->num_instructions     = 0;
        byte_code->instructions         = NULL;
        
        byte_code->num_export_addresses = 0;
        byte_code->export_addresses     = NULL;

        byte_code->num_import_addresses = 0;
        byte_code->import_addresses     = NULL;

        byte_code->num_invoke_idents    = 0;
        byte_code->invoke_idents        = NULL;
        byte_code->invoke_bindings      = NULL;
        return 1;
    }
    return 0;
}

/**
Allocates memory for the specified amount of byte code instructions.
\param[in,out] byte_code Pointer to the byte code object.
\param[in] num_instructions Specifies the number of instruction to allocate for the byte code.
\see xvm_bytecode_init
\note All instructions are uninitialized!
*/
STATIC int xvm_bytecode_create_instructions(xvm_bytecode* byte_code, unsigned int num_instructions)
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
\param[in] num_export_addresses Specifies the number of export addresses to allocate for the byte code.
\see xvm_bytecode_init
\see xvm_export_address
\note All export addresses are uninitialized!
*/
STATIC int xvm_bytecode_create_export_addresses(xvm_bytecode* byte_code, unsigned int num_export_addresses)
{
    if (byte_code != NULL && byte_code->export_addresses == NULL && num_export_addresses > 0)
    {
        byte_code->num_export_addresses = num_export_addresses;
        byte_code->export_addresses     = (xvm_export_address*)malloc(sizeof(xvm_export_address)*num_export_addresses);
        return 1;
    }
    return 0;
}

/**
Allocates memory for the specified amount of byte code import addresses.
\param[in,out] byte_code Pointer to the byte code object.
\param[in] num_export_addresses Specifies the number of export addresses to allocate for the byte code.
\see xvm_bytecode_init
\see xvm_import_address
\note All import addresses are uninitialized!
*/
STATIC int xvm_bytecode_create_import_addresses(xvm_bytecode* byte_code, unsigned int num_import_addresses)
{
    if (byte_code != NULL && byte_code->import_addresses == NULL && num_import_addresses > 0)
    {
        byte_code->num_import_addresses = num_import_addresses;
        byte_code->import_addresses     = (xvm_import_address*)malloc(sizeof(xvm_import_address)*num_import_addresses);
        return 1;
    }
    return 0;
}

//! Returns the export address with the specified label or NULL if there is no such export address.
STATIC const xvm_export_address* xvm_bytecode_find_export_address(const xvm_bytecode* byte_code, const char* label)
{
    if (byte_code != NULL && byte_code->export_addresses != NULL)
    {
        for (unsigned int i = 0; i < byte_code->num_export_addresses; ++i)
        {
            const xvm_export_address* export_addr = &(byte_code->export_addresses[i]);
            if (strcmp(export_addr->label.str, label) == 0)
                return export_addr;
        }
    }
    return NULL;
}

#if 0 // !INCOMPLETE!

/**
Dynamically links the client byte-code to the main byte-code.
This function will resolve as much import address (from both sides) as possible.
\note If the program termination of 'byte_code' only works when the program counter
runs at the end of the code, this may no longer work, when the client code is appended!
\see xvm_bytecode_create_export_addresses
\see xvm_bytecode_create_import_addresses
*/
STATIC int xvm_bytecode_dynamic_link(xvm_bytecode* byte_code, xvm_bytecode* client_byte_code)
{
    if ( byte_code == NULL || byte_code->instructions == NULL ||
         client_byte_code == NULL || client_byte_code->instructions == NULL )
    {
        return 0;
    }

    // Move instruction indices for client code
    int offset = (int)byte_code->num_instructions;
    instr_t* client_instrs = client_byte_code->instructions;

    for (unsigned int i = 0; i < client_byte_code->num_instructions; ++i)
        client_instrs[i] = _xvm_instr_move(client_instrs[i], offset);

    return 1;
}

#endif

/**
Allocates memory for the specified amount of byte code invocation identifiers and bindings.
\param[in,out] byte_code Pointer to the byte code object.
\param[in] num_invoke_idents Specifies the number of invocation identifiers to allocate for the byte code.
\see xvm_bytecode_init
\note All invocation identifiers are uninitialized!
But all invocation bindings will be initialized to a default procedure.
*/
STATIC int xvm_bytecode_create_invocations(xvm_bytecode* byte_code, unsigned int num_invoke_idents)
{
    if (byte_code != NULL && byte_code->invoke_idents == NULL && num_invoke_idents > 0)
    {
        // Create invocation identifiers
        byte_code->num_invoke_idents    = num_invoke_idents;
        byte_code->invoke_idents        = (xvm_string*)malloc(sizeof(xvm_string)*num_invoke_idents);

        // Create and initialize invocation bindings
        byte_code->invoke_bindings      = (xvm_invocation_proc*)malloc(sizeof(xvm_invocation_proc)*num_invoke_idents);
        for (unsigned int i = 0; i < num_invoke_idents; ++i)
            byte_code->invoke_bindings[i] = _xvm_null_invocation;
        return 1;
    }
    return 0;
}

STATIC int xvm_bytecode_bind_invocation(xvm_bytecode* byte_code, const char* ident, xvm_invocation_proc proc)
{
    if (byte_code != NULL && ident != NULL)
    {
        // Check byte code fields
        if (byte_code->invoke_bindings == NULL || byte_code->invoke_idents == NULL || byte_code->num_invoke_idents == 0)
            return 0;

        // Find identifier in byte code
        unsigned int i = 0;
        for (; i < byte_code->num_invoke_idents; ++i)
        {
            if (strcmp(byte_code->invoke_idents[i].str, ident) == 0)
                break;
        }

        if (i >= byte_code->num_invoke_idents)
            return 0;

        // Bind invocation
        if (proc == NULL)
            proc = _xvm_null_invocation;

        byte_code->invoke_bindings[i] = proc;

        return 1;
    }
    return 0;
}

//! Frees the memory for the specified byte code object.
STATIC int xvm_bytecode_free(xvm_bytecode* byte_code)
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
            // Free each each export address
            for (unsigned int i = 0; i < byte_code->num_export_addresses; ++i)
                xvm_export_address_free(&(byte_code->export_addresses[i]));

            // Free export address list
            free(byte_code->export_addresses);
            byte_code->export_addresses     = NULL;
            byte_code->num_export_addresses = 0;
        }

        if (byte_code->import_addresses != NULL)
        {
            // Free each each import address
            for (unsigned int i = 0; i < byte_code->num_import_addresses; ++i)
                xvm_import_address_free(&(byte_code->import_addresses[i]));

            // Free import address list
            free(byte_code->import_addresses);
            byte_code->import_addresses     = NULL;
            byte_code->num_import_addresses = 0;
        }

        if (byte_code->invoke_idents != NULL)
        {
            // Free each invocation identifier
            for (unsigned int i = 0; i < byte_code->num_invoke_idents; ++i)
                xvm_string_free(&(byte_code->invoke_idents[i]));

            // Free invocation identifier list
            free(byte_code->invoke_idents);
            byte_code->invoke_idents        = NULL;
            byte_code->num_invoke_idents    = 0;
        }

        if (byte_code->invoke_bindings != NULL)
        {
            // Free invocation bindings vector
            free(byte_code->invoke_bindings);
            byte_code->invoke_bindings = NULL;
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
xvm_bytecode_datafield_ascii(NULL, "Hello, World\n", &num_instr);
// num_instr == 4  ->  { "Hell", "o, W", "orld", "\n\0\0\0" }
xvm_bytecode_datafield_ascii(byte_code.instructions + current_instr_offset, "Hello, World\n", NULL);
\endcode
*/
STATIC int xvm_bytecode_datafield_ascii(instr_t* instr_ptr, const char* text, size_t* num_instructions)
{
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

    if (instr_ptr == NULL)
        return 1;

    // Fill data field
    for (; num_instr > 1; --num_instr)
    {
        *instr_ptr = *((instr_t*)text);
        ++instr_ptr;
        text += 4;
    }

    // Fill last data field
    *instr_ptr = 0;
    instr_byte = (char*)instr_ptr;

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

--- XBC file format spec (Version 1.31): ---

WORD: magic number (Must be *(int*)"XBCF")
WORD: version number (Must be 131 for "1.31")
WORD: number of instructions (n)
n times:
    WORD: instruction

--- XBC file format spec (Version 1.32): ---

WORD: magic number (Must be *(int*)"XBCF")
WORD: version number (Must be 132 for "1.32")
WORD: number of instructions (n1)
n1 times:
    WORD: instruction
WORD: number of export addresses (n2)
n2 times:
    WORD: address
    STR: name

--- XBC file format spec (Version 1.33): ---

WORD: magic number (Must be *(int*)"XBCF")
WORD: version number (Must be 133 for "1.33")
WORD: number of instructions (n1)
n1 times:
    WORD: instruction
WORD: number of export addresses (n2)
n2 times:
    WORD: address
    STR: name
WORD: number of invoke identifiers (n3)
n3 times:
    STR: identifier

--- XBC file format spec (Version 1.34): ---

WORD: magic number (Must be *(int*)"XBCF")
WORD: version number (Must be 134 for "1.34")
WORD: number of instructions (n1)
n1 times:
    WORD: instruction
WORD: number of export addresses (n2)
n2 times:
    WORD: address
    STR: name
WORD: number of import addresses (n3)
n3 times:
    WORD: number of instruction indices (n3b)
    n3b times:
        WORD: index
    STR: name
WORD: number of invoke identifiers (n4)
n4 times:
    STR: identifier

*/

#define XBC_FORMAT_MAGIC            (*((int*)("XBCF")))
#define XBC_FORMAT_VERSION_1_31     131
#define XBC_FORMAT_VERSION_1_32     132
#define XBC_FORMAT_VERSION_1_33     133
#define XBC_FORMAT_VERSION_1_34     134//!INCOMPLETE!
#define XBC_FORMAT_VERSION_LATEST   XBC_FORMAT_VERSION_1_33

/**
Reads a byte code form file.
\param[out] byte_code Specifies the resulting byte code object.
\param[in] filename Specifies the filename from which the byte code is to be read.
\return Zero on failure and non-zero otherwise.
*/
STATIC int xvm_bytecode_read_from_file(xvm_bytecode* byte_code, const char* filename)
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
    if (version != XBC_FORMAT_VERSION_1_31 && version != XBC_FORMAT_VERSION_1_32 && version != XBC_FORMAT_VERSION_1_33)
    {
        xvm_log_error("invalid version number (must be 1.31, 1.32, or 1.33)");
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
    if (version >= XBC_FORMAT_VERSION_1_32)
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
                // Read address and label and store it into the export address
                xvm_export_address* export_addr = &(byte_code->export_addresses[i]);

                unsigned int addr = xvm_file_read_uint(file);
                xvm_string label = xvm_string_read_from_file(file);

                xvm_export_address_setup(export_addr, addr, label);
            }
        }
    }

    // Read import addresses
    if (version >= XBC_FORMAT_VERSION_1_34)
    {
        unsigned int num_import_addr = xvm_file_read_uint(file);

        if (num_import_addr > 0)
        {
            if (xvm_bytecode_create_import_addresses(byte_code, num_import_addr) == 0)
            {
                xvm_log_error("creating byte code import addresses failed");
                fclose(file);
                return 0;
            }

            for (unsigned int i = 0; i < num_import_addr; ++i)
            {
                // Read indices and label and store it into the import address
                xvm_import_address* import_addr = &(byte_code->import_addresses[i]);

                unsigned int num_indices = xvm_file_read_uint(file);
                xvm_import_address_setup(import_addr, num_indices);

                for (unsigned int j = 0; j < num_indices; ++j)
                    import_addr->indices[j] = xvm_file_read_uint(file);

                import_addr->label = xvm_string_read_from_file(file);
            }
        }
    }

    // Read invocation identifiers
    if (version >= XBC_FORMAT_VERSION_1_33)
    {
        unsigned int num_invoke_idents = xvm_file_read_uint(file);

        if (num_invoke_idents > 0)
        {
            if (xvm_bytecode_create_invocations(byte_code, num_invoke_idents) == 0)
            {
                xvm_log_error("creating byte code invocation identifiers failed");
                fclose(file);
                return 0;
            }

            for (unsigned int i = 0; i < num_invoke_idents; ++i)
            {
                // Read string and store it into the invocation identifier
                byte_code->invoke_idents[i] = xvm_string_read_from_file(file);
            }
        }
    }

    // Close file and return with success
    fclose(file);

    return 1;
}

/**
Writes the specified byte code to file.
\param[in] byte_code specifies the byte code which is to be written to file.
\param[in] filename Specifies the filename which is to be used to create the file.
\param[in] version Specifies the format version. Must be one of the following values:
XBC_FORMAT_VERSION_1_11, XBC_FORMAT_VERSION_1_12, XBC_FORMAT_VERSION_1_13, or XBC_FORMAT_VERSION_LATEST.
\return Zero on failure and non-zero otherwise.
*/
STATIC int xvm_bytecode_write_to_file(const xvm_bytecode* byte_code, const char* filename, unsigned int version)
{
    // Check arguments
    if (byte_code == NULL || byte_code->instructions == NULL || filename == NULL)
    {
        xvm_log_error("invalid arguments to write byte code");
        return 0;
    }
    if (version != XBC_FORMAT_VERSION_1_31 && version != XBC_FORMAT_VERSION_1_32 && version != XBC_FORMAT_VERSION_1_33)
    {
        xvm_log_error("invalid version number (must be 1.31, 1.32, or 1.33)");
        return 0;
    }

    // Open file for writing
    FILE* file = fopen(filename, "wb");
    if (file == NULL)
    {
        xvm_log_error("unable to open file for writing");
        return 0;
    }

    // Write magic number
    xvm_file_write_uint(file, XBC_FORMAT_MAGIC);

    // Write version number
    xvm_file_write_uint(file, version);

    // Write instructions
    unsigned int num_instr = byte_code->num_instructions;
    xvm_file_write_uint(file, num_instr);

    fwrite(byte_code->instructions, sizeof(instr_t), num_instr, file);

    // Write export addresses
    if (version >= XBC_FORMAT_VERSION_1_32)
    {
        unsigned int num_export_addr = byte_code->num_export_addresses;
        xvm_file_write_uint(file, num_export_addr);

        for (unsigned int i = 0; i < num_export_addr; ++i)
        {
            const xvm_export_address* export_addr = &(byte_code->export_addresses[i]);

            xvm_file_write_uint(file, export_addr->addr);
            xvm_string_write_to_file(export_addr->label, file);
        }
    }

    // Write import addresses
    if (version >= XBC_FORMAT_VERSION_1_34)
    {
        unsigned int num_import_addr = byte_code->num_import_addresses;
        xvm_file_write_uint(file, num_import_addr);

        for (unsigned int i = 0; i < num_import_addr; ++i)
        {
            const xvm_import_address* import_addr = &(byte_code->import_addresses[i]);

            unsigned int num_indices = import_addr->num_indices;
            const unsigned int* indices = import_addr->indices;

            xvm_file_write_uint(file, num_indices);
            for (unsigned int j = 0; j < num_indices; ++j)
                xvm_file_write_uint(file, indices[j]);

            xvm_string_write_to_file(import_addr->label, file);
        }
    }

    // Write invocation identifiers
    if (version >= XBC_FORMAT_VERSION_1_33)
    {
        unsigned int num_invoke_idents = byte_code->num_invoke_idents;
        xvm_file_write_uint(file, num_invoke_idents);

        for (unsigned int i = 0; i < num_invoke_idents; ++i)
            xvm_string_write_to_file(byte_code->invoke_idents[i], file);
    }

    // Close file and return with success
    fclose(file);

    return 1;
}


/* ----- Module ----- */

typedef int (*xvm_module_proc_count_fnc)(void);
typedef xvm_invocation_proc (*xvm_module_fetch_proc_fnc)(int); 
typedef const char* (*xvm_module_fetch_ident_fnc)(int); 

//! XVM module library structure.
typedef struct
{
    #if defined(_WIN32)
    HMODULE                     handle;
    #elif defined(__linux__)
    void*                       handle;
    #endif
    xvm_module_proc_count_fnc   proc_count;
    xvm_module_fetch_proc_fnc   fetch_proc;
    xvm_module_fetch_ident_fnc  fetch_ident;
}
xvm_module;

//! Initializes the specified module with its default values.
STATIC int xvm_module_init(xvm_module* module)
{
    if (module != NULL)
    {
        #if defined(_WIN32)
        module->handle      = 0;
        #elif defined(__linux__)
        module->handle      = NULL;
        #endif
        module->proc_count  = NULL;
        module->fetch_proc  = NULL;
        module->fetch_ident = NULL;
        return 1;
    }
    return 0;
}

/**
Loads the specified module library filename.
\param[in] filename Specifies the XieXie module.
This must be a dynamic library (*.dll file on Win32, *.so file on GNU/Linux).
The library must contain functions with the following interfaces:
\code
int xx_module_proc_count();
xvm_invocation_proc xx_module_fetch_proc(int index);
const char* xx_module_fetch_ident(int index);
\endcode
*/
STATIC int xvm_module_load(xvm_module* module, const char* filename)
{
    if (module == NULL || filename == NULL)
        return 0;

    #if defined(_WIN32)
    
    // Load dynamic library
    module->handle = LoadLibraryA(filename);
    if (module->handle == 0)
    {
        printf("error: loading module \"%s\" failed\n", filename);
        return 0;
    }

    // Load module interface
    module->proc_count = (xvm_module_proc_count_fnc)GetProcAddress(module->handle, "xx_module_proc_count");
    module->fetch_proc = (xvm_module_fetch_proc_fnc)GetProcAddress(module->handle, "xx_module_fetch_proc");
    module->fetch_ident = (xvm_module_fetch_ident_fnc)GetProcAddress(module->handle, "xx_module_fetch_ident");

    #elif defined(__linux__)

    // Load dynamic library
    module->handle = dlopen(filename, RTLD_LAZY);
    if (module->handle == NULL)
    {
        printf("error: loading module \"%s\" failed\n", filename);
        return 0;
    }

    // Load module interface
    module->proc_count = (xvm_module_proc_count_fnc)dlsym(module->handle, "xx_module_proc_count");
    module->fetch_proc = (xvm_module_fetch_proc_fnc)dlsym(module->handle, "xx_module_fetch_proc");
    module->fetch_ident = (xvm_module_fetch_ident_fnc)dlsym(module->handle, "xx_module_fetch_ident");

    #endif

    if (module->proc_count == NULL || module->fetch_proc == NULL || module->fetch_ident == NULL)
    {
        printf("error: loading interface of module \"%s\" failed\n", filename);
        return 0;
    }

    return 1;
}

STATIC int xvm_module_unload(xvm_module* module)
{
    if (module != NULL)
    {
        #if defined(_WIN32)

        if (module->handle != 0)
        {
            FreeLibrary(module->handle);
            module->handle = 0;
        }

        #elif defined(__linux__)

        if (module->handle != NULL)
        {
            dlclose(module->handle);
            module->handle = NULL;
        }

        #endif

        module->proc_count  = NULL;
        module->fetch_proc  = NULL;
        module->fetch_ident = NULL;

        return 1;
    }
    return 0;
}

STATIC int _xvm_bytecode_bind_module_ext(xvm_bytecode* byte_code, const xvm_module* module, int unbindFlag)
{
    if (byte_code == NULL || module == NULL)
        return 0;

    if (module->proc_count == NULL || module->fetch_proc == NULL || module->fetch_ident == NULL)
    {
        xvm_log_error("can not bind/unbind module with invalid interface");
        return 0;
    }

    // Fetch number of invocation procedures
    int num_procs = module->proc_count();

    // Bind all module invocation procedures to byte code
    size_t num_errors = 0;

    for (int i = 0; i < num_procs; ++i)
    {
        // Fetch invocation identifier
        const char* ident = module->fetch_ident(i);
        if (ident == NULL)
            return 0;

        if (unbindFlag != 0)
        {
            // Unbind procedure invocation
            if (xvm_bytecode_bind_invocation(byte_code, ident, NULL) == 0)
                ++num_errors;
        }
        else
        {
            // Fetch procedure pointer
            xvm_invocation_proc proc = module->fetch_proc(i);

            // Bind procedure invocation
            if (xvm_bytecode_bind_invocation(byte_code, ident, proc) == 0)
                ++num_errors;
        }
    }

    // Check for errors during binding/unbinding
    if (num_errors > 0)
    {
        printf("error: %i invocation binding(s)/unbinding(s) from module to byte code failed\n", num_errors);
        return 0;
    }

    return 1;
}

STATIC int xvm_bytecode_bind_module(xvm_bytecode* byte_code, const xvm_module* module)
{
    return _xvm_bytecode_bind_module_ext(byte_code, module, 0);
}

STATIC int xvm_bytecode_unbind_module(xvm_bytecode* byte_code, const xvm_module* module)
{
    return _xvm_bytecode_bind_module_ext(byte_code, module, 1);
}


/* ----- Moduel Container ------ */

typedef struct _xvm_module_container_node
{
    xvm_module                          module;
    struct _xvm_module_container_node*  next;
}
_xvm_module_container_node;

//! XVM module contianer structure.
typedef struct xvm_module_container
{
    _xvm_module_container_node* first;
}
xvm_module_container;

//! Initializes the specified module container with its default values.
STATIC int xvm_module_container_init(xvm_module_container* container)
{
    if (container != NULL)
    {
        container->first = NULL;
        return 1;
    }
    return 0;
}

STATIC int xvm_module_container_add(xvm_module_container* container, xvm_module module)
{
    if (container != NULL)
    {
        _xvm_module_container_node* node = container->first;

        if (node == NULL)
        {
            // Create first node
            container->first = (_xvm_module_container_node*)malloc(sizeof(_xvm_module_container_node));
            node = container->first;
        }
        else
        {
            // Find last node
            while (node->next != NULL)
                node = node->next;

            // Create next node
            node->next = (_xvm_module_container_node*)malloc(sizeof(_xvm_module_container_node));
            node = node->next;
        }

        // Add module to new node and initialize its link
        node->module    = module;
        node->next      = NULL;

        return 1;
    }
    return 0;
}

STATIC int _xvm_module_container_node_free(_xvm_module_container_node* node)
{
    if (node != NULL)
    {
        // Unload module
        xvm_module_unload(&(node->module));

        // Check if there is a next node
        int result = 1;
        if (node->next != NULL)
        {
            // Free next node
            result = _xvm_module_container_node_free(node->next);
            free(node->next);
        }

        return result;
    }
    return 0;
}

/**
Clears the specified container and unloads all modules.
\note No byte code must then use any module which was loaded with this container!
*/
STATIC int xvm_module_container_clear(xvm_module_container* container)
{
    if (container != NULL)
    {
        if (container->first != NULL)
        {
            // Free node hierarchy
            int result = _xvm_module_container_node_free(container->first);

            // Free first node
            free(container->first);
            container->first = NULL;

            return result;
        }
        return 1;
    }
    return 0;
}

static _xvm_module_container_node* _xvm_module_iterator = NULL;

STATIC int xvm_module_iteration_start(const xvm_module_container* container)
{
    if (container != NULL)
    {
        _xvm_module_iterator = container->first;
        return 1;
    }
    return 0;
}

STATIC xvm_module* xvm_module_iteration_next()
{
    if (_xvm_module_iterator != NULL)
    {
        xvm_module* module = &(_xvm_module_iterator->module);
        _xvm_module_iterator = _xvm_module_iterator->next;
        return module;
    }
    return NULL;
}


/* ----- Virtual machine ----- */

STATIC void _xvm_call_intrinsic(unsigned int intrsc_id, xvm_stack* const stack, regi_t* const reg_sp, regi_t* const reg_ar)
{
    switch (intrsc_id)
    {
        /* --- Dynamic memory intrinsics --- */

        // void* AllocMem(uint sizeInBytes)
        case INSC_ALLOC_MEM:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            void* result = malloc((size_t)arg0);
            *reg_ar = (regi_t)result;
        }
        break;

        // void FreeMem(void* memoryAddress)
        case INSC_FREE_MEM:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            if (arg0 != 0)
                free((void*)arg0);
        }
        break;

        // void CopyMem(void* dstMemAddr, const void* srcMemAddr, uint sizeInBytes)
        case INSC_COPY_MEM:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            int arg1 = xvm_stack_pop(stack, reg_sp);
            int arg2 = xvm_stack_pop(stack, reg_sp);
            memcpy((void*)arg0, (const void*)arg1, (size_t)arg2);
        }
        break;

        // void FillMem(void* memoryAddress, uint sizeInByzes, int value)
        case INSC_FILL_MEM:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            int arg1 = xvm_stack_pop(stack, reg_sp);
            int arg2 = xvm_stack_pop(stack, reg_sp);
            memset((void*)arg0, arg2, (size_t)arg1);
        }
        break;

        /* --- Console intrinsics --- */

        case INSC_SYS_CALL:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            system(INT_TO_STR_REINTERPRET(arg0));
        }
        break;

        case INSC_PRINT:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            printf("%s", INT_TO_STR_REINTERPRET(arg0));
        }
        break;

        case INSC_PRINT_LN:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            puts(INT_TO_STR_REINTERPRET(arg0));
        }
        break;

        case INSC_PRINT_INT:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            printf("%i", arg0);
        }
        break;

        case INSC_PRINT_FLOAT:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            printf("%f", INT_TO_FLT_REINTERPRET(arg0));
        }
        break;

        case INSC_INPUT:
        {
            //todo...
        }
        break;

        case INSC_INPUT_INT:
        {
            scanf("%i", reg_ar);
        }
        break;

        case INSC_INPUT_FLOAT:
        {
            scanf("%f", reg_ar);
        }
        break;

        /* --- File intrinsics --- */

        //!!!!!!!!! TODO !!!!!!!!!
        case INSC_CREATE_FILE:
        case INSC_DELETE_FILE:
        case INSC_OPEN_FILE:
        case INSC_CLOSE_FILE:
        case INSC_FILE_SIZE:
        case INSC_SET_FILE_POS:
        case INSC_GET_FILE_POS:
        case INSC_FILE_EOF:
        case INSC_WRITE_BYTE:
        case INSC_WRITE_WORD:
        case INSC_READ_BYTE:
        case INSC_READ_WORD:
        break;

        /* --- Math intrinsics --- */

        case INSC_SIN:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            float result = sinf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        case INSC_COS:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            float result = cosf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        case INSC_TAN:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            float result = tanf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        case INSC_ASIN:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            float result = asinf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        case INSC_ACOS:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            float result = acosf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        case INSC_ATAN:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            float result = atanf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        case INSC_POW:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            int arg1 = xvm_stack_pop(stack, reg_sp);
            float result = powf(INT_TO_FLT_REINTERPRET(arg0), INT_TO_FLT_REINTERPRET(arg1));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        case INSC_SQRT:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            float result = sqrtf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        /* --- Other intrinsics --- */
        
        case INSC_RAND_INT:
        {
            *reg_ar = rand();
        }
        break;

        case INSC_RAND_FLOAT:
        {
            float result = ((float)rand()) / RAND_MAX;
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        case INSC_TIME:
        {
            clock_t ticks = clock() / (CLOCKS_PER_SEC / 1000);
            *reg_ar = (int)ticks;
        }
        break;

        #ifdef _ENABLE_OS_FEATURES_

        case INSC_SLEEP:
        {
            int arg0 = xvm_stack_pop(stack, reg_sp);
            #if defined(_WIN32)
            Sleep((DWORD)arg0);
            #elif defined(__linux__)
            usleep(((unsigned int)arg0) * 1000);
            #endif
        }
        break;

        #endif

        default:
            _xvm_exception_throw("invalid intrinsic", EXITCODE_INVALID_INTRINSIC);
            break;
    }
}

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
STATIC xvm_exit_codes xvm_execute_program_ext(
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
        regi_t i[XVM_NUM_REGISTERS];
        regf_t f[XVM_NUM_REGISTERS];
    }
    reg;

    const int num_instr = (int)byte_code->num_instructions;

    const regi_t instr_ptr_begin    = (regi_t)byte_code->instructions;
    const regi_t instr_ptr_end      = instr_ptr_begin + num_instr*4;

    regi_t* const reg_ar = (reg.i + REG_AR); // Reference to '$ar' register
    regi_t* const reg_cf = (reg.i + REG_CF); // Reference to '$cf' register
    regi_t* const reg_lb = (reg.i + REG_LB); // Reference to '$lb' register
    regi_t* const reg_sp = (reg.i + REG_SP); // Reference to '$sp' register
    regi_t* const reg_pc = (reg.i + REG_PC); // Reference to '$pc' register

    // Program start pointer is used to load memory from program "DATA" section
    const byte_t* const program_start_ptr = (const byte_t*)(byte_code->instructions);

    // Invocation environment
    _xvm_env_internal stack_env;
    stack_env.ptr_ref   = (stack_word_t**)reg_sp;
    stack_env.begin     = stack->storage;
    stack_env.end       = stack->storage + stack->stack_size;

    /* --- Temporary memory --- */
    instr_t         instr;          // Current instruction
    opcode_t        opcode;         // Current opcode
    reg_t           reg0;           // Register0 (Destination)
    reg_t           reg1;           // Register1 (LSource)
    reg_t           reg2;           // Register2 (RSource)

    int             sgn_value;      // Signed value
    unsigned int    unsgn_value;    // Unsigned value
    int             extra_value;    // Extra value (for 'call' and 'ret' instructions)

    union
    {
        byte_t*         byte;   // Memory address pointer (byte aligned)
        const byte_t*   cbyte;  // Memory address constant pointer (byte aligned)

        half_t*         half;   // Memory address pointer (half-word aligned)
        const half_t*   chalf;  // Memory address constant pointer (half-word aligned)

        word_t*         word;   // Memory address pointer (word aligned)
        const word_t*   cword;  // Memory address constant pointer (word aligned)
    }
    mem_addr;

    /* --- Initialize VM (only reset reserved registers) --- */
    *reg_lb = (regi_t)stack->storage;
    *reg_sp = (regi_t)stack->storage;
    *reg_pc = instr_ptr_begin;

    if (entry_point != NULL)
        *reg_pc += (entry_point->addr << 2);

    /* --- Catch exceptions --- */
    int exception_val = setjmp(_xvm_exception_envbuf);
    if (exception_val != 0)
    {
        xvm_log_error(_xvm_exception_err);
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
            _xvm_instr_print_debug_info(instr, prev_pc_index, reg.i);
        #endif

        /* Load next instruction */
        instr = *((instr_t*)(*reg_pc));

        opcode = _xvm_instr_get_opcode(instr);

        #ifdef _ENABLE_RUNTIME_DEBUGGER_
        prev_pc_index = *reg_pc - instr_ptr_begin;
        #endif

        /* Execute current instruction */
        switch (opcode)
        {
            /* --- 3-register opcodes --- */

            // Undefined behavior for floats
            case OPCODE_AND3:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg2 = _xvm_instr_get_reg2(instr);
                reg.i[reg0] = reg.i[reg1] & reg.i[reg2];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_OR3:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg2 = _xvm_instr_get_reg2(instr);
                reg.i[reg0] = reg.i[reg1] | reg.i[reg2];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_XOR3:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg2 = _xvm_instr_get_reg2(instr);
                reg.i[reg0] = reg.i[reg1] ^ reg.i[reg2];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_ADD3:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg2 = _xvm_instr_get_reg2(instr);
                reg.i[reg0] = reg.i[reg1] + reg.i[reg2];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_SUB3:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg2 = _xvm_instr_get_reg2(instr);
                reg.i[reg0] = reg.i[reg1] - reg.i[reg2];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_MUL3:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg2 = _xvm_instr_get_reg2(instr);
                reg.i[reg0] = reg.i[reg1] * reg.i[reg2];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_DIV3:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg2 = _xvm_instr_get_reg2(instr);
                sgn_value = reg.i[reg2];
                if (sgn_value == 0)
                    _xvm_exception_throw("division by zero (DIV3 instruction)", EXITCODE_DIVISION_BY_ZERO);
                reg.i[reg0] = reg.i[reg1] / sgn_value;
            }
            break;

            // Undefined behavior for floats
            case OPCODE_MOD3:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg2 = _xvm_instr_get_reg2(instr);
                sgn_value = reg.i[reg2];
                if (sgn_value == 0)
                    _xvm_exception_throw("division by zero (MOD3 instruction)", EXITCODE_DIVISION_BY_ZERO);
                reg.i[reg0] = reg.i[reg1] % sgn_value;
            }
            break;

            // Undefined behavior for floats
            case OPCODE_SLL3:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg2 = _xvm_instr_get_reg2(instr);
                reg.i[reg0] = reg.i[reg1] << reg.i[reg2];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_SLR3:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg2 = _xvm_instr_get_reg2(instr);
                reg.i[reg0] = reg.i[reg1] >> reg.i[reg2];
            }
            break;

            case OPCODE_ADDF:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg2 = _xvm_instr_get_reg2(instr);
                reg.f[reg0] = reg.f[reg1] + reg.f[reg2];
            }
            break;

            case OPCODE_SUBF:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg2 = _xvm_instr_get_reg2(instr);
                reg.f[reg0] = reg.f[reg1] - reg.f[reg2];
            }
            break;

            case OPCODE_MULF:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg2 = _xvm_instr_get_reg2(instr);
                reg.f[reg0] = reg.f[reg1] * reg.f[reg2];
            }
            break;

            case OPCODE_DIVF:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg2 = _xvm_instr_get_reg2(instr);
                reg.f[reg0] = reg.f[reg1] / reg.f[reg2];
            }
            break;

            case OPCODE_MODF:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg2 = _xvm_instr_get_reg2(instr);
                reg.f[reg0] = fmodf(reg.f[reg1], reg.f[reg2]);
            }
            break;

            /* --- 2-register opcodes --- */

            case OPCODE_MOV2:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg.i[reg0] = reg.i[reg1];
            }
            break;

            // Undefined behavior for floats
            case OPCODE_NOT:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg.i[reg0] = ~reg.i[reg1];
            }
            break;

            case OPCODE_FTI:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg.i[reg0] = (regi_t)reg.f[reg1];
            }
            break;

            case OPCODE_ITF:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg.f[reg0] = (regf_t)reg.i[reg1];
            }
            break;
            
            // Undefined behavior for floats
            case OPCODE_AND2:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg.i[reg0] = reg.i[reg1] & _xvm_instr_get_sgn_value16(instr);
            }
            break;

            // Undefined behavior for floats
            case OPCODE_OR2:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg.i[reg0] = reg.i[reg1] | _xvm_instr_get_sgn_value16(instr);
            }
            break;

            // Undefined behavior for floats
            case OPCODE_XOR2:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg.i[reg0] = reg.i[reg1] ^ _xvm_instr_get_sgn_value16(instr);
            }
            break;

            case OPCODE_ADD2:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg.i[reg0] = reg.i[reg1] + _xvm_instr_get_sgn_value16(instr);
            }
            break;

            case OPCODE_SUB2:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg.i[reg0] = reg.i[reg1] - _xvm_instr_get_sgn_value16(instr);
            }
            break;

            case OPCODE_MUL2:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg.i[reg0] = reg.i[reg1] * _xvm_instr_get_sgn_value16(instr);
            }
            break;

            case OPCODE_DIV2:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                sgn_value = _xvm_instr_get_sgn_value16(instr);
                if (sgn_value == 0)
                    _xvm_exception_throw("division by zero (DIV2 instruction)", EXITCODE_DIVISION_BY_ZERO);
                reg.i[reg0] = reg.i[reg1] / sgn_value;
            }
            break;

            // Undefined behavior for floats
            case OPCODE_MOD2:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                sgn_value = _xvm_instr_get_sgn_value16(instr);
                if (sgn_value == 0)
                    _xvm_exception_throw("division by zero (MOD2 instruction)", EXITCODE_DIVISION_BY_ZERO);
                reg.i[reg0] = reg.i[reg1] % sgn_value;
            }
            break;

            // Undefined behavior for floats
            case OPCODE_SLL2:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg.i[reg0] = reg.i[reg1] << _xvm_instr_get_sgn_value16(instr);
            }
            break;

            // Undefined behavior for floats
            case OPCODE_SLR2:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                reg.i[reg0] = reg.i[reg1] >> _xvm_instr_get_sgn_value16(instr);
            }
            break;

            case OPCODE_CMP:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                *reg_cf = (reg.i[reg0] - reg.i[reg1]);
            }
            break;

            case OPCODE_CMPF:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                *reg_cf = xvm_flt2int_signum(reg.f[reg0] - reg.f[reg1]);
            }
            break;

            /* --- 1-register opcodes --- */

            case OPCODE_PUSH:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                xvm_stack_push(stack, reg_sp, reg.i[reg0]);
            }
            break;

            case OPCODE_POP:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg.i[reg0] = xvm_stack_pop(stack, reg_sp);
            }
            break;

            case OPCODE_INC:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                ++reg.i[reg0];
            }
            break;

            case OPCODE_DEC:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                --reg.i[reg0];
            }
            break;

            case OPCODE_MOV1:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg.i[reg0] = _xvm_instr_get_sgn_value21(instr);
            }
            break;

            case OPCODE_LDA:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                unsgn_value = _xvm_instr_get_value21(instr);

                mem_addr.cbyte = program_start_ptr + (unsgn_value << 2);

                reg.i[reg0] = (regi_t)(mem_addr.cbyte);
            }
            break;

            case OPCODE_MOVI:
            {
                // Load WORD from next instruction
                reg0 = _xvm_instr_get_reg0(instr);
                *reg_pc += 4;
                reg.i[reg0] = (regi_t)*((instr_t*)(*reg_pc));
            }
            break;

            /* --- jump opcodes --- */

            // Undefined behavior for floats
            case OPCODE_JMP:
            {
                // Set program counter to (address + offset)
                reg0 = _xvm_instr_get_reg0(instr);
                sgn_value = _xvm_instr_get_sgn_value21(instr);

                if (sgn_value == XVM_SGN_VALUE21_MAX)
                {
                    // Jump to absolute procedure address (indirect jump)
                    *reg_pc = (regi_t)(program_start_ptr + (reg.i[reg0] << 2));
                }
                else
                {
                    // Jump to relative procedure address (direct jump)
                    *reg_pc = JUMP_ADDRESS(reg0, sgn_value);
                }

                continue;
            }
            break;

            // Undefined behavior for floats
            case OPCODE_JE:
            {
                if (*reg_cf == 0)
                {
                    // Set program counter to (address + offset)
                    reg0 = _xvm_instr_get_reg0(instr);
                    sgn_value = _xvm_instr_get_sgn_value21(instr);
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
                    reg0 = _xvm_instr_get_reg0(instr);
                    sgn_value = _xvm_instr_get_sgn_value21(instr);
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
                    reg0 = _xvm_instr_get_reg0(instr);
                    sgn_value = _xvm_instr_get_sgn_value21(instr);
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
                    reg0 = _xvm_instr_get_reg0(instr);
                    sgn_value = _xvm_instr_get_sgn_value21(instr);
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
                    reg0 = _xvm_instr_get_reg0(instr);
                    sgn_value = _xvm_instr_get_sgn_value21(instr);
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
                    reg0 = _xvm_instr_get_reg0(instr);
                    sgn_value = _xvm_instr_get_sgn_value21(instr);
                    *reg_pc = JUMP_ADDRESS(reg0, sgn_value);
                    continue;
                }
            }
            break;

            // Undefined behavior for floats
            case OPCODE_CALL:
            {
                sgn_value = _xvm_instr_get_sgn_value21(instr);

                // Push dynamic link (lb and pc registers)
                extra_value = *reg_lb;
                *reg_lb = *reg_sp;
                xvm_stack_push(stack, reg_sp, extra_value);
                xvm_stack_push(stack, reg_sp, *reg_pc);

                reg0 = _xvm_instr_get_reg0(instr);

                if (sgn_value == XVM_SGN_VALUE21_MAX)
                {
                    // Jump to absolute procedure address (indirect call)
                    *reg_pc = (regi_t)(program_start_ptr + (reg.i[reg0] << 2));
                }
                else
                {
                    // Jump to relative procedure address (direct call)
                    *reg_pc = JUMP_ADDRESS(reg0, sgn_value);
                }

                continue;
            }
            break;

            /* --- load/store opcodes --- */

            case OPCODE_LDB:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                sgn_value = _xvm_instr_get_sgn_value16(instr);

                mem_addr.cbyte = (const byte_t*)reg.i[reg1];
                mem_addr.cbyte += sgn_value;

                reg.i[reg0] = (regi_t)(*mem_addr.cbyte);
            }
            break;

            case OPCODE_STB:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                sgn_value = _xvm_instr_get_sgn_value16(instr);

                mem_addr.byte = (byte_t*)reg.i[reg1];
                mem_addr.byte += sgn_value;

                *mem_addr.byte = (byte_t)(reg.i[reg0]);
            }
            break;

            case OPCODE_LDH:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                sgn_value = _xvm_instr_get_sgn_value16(instr);

                mem_addr.cbyte = (const byte_t*)reg.i[reg1];
                mem_addr.cbyte += sgn_value;

                reg.i[reg0] = (regi_t)(*mem_addr.chalf);
            }
            break;

            case OPCODE_STH:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                sgn_value = _xvm_instr_get_sgn_value16(instr);

                mem_addr.byte = (byte_t*)reg.i[reg1];
                mem_addr.byte += sgn_value;

                *mem_addr.half = (word_t)(reg.i[reg0]);
            }
            break;

            case OPCODE_LDW:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                sgn_value = _xvm_instr_get_sgn_value16(instr);

                mem_addr.cbyte = (const byte_t*)reg.i[reg1];
                mem_addr.cbyte += sgn_value;

                reg.i[reg0] = (regi_t)(*mem_addr.cword);
            }
            break;

            case OPCODE_STW:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg1 = _xvm_instr_get_reg1(instr);
                sgn_value = _xvm_instr_get_sgn_value16(instr);

                mem_addr.byte = (byte_t*)reg.i[reg1];
                mem_addr.byte += sgn_value;

                *mem_addr.word = (word_t)(reg.i[reg0]);
            }
            break;

            /* --- special opcodes --- */

            case OPCODE_STOP:
                return EXITCODE_SUCCESS;

            case OPCODE_RET:
            {
                // Get result size (extra_value) and argument size (unsgn_value)
                // These sizes are in words (i.e. 4 bytes)
                extra_value = _xvm_instr_get_extra_value10(instr);
                unsgn_value = _xvm_instr_get_value16(instr);

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
                sgn_value = _xvm_instr_get_sgn_value26(instr);
                xvm_stack_push(stack, reg_sp, sgn_value);
            }
            break;

            case OPCODE_INVK:
            {
                unsgn_value = _xvm_instr_get_value26(instr);
                if (unsgn_value < byte_code->num_invoke_idents)
                {
                    // Invoke bounded procedure
                    xvm_invocation_proc invokeProc = byte_code->invoke_bindings[unsgn_value];
                    invokeProc((void*)&stack_env);
                }
                else
                    return EXITCODE_INVOCATION_VIOLATION;
            }
            break;

            case OPCODE_INSC:
            {
                // Call intrinsic procedure
                unsgn_value = _xvm_instr_get_value26(instr);
                _xvm_call_intrinsic(unsgn_value, stack, reg_sp, reg_ar);
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
STATIC xvm_exit_codes xvm_execute_program(
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
STATIC xvm_exit_codes xvm_execute_program_entry_point(
    const xvm_bytecode* const byte_code,
    xvm_stack* const stack,
    const char* entry_point)
{
    const xvm_export_address* entry_point_addr = NULL;
    
    if (byte_code == NULL)
        return EXITCODE_INVALID_BYTECODE;

    // Find entry point in export addresses
    entry_point_addr = xvm_bytecode_find_export_address(byte_code, entry_point);
    if (entry_point_addr == NULL)
        return EXITCODE_UNKNOWN_ENTRY_POINT;

    // Execute program from entry point
    return xvm_execute_program_ext(byte_code, stack, entry_point_addr);
}


#ifndef _DISABLE_SHELL_

/* ----- Shell ----- */

STATIC void _xvm_shell_print_help()
{
    xvm_log_println("usage:");
    xvm_log_println("  xvm [options] FILE         Executes the specified virtual program");
    xvm_log_println("options:");
    xvm_log_println("  -h, --help, help           Prints the help information");
    xvm_log_println("  --version                  Prints the version and license note");
    xvm_log_println("  --verbose                  Prints additional output before and after program execution");
    xvm_log_println("  -st, --stack-size SIZE     Sets the stack size (by default 256)");
    xvm_log_println("  -mod, --load-module FILE   Loads the module, specified by FILE");
}

STATIC void _xvm_shell_print_version()
{
    #ifdef _ENABLE_RUNTIME_DEBUGGER_
    xvm_log_println("XieXie 2.0 (Rev.1) VirtualMachine (XVM) with RuntimeDebugger (RTD)");
    #else
    xvm_log_println("XieXie 2.0 (Rev.1) VirtualMachine (XVM)");
    #endif
    xvm_log_println("");
    xvm_log_println("Copyright (C) 2014 Lukas Hermanns");
    xvm_log_println("All rights reserved.");
    xvm_log_println("");
    xvm_log_println("This software may be modified and distributed under the terms");
    xvm_log_println("of the BSD license.  See the LICENSE file for details.");
}

STATIC int _xvm_shell_parse_args(int argc, char* argv[])
{
    // Configuration memory
    int verbose = 0;
    const char* filename = NULL;
    size_t stack_size = 256;

    // Initialize module container
    xvm_module_container module_cont;
    xvm_module_container_init(&module_cont);

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
            _xvm_shell_print_help();
        else if (strcmp(arg, "--version") == 0)
            _xvm_shell_print_version();
        else if (strcmp(arg, "--verbose") == 0)
            verbose = 1;
        else if (strcmp(arg, "-mod") == 0 || strcmp(arg, "--load-module") == 0)
        {
            if (argc > 0)
            {
                // Get parameter from next argument
                arg = *argv;

                // Load module
                xvm_module module;
                xvm_module_init(&module);
                
                if (xvm_module_load(&module, arg) != 0)
                    xvm_module_container_add(&module_cont, module);

                ++argv;
                --argc;
            }
            else
            {
                xvm_log_error("expected argument after \"-mod\" and \"--load-module\" flag");
                return 0;
            }
        }
        else if (strcmp(arg, "-st") == 0 || strcmp(arg, "--stack-size") == 0)
        {
            if (argc > 0)
            {
                // Get parameter from next argument
                arg = *argv;
                int param = atoi(arg);

                if (param <= 0)
                {
                    xvm_log_error("stack size must be greater than zero");
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
                xvm_log_error("expected argument after \"-st\" and \"--stack-size\" flag");
                return 0;
            }
        }
        else if (strlen(arg) > 0)
        {
            if (filename != NULL)
            {
                xvm_log_error("only a single program can be executed at a time");
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

        // Bind modules
        if (xvm_module_iteration_start(&module_cont) != 0)
        {
            xvm_module* module = NULL;
            while ( ( module = xvm_module_iteration_next() ) != NULL )
                xvm_bytecode_bind_module(&byte_code, module);
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
            xvm_log_println("program terminated successful");

        // Clean up
        xvm_bytecode_free(&byte_code);
        xvm_stack_free(&stack);
    }

    return 1;
}

// Main function for the shell
int main(int argc, char* argv[])
{
    // Ignore program path argument, then parse all other arguments
    _xvm_shell_parse_args(--argc, ++argv);
    return 0;
}

#endif // /_DISABLE_SHELL_


// Undefine internal macros
#undef INLINE
#undef STATIC


#ifdef __cplusplus
}
#endif

