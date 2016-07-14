/*
 * XVMWrapper.h
 *
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_XVM_WRAPPER_H__
#define __XX_XVM_WRAPPER_H__


#include <xiexie/xvm.h>

#include <string>
#include <vector>
#include <map>
#include <memory>


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

        static const Register tr;   //!< $tr   ->  Temporary register for the compiler, e.g. to schedule indirect calls.
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
        static const Register& Get(size_t reg);

        /**
        Returns the specified register via name.
        \param[in] name Specifies the register name (e.g. "$r0" or "$sp").
        Valid register names are: "$r0" - "$r23", "$tr", "$ar", "$xr", "$gp", "$cf", "$lb", "$sp", and "$pc".
        \throws std::invalid_argument If the register name is invalid.
        \see Name
        */
        static const Register& Get(const std::string& name);

    private:

        Register(register_id reg);

        reg_t reg_;

};

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

        #if !defined(__clang__) && !defined(__GNUC__)
        template <> static bool InRange<26>(int value);
        template <> static bool InRange<21>(int value);
        template <> static bool InRange<16>(int value);

        template <> static bool InRange<26>(unsigned int value);
        template <> static bool InRange<21>(unsigned int value);
        template <> static bool InRange<16>(unsigned int value);
        #endif

        /* ------- Functions ------- */

        Instruction();
        Instruction(int code);

        //! Returns the entire 32-bit instruction code.
        instr_t Code() const;

        //! Returns the instruction opcode.
        opcode_t OpCode() const;

        /**
        Returns the instruction mnemonic. This string will be filled with
        spaces (' ') so that the string always consists of 4 characters.
        */
        std::string Mnemonic() const;

        /**
        Back-patches the constant value of this instruction.
        This can be used to back-patch labels which must be resolved later.
        This function can be used for 1-register-, jump-, memory-, memory-offset- and special instructions.
        */
        void BackPatch(int value);

        /* ------- Static functions ------- */

        /**
        Makes a 3-register instruction (mov, not, and, or, xor, add, sub, mul, div, mod, sll, slr, cmp, fti, itf).
        \throws std::invalid_argument If 'opcode' is invalid.
        */
        static Instruction MakeReg3(xvm_opcode opcode, const Register& reg0, const Register& reg1, const Register& reg2);

        /**
        Makes a 2-register instruction (mov, not, and, or, xor, add, sub, mul, div, mod, sll, slr, cmp, fti, itf).
        \throws std::invalid_argument If 'opcode' is invalid.
        \throws std::out_of_range If 'value' is out of SgnValue16 range.
        */
        static Instruction MakeReg2(xvm_opcode opcode, const Register& reg0, const Register& reg1, int value);

        /**
        Makes a 1-register instruction (mov, and, or, xor, add, sub, mul, div, mod, sll, slr, push, pop, inc, dec).
        \param[in] value 21-bit signed value.
        \throws std::invalid_argument If 'opcode' is invalid.
        \throws std::out_of_range If 'value' is out of SgnValue21 range.
        */
        static Instruction MakeReg1(xvm_opcode opcode, const Register& reg, int value);

        /**
        Makes a jump instruction (jmp, je, jne, jg, jl, jge, jle, call).
        \param[in] offset 21-bit signed jump offset. A jump offset is WORD aligned,
        i.e. to jump to the next instruction, use 1 and to jump to the previous instruction use -1.
        \throws std::invalid_argument If 'opcode' is invalid.
        \throws std::out_of_range If 'offset' is out of SgnValue21 range.
        */
        static Instruction MakeJump(xvm_opcode opcode, const Register& reg, int offset);

        /**
        Makes a memory-offset instruction (ldb, stb, ldh, sth, ldw, stw).
        \param[in] offset 16-bit signed offset. This offset is BYTE aligned.
        \throws std::invalid_argument If 'opcode' is invalid.
        \throws std::out_of_range If 'offset' is out of SgnValue16 range.
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
        \param[in] argSize 16-bit unsigned value.
        Specifies the number of procedure argument WORDs which will be poped from the stack.
        \throws std::invalid_argument If 'opcode' is invalid.
        \throws std::out_of_range If 'argSize' is out of Value16 range.
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

template <> inline bool Instruction::InRange<26>(int value)
{
    return InBitRange<SgnValue26>(value);
}
template <> inline bool Instruction::InRange<21>(int value)
{
    return InBitRange<SgnValue21>(value);
}
template <> inline bool Instruction::InRange<16>(int value)
{
    return InBitRange<SgnValue16>(value);
}

template <> inline bool Instruction::InRange<26>(unsigned int value)
{
    return InBitRange<Value26>(value);
}
template <> inline bool Instruction::InRange<21>(unsigned int value)
{
    return InBitRange<Value21>(value);
}
template <> inline bool Instruction::InRange<16>(unsigned int value)
{
    return InBitRange<Value16>(value);
}

using Instr = Instruction;


/* --- Intrinsics Class --- */

