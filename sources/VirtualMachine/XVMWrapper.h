/*
 * XVM wrapper header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_XVM_WRAPPER_H__
#define __XX_XVM_WRAPPER_H__


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
        
                ByteCode();
                ~ByteCode();

                bool ReadFromFile(const std::string& filename);
                bool WriteToFile(const std::string& filename);

            private:
        
                friend class VirtualMachine;

                xvm_bytecode byteCode_;

        };

        //! A virtual stack is required for to execute a program.
        class Stack
        {
            
            public:
                
                static const size_t defaultSize = 256;

                Stack(size_t size = defaultSize);
                ~Stack();

                size_t Size() const;

            private:
                
                xvm_stack stack_;

        };

        /**
        Executes the specified program using the specified stack.
        \return Exit code. 0 on success.
        \see ExitCodes
        */
        ExitCodes ExecuteProgram(const ByteCode& byteCode, Stack& stack);

};


} // /namespace XieXie


#endif



// ================================================================================