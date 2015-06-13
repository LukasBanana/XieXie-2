/*
 * xvm.c
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <xiexie/xvm.h>

#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <math.h>
#include <time.h>

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
#   define MODULE_GET_HANDLE(hndl) (HMODULE)(hndl)
#elif defined(__linux__)
#   include <unistd.h>
#   include <dlfcn.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

/* ----- Helper macros ----- */

#define _ENABLE_INLINEING_

#ifdef _ENABLE_INLINEING_
#   define INLINE __inline
#else
#   define INLINE // No inlining
#endif


/* ----- Helper functions ----- */

INLINE static int _xvm_flt2int_signum(float val)
{
    if (val > 0.0f)
        return 1;
    else if (val < 0.0f)
        return -1;
    return 0;
}


/* ----- Leak Detection ----- */

#ifdef _ENABLE_LEAK_DETECTION_

static long _mem_ref_count = 0;
static long _file_ref_count = 0;

int xvm_memory_ref_count()
{
    return _mem_ref_count;
}

int xvm_file_ref_count()
{
    return _file_ref_count;
}

INLINE static void* _xvm_malloc(size_t size)
{
    ++_mem_ref_count;
    return malloc(size);
}

INLINE static void _xvm_free(void* ptr)
{
    --_mem_ref_count;
    free(ptr);
}

INLINE static FILE* _xvm_fopen(const char* filename, const char* mode)
{
    ++_file_ref_count;
    return fopen(filename, mode);
}

INLINE static int _xvm_fclose(FILE* file)
{
    --_file_ref_count;
    return fclose(file);
}

#define XVM_MALLOC  _xvm_malloc
#define XVM_FREE    _xvm_free
#define XVM_FOPEN   _xvm_fopen
#define XVM_FCLOSE  _xvm_fclose

#else

int xvm_memory_ref_count()
{
    return 0;
}

int xvm_file_ref_count()
{
    return 0;
}

#define XVM_MALLOC  malloc
#define XVM_FREE    free
#define XVM_FOPEN   fopen
#define XVM_FCLOSE  fclose

#endif


/* ----- Registers ----- */

const char* xvm_register_get_name(reg_t reg)
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

        case REG_TR:  return "$tr";
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

/*

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


/* ----- Virtual machine exit codes ----- */

const char* xvm_exitcode_to_string(const xvm_exit_codes exit_code)
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

void xvm_log_print(const char* str)
{
    printf("%s", str);
}

void xvm_log_println(const char* str)
{
    puts(str);
}

void xvm_log_error(const char* format, ...)
{
    printf("error: ");

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    puts("");
}

void xvm_log_warning(const char* format, ...)
{
    printf("warning: ");

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    puts("");
}

void xvm_log_readfile_error(const char* filename)
{
    xvm_log_error("reading file \"%s\" failed", filename);
}

void xvm_log_exitcode_error(const xvm_exit_codes exit_code)
{
    const char* err = xvm_exitcode_to_string(exit_code);
    if (err != NULL)
        xvm_log_error("program terminated with error: \"%s\"", err);
}


/* ----- File helper ----- */

unsigned int xvm_file_read_uint(FILE* file)
{
    unsigned int value = 0;
    if (file != NULL)
        fread(&value, sizeof(value), 1, file);
    return value;
}

void xvm_file_write_uint(FILE* file, unsigned int value)
{
    if (file != NULL)
        fwrite(&value, sizeof(value), 1, file);
}


/* ----- Intrinsics ----- */

const char* xvm_intrinsic_get_ident(const xvm_intrinsic_id addr)
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
        case INSC_FILE_SET_POS: return "FileSetPos";
        case INSC_FILE_GET_POS: return "FileGetPos";
        case INSC_FILE_EOF:     return "FileEOF";
        case INSC_WRITE_BYTE:   return "WriteByte";
        case INSC_WRITE_WORD:   return "WriteWord";
        case INSC_WRITE_BUFFER: return "WriteBuffer";
        case INSC_READ_BYTE:    return "ReadByte";
        case INSC_READ_WORD:    return "ReadWord";
        case INSC_READ_BUFFER:  return "ReadBuffer";

        /* --- Math intrinsics --- */

        case INSC_SIN:          return "Sin";
        case INSC_COS:          return "Cos";
        case INSC_TAN:          return "Tan";
        case INSC_ASIN:         return "ASin";
        case INSC_ACOS:         return "ACos";
        case INSC_ATAN:         return "ATan";
        case INSC_SINH:         return "SinH";
        case INSC_COSH:         return "CosH";
        case INSC_TANH:         return "TanH";
        case INSC_EXP:          return "Exp";
        case INSC_EXP2:         return "Exp2";
        case INSC_POW:          return "Pow";
        case INSC_LOG:          return "Log";
        case INSC_LOG2:         return "Log2";
        case INSC_LOG10:        return "Log10";
        case INSC_SQRT:         return "Sqrt";
        case INSC_CEIL:         return "Ceil";
        case INSC_FLOOR:        return "Floor";

        /* --- Other intrinsics --- */

        case INSC_RAND_INT:     return "RandInt";
        case INSC_RAND_FLOAT:   return "RandFloat";
        case INSC_TIME:         return "Time";
        case INSC_SLEEP:        return "Sleep";
    }
    return "";
}


/* ----- Instruction ----- */

// Returns the instruction opcode.
INLINE static opcode_t _xvm_instr_get_opcode(const instr_t instr)
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

opcode_t xvm_instr_get_opcode(const instr_t instr)
{
    return _xvm_instr_get_opcode(instr);
}

// Returns the 26-bit unsigned value of the specified instruction.
INLINE static unsigned int _xvm_instr_get_value26(const instr_t instr)
{
    return (instr & XVM_VALUE26_MAX);
}

// Returns the 21-bit unsigned value of the specified instruction.
INLINE static unsigned int _xvm_instr_get_value21(const instr_t instr)
{
    return (instr & XVM_VALUE21_MAX);
}