//! Intrinsics helper class.
class Intrinsics
{

    public:

        using AddressMapType = std::map<std::string, unsigned int>;

        Intrinsics();

        /**
        Returns the intrinsic address by the specified intrinsic name or 0 if the name is invalid.
        \param[in] name Specifies the intrinsic name.
        \throws std::invalid_argument If 'name' is invalid.
        */
        unsigned int AddressByName(const std::string& name) const;

    private:

        AddressMapType addresses_;

};

//! Intrinsics unique pointer type.
using IntrinsicsPtr = std::unique_ptr<Intrinsics>;


/* --- Module Class --- */

//! Module wrapper class. This can be used for external invocations.
class Module
{

    public:

        Module(const Module&) = delete;
        Module& operator = (const Module&) = delete;

        Module();
        ~Module();

        /**
        Loads the module from the specified file.
        \param[in] filename Specifies the XieXie module.
        This must be a dynamic library (*.dll file on Win32, *.so file on GNU/Linux).
        */
        bool Load(const std::string& filename);

        //! Unloads this module.
        bool Unload();

    private:

        friend class ByteCode;

        xvm_module module_;

};

//! Moduel unique pointer type.
using ModulePtr = std::unique_ptr<Module>;


/* --- ByteCode Class --- */

//! The byte code class represents an entire virtual program.
class ByteCode
{

    public:

        /* === Structures/ Classes === */

        //! Export address structure.
        struct ExportAddress
        {
            std::string     label;
            unsigned int    address;
        };

        //! Import address structure.
        struct ImportAddress
        {
            std::vector<unsigned int> indices;
        };

        /**
        Entry point object. This can be used to find an entry point once
        and later use it several times, to save time for the search.
        \see ExecuteProgram
        */
        class EntryPoint
        {

            public:

                /**
                Fetches the exported address 'label' from the specified byte code.
                \throws std::invalid_argument If 'label' is not an exported address in 'byteCode'.
                */
                EntryPoint(const ByteCode& byteCode, const std::string& label);

            private:

