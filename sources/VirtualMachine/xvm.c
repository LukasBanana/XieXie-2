/**
 * XieXie 2.0 VirtualMachine (xvm) main ANSI-C file
 * 
 * Copyright (c) 2014 by Lukas Hermanns
 * 
 * This file has a separated license than the "XieXie2-Compiler" project.
 * It is licensed unter the terms of the GNU LESSER GENERAL PUBLIC LICENSE (LGPL) Version 3.
 */

/*

                   GNU LESSER GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007

 Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.


  This version of the GNU Lesser General Public License incorporates
the terms and conditions of version 3 of the GNU General Public
License, supplemented by the additional permissions listed below.

  0. Additional Definitions.

  As used herein, "this License" refers to version 3 of the GNU Lesser
General Public License, and the "GNU GPL" refers to version 3 of the GNU
General Public License.

  "The Library" refers to a covered work governed by this License,
other than an Application or a Combined Work as defined below.

  An "Application" is any work that makes use of an interface provided
by the Library, but which is not otherwise based on the Library.
Defining a subclass of a class defined by the Library is deemed a mode
of using an interface provided by the Library.

  A "Combined Work" is a work produced by combining or linking an
Application with the Library.  The particular version of the Library
with which the Combined Work was made is also called the "Linked
Version".

  The "Minimal Corresponding Source" for a Combined Work means the
Corresponding Source for the Combined Work, excluding any source code
for portions of the Combined Work that, considered in isolation, are
based on the Application, and not on the Linked Version.

  The "Corresponding Application Code" for a Combined Work means the
object code and/or source code for the Application, including any data
and utility programs needed for reproducing the Combined Work from the
Application, but excluding the System Libraries of the Combined Work.

  1. Exception to Section 3 of the GNU GPL.

  You may convey a covered work under sections 3 and 4 of this License
without being bound by section 3 of the GNU GPL.

  2. Conveying Modified Versions.

  If you modify a copy of the Library, and, in your modifications, a
facility refers to a function or data to be supplied by an Application
that uses the facility (other than as an argument passed when the
facility is invoked), then you may convey a copy of the modified
version:

   a) under this License, provided that you make a good faith effort to
   ensure that, in the event an Application does not supply the
   function or data, the facility still operates, and performs
   whatever part of its purpose remains meaningful, or

   b) under the GNU GPL, with none of the additional permissions of
   this License applicable to that copy.

  3. Object Code Incorporating Material from Library Header Files.

  The object code form of an Application may incorporate material from
a header file that is part of the Library.  You may convey such object
code under terms of your choice, provided that, if the incorporated
material is not limited to numerical parameters, data structure
layouts and accessors, or small macros, inline functions and templates
(ten or fewer lines in length), you do both of the following:

   a) Give prominent notice with each copy of the object code that the
   Library is used in it and that the Library and its use are
   covered by this License.

   b) Accompany the object code with a copy of the GNU GPL and this license
   document.

  4. Combined Works.

  You may convey a Combined Work under terms of your choice that,
taken together, effectively do not restrict modification of the
portions of the Library contained in the Combined Work and reverse
engineering for debugging such modifications, if you also do each of
the following:

   a) Give prominent notice with each copy of the Combined Work that
   the Library is used in it and that the Library and its use are
   covered by this License.

   b) Accompany the Combined Work with a copy of the GNU GPL and this license
   document.

   c) For a Combined Work that displays copyright notices during
   execution, include the copyright notice for the Library among
   these notices, as well as a reference directing the user to the
   copies of the GNU GPL and this license document.

   d) Do one of the following:

       0) Convey the Minimal Corresponding Source under the terms of this
       License, and the Corresponding Application Code in a form
       suitable for, and under terms that permit, the user to
       recombine or relink the Application with a modified version of
       the Linked Version to produce a modified Combined Work, in the
       manner specified by section 6 of the GNU GPL for conveying
       Corresponding Source.

       1) Use a suitable shared library mechanism for linking with the
       Library.  A suitable mechanism is one that (a) uses at run time
       a copy of the Library already present on the user's computer
       system, and (b) will operate properly with a modified version
       of the Library that is interface-compatible with the Linked
       Version.

   e) Provide Installation Information, but only if you would otherwise
   be required to provide such information under section 6 of the
   GNU GPL, and only to the extent that such information is
   necessary to install and execute a modified version of the
   Combined Work produced by recombining or relinking the
   Application with a modified version of the Linked Version. (If
   you use option 4d0, the Installation Information must accompany
   the Minimal Corresponding Source and Corresponding Application
   Code. If you use option 4d1, you must provide the Installation
   Information in the manner specified by section 6 of the GNU GPL
   for conveying Corresponding Source.)

  5. Combined Libraries.

  You may place library facilities that are a work based on the
Library side by side in a single library together with other library
facilities that are not Applications and are not covered by this
License, and convey such a combined library under terms of your
choice, if you do both of the following:

   a) Accompany the combined library with a copy of the same work based
   on the Library, uncombined with any other library facilities,
   conveyed under the terms of this License.

   b) Give prominent notice with the combined library that part of it
   is a work based on the Library, and explaining where to find the
   accompanying uncombined form of the same work.

  6. Revised Versions of the GNU Lesser General Public License.

  The Free Software Foundation may publish revised and/or new versions
of the GNU Lesser General Public License from time to time. Such new
versions will be similar in spirit to the present version, but may
differ in detail to address new problems or concerns.

  Each version is given a distinguishing version number. If the
Library as you received it specifies that a certain numbered version
of the GNU Lesser General Public License "or any later version"
applies to it, you have the option of following the terms and
conditions either of that published version or of any later version
published by the Free Software Foundation. If the Library as you
received it does not specify a version number of the GNU Lesser
General Public License, you may choose any version of the GNU Lesser
General Public License ever published by the Free Software Foundation.

  If the Library as you received it specifies that a proxy can decide
whether future versions of the GNU Lesser General Public License shall
apply, that proxy's public statement of acceptance of any version is
permanent authorization for you to choose that version for the
Library.

*/

