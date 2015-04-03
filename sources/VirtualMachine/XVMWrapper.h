/*
 * XVMWrapper.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_XVM_WRAPPER_H__
#define __XX_XVM_WRAPPER_H__


#include <string>
#include <vector>
#include <map>

#ifndef _DISABLE_SHELL_
#   define _DISABLE_SHELL_
#endif
#include "xvm.c"


namespace XieXie
{

/**
This is a C++ wrapper (written against the C++11 standard) for the
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
    InvocationViolation,
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
        
        static const Register r0;   //!< $r0   ->  General purpose register 0.
        static const Register r1;   //!< $r1   ->  General purpose register 1.
        static const Register r2;   //!< $r2   ->  General purpose register 2.
        static const Register r3;   //!< $r3   ->  General purpose register 3.
        static const Register r4;   //!< $r4   ->  General purpose register 4.
        static const Register r5;   //!< $r5   ->  General purpose register 5.
        static const Register r6;   //!< $r6   ->  General purpose register 6.
        static const Register r7;   //!< $r7   ->  General purpose register 7.
        static const Register r8;   //!< $r8   ->  General purpose register 8.
        static const Register r9;   //!< $r9   ->  General purpose register 9.
        static const Register r10;  //!< $r10  ->  General purpose register 10.
        static const Register r11;  //!< $r11  ->  General purpose register 11.
        static const Register r12;  //!< $r12  ->  General purpose register 12.
        static const Register r13;  //!< $r13  ->  General purpose register 13.
        static const Register r14;  //!< $r14  ->  General purpose register 14.
        static const Register r15;  //!< $r15  ->  General purpose register 15.
        static const Register r16;  //!< $r16  ->  General purpose register 16.
        static const Register r17;  //!< $r17  ->  General purpose register 17.
        static const Register r18;  //!< $r18  ->  General purpose register 18.
        static const Register r19;  //!< $r19  ->  General purpose register 19.
        static const Register r20;  //!< $r20  ->  General purpose register 20.
        static const Register r21;  //!< $r21  ->  General purpose register 21.
        static const Register r22;  //!< $r22  ->  General purpose register 22.
        static const Register r23;  //!< $r23  ->  General purpose register 23.
        static const Register r24;  //!< $r24  ->  General purpose register 24.

        static const Register ar;   //!< $ar   ->  Return value: can be used as return value.
        static const Register xr;   //!< $xr   ->  Extended register: can be used as assembler temporary or 'this' pointer of a class instance.
        static const Register gp;   //!< $gp   ->  Global pointer: should be used as the stack pointer where all global variables are stored.

        static const Register cf;   //!< $cf   ->  Conditional flag register: used for jump conditions.
        static const Register lb;   //!< $lb   ->  Local base pointer: pointer to the current stack frame.
        static const Register sp;   //!< $sp   ->  Stack pointer: pointer to the top of the stack.
        static const Register pc;   //!< $pc   ->  Program counter: pointer to the current instruction.

        Register(const Register&) = delete;
        Register& operator = (const Register&) = delete;

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
                case 0x00: return r0;
                case 0x01: return r1;
                case 0x02: return r2;
                case 0x03: return r3;
                case 0x04: return r4;
                case 0x05: return r5;
                case 0x06: return r6;
                case 0x07: return r7;
                case 0x08: return r8;
                case 0x09: return r9;
                case 0x0a: return r10;
                case 0x0b: return r11;
                case 0x0c: return r12;
                case 0x0d: return r13;
                case 0x0e: return r14;
                case 0x0f: return r15;
                case 0x10: return r16;
                case 0x11: return r17;
                case 0x12: return r18;
                case 0x13: return r19;
                case 0x14: return r20;
                case 0x15: return r21;
                case 0x16: return r22;
                case 0x17: return r23;
                case 0x18: return r24;
                case 0x19: return ar;
                case 0x1a: return xr;
                case 0x1b: return gp;
                case 0x1c: return cf;
                case 0x1d: return lb;
                case 0x1e: return sp;
                case 0x1f: return pc;
            }
            throw std::out_of_range("invalid register index (must be in rage: 0x0 - 0xf)");
            return r0;
        }

        /**
        Returns the specified register via name.
        \param[in] name Specifies the register name (e.g. "$r0" or "$sp").
        Valid register names are: "$r0" - "$r25", "$sr", "$gp", "$cf", "$lb", "$sp", and "$pc".
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
                else if (name == "$ar") return ar;
                else if (name == "$xr") return xr;
                else if (name == "$gp") return gp;
                else if (name == "$cf") return cf;
                else if (name == "$lb") return lb;
                else if (name == "$sp") return sp;
                else if (name == "$pc") return pc;
            }
            else if (name.size() == 4 && name[0] == '$' && name[1] == 'r')
            {
                if (name[2] == '1')
                {
                    switch (name[3])
                    {
                        case '0': return r10;
                        case '1': return r11;
                        case '2': return r12;
                        case '3': return r13;
                        case '4': return r14;
                        case '5': return r15;
                        case '6': return r16;
                        case '7': return r17;
                        case '8': return r18;
                        case '9': return r19;
                    }
                }
                else if (name[2] == '2')
                {
                    switch (name[3])
                    {
                        case '0': return r20;
                        case '1': return r21;
                        case '2': return r22;
                        case '3': return r23;
                        case '4': return r24;
                    }
                }
            }
            throw std::invalid_argument("invalid register name '" + name + "'");
            return r0;
        }

    private:
        
        Register(register_id reg) :
            reg_{ static_cast<reg_t>(reg) }
        {
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
const Register Register::r10(REG_R10);
const Register Register::r11(REG_R11);
const Register Register::r12(REG_R12);
const Register Register::r13(REG_R13);
const Register Register::r14(REG_R14);
const Register Register::r15(REG_R15);
const Register Register::r16(REG_R16);
const Register Register::r17(REG_R17);
const Register Register::r18(REG_R18);
const Register Register::r19(REG_R19);
const Register Register::r20(REG_R20);
const Register Register::r21(REG_R21);
const Register Register::r22(REG_R22);
const Register Register::r23(REG_R23);
const Register Register::r24(REG_R24);

const Register Register::ar(REG_AR);
const Register Register::xr(REG_XR);
const Register Register::gp(REG_GP);

const Register Register::cf(REG_CF);
const Register Register::lb(REG_LB);
const Register Register::sp(REG_SP);
const Register Register::pc(REG_PC);

using Reg = Register;


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

        //! Unsigned 21-bit value.
        struct Value21
        {
            static const unsigned int max = XVM_VALUE21_MAX;
            static const unsigned int min = XVM_VALUE21_MIN;
        };

        //! Unsigned 16-bit value.
        struct Value16
        {
            static const unsigned int max = XVM_VALUE16_MAX;
            static const unsigned int min = XVM_VALUE16_MIN;
        };

        //! Signed 26-bit value.
        struct SgnValue26
        {
            static const int max = XVM_SGN_VALUE26_MAX;
            static const int min = XVM_SGN_VALUE26_MIN;
        };

        //! Signed 21-bit value.
        struct SgnValue21
        {
            static const int max = XVM_SGN_VALUE21_MAX;
            static const int min = XVM_SGN_VALUE21_MIN;
        };

        //! Signed 16-bit value.
        struct SgnValue16
        {
            static const int max = XVM_SGN_VALUE16_MAX;
            static const int min = XVM_SGN_VALUE16_MIN;
        };

        /* ------- Templates ------- */

        //! Returns true if the specified value is inside the inside the bit range.
        template <int Bits> static bool InRange(int value)
        {
            return false;
        }

        //! Returns true if the specified value is inside the inside the bit range.
        template <int Bits> static bool InRange(unsigned int value)
        {
            return false;
        }

        /* ------- Functions ------- */

        Instruction() :
            code_{ 0 }
        {
        }
        Instruction(int code) :
            code_{ code }
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
            return _xvm_instr_get_opcode(code_);
        }

        /**
        Returns the instruction mnemonic. This string will be filled with
        spaces (' ') so that the string always consists of 4 characters.
        */
        std::string Mnemonic() const
        {
            return std::string(xvm_instr_get_mnemonic(OpCode()));
        }

        /**
        Back-patches the constant value of this instruction.
        This can be used to back-patch labels which must be resolved later.
        This function can be used for 1-register-, jump-, memory-, memory-offset- and special instructions.
        */
        void BackPatch(int value)
        {
            code_ = xvm_instr_patch_value(Code(), static_cast<unsigned int>(value));
        }

        /* ------- Static functions ------- */

        /**
        Makes a 3-register instruction (mov, not, and, or, xor, add, sub, mul, div, mod, sll, slr, cmp, fti, itf).
        \throws std::invalid_argument If 'opcode' is invalid.
        */
        static Instruction MakeReg3(xvm_opcode opcode, const Register& reg0, const Register& reg1, const Register& reg2);

        /**
        Makes a 2-register instruction (mov, not, and, or, xor, add, sub, mul, div, mod, sll, slr, cmp, fti, itf).
        \throws std::invalid_argument If 'opcode' is invalid.
        */
        static Instruction MakeReg2(xvm_opcode opcode, const Register& reg0, const Register& reg1, int value);

        /**
        Makes a 1-register instruction (mov, and, or, xor, add, sub, mul, div, mod, sll, slr, push, pop, inc, dec).
        \param[in] value 22-bit signed value.
        \throws std::invalid_argument If 'opcode' is invalid.
        \throws std::out_of_range If 'value' is out of SgnValue22 range.
        */
        static Instruction MakeReg1(xvm_opcode opcode, const Register& reg, int value);

        /**
        Makes a jump instruction (jmp, je, jne, jg, jl, jge, jle, call).
        \param[in] offset 22-bit signed jump offset. A jump offset is WORD aligned,
        i.e. to jump to the next instruction, use 1 and to jump to the previous instruction use -1.
        \throws std::invalid_argument If 'opcode' is invalid.
        \throws std::out_of_range If 'offset' is out of SgnValue22 range.
        */
        static Instruction MakeJump(xvm_opcode opcode, const Register& reg, int offset);

        /**
        Makes a memory-offset instruction (ldb, stb, ldw, stw).
        \param[in] offset 18-bit signed offset. This offset is BYTE aligned.
        \throws std::invalid_argument If 'opcode' is invalid.
        \throws std::out_of_range If 'offset' is out of SgnValue18 range.
        */
        static Instruction MakeLoadStore(xvm_opcode opcode, const Register& reg0, const Register& reg1, int offset);

        /**
        Makes a special instruction (stop, push).
        \param[in] value 26-bit signed value.
        \throws std::invalid_argument If 'opcode' is invalid.
        \throws std::out_of_range If 'value' is out of SgnValue26 range.
        */
        static Instruction MakeSpecial(xvm_opcode opcode, int value);

        /**
        Makes a special instruction (invk).
        \param[in] value 26-bit unsigned value.
        \throws std::invalid_argument If 'opcode' is invalid.
        \throws std::out_of_range If 'value' is out of Value26 range.
        */
        static Instruction MakeSpecial(xvm_opcode opcode, unsigned int value);

        /**
        Makes a specular instruction (ret).
        \param[in] argSize 18-bit unsigned value.
        Specifies the number of procedure argument WORDs which will be poped from the stack.
        \throws std::invalid_argument If 'opcode' is invalid.
        \throws std::out_of_range If 'argSize' is out of Value18 range.
        */
        static Instruction MakeSpecial(xvm_opcode opcode, unsigned int resultSize, unsigned int argSize);

    private:
        
        static void RangeAssert(bool inRange, const std::string& err)
        {
            if (!inRange)
                throw std::out_of_range(err);
        }

        static void OpCodeAssert(bool inRange, const std::string& err)
        {
            if (!inRange)
                throw std::invalid_argument(err);
        }

        template <class ValueClass> static std::string RangeStr()
        {
            return "[" + std::to_string(ValueClass::min) + " .. " + std::to_string(ValueClass::max) + "]";
        }

        template <typename OpCode> static void OpCodeAssert(
            const OpCode& opcode, const OpCode& minOpcode, const OpCode& maxOpcode, const char* err)
        {
            OpCodeAssert(opcode >= minOpcode && opcode <= maxOpcode, err);
        }

        instr_t code_;

};

