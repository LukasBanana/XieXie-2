/*
 * SafeStack.h
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SAFE_STACK_H__
#define __XX_SAFE_STACK_H__


#include "CompilerMessage.h"
#include "StringModifier.h"

#include <stack>
#include <exception>


//! Wrapper class for "std::stack" for exception safe "pop" and "top" functions.
template <typename T> class SafeStack
{
    
    public:
        
        void Push(const T& value)
        {
            container_.push(value);
        }
        void Push(T&& value)
        {
            container_.push(std::forward<T>(value));
        }

        void Pop()
        {
            AssertNotEmpty(__FUNCTION__);
            container_.pop();
        }

        const T& Top() const
        {
            AssertNotEmpty(__FUNCTION__);
            return container_.top();
        }
        T& Top()
        {
            AssertNotEmpty(__FUNCTION__);
            return container_.top();
        }

        bool Empty() const
        {
            return container_.empty();
        }

        typename std::stack<T>::size_type Size() const
        {
            return container_.size();
        }

    private:
        
        void AssertNotEmpty(const char* procName) const
        {
            if (container_.empty())
            {
                throw InternalError(
                    SyntaxAnalyzer::SourceArea::ignore,
                    "invalid access to empty stack (" + ToStr(procName) + ")"
                );
            }
        }

        std::stack<T> container_;

};


#endif



// ================================================================================