#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>


/* ----- Helper macros ----- */

#define XVM_MIN(a, b) ((a) < (b) ? (a) : (b))
#define XVM_MAX(a, b) ((a) > (b) ? (a) : (b))

#if 0
#   define INLINE __inline
#else
#   define INLINE /* No inlining */
#endif


/* ----- Helper functions ----- */

static int flt2int_signum(float val)
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

#define NUM_REGISTERS       16
#define IS_REG_FLOAT(r)     ((r) >= REG_F0)
#define REG_TO_STACK_PTR(r) ((stack_word_t*)(*(r)))


typedef enum
{
    REG_I0 = 0x00, //!< Integer register 0.
    REG_I1 = 0x01, //!< Integer register 1.
    REG_I2 = 0x02, //!< Integer register 2.
    REG_I3 = 0x03, //!< Integer register 3.
    REG_I4 = 0x04, //!< Integer register 4.
    REG_I5 = 0x05, //!< Integer register 5.

    REG_CF = 0x06, //!< Conditional flags: used for jump conditions.
    REG_LB = 0x07, //!< Local base pointer: pointer to the current local stack base.
    REG_SP = 0x08, //!< Stack-pointer.
    REG_PC = 0x09, //!< Program-counter.

    REG_F0 = 0x0a, //!< Floating-point register 0.
    REG_F1 = 0x0b, //!< Floating-point register 1.
    REG_F2 = 0x0c, //!< Floating-point register 2.
    REG_F3 = 0x0d, //!< Floating-point register 3.
    REG_F4 = 0x0e, //!< Floating-point register 4.
    REG_F5 = 0x0f, //!< Floating-point register 5.
}
register_id;


/* ----- OP-codes ----- */

typedef unsigned char opcode_t;

