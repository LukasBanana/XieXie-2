/*
 * XVM wrapper header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_XVM_WRAPPER_H__
#define __XX_XVM_WRAPPER_H__


#include <string>

#define _REMOVE_XVM_TESTSUITE_
#include "xvm.c"


namespace XieXie
{


//! This is a C++ wrapper class for the xvm (XieXie VirtualMachine) which is written in plain C (c99).
class VirtualMachine
{
    
    public:
        
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

                bool ReadFromFile(const std::string& filename)
                {
                    return xvm_bytecode_read_from_file(&byteCode_, filename.c_str()) != 0;
                }

                bool WriteToFile(const std::string& filename)
                {
                    return xvm_bytecode_write_to_file(&byteCode_, filename.c_str()) != 0;
                }

            private:
        
                friend class VirtualMachine;

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
                
                friend class VirtualMachine;

                xvm_stack stack_;

        };

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

    private:
        
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
                    return ExitCodes::UnknwonEntryPoint;
                /*case EXITCODE_MEMORY_VIOLATION:
                    return ExitCodes::MemoryViolation;*/
            }
            return ExitCodes::__Unknown__;
        }

};


} // /namespace XieXie


#endif



// ================================================================================