// Returns the 16-bit unsigned value of the specified instruction.
INLINE static unsigned int _xvm_instr_get_value16(const instr_t instr)
{
    return (instr & XVM_VALUE16_MAX);
}

// Returns the 26-bit signed value of the specified instruction.
INLINE static int _xvm_instr_get_sgn_value26(const instr_t instr)
{
    unsigned int val = _xvm_instr_get_value26(instr);

    // Sign extend
    if ((val & 0x02000000) != 0)
        val |= 0xfc000000;

    return (int)val;
}

// Returns the 21-bit signed value of the specified instruction.
INLINE static int _xvm_instr_get_sgn_value21(const instr_t instr)
{
    unsigned int val = _xvm_instr_get_value21(instr);

    // Sign extend
    if ((val & 0x00100000) != 0)
        val |= 0xffe00000;

    return (int)val;
}

// Returns the 16-bit signed value of the specified instruction.
INLINE static int _xvm_instr_get_sgn_value16(const instr_t instr)
{
    unsigned int val = _xvm_instr_get_value16(instr);

    // Sign extend
    if ((val & 0x00008000) != 0)
        val |= 0xffff0000;

    return (int)val;
}

// Returns the extra value of the special instruction RET.
INLINE static int _xvm_instr_get_extra_value10(const instr_t instr)
{
    return (instr & 0x03ff0000) >> 16;
}

// Returns the index of the register 0.
INLINE static reg_t _xvm_instr_get_reg0(const instr_t instr)
{
    return (instr & 0x03e00000) >> 21;
}

// Returns the index of the register 1.
INLINE static reg_t _xvm_instr_get_reg1(const instr_t instr)
{
    return (instr & 0x001f0000) >> 16;
}

// Returns the index of the register 2.
INLINE static reg_t _xvm_instr_get_reg2(const instr_t instr)
{
    return (instr & 0x0000f800) >> 11;
}

const char* xvm_instr_get_mnemonic(const opcode_t opcode)
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

/*
Prints debug information for the specified instruction
with the current state of the specified register set.
*/
static void _xvm_instr_print_debug_info(const instr_t instr, regi_t instr_index, const regi_t* reg_ptr)
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

        printf("%s, %s       ($cf = %i)", reg0name, reg1name, _xvm_flt2int_signum(lhs - rhs));
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

    puts("");
}

#endif


/* ----- Instruction constructors ----- */

instr_t xvm_instr_make_reg3(xvm_opcode opcode, reg_t reg0, reg_t reg1, reg_t reg2)
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

instr_t xvm_instr_make_reg2(xvm_opcode opcode, reg_t reg0, reg_t reg1, unsigned int value)
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

instr_t xvm_instr_make_reg1(xvm_opcode opcode, reg_t reg, unsigned int value)
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

instr_t xvm_instr_make_jump(xvm_opcode opcode, reg_t reg, unsigned int offset)
{
    return xvm_instr_make_reg1(opcode, reg, offset);
}

instr_t xvm_instr_make_loadstore(xvm_opcode opcode, reg_t reg0, reg_t reg1, unsigned int offset)
{
    return xvm_instr_make_reg2(opcode, reg0, reg1, offset);
}

instr_t xvm_instr_make_special1(xvm_opcode opcode, unsigned int value)
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

instr_t xvm_instr_make_special2(xvm_opcode opcode, unsigned int result_size, unsigned int arg_size)
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

static instr_t _xvm_instr_patch_value26(instr_t instr, unsigned int value)
{
    return (instr & 0xfc000000) | (value & XVM_VALUE26_MAX);
}

static instr_t _xvm_instr_patch_value21(instr_t instr, unsigned int value)
{
    return (instr & 0xffe00000) | (value & XVM_VALUE21_MAX);
}

static instr_t _xvm_instr_patch_value16(instr_t instr, unsigned int value)
{
    return (instr & 0xffff0000) | (value & XVM_VALUE16_MAX);
}

instr_t xvm_instr_patch_value(instr_t instr, unsigned int value)
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

xvm_string xvm_string_init()
{
    xvm_string string;
    string.len = 0;
    string.str = NULL;
    return string;
}

xvm_string xvm_string_create(size_t len)
{
    xvm_string string;

    // Allocate memory for the string and store string length
    string.len = len;
    string.str = (char*)XVM_MALLOC(sizeof(char)*(len + 1));

    // Initialize last string entry with the null character
    string.str[len] = '\0';

    return string;
}

xvm_string xvm_string_create_from(const char* str)
{
    // Create string and copy data
    size_t len = strlen(str);
    xvm_string string = xvm_string_create(len);
    memcpy(string.str, str, sizeof(char)*(len + 1));
    return string;
}

xvm_string xvm_string_create_from_sub(const char* str, size_t len)
{
    // Determine length of new string
    size_t maxlen = strlen(str);
    xvm_string string;
    if (len > maxlen)
        len = maxlen;

    // Create new string and copy sub data
    string = xvm_string_create(len);
    memcpy(string.str, str, sizeof(char)*len);
    string.str[len] = '\0';
    return string;
}

int xvm_string_append(xvm_string* string, const char* append_str)
{
    if (string != NULL && string->str != NULL && append_str != NULL)
    {
        size_t append_len = strlen(append_str);

        // Create new string
        size_t len = string->len + append_len;
        char* str = (char*)XVM_MALLOC(sizeof(char)*(len + 1));
        str[len] = '\0';

        // Copy data from original string
        memcpy(str, string->str, sizeof(char)*string->len);

        // Copy data from append string
        memcpy(str + string->len, append_str, append_len);

        // Replace original string
        XVM_FREE(string->str);
        string->str = str;
        string->len = len;

        return 1;
    }
    return 0;
}

int xvm_string_free(xvm_string* string)
{
    if (string != NULL && string->str != NULL)
    {
        // Free memory and reset data
        XVM_FREE(string->str);
        string->len = 0;
        string->str = NULL;
        return 1;
    }
    return 0;
}