typedef enum
{
    OPCODE_MOV2     = 0x01, // MOV  reg0, reg1  ->  *reg0 = *reg1
    OPCODE_NOT2     = 0x02, // NOT  reg0, reg1  ->  *reg0 = ~*reg1
    OPCODE_AND2     = 0x03, // AND  reg0, reg1  ->  *reg0 &= *reg1
    OPCODE_OR2      = 0x04, // OR   reg0, reg1  ->  *reg0 |= *reg1
    OPCODE_XOR2     = 0x05, // XOR  reg0, reg1  ->  *reg0 ^= *reg1
    OPCODE_ADD2     = 0x06, // ADD  reg0, reg1  ->  *reg0 += *reg1
    OPCODE_SUB2     = 0x07, // SUB  reg0, reg1  ->  *reg0 -= *reg1
    OPCODE_MUL2     = 0x08, // MUL  reg0, reg1  ->  *reg0 *= *reg1
    OPCODE_DIV2     = 0x09, // DIV  reg0, reg1  ->  *reg0 /= *reg1
    OPCODE_MOD2     = 0x0a, // MOD  reg0, reg1  ->  *reg0 %= *reg1
    OPCODE_SLL2     = 0x0b, // SLL  reg0, reg1  ->  *reg0 <<= *reg1
    OPCODE_SLR2     = 0x0c, // SLR  reg0, reg1  ->  *reg0 >>= *reg1
    OPCODE_CMP      = 0x0d, // CMP  reg0, reg1  ->  REG_CF = (*reg0 - *reg1)
    OPCODE_FTI      = 0x0e, // FTI  reg0, reg1  ->  *reg0 = (int)*reg1
    OPCODE_ITF      = 0x0f, // ITF  reg0, reg1  ->  *reg0 = (float)*reg1
}
opcode_reg2;

typedef enum
{
    OPCODE_MOV1     = 0x10, // MOV  reg, c  ->  *reg = c
    OPCODE_AND1     = 0x11, // AND  reg, c  ->  *reg &= c
    OPCODE_OR1      = 0x12, // OR   reg, c  ->  *reg |= c
    OPCODE_XOR1     = 0x13, // XOR  reg, c  ->  *reg ^= c
    OPCODE_ADD1     = 0x14, // ADD  reg, c  ->  *reg += c
    OPCODE_SUB1     = 0x15, // SUB  reg, c  ->  *reg -= c
    OPCODE_MUL1     = 0x16, // MUL  reg, c  ->  *reg *= c
    OPCODE_DIV1     = 0x17, // DIV  reg, c  ->  *reg /= c
    OPCODE_MOD1     = 0x18, // MOD  reg, c  ->  *reg %= c
    OPCODE_SLL1     = 0x19, // SLL  reg, c  ->  *reg <<= c
    OPCODE_SLR1     = 0x1a, // SLR  reg, c  ->  *reg >>= c
    OPCODE_PUSH     = 0x1b, // PUSH reg     ->  stack.push(*reg)
    OPCODE_POP      = 0x1c, // POP  reg     ->  *reg = stack.pop()
    OPCODE_INC      = 0x1d, // INC  reg     ->  ++*reg
    OPCODE_DEC      = 0x1e, // DEV  reg     ->  --*reg
}
opcode_reg1;

typedef enum
{
    OPCODE_JMP      = 0x20, // JMP  addr  -> goto addr
    OPCODE_JE       = 0x21, // JE   addr  -> if (REG_CF == 0) then goto addr
    OPCODE_JNE      = 0x22, // JNE  addr  -> if (REG_CF != 0) then goto addr
    OPCODE_JG       = 0x23, // JG   addr  -> if (REG_CF  > 0) then goto addr
    OPCODE_JL       = 0x24, // JL   addr  -> if (REG_CF  < 0) then goto addr
    OPCODE_JGE      = 0x25, // JGE  addr  -> if (REG_CF >= 0) then goto addr
    OPCODE_JLE      = 0x26, // JLE  addr  -> if (REG_CF <= 0) then goto addr
    OPCODE_CALL     = 0x27, // CALL addr  -> PUSH pc ; PUSH lb ; JMP addr
    /*
    Unused          = 0x28,
    Unused          = 0x29,
    Unused          = 0x2a,
    Unused          = 0x2b,
    Unused          = 0x2c,
    Unused          = 0x2d,
    Unused          = 0x2e,
    Unused          = 0x2f,
    */
}
opcode_jump;

typedef enum
{
    OPCODE_LDB      = 0x38, // LDB reg0, addr  ->  *reg0 = programDataSectionByteAligned[addr]
    //Unused        = 0x39,
    OPCODE_LDW      = 0x3a, // STB reg0, addr  ->  *reg0 = programDataSectionWorldAligned[addr]
    //Unused        = 0x3b,
}
opcode_mem;

