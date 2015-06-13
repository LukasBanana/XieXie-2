/*
 * XVMWrapper.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <xiexie/xvm_wrapper.h>


namespace XieXie
{

namespace VirtualMachine
{


/* --- Register Class --- */

Register::Register(register_id reg) :
    reg_{ static_cast<reg_t>(reg) }
{
}

const Register& Register::Get(size_t reg)
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
        case 0x18: return tr;
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

const Register& Register::Get(const std::string& name)
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
        else if (name == "$tr") return tr;
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
            }
        }
    }
    throw std::invalid_argument("invalid register name '" + name + "'");
    return r0;
}

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

const Register Register::tr(REG_TR);
const Register Register::ar(REG_AR);
const Register Register::xr(REG_XR);
const Register Register::gp(REG_GP);

const Register Register::cf(REG_CF);
const Register Register::lb(REG_LB);
const Register Register::sp(REG_SP);
const Register Register::pc(REG_PC);


/* --- Instruction Class --- */

Instruction::Instruction() :
    code_{ 0 }
{
}
Instruction::Instruction(int code) :
    code_{ code }
{
}

instr_t Instruction::Code() const
{
    return code_;
}

opcode_t Instruction::OpCode() const
{
    return xvm_instr_get_opcode(code_);
}

std::string Instruction::Mnemonic() const
{
    return std::string(xvm_instr_get_mnemonic(OpCode()));
}

void Instruction::BackPatch(int value)
{
    code_ = xvm_instr_patch_value(Code(), static_cast<unsigned int>(value));
}

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


/*
 * ======= Private: =======
 */
        
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


/* --- Intrinsics Class --- */

Intrinsics::Intrinsics()
{
    addresses_ = std::move(AddressMapType
    {
        { "AllocMem",    INSC_ALLOC_MEM    },
        { "FreeMem",     INSC_FREE_MEM     },
        { "CopyMem",     INSC_COPY_MEM     },
        { "FillMem",     INSC_FILL_MEM     },
        { "SysCall",     INSC_SYS_CALL     },
        { "Print",       INSC_PRINT        },
        { "PrintLn",     INSC_PRINT_LN     },
        { "PrintInt",    INSC_PRINT_INT    },
        { "PrintFloat",  INSC_PRINT_FLOAT  },
        { "Input",       INSC_INPUT        },
        { "InputInt",    INSC_INPUT_INT    },
        { "InputFloat",  INSC_INPUT_FLOAT  },
        { "CreateFile",  INSC_CREATE_FILE  },
        { "DeleteFile",  INSC_DELETE_FILE  },
        { "OpenFile",    INSC_OPEN_FILE    },
        { "CloseFile",   INSC_CLOSE_FILE   },
        { "FileSetPos",  INSC_FILE_SET_POS },
        { "FileGetPos",  INSC_FILE_GET_POS },
        { "FileEOF",     INSC_FILE_EOF     },
        { "WriteByte",   INSC_WRITE_BYTE   },
        { "WriteWord",   INSC_WRITE_WORD   },
        { "WriteBuffer", INSC_WRITE_BUFFER },
        { "ReadByte",    INSC_READ_BYTE    },
        { "ReadWord",    INSC_READ_WORD    },
        { "ReadBuffer",  INSC_READ_BUFFER  },
        { "Sin",         INSC_SIN          },
        { "Cos",         INSC_COS          },
        { "Tan",         INSC_TAN          },
        { "ASin",        INSC_ASIN         },
        { "ACos",        INSC_ACOS         },
        { "ATan",        INSC_ATAN         },
        { "SinH",        INSC_SINH         },
        { "CosH",        INSC_COSH         },
        { "TanH",        INSC_TANH         },
        { "Exp",         INSC_EXP          },
        { "Exp2",        INSC_EXP2         },
        { "Pow",         INSC_POW          },
        { "Log",         INSC_LOG          },
        { "Log2",        INSC_LOG2         },
        { "Log10",       INSC_LOG10        },
        { "Sqrt",        INSC_SQRT         },
        { "Ceil",        INSC_CEIL         },
        { "Floor",       INSC_FLOOR        },
        { "RandInt",     INSC_RAND_INT     },
        { "RandFloat",   INSC_RAND_FLOAT   },
        { "Time",        INSC_TIME         },
        { "Sleep",       INSC_SLEEP        }
    });
}

unsigned int Intrinsics::AddressByName(const std::string& name) const
{
    auto it = addresses_.find(name);
    if (it == addresses_.end())
        throw std::invalid_argument("unknown intrinsic \"" + name + "\"");
    return it->second;
}


/* --- Module Class --- */