// >> Template specialization outside class declaration required for GCC

template <> bool Instruction::InRange<26>(int value)
{
    return InBitRange<SgnValue26>(value);
}
template <> bool Instruction::InRange<21>(int value)
{
    return InBitRange<SgnValue21>(value);
}
template <> bool Instruction::InRange<16>(int value)
{
    return InBitRange<SgnValue16>(value);
}

template <> bool Instruction::InRange<26>(unsigned int value)
{
    return InBitRange<Value26>(value);
}
template <> bool Instruction::InRange<21>(unsigned int value)
{
    return InBitRange<Value21>(value);
}
template <> bool Instruction::InRange<16>(unsigned int value)
{
    return InBitRange<Value16>(value);
}

/* ------- Static functions ------- */

Instruction Instruction::MakeReg3(xvm_opcode opcode, const Register& reg0, const Register& reg1, const Register& reg2)
{
    OpCodeAssert(opcode, OPCODE_AND3, OPCODE_MODF, "invalid opcode for 3-register instruction");
    return Instruction(xvm_instr_make_reg3(opcode, reg0, reg1, reg2));
}

Instruction Instruction::MakeReg2(xvm_opcode opcode, const Register& reg0, const Register& reg1, int value)
{
    OpCodeAssert(opcode, OPCODE_MOV2, OPCODE_CMPF, "invalid opcode for 2-register instruction");
    RangeAssert(InRange<16>(value), "'value' is out of range " + RangeStr<SgnValue16>() + " in 2-register instruction");
    return Instruction(xvm_instr_make_reg2(opcode, reg0, reg1, static_cast<unsigned int>(value)));
}

