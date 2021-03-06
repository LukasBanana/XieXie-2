/*
 * CompilerMessage.h
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_COMPILER_MESSAGE_H__
#define __XX_COMPILER_MESSAGE_H__


#include "SourceArea.h"

#include <exception>
#include <string>


class CompilerMessage : public std::exception
{
    
    public:
        
        /* === Enumerations === */

        enum class Categories
        {
            Message,
            Warning,
            SyntaxError,
            ContextError,
            CodeGenError,
            StateError,
            FileError,
            AsmError,
            InternalError,
        };

        /* === Constructors & destructor === */

        CompilerMessage(const Categories& category = Categories::Message);
        CompilerMessage(
            const std::string& message,
            const Categories& Category = Categories::Message
        );
        CompilerMessage(
            const SyntaxAnalyzer::SourceArea& sourceArea, const std::string& message,
            const Categories& Category = Categories::Message
        );
        CompilerMessage(
            const SyntaxAnalyzer::SourceArea& sourceArea, const std::string& message,
            const std::string& line, const std::string& marker,
            const Categories& category = Categories::Message
        );
        virtual ~CompilerMessage();

        /* === Functions === */

        const char* what() const throw();

        bool IsError() const;
        bool IsWarning() const;

        std::string Message() const;

        /* === Static functions === */

        static std::string ConstructMessage(
            const Categories category,
            const SyntaxAnalyzer::SourceArea& sourceArea,
            const std::string& message
        );
        static std::string GetCategoryString(const Categories category);

        /* === Inline functions === */

        Categories GetCategory() const
        {
            return category_;
        }

        const SyntaxAnalyzer::SourceArea& GetSourceArea() const
        {
            return sourceArea_;
        }

        const std::string& GetMessage() const
        {
            return message_;
        }
        const std::string& GetLine() const
        {
            return line_;
        }
        const std::string& GetMarker() const
        {
            return marker_;
        }

        bool HasLineMarker() const
        {
            return !line_.empty() && !marker_.empty();
        }

    private:
        
        /* === Members === */

        Categories                  category_;

        SyntaxAnalyzer::SourceArea  sourceArea_;

        std::string                 message_;       //!< Output message.
        std::string                 line_;          //!< Optional line where the error has been occured.
        std::string                 marker_;        //!< Optional mark to show where the error has been occured.

};


//! Compiler message comfort class (inherits from CompilerMessage but does not extend the functionality).
template <CompilerMessage::Categories C> class CommonCompilerMessage : public CompilerMessage
{

    public:

        CommonCompilerMessage(const std::string& message) :
            CompilerMessage{ SyntaxAnalyzer::SourceArea::ignore, message, C }
        {
        }
        CommonCompilerMessage(const SyntaxAnalyzer::SourceArea& sourceArea, const std::string& message) :
            CompilerMessage{ sourceArea, message, C }
        {
        }
        CommonCompilerMessage(
            const SyntaxAnalyzer::SourceArea& sourceArea, const std::string& message,
            const std::string& line, const std::string& marker) :
                CompilerMessage{ sourceArea, message, line, marker, C }
        {
        }

};


using CompilerWarning   = CommonCompilerMessage< CompilerMessage::Categories::Warning      >;
using SyntaxError       = CommonCompilerMessage< CompilerMessage::Categories::SyntaxError  >;
using ContextError      = CommonCompilerMessage< CompilerMessage::Categories::ContextError >;
using CodeGenError      = CommonCompilerMessage< CompilerMessage::Categories::CodeGenError >;
using StateError        = CommonCompilerMessage< CompilerMessage::Categories::StateError   >;
using FileError         = CommonCompilerMessage< CompilerMessage::Categories::FileError    >;
using AssemblerError    = CommonCompilerMessage< CompilerMessage::Categories::AsmError     >;
using InternalError     = CommonCompilerMessage< CompilerMessage::Categories::InternalError>;


#endif



// ================================================================================