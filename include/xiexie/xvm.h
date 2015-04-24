/*
 * xvm.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_XVM_H__
#define __XX_XVM_H__


#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <math.h>
#include <time.h>

/* ----- Compilation configuration ----- */

//! Enables runtimer debugger in virtual machine
#if defined(_DEBUG) && !defined(_ENABLE_RUNTIME_DEBUGGER_)
#   define _ENABLE_RUNTIME_DEBUGGER_
#endif

//! Shows the instruction indices as hex numbers.
//#define _SHOW_RUNTIME_HEXLINES_

/**
Enables a simple mechanism to detect if the VM produced memory leaks.
Only the number of memory leaks can be detected, after the program has finished.
*/
#define _ENABLE_LEAK_DETECTION_

//! Enables opcode extraction optimization (safes one SLL instruction in x86 code)
#define _OPTIMIZE_OPCODE_EXTRACTION_


#ifdef __cplusplus
extern "C" {
#endif

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
const char* xvm_register_get_name(reg_t reg);


/* ----- OP-codes ----- */

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


/* ----- VM common functions ----- */

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

/**
Returns the specified exit code as string.
\see xvm_exit_codes
*/
const char* xvm_exitcode_to_string(const xvm_exit_codes exit_code);

/**
Returns the count of all memory references for this VM.
\note If '_ENABLE_LEAK_DETECTION_' was not defined, the return value is always 0.
\see _ENABLE_LEAK_DETECTION_
*/
int xvm_memory_ref_count();

/**
Returns the count of all open file references for this VM.
\note If '_ENABLE_LEAK_DETECTION_' was not defined, the return value is always 0.
\see _ENABLE_LEAK_DETECTION_
*/
int xvm_file_ref_count();


/* ----- Debug log ----- */

//! Prints the specified string to the log output.
void xvm_log_print(const char* str);

//! Prints the specified string with a new-line character to the log output.
void xvm_log_println(const char* str);

//! Prints the specified string as error message to the log output, e.g. "error: $s\\n".
void xvm_log_error(const char* format, ...);

//! Prints the specified string as warning message to the log output, e.g. "warning: $s\\n".
void xvm_log_warning(const char* format, ...);

//! Prints a 'read file error' for the specified filename to the log output.
void xvm_log_readfile_error(const char* filename);

//! Prints the specified exit code to the log output.
void xvm_log_exitcode_error(const xvm_exit_codes exit_code);


/* ----- File helper ----- */

//! Reads an 32-bit unsigned integer from the file.
unsigned int xvm_file_read_uint(FILE* file);

//! Writes the specified 32-bit unsigned integer to file.
void xvm_file_write_uint(FILE* file, unsigned int value);


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
    INSC_SYS_CALL,      // void SysCall(const byte* stringPtr)
    INSC_PRINT,         // void Print(const byte* stringPtr)
    INSC_PRINT_LN,      // void PrintLn(const byte* stringPtr)
    INSC_PRINT_INT,     // void PrintInt(int value)
    INSC_PRINT_FLOAT,   // void PrintFloat(float value)
    INSC_INPUT,         // void Input(byte* stringPtr, int maxLen)
    INSC_INPUT_INT,     // int InputInt()
    INSC_INPUT_FLOAT,   // float InputFloat()

    /* --- File intrinsics --- */
    INSC_CREATE_FILE,   // int CreateFile(const byte* filenamePtr)
    INSC_DELETE_FILE,   // int DeleteFile(const byte* filenamePtr)
    INSC_OPEN_FILE,     // void* OpenFile(const byte* filenamePtr, const byte* modePtr)
    INSC_CLOSE_FILE,    // void CloseFile(void* fileHandle)
    INSC_FILE_SET_POS,  // void FileSetPos(void* fileHandle, int offset, int origin)
    INSC_FILE_GET_POS,  // int FileGetPos(void* fileHandle)
    INSC_FILE_EOF,      // int FileEOF(void* fileHandle)
    INSC_WRITE_BYTE,    // void WriteByte(void* fileHandle, int value)
    INSC_WRITE_WORD,    // void WriteWord(void* fileHandle, int value)
    INSC_WRITE_BUFFER,  // void WriteBuffer(void* fileHandle, const void* memoryAddress, int size)
    INSC_READ_BYTE,     // int ReadByte(void* fileHandle)
    INSC_READ_WORD,     // int ReadWord(void* fileHandle)
    INSC_READ_BUFFER,   // void ReadBuffer(void* fileHandle, void* memoryAddress, int size)

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
const char* xvm_intrinsic_get_ident(const xvm_intrinsic_id addr);


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
opcode_t xvm_instr_get_opcode(const instr_t instr);

/**
Returns the mnemonic of the specified instruction opcode or an empty string if the opcode is invalid.
If the opcode is valid, the returned string will always consist of 4 characters plus the null terminator '\0'.
*/
const char* xvm_instr_get_mnemonic(const opcode_t opcode);


/* ----- Instruction constructors ----- */

//! Makes a 3-register instruction.
instr_t xvm_instr_make_reg3(xvm_opcode opcode, reg_t reg0, reg_t reg1, reg_t reg2);

//! Makes a 2-register instruction.
instr_t xvm_instr_make_reg2(xvm_opcode opcode, reg_t reg0, reg_t reg1, unsigned int value);

//! Makes a 1-register instruction.
instr_t xvm_instr_make_reg1(xvm_opcode opcode, reg_t reg, unsigned int value);

//! Makes a jump instruction.
instr_t xvm_instr_make_jump(xvm_opcode opcode, reg_t reg, unsigned int offset);

//! Makes a load/store instruction.
instr_t xvm_instr_make_loadstore(xvm_opcode opcode, reg_t reg0, reg_t reg1, unsigned int offset);

//! Makes a special-1 instruction.
instr_t xvm_instr_make_special1(xvm_opcode opcode, unsigned int value);

//! Makes a special-2 instruction.
instr_t xvm_instr_make_special2(xvm_opcode opcode, unsigned int result_size, unsigned int arg_size);

//! Patches the 'value' back to the specified instruction.
instr_t xvm_instr_patch_value(instr_t instr, unsigned int value);


/* ----- String ----- */

//! XVM string structure.
typedef struct
{
    size_t  len;
    char*   str;
}
xvm_string;

//! Returns an empty string object
xvm_string xvm_string_init();

//! Creates a new string object with the specified length (plus the null terminator '\0').
xvm_string xvm_string_create(size_t len);

//! Creates a string from the specified string literal.
xvm_string xvm_string_create_from(const char* str);

//! Creates a string from the specified string literal of the first 'len' characters.
xvm_string xvm_string_create_from_sub(const char* str, size_t len);

//! Appends the string 'append_str' to the specified string object.
int xvm_string_append(xvm_string* string, const char* append_str);

//! Frees the memory of the specified string.
int xvm_string_free(xvm_string* string);

//! Reads a string from the specified file
xvm_string xvm_string_read_from_file(FILE* file);

//! Reads a string from the specified file
int xvm_string_write_to_file(xvm_string string, FILE* file);


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
int xvm_stack_init(xvm_stack* stack);

/**
Clears all entries in the stack with the specified value
\see xvm_stack_create
*/
int xvm_stack_clear(xvm_stack* stack, stack_word_t value);

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
int xvm_stack_create(xvm_stack* stack, size_t stack_size);

//! Fress the memory for the specified stack.
int xvm_stack_free(xvm_stack* stack);


/* ----- Stack Environment ----- */

//! XVM environment state type.
typedef void* xvm_env;

/**
Invocation procedure signature. This is the signature for external procedure invocations in ANSI C.
\param[in] env Environment handle to access the program state (e.g. the virtual stack).
*/
typedef void (*xvm_invocation_proc)(xvm_env env);


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
int xvm_export_address_init(xvm_export_address* export_address);

//! Initializes the export address with the specified startup values.
int xvm_export_address_setup(xvm_export_address* export_address, unsigned int addr, xvm_string label);

//! Frees the memory for the specified export address object.
int xvm_export_address_free(xvm_export_address* export_address);


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
int xvm_import_address_init(xvm_import_address* import_address);

/**
Initializes the import address with the specified startup values.
\note All indices are uninitialized!
*/
int xvm_import_address_setup(xvm_import_address* import_address, unsigned int num_indices);

//! Frees the memory for the specified import address object.
int xvm_import_address_free(xvm_import_address* import_address);


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

    unsigned int            num_module_names;       //!< Number of module names. By default 0.
    xvm_string*             module_names;           //!< Module name array. By default NULL.
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
int xvm_bytecode_init(xvm_bytecode* byte_code);

/**
Allocates memory for the specified amount of byte code instructions.
\param[in,out] byte_code Pointer to the byte code object.
\param[in] num_instructions Specifies the number of instruction to allocate for the byte code.
\see xvm_bytecode_init
\note All instructions are uninitialized!
*/
int xvm_bytecode_create_instructions(xvm_bytecode* byte_code, unsigned int num_instructions);

/**
Allocates memory for the specified amount of byte code export addresses.
\param[in,out] byte_code Pointer to the byte code object.
\param[in] num_export_addresses Specifies the number of export addresses to allocate for the byte code.
\see xvm_bytecode_init
\see xvm_export_address
\note All export addresses are uninitialized!
*/
int xvm_bytecode_create_export_addresses(xvm_bytecode* byte_code, unsigned int num_export_addresses);

/**
Allocates memory for the specified amount of byte code import addresses.
\param[in,out] byte_code Pointer to the byte code object.
\param[in] num_export_addresses Specifies the number of export addresses to allocate for the byte code.
\see xvm_bytecode_init
\see xvm_import_address
\note All import addresses are uninitialized!
*/
int xvm_bytecode_create_import_addresses(xvm_bytecode* byte_code, unsigned int num_import_addresses);

//! Returns the export address with the specified label or NULL if there is no such export address.
const xvm_export_address* xvm_bytecode_find_export_address(const xvm_bytecode* byte_code, const char* label);

#if 0 // !INCOMPLETE!

/**
Dynamically links the client byte-code to the main byte-code.
This function will resolve as much import address (from both sides) as possible.
\note If the program termination of 'byte_code' only works when the program counter
runs at the end of the code, this may no longer work, when the client code is appended!
\see xvm_bytecode_create_export_addresses
\see xvm_bytecode_create_import_addresses
*/
int xvm_bytecode_dynamic_link(xvm_bytecode* byte_code, xvm_bytecode* client_byte_code);

#endif

/**
Allocates memory for the specified amount of byte code invocation identifiers and bindings.
\param[in,out] byte_code Pointer to the byte code object.
\param[in] num_invoke_idents Specifies the number of invocation identifiers to allocate for the byte code.
\see xvm_bytecode_init
\note All invocation identifiers are uninitialized!
But all invocation bindings will be initialized to a default procedure.
*/
int xvm_bytecode_create_invocations(xvm_bytecode* byte_code, unsigned int num_invoke_idents);

/**
Binds a single invocation callback with the identifier 'ident to the specified byte code.
\param[in,out] byte_code Pointer to the byte code object.
\param[in] ident String which specifies the invocation identifier.
\param[in] proc Function pointer to the invocation callback.
\see xvm_invocation_proc
*/
int xvm_bytecode_bind_invocation(xvm_bytecode* byte_code, const char* ident, xvm_invocation_proc proc);

/**
Allocates memory for the specified amount of byte code module names.
\param[in,out] byte_code Pointer to the byte code object.
\param[in] num_module_names Specifies the number of module names to allocate for the byte code.
\note All module names are uninitialized!
*/
int xvm_bytecode_create_module_names(xvm_bytecode* byte_code, unsigned int num_module_names);

//! Frees the memory for the specified byte code object.
int xvm_bytecode_free(xvm_bytecode* byte_code);

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
int xvm_bytecode_datafield_ascii(instr_t* instr_ptr, const char* text, size_t* num_instructions);

#define XBC_FORMAT_MAGIC            (*((int*)("XBCF")))
#define XBC_FORMAT_VERSION_1_31     131
#define XBC_FORMAT_VERSION_1_32     132
#define XBC_FORMAT_VERSION_1_33     133
#define XBC_FORMAT_VERSION_1_34     134
#define XBC_FORMAT_VERSION_LATEST   XBC_FORMAT_VERSION_1_34

/**
Reads a byte code form file.
\param[out] byte_code Specifies the resulting byte code object.
\param[in] filename Specifies the filename from which the byte code is to be read.
\return Zero on failure and non-zero otherwise.
*/
int xvm_bytecode_read_from_file(xvm_bytecode* byte_code, const char* filename);

/**
Writes the specified byte code to file.
\param[in] byte_code specifies the byte code which is to be written to file.
\param[in] filename Specifies the filename which is to be used to create the file.
\param[in] version Specifies the format version. Must be one of the following values:
XBC_FORMAT_VERSION_1_11, XBC_FORMAT_VERSION_1_12, XBC_FORMAT_VERSION_1_13, or XBC_FORMAT_VERSION_LATEST.
\return Zero on failure and non-zero otherwise.
*/
int xvm_bytecode_write_to_file(const xvm_bytecode* byte_code, const char* filename, unsigned int version);


/* ----- Module ----- */

typedef int (*xvm_module_proc_count_fnc)(void);
typedef xvm_invocation_proc (*xvm_module_fetch_proc_fnc)(int); 
typedef const char* (*xvm_module_fetch_ident_fnc)(int); 

//! XVM module library structure.
typedef struct
{
    void*                       handle;
    xvm_module_proc_count_fnc   proc_count;
    xvm_module_fetch_proc_fnc   fetch_proc;
    xvm_module_fetch_ident_fnc  fetch_ident;
}
xvm_module;

//! Initializes the specified module with its default values.
int xvm_module_init(xvm_module* module);

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
int xvm_module_load(xvm_module* module, const char* filename);

/**
Unloads the specified module library.
\see xvm_module_load
*/
int xvm_module_unload(xvm_module* module);

//! Binds the specified module to 'byte_code'.
int xvm_bytecode_bind_module(xvm_bytecode* byte_code, const xvm_module* module);

//! Unbinds the specified module form 'byte_code'.
int xvm_bytecode_unbind_module(xvm_bytecode* byte_code, const xvm_module* module);


/* ----- Moduel Container ------ */

struct _xvm_module_container_node;

//! XVM module contianer structure.
typedef struct
{
    struct _xvm_module_container_node* first;
}
xvm_module_container;

//! Initializes the specified module container with its default values.
int xvm_module_container_init(xvm_module_container* container);

int xvm_module_container_add(xvm_module_container* container, xvm_module module);

/**
Clears the specified container and unloads all modules.
\note No byte code must then use any module which was loaded with this container!
*/
int xvm_module_container_clear(xvm_module_container* container);

int xvm_module_iteration_start(const xvm_module_container* container);

xvm_module* xvm_module_iteration_next();


/* ----- Virtual machine ----- */

/**
Program execution flag: enable run-time debugging.
\note Only available if '_ENABLE_RUNTIME_DEBUGGER_' was defined.
*/
#define XVM_EXECUTION_FLAG_DEBUG (0x00000001)

//! XVM program execution options structure.
typedef struct
{
    int flags;
}
xvm_execution_options;

/**
Executes the specified XBC (XieXie Byte Code) program within the XVM (XieXie Virtual Machine).
\param[in] byte_code Pointer to the byte code to execute.
\param[in] stack Pointer to the stack which is to be used during execution.
\param[in] entry_point Optional pointer to the entry point address.
This may also be NULL to start the program at the top.
\param[in] options Optional pointer to execution options. This may also be NULL to use the default options.
\remarks This is the main function for the entire virtual machine.
All instructions are implemented inside this function and its large switch-case statement.
\see xvm_bytecode_create_instructions
\see xvm_stack_create
*/
xvm_exit_codes xvm_execute_program_ext(
    const xvm_bytecode* const byte_code,
    xvm_stack* const stack,
    const xvm_export_address* entry_point,
    const xvm_execution_options* options
);

/**
Executes the specified XBC (XieXie Byte Code) program within the XVM (XieXie Virtual Machine) from the beginning.
\see xvm_execute_program_ext
*/
xvm_exit_codes xvm_execute_program(
    const xvm_bytecode* const byte_code,
    xvm_stack* const stack,
    const xvm_execution_options* options
);

/**
Executes the specified XBC (XieXie Byte Code) program within the XVM (XieXie Virtual Machine) from the specified entry point.
\param[in] entry_point Specifies the entry point. If this is an unkown entry point, the return value is EXITCODE_UNKNOWN_ENTRY_POINT.
\see xvm_execute_program_ext
*/
xvm_exit_codes xvm_execute_program_entry_point(
    const xvm_bytecode* const byte_code,
    xvm_stack* const stack,
    const char* entry_point,
    const xvm_execution_options* options
);


#ifdef __cplusplus
}
#endif


#endif // /__XX_XVM_H__