typedef enum
{
    OPCODE_LDBO     = 0x3c, // LDB reg0, (reg1) c  ->  *reg0 = dynamicMemoryByteAligned[*reg1 + c]
    OPCODE_STBO     = 0x3d, // STB reg0, (reg1) c  ->  dynamicMemoryByteAligned[*reg1 + c] = *reg0
    OPCODE_LDWO     = 0x3e, // LDW reg0, (reg1) c  ->  *reg0 = dynamicMemoryWordAligned[*reg1 + c]
    OPCODE_STWO     = 0x3f, // STW reg0, (reg1) c  ->  dynamicMemoryWordAligned[*reg1 + c] = *reg0
}
opcode_memoff;

typedef enum
{
    OPCODE_STOP     = 0x00, // STOP          ->  exit(0)
    OPCODE_RET      = 0x30, // RET  (c0) c1  ->  return
    OPCODE_PUSHC    = 0x31, // PUSH c        ->  stack.push(c)
    /*
    Unused          = 0x32,
    Unused          = 0x33,
    Unused          = 0x34,
    Unused          = 0x35,
    Unused          = 0x36,
    Unused          = 0x37,
    */
}
opcode_special;


/* ----- Virtual machine exit codes ----- */

typedef enum
{
    VMEXITCODE_SUCCESS          = 0,
    VMEXITCODE_INVALID_BYTECODE = -1,
    VMEXITCODE_INVALID_STACK    = -2,
    VMEXITCODE_INVALID_OPCODE   = -3,
    VMEXITCODE_STACK_OVERFLOW   = -4,
    VMEXITCODE_STACK_UNDERFLOW  = -5,
    VMEXITCODE_DIVISION_BY_ZERO = -6,
}
xvm_exit_codes;


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
    printf("ERROR: %s!", str);
}


/* ----- Instruction ----- */

typedef unsigned int instr_t;

INLINE static opcode_t instr_get_opcode(const instr_t instr)
{
    return (instr & 0xfc000000) >> 26;
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
        ((opcode & 0x3f      ) << 26) |
        ((reg0   & 0x0f      ) << 22) |
        ((reg1   & 0x0f      ) << 18) |
         (flags  & 0x0003ffff)
    );
}

static instr_t instr_make_reg1(opcode_reg1 opcode, reg_t reg, unsigned int value)
{
    return (instr_t)(
        ((opcode & 0x3f      ) << 26) |
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
        ((opcode & 0x3f      ) << 26) |
         (value  & 0x03ffffff)
    );
}

static instr_t instr_make_special2(opcode_special opcode, unsigned int result_size, unsigned int arg_size)
{
    return (instr_t)(
        ((opcode      & 0x3f      ) << 26) |
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
    xvm_exception_err = error_message;
    longjmp(xvm_exception_envbuf, error_code);
}


/* ----- Stack ----- */

typedef int stack_word_t;

typedef struct
{
    size_t          stack_size;
    stack_word_t*   storage;
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
        stack->stack_size   = 0;
        stack->storage      = NULL;
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

        stack->stack_size   = 0;
        stack->storage      = NULL;

        return 1;
    }
    return 0;
}

static void xvm_stack_push(xvm_stack* stack, regi_t* reg_sp, stack_word_t value)
{
    stack_word_t* stack_ptr = REG_TO_STACK_PTR(reg_sp);
    if (stack_ptr < stack->storage + stack->stack_size)
    {
        *stack_ptr = value;
        (*reg_sp) += sizeof(stack_word_t);
    }
    else
        xvm_exception_throw("Stack overflow", VMEXITCODE_STACK_OVERFLOW);
}

static stack_word_t xvm_stack_pop(xvm_stack* stack, regi_t* reg_sp)
{
    stack_word_t* stack_ptr = REG_TO_STACK_PTR(reg_sp);
    if (stack_ptr > stack->storage)
        (*reg_sp) -= sizeof(stack_word_t);
    else
        xvm_exception_throw("Stack underflow", VMEXITCODE_STACK_OVERFLOW);
    return *REG_TO_STACK_PTR(reg_sp);
}

static void xvm_stack_debug(xvm_stack* stack, size_t num_entries)
{
    if (stack != NULL)
    {
        const size_t n = XVM_MIN(num_entries, stack->stack_size);
        for (size_t i = 0; i < n; ++i)
            printf("stack[%i] = %i\n", i, stack->storage[i]);
    }
}


