/*
 * XVM wrapper header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_XVM_WRAPPER_H__
#define __XX_XVM_WRAPPER_H__


#include <string>
#include <vector>

#define _REMOVE_XVM_TESTSUITE_
#include "xvm.c"


namespace XieXie
{

/**
This is a C++ wrapper class (written against the C++98 standard) for the
xvm (XieXie VirtualMachine) which is written in plain C (c99).
\code
// Read byte code from file
VirtualMachine::ByteCode byteCode;
byteCode.ReadFromFile("xvm_byte_code.xbc");

// Create stack (by default 256 entries).
VirtualMachine::Stack stack;

// Run program
VirtualMachine::ExecuteProgram(byteCode, stack);
\endcode
*/
namespace VirtualMachine
{


//! Exit codes of an executed program.
enum class ExitCodes
{
    __Unknown__,
    Success,
    InvalidByteCode,
    InvalidStack,
    InvalidOpCode,
    InvalidIntrinsic,
    StackOverflow,
    StackUnderflow,
    DivisionByZero,
    UnknownEntryPoint,
    MemoryViolation,
};


class ByteCode;
class Stack;

//! Forward declaration.
ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack);
//! Forward declaration.
ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack, const std::string& entryPoint);


//! Virtual register class.
class Register
{
    
    public:
        
        static const Register r0; //!< $r0  ->  General purpose register 0.
        static const Register r1; //!< $r1  ->  General purpose register 1.
        static const Register r2; //!< $r2  ->  General purpose register 2.
        static const Register r3; //!< $r3  ->  General purpose register 3.
        static const Register r4; //!< $r4  ->  General purpose register 4.
        static const Register r5; //!< $r5  ->  General purpose register 5.
        static const Register r6; //!< $r6  ->  General purpose register 6.
        static const Register r7; //!< $r7  ->  General purpose register 7.
        static const Register r8; //!< $r8  ->  General purpose register 8.
        static const Register r9; //!< $r9  ->  General purpose register 9.

        static const Register op; //!< $op  ->  Object pointer: should be used as the "this" pointer.
        static const Register gp; //!< $gp  ->  Global pointer: should be used as the stack pointer where all global variables are stored.

        static const Register cf; //!< $cf  ->  Conditional flag register: used for jump conditions.
        static const Register lb; //!< $lb  ->  Local base pointer: pointer to the current stack frame.
        static const Register sp; //!< $sp  ->  Stack pointer: pointer to the top of the stack.
        static const Register pc; //!< $pc  ->  Program counter: pointer to the current instruction.

        operator reg_t () const
        {
            return reg_;
        }

        //! Returns the register name.
        std::string Name() const
        {
            return std::string(xvm_register_get_name(reg_));
        }

        /**
        Returns the specified register.
        \param[in] reg Specifies the register index. Must be in the range [0x0 .. 0xf].
        \throws std::out_of_range If the register index is out of range.
        */
        static const Register& Get(size_t reg)
        {
            switch (reg)
            {
                case 0x0: return r0;
                case 0x1: return r1;
                case 0x2: return r2;
                case 0x3: return r3;
                case 0x4: return r4;
                case 0x5: return r5;
                case 0x6: return r6;
                case 0x7: return r7;
                case 0x8: return r8;
                case 0x9: return r9;
                case 0xa: return op;
                case 0xb: return gp;
                case 0xc: return cf;
                case 0xd: return lb;
                case 0xe: return sp;
                case 0xf: return pc;
            }
            throw std::out_of_range("invalid register index (must be in rage: 0x0 - 0xf)");
            return r0;
        }

    private:
        
        Register(register_id reg) :
            reg_(static_cast<reg_t>(reg))
        {
        }
        Register(const Register&)
        {
            /* Not used */
        }
        Register& operator = (const Register&)
        {
            /* Not used */
            return *this;
        }

        reg_t reg_;

};

const Register Register::r0(REG_R0);
const Register Register::r1(REG_R1);
const Register Register::r2(REG_R2);
const Register Register::r3(REG_R3);
const Register Register::r4(REG_R4);
const Register Register::r5(REG_R5);
const Register Register::r6(REG_R6);
const Register Register::r7(REG_R7);
const Register Register::r8(REG_R8);
const Register Register::r9(REG_R9);

const Register Register::op(REG_OP);
const Register Register::gp(REG_GP);

const Register Register::cf(REG_CF);
const Register Register::lb(REG_LB);
const Register Register::sp(REG_SP);
const Register Register::pc(REG_PC);

typedef Register Reg;


//! The instruction class only stores the 32-bit code of a single XVM instruction.
class Instruction
{
    
    public:
        
        Instruction() :
            code_(0)
        {
        }
        Instruction(int code) :
            code_(code)
        {
        }

        //! Returns the entire 32-bit instruction code.
        instr_t Code() const
        {
            return code_;
        }

        //! Returns the instruction opcode.
        opcode_t OpCode() const
        {
            return xvm_instr_get_opcode(code_);
        }