Instruction Instruction::MakeReg1(xvm_opcode opcode, const Register& reg, int value)
{
    OpCodeAssert(opcode, OPCODE_PUSH, OPCODE_MOVI, "invalid opcode for 1-register instruction");
    RangeAssert(InRange<21>(value), "'value' is out of range " + RangeStr<SgnValue21>() + " in 1-register instruction");
    return Instruction(xvm_instr_make_reg1(opcode, reg, static_cast<unsigned int>(value)));
}

Instruction Instruction::MakeJump(xvm_opcode opcode, const Register& reg, int offset)
{
    OpCodeAssert(opcode, OPCODE_JMP, OPCODE_CALL, "invalid opcode for jump instruction");
    RangeAssert(InRange<21>(offset), "'offset' is out of range " + RangeStr<SgnValue21>() + " in jump instruction");
    return Instruction(xvm_instr_make_jump(opcode, reg, static_cast<unsigned int>(offset)));
}

Instruction Instruction::MakeLoadStore(xvm_opcode opcode, const Register& reg0, const Register& reg1, int offset)
{
    OpCodeAssert(opcode, OPCODE_LDB, OPCODE_STW, "invalid opcode for memory-offset instruction");
    RangeAssert(InRange<16>(offset), "'offset' is out of range " + RangeStr<SgnValue16>() + " in memory-offset instruction");
    return Instruction(xvm_instr_make_loadstore(opcode, reg0, reg1, static_cast<unsigned int>(offset)));
}