Module::Module()
{
    xvm_module_init(&module_);
}
Module::~Module()
{
    xvm_module_unload(&module_);
}

bool Module::Load(const std::string& filename)
{
    return xvm_module_load(&module_, filename.c_str()) != 0;
}

bool Module::Unload()
{
    return xvm_module_unload(&module_) != 0;
}


/* --- ByteCode::EntryPoint Class --- */

ByteCode::EntryPoint::EntryPoint(const ByteCode& byteCode, const std::string& label) :
    owner_{ &byteCode }
{
    addr_ = xvm_bytecode_find_export_address(&(byteCode.byteCode_), label.c_str());
    if (!addr_)
        throw std::invalid_argument("invalid entry point \"" + label + "\"");
}


/* --- ByteCode Class --- */

ByteCode::ByteCode()
{
    xvm_bytecode_init(&byteCode_);
}
ByteCode::~ByteCode()
{
    xvm_bytecode_free(&byteCode_);
}

bool ByteCode::ReadFromFile(const std::string& filename)
{
    /* Read byte-code from file */
    if (!xvm_bytecode_read_from_file(&byteCode_, filename.c_str()))
        return false;

    /* Extract export addresses */
    exportAddresses_.resize(byteCode_.num_export_addresses);
    for (size_t i = 0; i < byteCode_.num_export_addresses; ++i)
    {
        exportAddresses_[i].address = byteCode_.export_addresses[i].addr;
        exportAddresses_[i].label   = std::string(byteCode_.export_addresses[i].label.str);
    }

    /* Extract module names */
    moduleNames_.resize(byteCode_.num_module_names);
    for (size_t i = 0; i < byteCode_.num_module_names; ++i)
        moduleNames_[i] = std::string(byteCode_.module_names[i].str);

    return true;
}

bool ByteCode::WriteToFile(const std::string& filename)
{
    /* Write byte-code to file */
    return xvm_bytecode_write_to_file(&byteCode_, filename.c_str(), XBC_FORMAT_VERSION_LATEST) != 0;
}

void ByteCode::AddInstr(const Instruction& instr)
{
    instructions_.push_back(instr);
}

const Instruction& ByteCode::GetInstr(size_t index) const
{
    return instructions_.at(index);
}

Instruction& ByteCode::GetInstr(size_t index)
{
    return instructions_.at(index);
}

size_t ByteCode::NumInstr() const
{
    return instructions_.size();
}

size_t ByteCode::NextInstrIndex() const
{
    return NumInstr();
}

void ByteCode::AddDataField(int wordDataField)
{
    AddInstr(wordDataField);
}

void ByteCode::AddDataField(float floatDataField)
{
    AddInstr(*reinterpret_cast<int*>(&floatDataField));
}

void ByteCode::AddDataField(const std::string& asciiDataField)
{
    size_t numInstr = 0;
    xvm_bytecode_datafield_ascii(NULL, asciiDataField.c_str(), &numInstr);

    auto instrIndex = NextInstrIndex();
    instructions_.resize(instructions_.size() + numInstr);

    xvm_bytecode_datafield_ascii(
        reinterpret_cast<instr_t*>(&(instructions_[instrIndex])),
        asciiDataField.c_str(),
        NULL
    );
}

void ByteCode::AddExportAddress(const std::string& label, unsigned int address)
{
    exportAddresses_.push_back({ label, address });
}

void ByteCode::AddImportAddress(const std::string& label)
{
    importAddresses_[label];
}

void ByteCode::AssignImportAddress(const std::string& label, unsigned int index)
{
    auto it = importAddresses_.find(label);
    if (it != importAddresses_.end())
        it->second.indices.push_back(index);
}

void ByteCode::AddInvokeIdent(const std::string& ident)
{
    invokeIdentifiers_.push_back(ident);
}

void ByteCode::AddModuleName(const std::string& name)
{
    moduleNames_.push_back(name);
}

bool ByteCode::BindInvocation(const std::string& ident, xvm_invocation_proc proc)
{
    return xvm_bytecode_bind_invocation(&byteCode_, ident.c_str(), proc) != 0;
}

bool ByteCode::BindModule(const Module& module)
{
    return xvm_bytecode_bind_module(&byteCode_, &(module.module_)) != 0;
}

bool ByteCode::BindAutomaticModules(const std::string& modulePath)
{
    bool result = true;

    automaticModules_.clear();

    for (const auto& name : GetModuleNames())
    {
        /* Determine module filename */
        auto filename = modulePath + name;

        #if defined(_WIN32)
        filename += ".dll";
        #elif defined(__linux__)
        filename += ".so";
        #endif

        /* Load and bind module to byte code */
        auto module = std::unique_ptr<Module>(new Module());
        if (!module->Load(filename) || !BindModule(*module))
            result = false;

        /* Add module to list */
        automaticModules_.emplace_back(std::move(module));
    }

    return result;
}