xvm_string xvm_string_read_from_file(FILE* file)
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

int xvm_string_write_to_file(xvm_string string, FILE* file)
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

static void _xvm_exception_throw(const char* error_message, int error_code)
{
    // Setup exception error message and make a long jump
    _xvm_exception_err = error_message;
    longjmp(_xvm_exception_envbuf, error_code);
}

static void _xvm_exception_stack_overflow()
{
    _xvm_exception_throw("stack overflow", EXITCODE_STACK_OVERFLOW);
}

static void _xvm_exception_stack_underflow()
{
    _xvm_exception_throw("stack underflow", EXITCODE_STACK_UNDERFLOW);
}


/* ----- Stack ----- */

int xvm_stack_init(xvm_stack* stack)
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

int xvm_stack_clear(xvm_stack* stack, stack_word_t value)
{
    if (stack != NULL && stack->stack_size > 0 && stack->storage != NULL)
    {
        for (size_t i = 0; i < stack->stack_size; ++i)
            stack->storage[i] = value;
        return 1;
    }
    return 0;
}

int xvm_stack_create(xvm_stack* stack, size_t stack_size)
{
    if (stack != NULL && stack->storage == NULL && stack_size != 0)
    {
        stack->stack_size   = stack_size;
        stack->storage      = (stack_word_t*)XVM_MALLOC(sizeof(stack_word_t)*stack_size);
        return 1;
    }
    return 0;
}

int xvm_stack_free(xvm_stack* stack)
{
    if (stack != NULL)
    {
        if (stack->storage != NULL)
            XVM_FREE(stack->storage);

        // Reset stack data
        stack->stack_size   = 0;
        stack->storage      = NULL;

        return 1;
    }
    return 0;
}

INLINE static void _xvm_stack_push(xvm_stack* stack, regi_t* reg_sp, stack_word_t value)
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

INLINE static stack_word_t _xvm_stack_pop(xvm_stack* stack, regi_t* reg_sp)
{
    stack_word_t* stack_ptr = REG_TO_STACK_PTR(reg_sp);
    if (stack_ptr > stack->storage)
        (*reg_sp) -= sizeof(stack_word_t);
    else
        _xvm_exception_stack_underflow();
    return *REG_TO_STACK_PTR(reg_sp);
}


/* ----- Stack Environment ----- */

typedef struct
{
    int* reg_ref;       // reference to registers
    int* stack_begin;   // stack begin
    int* stack_end;     // stack end
}
_xvm_env_internal;

//! Null invocation procedure -> throws EXITCODE_INVOCATION_VIOLATION.
static void _xvm_null_invocation(xvm_env env)
{
    _xvm_exception_throw("invocation of unbound external procedure", EXITCODE_INVOCATION_VIOLATION);
}


/* ----- Export Address ----- */

int xvm_export_address_init(xvm_export_address* export_address)
{
    if (export_address != NULL)
    {
        export_address->addr    = 0;
        export_address->label   = xvm_string_init();
        return 1;
    }
    return 0;
}

int xvm_export_address_setup(xvm_export_address* export_address, unsigned int addr, xvm_string label)
{
    if (export_address != NULL)
    {
        export_address->addr    = addr;
        export_address->label   = label;
        return 1;
    }
    return 0;
}

int xvm_export_address_free(xvm_export_address* export_address)
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

int xvm_import_address_init(xvm_import_address* import_address)
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

int xvm_import_address_setup(xvm_import_address* import_address, unsigned int num_indices)
{
    if (import_address != NULL)
    {
        import_address->num_indices = num_indices;
        import_address->indices     = (unsigned int*)XVM_MALLOC(sizeof(unsigned int) * num_indices);
        import_address->label       = xvm_string_init();
        return 1;
    }
    return 0;
}

int xvm_import_address_free(xvm_import_address* import_address)
{
    if (import_address != NULL)
    {
        if (import_address->indices != NULL)
        {
            XVM_FREE(import_address->indices);
            import_address->indices = NULL;
        }
        import_address->num_indices = 0;
        xvm_string_free(&(import_address->label));
        return 1;
    }
    return 0;
}


/* ----- Runtime Debugging ----- */

#ifdef _ENABLE_RUNTIME_DEBUGGER_

static int _xvm_debug_info_init(xvm_debug_info* debug_info)
{
    if (debug_info != NULL)
    {
        // Initialize debug information data
        debug_info->flags                   = 0;
        
        debug_info->num_source_filenames    = 0;
        debug_info->source_filenames        = NULL;

        debug_info->num_break_points        = 0;
        debug_info->break_points            = NULL;

        debug_info->debug_instructions      = NULL;

        return 1;
    }
    return 0;
}

static int _xvm_debug_info_free(xvm_debug_info* debug_info)
{
    if (debug_info != NULL)
    {
        if (debug_info->source_filenames != NULL)
        {
            // Free source filename list
            XVM_FREE(debug_info->source_filenames);
            debug_info->source_filenames        = NULL;
            debug_info->num_source_filenames    = 0;
        }

        if (debug_info->break_points != NULL)
        {
            // Free break points list
            XVM_FREE(debug_info->break_points);
            debug_info->break_points        = NULL;
            debug_info->num_break_points    = 0;
        }

        if (debug_info->debug_instructions != NULL)
        {
            // Free debug instruction list
            XVM_FREE(debug_info->debug_instructions);
            debug_info->debug_instructions = NULL;
        }

        return 1;
    }
    return 0;
}

int xvm_debug_info_create_break_points(xvm_debug_info* debug_info, unsigned int num_break_points)
{
    if (debug_info != NULL)
    {
        // Free previous break points
        if (debug_info->break_points != NULL)
            XVM_FREE(debug_info->break_points);

        if (num_break_points > 0)
        {
            // Allocate new break points
            debug_info->num_break_points    = num_break_points;
            debug_info->break_points        = (unsigned int*)XVM_MALLOC(sizeof(unsigned int)*num_break_points);
        }
        else
        {
            // Reset break points
            debug_info->num_break_points    = 0;
            debug_info->break_points        = NULL;
        }
        return 1;
    }
    return 0;
}