Instruction Instruction::MakeSpecial(xvm_opcode opcode, int value)
{
    OpCodeAssert(opcode == OPCODE_STOP || opcode == OPCODE_PUSHC, "invalid opcode for special instruction");
    RangeAssert(InRange<26>(value), "'value' is out of range " + RangeStr<SgnValue26>() + " in special instruction");
    return Instruction(xvm_instr_make_special1(opcode, static_cast<unsigned int>(value)));
}

Instruction Instruction::MakeSpecial(xvm_opcode opcode, unsigned int value)
{
    OpCodeAssert(opcode == OPCODE_INVK || opcode == OPCODE_INSC, "invalid opcode for special instruction");
    RangeAssert(InRange<26>(value), "'value' is out of range " + RangeStr<Value26>() + " in special instruction");
    return Instruction(xvm_instr_make_special1(opcode, value));
}

Instruction Instruction::MakeSpecial(xvm_opcode opcode, unsigned int resultSize, unsigned int argSize)
{
    OpCodeAssert(opcode == OPCODE_RET, "invalid opcode for special instruction");
    RangeAssert(InRange<16>(argSize), "'argSize' is out of range " + RangeStr<Value16>() + " in special instruction");
    return Instruction(xvm_instr_make_special2(opcode, resultSize, argSize));
}

using Instr = Instruction;


//! Intrinsics helper class.
class Intrinsics
{
    
    public:
        
        using AddressMapType = std::map<std::string, unsigned int>;