bool ByteCode::Finalize()
{
    /* Copy instructions into XVM byte code */
    auto numInstr = instructions_.size();
    if (xvm_bytecode_create_instructions(&byteCode_, static_cast<int>(numInstr)) == 0)
        return false;
            
    for (size_t i = 0; i < numInstr; ++i)
        byteCode_.instructions[i] = instructions_[i].Code();

    /* Copy export-addresses into XVM byte code */
    auto numExportAddr = exportAddresses_.size();
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

    /* Copy import-addresses into XVM byte code */
    //todo...

    /* Coyp invocation identifiers into XVM byte code */
    auto numInvokeIdents = invokeIdentifiers_.size();
    if (numInvokeIdents > 0)
    {
        if (xvm_bytecode_create_invocations(&byteCode_, static_cast<unsigned int>(numInvokeIdents)) == 0)
            return false;

        for (size_t i = 0; i < numInvokeIdents; ++i)
        {
            /* Setup final invocation identifier */
            auto ident = invokeIdentifiers_[i].c_str();
            byteCode_.invoke_idents[i] = xvm_string_create_from(ident);
        }
    }

    /* Copy module names into XVM byte code */
    auto numModuleNames = moduleNames_.size();
    if (numModuleNames > 0)
    {
        if (xvm_bytecode_create_module_names(&byteCode_, static_cast<unsigned int>(numModuleNames)) == 0)
            return false;

        for (size_t i = 0; i < numModuleNames; ++i)
        {
            /* Setup final module names */
            auto name = moduleNames_[i].c_str();
            byteCode_.module_names[i] = xvm_string_create_from(name);
        }
    }

    return true;
}


/* --- Stack Class --- */

Stack::Stack(size_t size)
{
    xvm_stack_init(&stack_);
    xvm_stack_create(&stack_, size);
}
Stack::~Stack()
{
    xvm_stack_free(&stack_);
}

size_t Stack::Size() const
{
    return stack_.stack_size;
}


/* --- Globals --- */

// map: xvm_exit_codes -> ExitCodes
static ExitCodes MapExitCode(xvm_exit_codes code)
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

// map: ExitCodes -> xvm_exit_codes
static xvm_exit_codes MapExitCode(ExitCodes code)
{
    switch (code)
    {
        case ExitCodes::Success:
            return EXITCODE_SUCCESS;
        case ExitCodes::InvalidByteCode:
            return EXITCODE_INVALID_BYTECODE;
        case ExitCodes::InvalidStack:
            return EXITCODE_INVALID_STACK;
        case ExitCodes::InvalidOpCode:
            return EXITCODE_INVALID_OPCODE;
        case ExitCodes::InvalidIntrinsic:
            return EXITCODE_INVALID_INTRINSIC;
        case ExitCodes::StackOverflow:
            return EXITCODE_STACK_OVERFLOW;
        case ExitCodes::StackUnderflow:
            return EXITCODE_STACK_UNDERFLOW;
        case ExitCodes::DivisionByZero:
            return EXITCODE_DIVISION_BY_ZERO;
        case ExitCodes::UnknownEntryPoint:
            return EXITCODE_UNKNOWN_ENTRY_POINT;
        case ExitCodes::InvocationViolation:
            return EXITCODE_INVOCATION_VIOLATION;
    }
    return EXITCODE_SUCCESS;
}

ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack)
{
    xvm_execution_options options;
    options.flags = 0;

    auto code = xvm_execute_program(
        &(byteCode.byteCode_),
        &(stack.stack_),
        &options
    );
    return MapExitCode(code);
}

ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack, const std::string& entryPoint)
{
    xvm_execution_options options;
    options.flags = 0;

    auto code = xvm_execute_program_entry_point(
        &(byteCode.byteCode_),
        &(stack.stack_),
        entryPoint.c_str(),
        &options
    );

    return MapExitCode(code);
}

ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack, const ByteCode::EntryPoint& entryPoint)
{
    if (entryPoint.owner_ == (&byteCode))
    {
        xvm_execution_options options;
        options.flags = 0;

        auto code = xvm_execute_program_ext(
            &(byteCode.byteCode_),
            &(stack.stack_),
            entryPoint.addr_,
            &options
        );

        return MapExitCode(code);
    }
    return ExitCodes::InvalidByteCode;
}

std::string ExitCodeString(const ExitCodes exitCode)
{
    return std::string(xvm_exitcode_to_string(MapExitCode(exitCode)));
}


} // /namespace VirtualMachine

} // /namespace XieXie



// ================================================================================
