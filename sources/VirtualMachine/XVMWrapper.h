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
        Returns the specified register via index.
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

        /**
        Returns the specified register via name.
        \param[in] name Specifies the register name (e.g. "$r0" or "$sp").
        Valid register names are: "$r0" - "$r9", "$op", "$gp", "$cf", "$lb", "$sp" and "$pc".
        \throws std::invalid_argument If the register name is invalid.
        \see Name
        */
        static const Register& Get(const std::string& name)
        {
            if (name.size() == 3 && name[0] == '$')
            {
                if (name[1] == 'r')
                {
                    switch (name[2])
                    {
                        case '0': return r0;
                        case '1': return r1;
                        case '2': return r2;
                        case '3': return r3;
                        case '4': return r4;
                        case '5': return r5;
                        case '6': return r6;
                        case '7': return r7;
                        case '8': return r8;
                        case '9': return r9;
                    }
                }
                else if (name == "$op") return op;
                else if (name == "$gp") return gp;
                else if (name == "$cf") return cf;
                else if (name == "$lb") return lb;
                else if (name == "$sp") return sp;
                else if (name == "$pc") return pc;
            }
            throw std::invalid_argument("invalid register name '" + name + "'");
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
    
    private:
        
        template <class ValueClass> static bool InBitRange(int value)
        {
            return value >= ValueClass::min && value <= ValueClass::max;
        }
        template <class ValueClass> static bool InBitRange(unsigned int value)
        {
            return value >= ValueClass::min && value <= ValueClass::max;
        }

    public:
        
        /* ------- Structures ------- */

        //! Unsigned 26-bit value.
        struct Value26
        {
            static const unsigned int max = XVM_VALUE26_MAX;
            static const unsigned int min = XVM_VALUE26_MIN;
        };

        //! Unsigned 22-bit value.
        struct Value22
        {
            static const unsigned int max = XVM_VALUE22_MAX;
            static const unsigned int min = XVM_VALUE22_MIN;
        };

        //! Unsigned 18-bit value.
        struct Value18
        {
            static const unsigned int max = XVM_VALUE18_MAX;
            static const unsigned int min = XVM_VALUE18_MIN;
        };

        //! Signed 26-bit value.
        struct SgnValue26
        {
            static const int max = XVM_SGN_VALUE26_MAX;
            static const int min = XVM_SGN_VALUE26_MIN;
        };

        //! Signed 22-bit value.
        struct SgnValue22
        {
            static const int max = XVM_SGN_VALUE22_MAX;
            static const int min = XVM_SGN_VALUE22_MIN;
        };

        //! Signed 18-bit value.
        struct SgnValue18
        {
            static const int max = XVM_SGN_VALUE18_MAX;
            static const int min = XVM_SGN_VALUE18_MIN;
        };

        /* ------- Templates ------- */

        //! Returns true if the specified value is inside the inside the bit range.
        template <int Bits> static bool InRange(int value)
        {
            return false;
        }

        template <> static bool InRange<26>(int value)
        {
            return InBitRange<SgnValue26>(value);
        }
        template <> static bool InRange<22>(int value)
        {
            return InBitRange<SgnValue22>(value);
        }
        template <> static bool InRange<18>(int value)
        {
            return InBitRange<SgnValue18>(value);
        }

        //! Returns true if the specified value is inside the inside the bit range.
        template <int Bits> static bool InRange(unsigned int value)
        {
            return false;
        }

        template <> static bool InRange<26>(unsigned int value)
        {
            return InBitRange<Value26>(value);
        }
        template <> static bool InRange<22>(unsigned int value)
        {
            return InBitRange<Value22>(value);
        }
        template <> static bool InRange<18>(unsigned int value)
        {
            return InBitRange<Value18>(value);
        }

        /* ------- Functions ------- */

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

        /* ------- Static functions ------- */

        /**
        Makes a 2-register instruction (mov, not, and, or, xor, add, sub, mul, div, mod, sll, slr, cmp, fti, itf).
        \throws std::invalid_argument If 'opcode' is invalid.
        */
        static Instruction MakeReg2(opcode_reg2 opcode, const Register& reg0, const Register& reg1)
        {
            OpCodeAssert(opcode, OPCODE_MOV2, OPCODE_ITF, "invalid opcode for 2-register instruction");
            return Instruction(xvm_instr_make_reg2(opcode, reg0, reg1));
        }

        /**
        Makes a 1-register instruction (mov, and, or, xor, add, sub, mul, div, mod, sll, slr, push, pop, inc, dec).
        \param[in] value 22-bit signed value.
        \throws std::invalid_argument If 'opcode' is invalid.
        \throws std::out_of_range If 'value' is out of SgnValue22 range.
        */
        static Instruction MakeReg1(opcode_reg1 opcode, const Register& reg, int value)
        {
            OpCodeAssert(opcode, OPCODE_MOV1, OPCODE_DEC, "invalid opcode for 1-register instruction");
            RangeAssert(InRange<22>(value), "'value' is out of range in 1-register instruction");
            return Instruction(xvm_instr_make_reg1(opcode, reg, static_cast<unsigned int>(value)));
        }

        /**
        Makes a jump instruction (jmp, je, jne, jg, jl, jge, jle, call).
        \param[in] offset 22-bit signed jump offset. A jump offset is WORD aligned,
        i.e. to jump to the next instruction, use 1 and to jump to the previous instruction use -1.
        \throws std::invalid_argument If 'opcode' is invalid.
        \throws std::out_of_range If 'offset' is out of SgnValue22 range.
        */
        static Instruction MakeJump(opcode_jump opcode, const Register& reg, int offset)
        {
            OpCodeAssert(opcode, OPCODE_JMP, OPCODE_CALL, "invalid opcode for jump instruction");
            RangeAssert(InRange<22>(offset), "'offset' is out of range in jump instruction");
            return Instruction(xvm_instr_make_jump(opcode, reg, static_cast<unsigned int>(offset)));
        }

        /**
        Makes a float instruction (addf, subf, mulf, divf, cmpf).
        \throws std::invalid_argument If 'opcode' is invalid.
        */
        static Instruction MakeFloat(opcode_float opcode, const Register& reg0, const Register& reg1)
        {
            OpCodeAssert(opcode, OPCODE_ADDF, OPCODE_CMPF, "invalid opcode for float instruction");
            return Instruction(xvm_instr_make_float(opcode, reg0, reg1));
        }

        /**
        Makes a memory instruction (lda).
        \param[in] address 22-bit unsigned address. This address is (like jump offsets) WORD aligned,
        i.e. to address the 5th instruction use 5.
        \throws std::invalid_argument If 'opcode' is invalid.
        \throws std::out_of_range If 'address' is out of Value22 range.
        */
        static Instruction MakeMem(opcode_mem opcode, const Register& reg, unsigned int address)
        {
            OpCodeAssert(opcode == OPCODE_LDA, "invalid opcode for memory instruction");
            RangeAssert(InRange<22>(address), "'address' is out of range in memory instruction");
            return Instruction(xvm_instr_make_mem(opcode, reg, address));
        }

        /**
        Makes a memory-offset instruction (ldb, stb, ldw, stw).
        \param[in] offset 18-bit signed offset. This offset is BYTE aligned.
        \throws std::invalid_argument If 'opcode' is invalid.
        \throws std::out_of_range If 'offset' is out of SgnValue18 range.
        */
        static Instruction MakeMemOff(opcode_memoff opcode, const Register& reg0, const Register& reg1, int offset)
        {
            OpCodeAssert(opcode, OPCODE_LDB, OPCODE_STW, "invalid opcode for memory-offset instruction");
            RangeAssert(InRange<18>(offset), "'offset' is out of range in memory-offset instruction");
            return Instruction(xvm_instr_make_memoff(opcode, reg0, reg1, static_cast<unsigned int>(offset)));
        }

        /**
        Makes a special instruction (stop, push).
        \param[in] value 26-bit signed value.
        \throws std::invalid_argument If 'opcode' is invalid.
        \throws std::out_of_range If 'value' is out of SgnValue26 range.
        */
        static Instruction MakeSpecial(opcode_special opcode, int value)
        {
            OpCodeAssert(opcode == OPCODE_STOP || opcode == OPCODE_PUSHC, "invalid opcode for special instruction");
            RangeAssert(InRange<26>(value), "'value' is out of range in special instruction");
            return Instruction(xvm_instr_make_special1(opcode, static_cast<unsigned int>(value)));
        }

        /**
        Makes a special instruction (invk).
        \param[in] value 26-bit unsigned value.
        \throws std::invalid_argument If 'opcode' is invalid.
        \throws std::out_of_range If 'value' is out of Value26 range.
        */
        static Instruction MakeSpecial(opcode_special opcode, unsigned int value)
        {
            OpCodeAssert(opcode == OPCODE_INVK, "invalid opcode for special instruction");
            RangeAssert(InRange<26>(value), "'value' is out of range in special instruction");
            return Instruction(xvm_instr_make_special1(opcode, value));
        }

        /**
        Makes a specular instruction (ret).
        \param[in] argSize 18-bit unsigned value.
        Specifies the number of procedure argument WORDs which will be poped from the stack.
        \throws std::invalid_argument If 'opcode' is invalid.
        \throws std::out_of_range If 'argSize' is out of Value18 range.
        */
        static Instruction MakeSpecial(opcode_special opcode, unsigned int resultSize, unsigned int argSize)
        {
            OpCodeAssert(opcode == OPCODE_RET, "invalid opcode for special instruction");
            RangeAssert(InRange<18>(argSize), "'argSize' is out of range in special instruction");
            return Instruction(xvm_instr_make_special2(opcode, resultSize, argSize));
        }

    private:
        
        static void RangeAssert(bool inRange, const char* err)
        {
            if (!inRange)
                throw std::out_of_range(err);
        }

        static void OpCodeAssert(bool inRange, const char* err)
        {
            if (!inRange)
                throw std::invalid_argument(err);
        }

        template <typename OpCode> static void OpCodeAssert(
            const OpCode& opcode, const OpCode& minOpcode, const OpCode& maxOpcode, const char* err)
        {
            OpCodeAssert(opcode >= minOpcode && opcode <= maxOpcode, err);
        }

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