        Intrinsics()
        {
            addresses_ = std::move(AddressMapType
            {
                { "AllocMem",   INSC_ALLOC_MEM    },
                { "FreeMem",    INSC_FREE_MEM     },
                { "CopyMem",    INSC_COPY_MEM     },
                { "SysCall",    INSC_SYS_CALL     },
                { "Clear",      INSC_CLEAR        },
                { "Print",      INSC_PRINT        },
                { "PrintLn",    INSC_PRINT_LN     },
                { "PrintInt",   INSC_PRINT_INT    },
                { "PrintFloat", INSC_PRINT_FLOAT  },
                { "Input",      INSC_INPUT        },
                { "InputInt",   INSC_INPUT_INT    },
                { "InputFloat", INSC_INPUT_FLOAT  },
                { "CreateFile", INSC_CREATE_FILE  },
                { "DeleteFile", INSC_DELETE_FILE  },
                { "OpenFile",   INSC_OPEN_FILE    },
                { "CloseFile",  INSC_CLOSE_FILE   },
                { "FileSize",   INSC_FILE_SIZE    },
                { "SetFilePos", INSC_SET_FILE_POS },
                { "GetFilePos", INSC_GET_FILE_POS },
                { "FileEOF",    INSC_FILE_EOF     },
                { "WriteByte",  INSC_WRITE_BYTE   },
                { "WriteWord",  INSC_WRITE_WORD   },
                { "ReadByte",   INSC_READ_BYTE    },
                { "ReadWord",   INSC_READ_WORD    },
                { "Sin",        INSC_SIN          },
                { "Cos",        INSC_COS          },
                { "Tan",        INSC_TAN          },
                { "ASin",       INSC_ASIN         },
                { "ACos",       INSC_ACOS         },
                { "ATan",       INSC_ATAN         },
                { "Pow",        INSC_POW          },
                { "Sqrt",       INSC_SQRT         },
                { "RandInt",    INSC_RAND_INT     },
                { "RandFloat",  INSC_RAND_FLOAT   },
                { "Time",       INSC_TIME         },
                { "Sleep",      INSC_SLEEP        }
            });
        }

        //! Returns the intrinsic address by the specified intrinsic name or 0 if the name is invalid.
        unsigned int AddressByName(const std::string& name)
        {
            AddressMapType::iterator it = addresses_.find(name);
            if (it != addresses_.end())
                return it->second;
            return 0u;
        }

    private:
        
        AddressMapType addresses_;

};


//! Module wrapper class. This can be used for external invocations.
class Module
{
    
    public:
        
        Module(const Module&) = delete;
        Module& operator = (const Module&) = delete;

        Module()
        {
            xvm_module_init(&module_);
        }
        ~Module()
        {
            xvm_module_unload(&module_);
        }

        /**
        Loads the module from the specified file.
        \param[in] filename Specifies the XieXie module.
        This must be a dynamic library (*.dll file on Win32, *.so file on GNU/Linux).
        */
        bool Load(const std::string& filename)
        {
            return xvm_module_load(&module_, filename.c_str()) != 0;
        }

        //! Loads this module.
        bool Unload()
        {
            return xvm_module_unload(&module_) != 0;
        }

    private:
        
        friend class ByteCode;

        xvm_module module_;

};


//! The byte code class represents an entire virtual program.
class ByteCode
{
    
    public:

        //! Export address structure.
        struct ExportAddress
        {
            std::string     label;
            unsigned int    address;
        };

        //! Import address structure.
        struct ImportAddress
        {
            std::vector<size_t> indices;
        };

