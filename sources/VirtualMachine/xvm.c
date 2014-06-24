/**
 * XieXie 2.0 VirtualMachine (xvm) main ANSI-C file
 * 
 * This file is part of the "XieXie2-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <stdlib.h>
#include <stdio.h>


/* ----- Registers ----- */

typedef unsigned char   reg_t;
typedef int             regi_t;
typedef float           regf_t;

#define NUM_REGISTERS   16
#define IS_REG_FLOAT(r) ((r) >= REG_F0)

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
    OPCODE_LDB      = 0x38, // LDB reg0, addr  ->  *reg0 = byteMemory[addr]
    OPCODE_STB      = 0x39, // STB reg0, addr  ->  byteMemory[addr] = *reg0
    OPCODE_LDW      = 0x3a, // LDW reg0, addr  ->  *reg0 = wordMemory[addr]
    OPCODE_STW      = 0x3b, // STW reg0, addr  ->  wordMemory[addr] = *reg0
}
opcode_mem;

typedef enum
{
    OPCODE_LDBO     = 0x3c, // LDB reg0, (reg1) c  ->  *reg0 = byteMemory[*reg1 + c]
    OPCODE_STBO     = 0x3d, // STB reg0, (reg1) c  ->  byteMemory[*reg1 + c] = *reg0
    OPCODE_LDWO     = 0x3e, // LDW reg0, (reg1) c  ->  *reg0 = wordMemory[*reg1 + c]
    OPCODE_STWO     = 0x3f, // STW reg0, (reg1) c  ->  wordMemory[*reg1 + c] = *reg0
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


/* ----- Instruction ----- */

typedef unsigned int instr_t;

static opcode_t instr_get_opcode(const instr_t instr)
{
    return (instr & 0xfc000000) >> 26;
}

static unsigned int instr_get_value26(const instr_t instr)
{
    return (instr & 0x03ffffff);
}

static unsigned int instr_get_value22(const instr_t instr)
{
    return (instr & 0x003fffff);
}

static unsigned int instr_get_value18(const instr_t instr)
{
    return (instr & 0x0003ffff);
}

static unsigned int instr_get_sgn_value26(const instr_t instr)
{
    unsigned int val = instr_get_value26(instr);

    if ((val & 0x02000000) != 0)
        val |= 0xfc000000;

    return (int)val;
}

static unsigned int instr_get_sgn_value22(const instr_t instr)
{
    unsigned int val = instr_get_value22(instr);

    if ((val & 0x00200000) != 0)
        val |= 0xffc00000;

    return (int)val;
}

static unsigned int instr_get_sgn_value18(const instr_t instr)
{
    unsigned int val = instr_get_value18(instr);

    if ((val & 0x00020000) != 0)
        val |= 0xfffc0000;

    return (int)val;
}

static reg_t instr_get_reg0(const instr_t instr)
{
    return (instr & 0x03c00000) >> 22;
}

static reg_t instr_get_reg1(const instr_t instr)
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

static instr_t instr_make_jump()
{
    return 0;//!!!
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
vm_bytecode;

static int vm_bytecode_init(vm_bytecode* byte_code)
{
    if (byte_code != NULL)
    {
        byte_code->num_instructions = 0;
        byte_code->instructions     = NULL;
        return 1;
    }
    return 0;
}

static int vm_bytecode_create(vm_bytecode* byte_code, int num_instructions)
{
    if (byte_code != NULL && byte_code->instructions == NULL && num_instructions > 0)
    {
        byte_code->num_instructions = num_instructions;
        byte_code->instructions     = (instr_t*)malloc(sizeof(instr_t)*num_instructions);
        return 1;
    }
    return 0;
}

static int vm_bytecode_free(vm_bytecode* byte_code)
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

static int vm_bytecode_read_from_file(vm_bytecode* byte_code, const char* filename)
{
    
    //...

    return 0;
}


/* ----- Stack ----- */

typedef int stack_word_t;

typedef struct
{
    size_t          stack_size;
    stack_word_t*   storage;
    stack_word_t*   ptr;
}
vm_stack;

static int vm_stack_init(vm_stack* stack)
{
    if (stack != NULL)
    {
        stack->stack_size   = 0;
        stack->storage      = NULL;
        stack->ptr          = NULL;
        return 1;
    }
    return 0;
}

static int vm_stack_create(vm_stack* stack, size_t stack_size)
{
    if (stack != NULL && stack->storage == NULL && stack_size != 0)
    {
        stack->stack_size   = stack_size;
        stack->storage      = (stack_word_t*)malloc(sizeof(stack_word_t)*stack_size);
        stack->ptr          = stack->storage;
        return 1;
    }
    return 0;
}

static int vm_stack_free(vm_stack* stack)
{
    if (stack != NULL)
    {
        if (stack->storage != NULL)
            free(stack->storage);

        stack->stack_size   = 0;
        stack->storage      = NULL;
        stack->ptr          = NULL;

        return 1;
    }
    return 0;
}

static void vm_stack_push(vm_stack* stack, stack_word_t value)
{
    *stack->ptr = value;
    ++stack->ptr;
}

static stack_word_t vm_stack_pop(vm_stack* stack)
{
    --stack->ptr;
    return *stack->ptr;
}


/* ----- Virtual machine ----- */

typedef enum
{
    VMEXITCODE_SUCCESS          = 0,
    VMEXITCODE_INVALID_BYTECODE = -1,
    VMEXITCODE_INVALID_STACK    = -2,
    VMEXITCODE_INVALID_OPCODE   = -3,
}
vm_exit_codes;

typedef struct
{
    int pc_reset;
    //int max_num_cycles;
}
vm_execution_state;

/**
Executes the specified XBC (XieXie Byte Code) program within the XVM (XieXie Virtual Machine).
\param[in] byte_code Pointer to the byte code to execute.
\param[in] stack Pointer to the stack which is to be used during execution.
\param[in,out] exe_state Optional pointer to the execution state. This may also be NULL.
\see bytecode_create
\see vm_stack_create
*/
static vm_exit_codes vm_execute_program(
    const vm_bytecode* const byte_code,
    vm_stack* const stack,
    vm_execution_state* const exe_state)
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

    instr_t instr;

    const instr_t* const instr_ptr = byte_code->instructions;
    const int num_instr = byte_code->num_instructions;

    opcode_t opcode;
    reg_t reg0, reg1;
    regi_t* const reg_pc = (reg.i + REG_PC);

    /* --- Initialize VM (only reset reserved registers) --- */
    reg.i[REG_LB] = 0;
    reg.i[REG_SP] = 0;

    if (exe_state != NULL)
        reg.i[REG_PC] = exe_state->pc_reset;
    else
        reg.i[REG_PC] = 0;

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

            case OPCODE_AND1:
            case OPCODE_OR1:
            case OPCODE_XOR1:
            case OPCODE_ADD1:
            case OPCODE_SUB1:
            case OPCODE_MUL1:
            case OPCODE_DIV1:
            case OPCODE_MOD1:
            case OPCODE_SLL1:
            case OPCODE_SLR1:
            break;

            case OPCODE_PUSH:
            {
                reg0 = instr_get_reg0(instr);
                vm_stack_push(stack, reg.i[reg0]);
            }
            break;

            case OPCODE_POP:
            {
                reg0 = instr_get_reg0(instr);
                reg.i[reg0] = vm_stack_pop(stack);
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
            case OPCODE_JMP:
            case OPCODE_JE:
            case OPCODE_JNE:
            case OPCODE_JG:
            case OPCODE_JL:
            case OPCODE_JGE:
            case OPCODE_JLE:
            case OPCODE_CALL:
            break;

            /* --- opcode_mem --- */
            case OPCODE_LDB:
            case OPCODE_STB:
            case OPCODE_LDW:
            case OPCODE_STW:
            break;

            /* --- opcode_memoff --- */
            case OPCODE_LDBO:
            case OPCODE_STBO:
            case OPCODE_LDWO:
            case OPCODE_STWO:
            break;

            /* --- opcode_special --- */
            case OPCODE_STOP:
            {
                return VMEXITCODE_SUCCESS;
            }

            case OPCODE_RET:
            case OPCODE_PUSHC:
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

    vm_stack stack;
    vm_stack_init(&stack);
    vm_stack_create(&stack, 256);

    vm_bytecode byte_code;
    vm_bytecode_init(&byte_code);
    vm_bytecode_create(&byte_code, 7);

    byte_code.instructions[0] = instr_make_reg1(OPCODE_MOV1, REG_I0, 5);
    byte_code.instructions[1] = instr_make_reg1(OPCODE_MOV1, REG_I1, 7);
    byte_code.instructions[2] = instr_make_reg2(OPCODE_MOV2, REG_I2, REG_I1, 0);
    byte_code.instructions[3] = instr_make_reg2(OPCODE_ADD2, REG_I2, REG_I0, 0);
    byte_code.instructions[4] = instr_make_reg1(OPCODE_PUSH, REG_I0, 0);
    byte_code.instructions[5] = instr_make_reg1(OPCODE_PUSH, REG_I1, 0);
    byte_code.instructions[6] = instr_make_reg1(OPCODE_PUSH, REG_I2, 0);

    const vm_exit_codes exitCode = vm_execute_program(&byte_code, &stack, NULL);

    if (exitCode != VMEXITCODE_SUCCESS)
    {
        printf("\nProgram terminated with error code: %i\n\n", exitCode);
    }
    else
    {
        printf(
            "\ni0 = %i, i1 = %i, i2 = %i\n\n",
            stack.storage[0], stack.storage[1], stack.storage[2]
        );
    }

    vm_stack_free(&stack);
    vm_bytecode_free(&byte_code);

    system("pause");

    #endif

    return 0;
}