/* ----- Virtual machine ----- */

typedef struct
{
    int pc_reset;
    //int max_num_cycles;
}
xvm_execution_state;

/**
Executes the specified XBC (XieXie Byte Code) program within the XVM (XieXie Virtual Machine).
\param[in] byte_code Pointer to the byte code to execute.
\param[in] stack Pointer to the stack which is to be used during execution.
\param[in,out] exe_state Optional pointer to the execution state. This may also be NULL.
\see xvm_bytecode_create
\see xvm_stack_create
*/
static xvm_exit_codes xvm_execute_program(
    const xvm_bytecode* const byte_code,
    xvm_stack* const stack,
    xvm_execution_state* const exe_state)
{
    if (byte_code == NULL)
        return VMEXITCODE_INVALID_BYTECODE;
    if (stack == NULL)
        return VMEXITCODE_INVALID_STACK;

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

    //! Program start pointer is used to load memory from program "DATA" section.
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
                    xvm_exception_throw("Division by zero", VMEXITCODE_DIVISION_BY_ZERO);
                reg.i[reg0] /= sgn_value;
            }
            break;

            // Undefined behavior for floats
            case OPCODE_MOD1:
            {
                reg0 = instr_get_reg0(instr);
                sgn_value = instr_get_sgn_value22(instr);
                if (sgn_value == 0)
                    xvm_exception_throw("Division by zero", VMEXITCODE_DIVISION_BY_ZERO);
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
            {
                return VMEXITCODE_SUCCESS;
            }

            case OPCODE_RET:
            break;

            case OPCODE_PUSHC:
            {
                sgn_value = instr_get_sgn_value26(instr);
                xvm_stack_push(stack, reg_sp, sgn_value);
            }
            break;

            default:
            {
                /* Unknown opcode -> return with error */
                return VMEXITCODE_INVALID_OPCODE;
            }
        }

        /* Increase program-counter register */
        ++(*reg_pc);
    }

    /* Return program counter if set */
    if (exe_state != NULL)
        exe_state->pc_reset = *reg_pc;

    return VMEXITCODE_SUCCESS;
}


/* ----- Main ----- */

int main(int argc, char* argv[])
{
    
    #if 1

    // Create a virtual stack
    xvm_stack stack;
    xvm_stack_init(&stack);
    xvm_stack_create(&stack, 256);

    // Create the byte code
    xvm_bytecode byte_code;
    xvm_bytecode_init(&byte_code);
    xvm_bytecode_create(&byte_code, 50);

    size_t i = 0;
    #define ADD_INSTR(INSTR) byte_code.instructions[i++] = INSTR;

    /*
    for int i := 0 ; i < 10; i++ {
        Stack.Push(i)
    }
    */
    /*
            i0 = 0
            i1 = 10
    l_for:  cmp i0, i1
            jge l_end
            push i0
            inc i0
            jmp l_for
    l_end:  stop
    */
    ADD_INSTR(instr_make_reg1(OPCODE_MOV1, REG_I0, 0))
    ADD_INSTR(instr_make_reg1(OPCODE_MOV1, REG_I1, 10))
    ADD_INSTR(instr_make_reg2(OPCODE_CMP, REG_I0, REG_I1, 0))
    ADD_INSTR(instr_make_jump(OPCODE_JGE, REG_PC, 5))
    ADD_INSTR(instr_make_reg1(OPCODE_PUSH, REG_I0, 0))
    ADD_INSTR(instr_make_reg1(OPCODE_INC, REG_I0, 0))
    ADD_INSTR(instr_make_reg1(OPCODE_JMP, REG_PC, (unsigned int)(-4)))
    ADD_INSTR(instr_make_special1(OPCODE_STOP, 0))
    
    #undef ADD_INSTR

    // Execute the virtual program
    const xvm_exit_codes exitCode = xvm_execute_program(&byte_code, &stack, NULL);

    if (exitCode != VMEXITCODE_SUCCESS)
        printf("\nProgram terminated with error code: %i\n\n", exitCode);

    // Show stack output for the 20th first values
    log_println("-- Stack content: --");
    xvm_stack_debug(&stack, 20);

    xvm_stack_free(&stack);
    xvm_bytecode_free(&byte_code);

    system("pause");

    #endif

    return 0;
}