        ByteCode(const ByteCode&) = delete;
        ByteCode& operator = (const ByteCode&) = delete;

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
            return xvm_bytecode_write_to_file(&byteCode_, filename.c_str(), XBC_FORMAT_VERSION_LATEST) != 0;
        }

        //! Adds the specifies instruction
        void AddInstr(const Instruction& instr)
        {
            instructions_.push_back(instr);
        }

        //! Returns the instruction at the specified index.
        const Instruction& GetInstr(size_t index) const
        {
            return instructions_.at(index);
        }

        //! Returns the instruction at the specified index.
        Instruction& GetInstr(size_t index)
        {
            return instructions_.at(index);
        }

        //! Returns the number of instructions.
        size_t NumInstr() const
        {
            return instructions_.size();
        }

        //! Returns the index for the next instruction.
        size_t NextInstrIndex() const
        {
            return NumInstr();
        }

        //! Adds the specified integer literal word as instruction data field.
        void AddDataField(int wordDataField)
        {
            AddInstr(wordDataField);
        }

        //! Adds the specified floating-point literal word as instruction data field.
        void AddDataField(float floatDataField)
        {
            AddInstr(*reinterpret_cast<int*>(&floatDataField));
        }

        //! Adds the specified ascii literal as instruction data fields.
        void AddDataField(const std::string& asciiDataField)
        {
            size_t numInstr = 0;
            xvm_bytecode_datafield_ascii(NULL, asciiDataField.c_str(), &numInstr);

            size_t instrIndex = NextInstrIndex();
            instructions_.resize(instructions_.size() + numInstr);

            xvm_bytecode_datafield_ascii(
                reinterpret_cast<instr_t*>(&(instructions_[instrIndex])),
                asciiDataField.c_str(),
                NULL
            );
        }

        //! Adds a new export address.
        void AddExportAddress(const std::string& label, unsigned int address)
        {
            exportAddresses_.push_back({ label, address });
        }

        //! Adds a new import address.
        void AddImportAddress(const std::string& label)
        {
            importAddresses_[label];
        }

        //! Assigns the specified instruction index to the import address specified by 'label'.
        void AssignImportAddress(const std::string& label, size_t index)
        {
            auto it = importAddresses_.find(label);
            if (it != importAddresses_.end())
                it->second.indices.push_back(index);
        }

        //! Adds a new invocation identifier.
        void AddInvokeIdent(const std::string& ident)
        {
            invokeIdentifiers_.push_back(ident);
        }

        /**
        Binds an procedure to an invocation identifier.
        \param[in] ident Specifies the invocation identifier.
        \param[in] proc Specifies the procedure callback. If this is null, the default dummy procedure is used.
        */
        bool BindInvocation(const std::string& ident, XVM_INVOCATION_PROC proc)
        {
            return xvm_bytecode_bind_invocation(&byteCode_, ident.c_str(), proc) != 0;
        }

        /**
        Binds all procedure invocations from the specified module to this byte code.
        \see Module
        \see BindInvocation
        */
        bool BindModule(const Module& module)
        {
            return xvm_bytecode_bind_module(&byteCode_, &(module.module_)) != 0;
        }

        /**
        Finalizes the instruction building. After this call no further
        instructions can be added to this byte code object.
        \see instructions
        */
        bool Finalize()
        {
            /* Copy instructions into XVM byte code */
            size_t numInstr = instructions_.size();
            if (xvm_bytecode_create_instructions(&byteCode_, static_cast<int>(numInstr)) == 0)
                return false;
            
            for (size_t i = 0; i < numInstr; ++i)
                byteCode_.instructions[i] = instructions_[i].Code();

            /* Copy export-addresses into XVM byte code */
            size_t numExportAddr = exportAddresses_.size();
            if (numExportAddr > 0)
            {
                if (xvm_bytecode_create_export_addresses(&byteCode_, static_cast<unsigned int>(numExportAddr)) == 0)
                    return false;

                for (size_t i = 0; i < numExportAddr; ++i)
                {
                    const ExportAddress& addr = exportAddresses_[i];
                    xvm_export_address* exportAddr = &(byteCode_.export_addresses[i]);

                    /* Setup final export address */
                    xvm_export_address_setup(
                        exportAddr,
                        addr.address,
                        xvm_string_create_from(addr.label.c_str())
                    );
                }
            }

            /* Coyp invocation identifiers into XVM byte code */
            size_t numInvokeIdents = invokeIdentifiers_.size();
            if (numInvokeIdents > 0)
            {
                if (xvm_bytecode_create_invocations(&byteCode_, static_cast<unsigned int>(numInvokeIdents)) == 0)
                    return false;

                for (size_t i = 0; i < numInvokeIdents; ++i)
                {
                    /* Setup final invocation identifier */
                    const char* ident = invokeIdentifiers_[i].c_str();
                    byteCode_.invoke_idents[i] = xvm_string_create_from(ident);
                }
            }

            return true;
        }

    private:
        
        friend ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack);
        friend ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack, const std::string& entryPoint);

        xvm_bytecode                            byteCode_;

        //! Array list of all instructions.
        std::vector<Instruction>                instructions_;

        //! Array list of all export addresses.
        std::vector<ExportAddress>              exportAddresses_;

        //! Map of all import addresses.
        std::map<std::string, ImportAddress>    importAddresses_;

        //! Array list of all invocation identifiers.
        std::vector<std::string>                invokeIdentifiers_;

};


//! Virtual stack wrapper class. This is required to execute a program.
class Stack
{

    public:

        //! Default stack size (in 32-bit word size). By default 256 words = 1024 bytes.
        static const size_t defaultSize = 256;

        Stack(const Stack&) = delete;
        Stack& operator = (const Stack&) = delete;

        Stack(size_t size = Stack::defaultSize)
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
        case EXITCODE_INVOCATION_VIOLATION:
            return ExitCodes::InvocationViolation;
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