int xvm_debug_info_has_break_point(const xvm_debug_info* debug_info, unsigned int break_point)
{
    if (debug_info != NULL)
    {
        for (unsigned int i = 0; i < debug_info->num_break_points; ++i)
        {
            if (debug_info->break_points[i] == break_point)
                return 1;
        }
    }
    return 0;
}

#endif


/* ----- Byte Code ----- */

int xvm_bytecode_init(xvm_bytecode* byte_code)
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

        byte_code->num_module_names     = 0;
        byte_code->module_names         = NULL;
        
        #ifdef _ENABLE_RUNTIME_DEBUGGER_
        _xvm_debug_info_init(&(byte_code->debug_info));
        #endif

        return 1;
    }
    return 0;
}

int xvm_bytecode_create_instructions(xvm_bytecode* byte_code, unsigned int num_instructions)
{
    if (byte_code != NULL && byte_code->instructions == NULL && num_instructions > 0)
    {
        byte_code->num_instructions = num_instructions;
        byte_code->instructions     = (instr_t*)XVM_MALLOC(sizeof(instr_t)*num_instructions);
        return 1;
    }
    return 0;
}

int xvm_bytecode_create_export_addresses(xvm_bytecode* byte_code, unsigned int num_export_addresses)
{
    if (byte_code != NULL && byte_code->export_addresses == NULL && num_export_addresses > 0)
    {
        byte_code->num_export_addresses = num_export_addresses;
        byte_code->export_addresses     = (xvm_export_address*)XVM_MALLOC(sizeof(xvm_export_address)*num_export_addresses);
        return 1;
    }
    return 0;
}

int xvm_bytecode_create_import_addresses(xvm_bytecode* byte_code, unsigned int num_import_addresses)
{
    if (byte_code != NULL && byte_code->import_addresses == NULL && num_import_addresses > 0)
    {
        byte_code->num_import_addresses = num_import_addresses;
        byte_code->import_addresses     = (xvm_import_address*)XVM_MALLOC(sizeof(xvm_import_address)*num_import_addresses);
        return 1;
    }
    return 0;
}

const xvm_export_address* xvm_bytecode_find_export_address(const xvm_bytecode* byte_code, const char* label)
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

int xvm_bytecode_dynamic_link(xvm_bytecode* byte_code, xvm_bytecode* client_byte_code)
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

int xvm_bytecode_create_invocations(xvm_bytecode* byte_code, unsigned int num_invoke_idents)
{
    if (byte_code != NULL && byte_code->invoke_idents == NULL && num_invoke_idents > 0)
    {
        // Create invocation identifiers
        byte_code->num_invoke_idents    = num_invoke_idents;
        byte_code->invoke_idents        = (xvm_string*)XVM_MALLOC(sizeof(xvm_string)*num_invoke_idents);

        // Create and initialize invocation bindings
        byte_code->invoke_bindings      = (xvm_invocation_proc*)XVM_MALLOC(sizeof(xvm_invocation_proc)*num_invoke_idents);
        for (unsigned int i = 0; i < num_invoke_idents; ++i)
            byte_code->invoke_bindings[i] = _xvm_null_invocation;
        return 1;
    }
    return 0;
}

int xvm_bytecode_bind_invocation(xvm_bytecode* byte_code, const char* ident, xvm_invocation_proc proc)
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

int xvm_bytecode_create_module_names(xvm_bytecode* byte_code, unsigned int num_module_names)
{
    if (byte_code != NULL && byte_code->module_names == NULL && num_module_names > 0)
    {
        // Create module names
        byte_code->num_module_names = num_module_names;
        byte_code->module_names     = (xvm_string*)XVM_MALLOC(sizeof(xvm_string)*num_module_names);
        return 1;
    }
    return 0;
}

int xvm_bytecode_free(xvm_bytecode* byte_code)
{
    if (byte_code != NULL)
    {
        if (byte_code->instructions != NULL)
        {
            // Free instruction list
            XVM_FREE(byte_code->instructions);
            byte_code->instructions     = NULL;
            byte_code->num_instructions = 0;
        }

        if (byte_code->export_addresses != NULL)
        {
            // Free each each export address
            for (unsigned int i = 0; i < byte_code->num_export_addresses; ++i)
                xvm_export_address_free(&(byte_code->export_addresses[i]));

            // Free export address list
            XVM_FREE(byte_code->export_addresses);
            byte_code->export_addresses     = NULL;
            byte_code->num_export_addresses = 0;
        }

        if (byte_code->import_addresses != NULL)
        {
            // Free each each import address
            for (unsigned int i = 0; i < byte_code->num_import_addresses; ++i)
                xvm_import_address_free(&(byte_code->import_addresses[i]));

            // Free import address list
            XVM_FREE(byte_code->import_addresses);
            byte_code->import_addresses     = NULL;
            byte_code->num_import_addresses = 0;
        }

        if (byte_code->invoke_idents != NULL)
        {
            // Free each invocation identifier
            for (unsigned int i = 0; i < byte_code->num_invoke_idents; ++i)
                xvm_string_free(&(byte_code->invoke_idents[i]));

            // Free invocation identifier list
            XVM_FREE(byte_code->invoke_idents);
            byte_code->invoke_idents        = NULL;
            byte_code->num_invoke_idents    = 0;
        }

        if (byte_code->invoke_bindings != NULL)
        {
            // Free invocation bindings vector
            XVM_FREE(byte_code->invoke_bindings);
            byte_code->invoke_bindings = NULL;
        }

        if (byte_code->module_names != NULL)
        {
            // Free each module name
            for (unsigned int i = 0; i < byte_code->num_module_names; ++i)
                xvm_string_free(&(byte_code->module_names[i]));

            // Free module names
            XVM_FREE(byte_code->module_names);
            byte_code->module_names     = NULL;
            byte_code->num_module_names = 0;
        }
        
        #ifdef _ENABLE_RUNTIME_DEBUGGER_
        _xvm_debug_info_free(&(byte_code->debug_info));
        #endif

        return 1;
    }
    return 0;
}