                friend ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack);
                friend ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack, const std::string& entryPoint);
                friend ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack, const EntryPoint& entryPoint);

                const ByteCode*             owner_  = nullptr;
                const xvm_export_address*   addr_   = nullptr;

        };

        /* === Functions === */

        ByteCode(const ByteCode&) = delete;
        ByteCode& operator = (const ByteCode&) = delete;

        ByteCode();
        ~ByteCode();

        //! Reads the instructions from the specified file.
        bool ReadFromFile(const std::string& filename);
        //! Writes the instructions to the specified file.
        bool WriteToFile(const std::string& filename);

        //! Adds the specifies instruction
        void AddInstr(const Instruction& instr);

        //! Returns the instruction at the specified index.
        const Instruction& GetInstr(size_t index) const;
        //! Returns the instruction at the specified index.
        Instruction& GetInstr(size_t index);

        //! Returns the number of instructions.
        size_t NumInstr() const;
        //! Returns the number of data fields.
        size_t NumDataFields() const;
        //! Returns the index for the next instruction (including the data fields inside the instruction buffer!).
        size_t NextInstrIndex() const;

        //! Adds the specified integer literal word as instruction data field.
        void AddDataField(int wordDataField);
        //! Adds the specified floating-point literal word as instruction data field.
        void AddDataField(float floatDataField);
        //! Adds the specified ascii literal as instruction data fields.
        void AddDataField(const std::string& asciiDataField);

        //! Adds a new export address.
        void AddExportAddress(const std::string& label, unsigned int address);
        //! Adds a new import address.
        void AddImportAddress(const std::string& label);

        //! Assigns the specified instruction index to the import address specified by 'label'.
        void AssignImportAddress(const std::string& label, unsigned int index);

        //! Adds a new invocation identifier.
        void AddInvokeIdent(const std::string& ident);

        //! Adds a new module name.
        void AddModuleName(const std::string& name);

        /**
        Binds an procedure to an invocation identifier.
        \param[in] ident Specifies the invocation identifier.
        \param[in] proc Specifies the procedure callback. If this is null, the default dummy procedure is used.
        */
        bool BindInvocation(const std::string& ident, xvm_invocation_proc proc);
        /**
        Binds all procedure invocations from the specified module to this byte code.
        \see Module
        \see BindInvocation
        */
        bool BindModule(const Module& module);
        /**
        Loads and binds all automatic modules, this byte code has.
        \param[in] modulePath Optional path where the modules are stored, e.g. "XieXie/modules/".
        \return True if all automatic modules have been bound successful.
        \see GetModuleNames
        \see BindModule
        */
        bool BindAutomaticModules(const std::string& modulePath = "");

        /**
        Finalizes the instruction building. After this call no further
        instructions can be added to this byte code object.
        \see instructions
        */
        bool Finalize();

        //! Returns the instructions.
        const std::vector<Instruction>& GetInstructions() const
        {
            return instructions_;
        }
        //! Returns the 32-bit data fields.
        const std::vector<unsigned int>& GetDataFields() const
        {
            return dataFields_;
        }
        //! Returns the export addresses.
        const std::vector<ExportAddress>& GetExportAddresses() const
        {
            return exportAddresses_;
        }
        //! Returns the import addresses.
        const std::map<std::string, ImportAddress>& GetImportAddresses() const
        {
            return importAddresses_;
        }
        //! Returns the invoke identifiers.
        const std::vector<std::string>& GetInvokeIdentifiers() const
        {
            return invokeIdentifiers_;
        }
        //! Returns the module names.
        const std::vector<std::string>& GetModuleNames() const
        {
            return moduleNames_;
        }

    private:

        friend ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack);
        friend ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack, const std::string& entryPoint);
        friend ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack, const ByteCode::EntryPoint& entryPoint);
        friend class EntryPoint;

        //! XVM byte code object.
        xvm_bytecode                            byteCode_;

        //! Array list of all instructions.
        std::vector<Instruction>                instructions_;

        //! Array list of all 32-bit data fields.
        std::vector<unsigned int>               dataFields_;

        //! Array list of all export addresses.
        std::vector<ExportAddress>              exportAddresses_;

        //! Map of all import addresses.
        std::map<std::string, ImportAddress>    importAddresses_;

        //! Array list of all invocation identifiers.
        std::vector<std::string>                invokeIdentifiers_;

        //! Array list of all module names.
        std::vector<std::string>                moduleNames_;

        //! List of all automatic modules.
        std::vector<ModulePtr>                  automaticModules_;

};

//! ByteCode unique pointer type.
using ByteCodePtr = std::unique_ptr<ByteCode>;


/* --- Stack Class --- */

//! Virtual stack wrapper class. This is required to execute a program.
class Stack
{

    public:

        //! Default stack size (in 32-bit word size). By default 2048 words = 8192 bytes.
        static const size_t defaultSize = 2048;

        Stack(const Stack&) = delete;
        Stack& operator = (const Stack&) = delete;

        Stack(size_t size = Stack::defaultSize);
        ~Stack();

        //! Returns the number of WORD (32-bit) entries in the stack.
        size_t Size() const;

    private:

        friend ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack);
        friend ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack, const std::string& entryPoint);
        friend ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack, const ByteCode::EntryPoint& entryPoint);

        xvm_stack stack_;

};


/* --- Globals --- */

/**
Executes the specified program using the specified stack.
\see ExitCodes
\see ByteCode
\see Stack
*/
ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack);

/**
Executes the program 'byteCode' from the specified entry point.
\see ExitCodes
\see ByteCode
\see Stack
*/
ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack, const std::string& entryPoint);

/**
Executes the program 'byteCode' from the specified entry point.
\see ExitCodes
\see ByteCode
\see ByteCode::EntryPoint
\see Stack
*/
ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack, const ByteCode::EntryPoint& entryPoint);

//! Returns a string for the specified exit code.
std::string ExitCodeString(const ExitCodes exitCode);


} // /namespace VirtualMachine

} // /namespace XieXie


#endif



// ================================================================================