        /**
        Returns the instruction mnemonic. This string will be filled with
        spaces (' ') so that the string always consists of 4 characters.
        */
        std::string Mnemonic() const
        {
            return std::string(xvm_instr_get_mnemonic(OpCode()));
        }

        //! Makes a 2-register instruction.
        static Instruction MakeReg2(opcode_reg2 opcode, const Register& reg0, const Register& reg1)
        {
            return Instruction(xvm_instr_make_reg2(opcode, reg0, reg1));
        }

        //! Makes a 1-register instruction.
        static Instruction MakeReg1(opcode_reg1 opcode, const Register& reg, unsigned int value)
        {
            return Instruction(xvm_instr_make_reg1(opcode, reg, value));
        }

    private:
        
        instr_t code_;

};

typedef Instruction Instr;


//! The byte code class represents an entire virtual program.
class ByteCode
{
    
    public:

        ByteCode()
        {
            xvm_bytecode_init(&byteCode_);
        }
        ~ByteCode()
        {
            xvm_bytecode_free(&byteCode_);
        }

        //! Reads the instructions from the specified file.
        bool ReadFromFile(const std::string& filename)
        {
            return xvm_bytecode_read_from_file(&byteCode_, filename.c_str()) != 0;
        }

        //! Writes the instructions to the specified file.
        bool WriteToFile(const std::string& filename)
        {
            return xvm_bytecode_write_to_file(&byteCode_, filename.c_str()) != 0;
        }

        /**
        Finalizes the instruction building. After this call no further
        instructions can be added to this byte code object.
        \see instructions
        */
        bool Finalize()
        {
            size_t numInstr = instructions.size();
            if (xvm_bytecode_create_instructions(&byteCode_, static_cast<int>(numInstr)) != 0)
            {
                for (size_t i = 0; i < numInstr; ++i)
                    byteCode_.instructions[i] = instructions[i].Code();
                return true;
            }
            return false;
        }

        //! Array list of all instructions.
        std::vector<Instruction> instructions;

    private:
        
        ByteCode(const ByteCode&)
        {
            /* Not used */
        }
        ByteCode& operator = (const ByteCode&)
        {
            /* Not used */
            return *this;
        }

        friend ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack);
        friend ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack, const std::string& entryPoint);

        xvm_bytecode byteCode_;

};


//! A virtual stack is required for to execute a program.
class Stack
{

    public:

        static const size_t defaultSize = 256;

        Stack(size_t size = defaultSize)
        {
            xvm_stack_init(&stack_);
            xvm_stack_create(&stack_, size);
        }
        ~Stack()
        {
            xvm_stack_free(&stack_);
        }

        //! Returns the number of WORD (32-bit) entries in the stack.
        size_t Size() const
        {
            return stack_.stack_size;
        }

    private:
        
        Stack(const Stack&)
        {
            /* Not used */
        }
        Stack& operator = (const Stack&)
        {
            /* Not used */
            return *this;
        }

        friend ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack);
        friend ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack, const std::string& entryPoint);

        xvm_stack stack_;

};


/**
Maps the XVM exit codes from 'xvm_exit_codes' to 'XieXie::VirtualMachien::ExitCodes'.
\see ExitCodes
\see xvm_exit_codes
*/
ExitCodes MapExitCode(xvm_exit_codes code)
{
    switch (code)
    {
        case EXITCODE_SUCCESS:
            return ExitCodes::Success;
        case EXITCODE_INVALID_BYTECODE:
            return ExitCodes::InvalidByteCode;
        case EXITCODE_INVALID_STACK:
            return ExitCodes::InvalidStack;
        case EXITCODE_INVALID_OPCODE:
            return ExitCodes::InvalidOpCode;
        case EXITCODE_INVALID_INTRINSIC:
            return ExitCodes::InvalidIntrinsic;
        case EXITCODE_STACK_OVERFLOW:
            return ExitCodes::StackOverflow;
        case EXITCODE_STACK_UNDERFLOW:
            return ExitCodes::StackUnderflow;
        case EXITCODE_DIVISION_BY_ZERO:
            return ExitCodes::DivisionByZero;
        case EXITCODE_UNKNOWN_ENTRY_POINT:
            return ExitCodes::UnknownEntryPoint;
        /*case EXITCODE_MEMORY_VIOLATION:
            return ExitCodes::MemoryViolation;*/
    }
    return ExitCodes::__Unknown__;
}

/**
Executes the specified program using the specified stack.
\see ExitCodes
*/
ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack)
{
    const xvm_exit_codes code = xvm_execute_program(
        &(byteCode.byteCode_),
        &(stack.stack_)
    );
    return VirtualMachine::MapExitCode(code);
}

/**
Executes the specified program from the specified entry point using the specified stack.
\see ExitCodes
*/
ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack, const std::string& entryPoint)
{
    const xvm_exit_codes code = xvm_execute_program_entry_point(
        &(byteCode.byteCode_),
        &(stack.stack_),
        entryPoint.c_str()
    );
    return VirtualMachine::MapExitCode(code);
}


} // /namespace VirtualMachine

} // /namespace XieXie


#endif



// ================================================================================