int xvm_bytecode_datafield_ascii(instr_t* instr_ptr, const char* text, size_t* num_instructions)
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
STRING:
    WORD:           length
    Byte[length]:   data

--- XBC file format spec (Version 1.35): ---

WORD: magic number (Must be *(int*)"XBCF")
WORD: version number (Must be 135 for "1.35")
WORD: number of instructions (n1)
n1 times:
    WORD: instruction
WORD: has debug info? (1 = true, 0 = false)
if has debug info:
    WORD: number of source filenames (n2)
    n2 times:
        STRING: filename
    n1 times:
        WORD: filename index
        WORD: source line
WORD: number of export addresses (n3)
n3 times:
    WORD: address
    STRING: name
WORD: number of invoke identifiers (n4)
n4 times:
    STRING: identifier
WORD: number of module names (n5)
n5 times:
    STRING: name

*/

// Close specified file and print error to the log.
static int _xvm_close_file_with_error(FILE* file, const char* err)
{
    xvm_log_error(err);
    XVM_FCLOSE(file);
    return 0;
}

int xvm_bytecode_read_from_file(xvm_bytecode* byte_code, const char* filename)
{
    // Check arguments
    if (byte_code == NULL || filename == NULL)
    {
        xvm_log_error("invalid arguments to read byte code");
        return 0;
    }

    // Open file for reading
    FILE* file = XVM_FOPEN(filename, "rb");
    if (file == NULL)
    {
        xvm_log_error("unable to open file for reading");
        return 0;
    }

    // Read magic number
    unsigned int magic = xvm_file_read_uint(file);
    if (magic != XBC_FORMAT_MAGIC)
        return _xvm_close_file_with_error(file, "invalid magic number (must be \"XBCF\")");

    // Read version number
    unsigned int version = xvm_file_read_uint(file);
    if (version < XBC_FORMAT_VERSION_1_31 || version > XBC_FORMAT_VERSION_1_35)
        return _xvm_close_file_with_error(file, "invalid version number (must be 1.31, 1.32, 1.33, 1.34, or 1.35)");

    // Read instructions
    unsigned int num_instr = xvm_file_read_uint(file);

    if (xvm_bytecode_create_instructions(byte_code, (int)num_instr) == 0)
        return _xvm_close_file_with_error(file, "creating byte code instructions failed");

    fread(byte_code->instructions, sizeof(instr_t), num_instr, file);

    // Read export addresses
    if (version >= XBC_FORMAT_VERSION_1_32)
    {
        unsigned int num_export_addr = xvm_file_read_uint(file);

        if (num_export_addr > 0)
        {
            if (xvm_bytecode_create_export_addresses(byte_code, num_export_addr) == 0)
                return _xvm_close_file_with_error(file, "creating byte code export addresses failed");

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
    /*if (version >= ???)
    {
        unsigned int num_import_addr = xvm_file_read_uint(file);

        if (num_import_addr > 0)
        {
            if (xvm_bytecode_create_import_addresses(byte_code, num_import_addr) == 0)
                return _xvm_close_file_with_error(file, "creating byte code import addresses failed");

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
    }*/

    // Read invocation identifiers
    if (version >= XBC_FORMAT_VERSION_1_33)
    {
        unsigned int num_invoke_idents = xvm_file_read_uint(file);

        if (num_invoke_idents > 0)
        {
            if (xvm_bytecode_create_invocations(byte_code, num_invoke_idents) == 0)
                return _xvm_close_file_with_error(file, "creating byte code invocation identifiers failed");

            // Read strings and store them into the invocation identifier
            for (unsigned int i = 0; i < num_invoke_idents; ++i)
                byte_code->invoke_idents[i] = xvm_string_read_from_file(file);
        }
    }

    // Read module names
    if (version >= XBC_FORMAT_VERSION_1_34)
    {
        unsigned int num_module_names = xvm_file_read_uint(file);

        if (num_module_names > 0)
        {
            if (xvm_bytecode_create_module_names(byte_code, num_module_names) == 0)
                return _xvm_close_file_with_error(file, "creating byte code module names failed");

            // Read strings and store them into the module name
            for (unsigned int i = 0; i < num_module_names; ++i)
                byte_code->module_names[i] = xvm_string_read_from_file(file);
        }
    }

    // Close file and return with success
    XVM_FCLOSE(file);

    return 1;
}

int xvm_bytecode_write_to_file(const xvm_bytecode* byte_code, const char* filename, unsigned int version)
{
    // Check arguments
    if (byte_code == NULL || byte_code->instructions == NULL || filename == NULL)
    {
        xvm_log_error("invalid arguments to write byte code");
        return 0;
    }
    if (version < XBC_FORMAT_VERSION_1_31 || version > XBC_FORMAT_VERSION_1_35)
    {
        xvm_log_error("invalid version number (must be 1.31, 1.32, 1.33, 1.34, or 1.35)");
        return 0;
    }

    // Open file for writing
    FILE* file = XVM_FOPEN(filename, "wb");
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

    // Write debug instructions
    if (version >= XBC_FORMAT_VERSION_1_35)
    {
        #ifdef _ENABLE_RUNTIME_DEBUGGER_
        
        if (byte_code->debug_info.flags != 0)
        {
            // Write enable-flag
            xvm_file_write_uint(file, byte_code->debug_info.flags);

            // Write source filenames
            unsigned int num_sources = byte_code->debug_info.num_source_filenames;
            xvm_file_write_uint(file, num_sources);

            for (unsigned int i = 0; i < num_sources; ++i)
                xvm_string_write_to_file(byte_code->debug_info.source_filenames[i], file);

            // Write debug instructions
            fwrite(byte_code->debug_info.debug_instructions, sizeof(xvm_debug_info_instruction), num_instr, file);
        }
        else
        
        #endif

        {
            // Write disable-flag
            xvm_file_write_uint(file, 0);
        }
    }

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
    /*if (version >= ???)
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
    }*/

    // Write invocation identifiers
    if (version >= XBC_FORMAT_VERSION_1_33)
    {
        unsigned int num_invoke_idents = byte_code->num_invoke_idents;
        xvm_file_write_uint(file, num_invoke_idents);

        for (unsigned int i = 0; i < num_invoke_idents; ++i)
            xvm_string_write_to_file(byte_code->invoke_idents[i], file);
    }

    // Write module names
    if (version >= XBC_FORMAT_VERSION_1_34)
    {
        unsigned int num_module_names = byte_code->num_module_names;
        xvm_file_write_uint(file, num_module_names);

        for (unsigned int i = 0; i < num_module_names; ++i)
            xvm_string_write_to_file(byte_code->module_names[i], file);
    }

    // Close file and return with success
    XVM_FCLOSE(file);

    return 1;
}


/* ----- Module ----- */

int xvm_module_init(xvm_module* module)
{
    if (module != NULL)
    {
        module->handle      = NULL;
        module->proc_count  = NULL;
        module->fetch_proc  = NULL;
        module->fetch_ident = NULL;
        return 1;
    }
    return 0;
}

int xvm_module_load(xvm_module* module, const char* filename)
{
    if (module == NULL || filename == NULL)
        return 0;

    #if defined(_WIN32)
    
    // Load dynamic library
    module->handle = LoadLibraryA(filename);
    if (module->handle == 0)
    {
        xvm_log_error("loading module \"%s\" failed", filename);
        return 0;
    }

    // Load module interface
    module->proc_count = (xvm_module_proc_count_fnc)GetProcAddress(MODULE_GET_HANDLE(module->handle), "xx_module_proc_count");
    module->fetch_proc = (xvm_module_fetch_proc_fnc)GetProcAddress(MODULE_GET_HANDLE(module->handle), "xx_module_fetch_proc");
    module->fetch_ident = (xvm_module_fetch_ident_fnc)GetProcAddress(MODULE_GET_HANDLE(module->handle), "xx_module_fetch_ident");

    #elif defined(__linux__)

    // Load dynamic library
    module->handle = dlopen(filename, RTLD_LAZY);
    if (module->handle == NULL)
    {
        xvm_log_error("loading module \"%s\" failed", filename);
        return 0;
    }

    // Load module interface
    module->proc_count = (xvm_module_proc_count_fnc)dlsym(module->handle, "xx_module_proc_count");
    module->fetch_proc = (xvm_module_fetch_proc_fnc)dlsym(module->handle, "xx_module_fetch_proc");
    module->fetch_ident = (xvm_module_fetch_ident_fnc)dlsym(module->handle, "xx_module_fetch_ident");

    #endif

    if (module->proc_count == NULL || module->fetch_proc == NULL || module->fetch_ident == NULL)
    {
        xvm_log_error("loading interface of module \"%s\" failed", filename);
        return 0;
    }

    return 1;
}

int xvm_module_unload(xvm_module* module)
{
    if (module != NULL)
    {
        #if defined(_WIN32)

        if (module->handle != 0)
        {
            FreeLibrary(MODULE_GET_HANDLE(module->handle));
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

/*
This function binds all invocations of the specified module to the byte code.
All module invocations, which are not part of the byte code, are ignored.
*/
static int _xvm_bytecode_bind_module_ext(xvm_bytecode* byte_code, const xvm_module* module, int unbindFlag)
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
    for (int i = 0; i < num_procs; ++i)
    {
        // Fetch invocation identifier
        const char* ident = module->fetch_ident(i);
        if (ident == NULL)
            return 0;

        if (unbindFlag != 0)
        {
            // Unbind procedure invocation
            xvm_bytecode_bind_invocation(byte_code, ident, NULL);
        }
        else
        {
            // Fetch procedure pointer
            xvm_invocation_proc proc = module->fetch_proc(i);

            // Bind procedure invocation
            xvm_bytecode_bind_invocation(byte_code, ident, proc);
        }
    }

    return 1;
}

int xvm_bytecode_bind_module(xvm_bytecode* byte_code, const xvm_module* module)
{
    return _xvm_bytecode_bind_module_ext(byte_code, module, 0);
}

int xvm_bytecode_unbind_module(xvm_bytecode* byte_code, const xvm_module* module)
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

int xvm_module_container_init(xvm_module_container* container)
{
    if (container != NULL)
    {
        container->first = NULL;
        return 1;
    }
    return 0;
}

int xvm_module_container_add(xvm_module_container* container, xvm_module module)
{
    if (container != NULL)
    {
        _xvm_module_container_node* node = container->first;

        if (node == NULL)
        {
            // Create first node
            container->first = (_xvm_module_container_node*)XVM_MALLOC(sizeof(_xvm_module_container_node));
            node = container->first;
        }
        else
        {
            // Find last node
            while (node->next != NULL)
                node = node->next;

            // Create next node
            node->next = (_xvm_module_container_node*)XVM_MALLOC(sizeof(_xvm_module_container_node));
            node = node->next;
        }

        // Add module to new node and initialize its link
        node->module    = module;
        node->next      = NULL;

        return 1;
    }
    return 0;
}

static int _xvm_module_container_node_free(_xvm_module_container_node* node)
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
            XVM_FREE(node->next);
        }

        return result;
    }
    return 0;
}

int xvm_module_container_clear(xvm_module_container* container)
{
    if (container != NULL)
    {
        if (container->first != NULL)
        {
            // Free node hierarchy
            int result = _xvm_module_container_node_free(container->first);

            // Free first node
            XVM_FREE(container->first);
            container->first = NULL;

            return result;
        }
        return 1;
    }
    return 0;
}

static _xvm_module_container_node* _xvm_module_iterator = NULL;

int xvm_module_iteration_start(const xvm_module_container* container)
{
    if (container != NULL)
    {
        _xvm_module_iterator = container->first;
        return 1;
    }
    return 0;
}

xvm_module* xvm_module_iteration_next()
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

static void _xvm_call_intrinsic(unsigned int intrsc_id, xvm_stack* const stack, regi_t* const reg_sp, regi_t* const reg_ar)
{
    switch (intrsc_id)
    {
        /* --- Dynamic memory intrinsics --- */

        // void* AllocMem(uint sizeInBytes)
        case INSC_ALLOC_MEM:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            void* result = XVM_MALLOC((size_t)arg0);
            *reg_ar = (regi_t)result;
        }
        break;

        // void FreeMem(void* memoryAddr)
        case INSC_FREE_MEM:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            if (arg0 != 0)
                XVM_FREE((void*)arg0);
        }
        break;

        // void CopyMem(void* dstMemAddr, const void* srcMemAddr, uint sizeInBytes)
        case INSC_COPY_MEM:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            int arg1 = _xvm_stack_pop(stack, reg_sp);
            int arg2 = _xvm_stack_pop(stack, reg_sp);
            memcpy((void*)arg0, (const void*)arg1, (size_t)arg2);
        }
        break;

        // void FillMem(void* memoryAddr, uint sizeInByzes, int value)
        case INSC_FILL_MEM:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            int arg1 = _xvm_stack_pop(stack, reg_sp);
            int arg2 = _xvm_stack_pop(stack, reg_sp);
            memset((void*)arg0, arg2, (size_t)arg1);
        }
        break;

        /* --- Console intrinsics --- */

        // void SysCall(const byte* stringPtr)
        case INSC_SYS_CALL:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            if (arg0)
                system(INT_TO_STR_REINTERPRET(arg0));
        }
        break;

        // void Print(const byte* stringPtr)
        case INSC_PRINT:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            if (arg0)
                printf("%s", INT_TO_STR_REINTERPRET(arg0));
        }
        break;

        // void PrintLn(const byte* stringPtr)
        case INSC_PRINT_LN:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            if (arg0)
                puts(INT_TO_STR_REINTERPRET(arg0));
            else
                puts("");
        }
        break;

        // void PrintInt(int value)
        case INSC_PRINT_INT:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            printf("%i", arg0);
        }
        break;

        // void PrintFloat(float value)
        case INSC_PRINT_FLOAT:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            printf("%f", INT_TO_FLT_REINTERPRET(arg0));
        }
        break;

        // void Input(byte* stringPtr, int maxLen)
        case INSC_INPUT:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            int arg1 = _xvm_stack_pop(stack, reg_sp);
            fgets((char*)arg0, arg1, stdin);
        }
        break;

        // int InputInt()
        case INSC_INPUT_INT:
        {
            scanf("%i", reg_ar);
        }
        break;

        // float InputFloat()
        case INSC_INPUT_FLOAT:
        {
            scanf("%f", (float*)reg_ar);
        }
        break;

        /* --- File intrinsics --- */

        // int CreateFile(const byte* filenamePtr)
        case INSC_CREATE_FILE:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            FILE* file = XVM_FOPEN(INT_TO_STR_REINTERPRET(arg0), "w");
            if (file != NULL)
            {
                XVM_FCLOSE(file);
                *reg_ar = 1;
            }
            else
                *reg_ar = 0;
        }
        break;

        // int DeleteFile(const byte* filenamePtr)
        case INSC_DELETE_FILE:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            *reg_ar = remove(INT_TO_STR_REINTERPRET(arg0));
        }
        break;

        // void* OpenFile(const byte* filenamePtr, const byte* modePtr)
        case INSC_OPEN_FILE:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            int arg1 = _xvm_stack_pop(stack, reg_sp);
            *reg_ar = (regi_t)XVM_FOPEN(INT_TO_STR_REINTERPRET(arg0), INT_TO_STR_REINTERPRET(arg1));
        }
        break;

        // void CloseFile(void* fileHandle)
        case INSC_CLOSE_FILE:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            XVM_FCLOSE((FILE*)arg0);
        }
        break;

        // void FileSetPos(void* fileHandle, int offset, int origin)
        case INSC_FILE_SET_POS:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            int arg1 = _xvm_stack_pop(stack, reg_sp);
            int arg2 = _xvm_stack_pop(stack, reg_sp);
            fseek((FILE*)arg0, (long)arg1, arg2);
        }
        break;

        // int FileGetPos(void* fileHandle)
        case INSC_FILE_GET_POS:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            *reg_ar = (int)ftell((FILE*)arg0);
        }
        break;

        // int FileEOF(void* fileHandle)
        case INSC_FILE_EOF:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            *reg_ar = feof((FILE*)arg0);
        }
        break;

        // void WriteByte(void* fileHandle, int value)
        case INSC_WRITE_BYTE:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            int arg1 = _xvm_stack_pop(stack, reg_sp);
            fwrite(&arg1, sizeof(char), 1, (FILE*)arg0);
        }
        break;

        // void WriteWord(void* fileHandle, int value)
        case INSC_WRITE_WORD:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            int arg1 = _xvm_stack_pop(stack, reg_sp);
            fwrite(&arg1, sizeof(int), 1, (FILE*)arg0);
        }
        break;

        // void WriteBuffer(void* fileHandle, const void* memoryAddress, int size)
        case INSC_WRITE_BUFFER:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            int arg1 = _xvm_stack_pop(stack, reg_sp);
            int arg2 = _xvm_stack_pop(stack, reg_sp);
            fwrite((const void*)arg1, sizeof(char), arg2, (FILE*)arg0);
        }
        break;

        // int ReadByte(void* fileHandle)
        case INSC_READ_BYTE:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            fread(reg_ar, sizeof(char), 1, (FILE*)arg0);
        }
        break;

        // int ReadWord(void* fileHandle)
        case INSC_READ_WORD:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            fread(reg_ar, sizeof(int), 1, (FILE*)arg0);
        }
        break;

        // void ReadBuffer(void* fileHandle, void* memoryAddress, int size)
        case INSC_READ_BUFFER:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            int arg1 = _xvm_stack_pop(stack, reg_sp);
            int arg2 = _xvm_stack_pop(stack, reg_sp);
            fread((void*)arg1, sizeof(char), arg2, (FILE*)arg0);
        }
        break;

        /* --- Math intrinsics --- */

        // float Sin(float x)
        case INSC_SIN:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            float result = sinf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        // float Cos(float x)
        case INSC_COS:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            float result = cosf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        // float Tan(float x)
        case INSC_TAN:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            float result = tanf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        // float ASin(float x)
        case INSC_ASIN:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            float result = asinf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        // float ACos(float x)
        case INSC_ACOS:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            float result = acosf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        // float ATan(float x)
        case INSC_ATAN:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            float result = atanf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        // float SinH(float x)
        case INSC_SINH:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            float result = sinhf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        // float CosH(float x)
        case INSC_COSH:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            float result = coshf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        // float TanH(float x)
        case INSC_TANH:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            float result = tanhf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        // float Exp(float x)
        case INSC_EXP:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            float result = expf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        // float Exp2(float x)
        case INSC_EXP2:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            float result = exp2f(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        // float Pow(float base, float exp)
        case INSC_POW:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            int arg1 = _xvm_stack_pop(stack, reg_sp);
            float result = powf(INT_TO_FLT_REINTERPRET(arg0), INT_TO_FLT_REINTERPRET(arg1));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        // float Log(float x)
        case INSC_LOG:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            float result = logf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        // float Log2(float x)
        case INSC_LOG2:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            float result = log2f(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        // float Log10(float x)
        case INSC_LOG10:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            float result = log10f(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        // float Sqrt(float x)
        case INSC_SQRT:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            float result = sqrtf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        // float Ceil(float x)
        case INSC_CEIL:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            float result = ceilf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        // float Floor(float x)
        case INSC_FLOOR:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
            float result = floorf(INT_TO_FLT_REINTERPRET(arg0));
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        /* --- Other intrinsics --- */
        
        // int RandInt() -> In range [0 .. MAX_INT]
        case INSC_RAND_INT:
        {
            *reg_ar = rand();
        }
        break;

        // float RandFloat() -> In range [0.0 .. 1.0]
        case INSC_RAND_FLOAT:
        {
            float result = ((float)rand()) / RAND_MAX;
            *reg_ar = FLT_TO_INT_REINTERPRET(result);
        }
        break;

        // int Time() -> Ellapsed time since program start (in ms.)
        case INSC_TIME:
        {
            clock_t ticks = clock() / (CLOCKS_PER_SEC / 1000);
            *reg_ar = (int)ticks;
        }
        break;

        #ifdef _ENABLE_OS_FEATURES_

        // void Sleep(int duration)
        case INSC_SLEEP:
        {
            int arg0 = _xvm_stack_pop(stack, reg_sp);
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

xvm_exit_codes xvm_execute_program_ext(
    const xvm_bytecode* const byte_code,
    xvm_stack* const stack,
    const xvm_export_address* entry_point,
    const xvm_execution_options* options)
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
    _xvm_env_internal rt_env;
    rt_env.reg_ref      = (int*)reg.i;
    rt_env.stack_begin  = stack->storage;
    rt_env.stack_end    = stack->storage + stack->stack_size;

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
        if (options != NULL && (options->flags & XVM_EXECUTION_FLAG_DEBUG) != 0 && instr != 0)
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
                *reg_cf = _xvm_flt2int_signum(reg.f[reg0] - reg.f[reg1]);
            }
            break;

            /* --- 1-register opcodes --- */

            case OPCODE_PUSH:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                _xvm_stack_push(stack, reg_sp, reg.i[reg0]);
            }
            break;

            case OPCODE_POP:
            {
                reg0 = _xvm_instr_get_reg0(instr);
                reg.i[reg0] = _xvm_stack_pop(stack, reg_sp);
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
                _xvm_stack_push(stack, reg_sp, extra_value);
                _xvm_stack_push(stack, reg_sp, *reg_pc);

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
                _xvm_stack_push(stack, reg_sp, sgn_value);
            }
            break;

            case OPCODE_INVK:
            {
                unsgn_value = _xvm_instr_get_value26(instr);
                if (unsgn_value < byte_code->num_invoke_idents)
                {
                    // Invoke bounded procedure
                    xvm_invocation_proc invokeProc = byte_code->invoke_bindings[unsgn_value];
                    invokeProc((void*)&rt_env);
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
xvm_exit_codes xvm_execute_program(
    const xvm_bytecode* const byte_code,
    xvm_stack* const stack,
    const xvm_execution_options* options)
{
    return xvm_execute_program_ext(byte_code, stack, NULL, options);
}

/**
Executes the specified XBC (XieXie Byte Code) program within the XVM (XieXie Virtual Machine) from the specified entry point.
\param[in] entry_point Specifies the entry point. If this is an unkown entry point, the return value is EXITCODE_UNKNOWN_ENTRY_POINT.
\see xvm_execute_program_ext
*/
xvm_exit_codes xvm_execute_program_entry_point(
    const xvm_bytecode* const byte_code,
    xvm_stack* const stack,
    const char* entry_point,
    const xvm_execution_options* options)
{
    const xvm_export_address* entry_point_addr = NULL;
    
    if (byte_code == NULL)
        return EXITCODE_INVALID_BYTECODE;

    // Find entry point in export addresses
    entry_point_addr = xvm_bytecode_find_export_address(byte_code, entry_point);
    if (entry_point_addr == NULL)
        return EXITCODE_UNKNOWN_ENTRY_POINT;

    // Execute program from entry point
    return xvm_execute_program_ext(byte_code, stack, entry_point_addr, options);
}


// Undefine temporary macros
#undef INLINE


#ifdef __cplusplus
}
#endif

