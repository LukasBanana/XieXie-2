/*
 * Compiler message header
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

        /* === Static functions === */

        static std::string ConstructMessage(
            const Categories category,
            const SyntaxAnalyzer::SourceArea& sourceArea,
            const std::string& message
        );
        static std::string GetCategoryString(const Categories category);

        /* === Inline functions === */

        Categories Category() const
        {
            return category_;
        }

        const std::string& Message() const
        {
            return message_;
        }
        const std::string& Line() const
        {
            return line_;
        }
        const std::string& Marker() const
        {
            return marker_;
        }

        bool HasLineMarker() const
        {
            return !line_.empty() && !marker_.empty();
        }

    private:
        
        /* === Members === */

        Categories category_;

        SyntaxAnalyzer::SourceArea sourceArea_;

        std::string message_;   //!< Output message.
        std::string line_;      //!< Optional line where the error has been occured.
        std::string marker_;    //!< Optional mark to show where the error has been occured.

};


//! Compiler message comfort class (inherits from CompilerMessage but does not extend the functionality).
template <CompilerMessage::Categories C> class CommonCompilerMessage : public CompilerMessage
{
    public:
        CommonCompilerMessage(const std::string& message) :
            CompilerMessage(message, C)
        {
        }
        CommonCompilerMessage(const SyntaxAnalyzer::SourceArea& sourceArea, const std::string& message) :
            CompilerMessage(sourceArea, message, C)
        {
        }
        CommonCompilerMessage(
            const SyntaxAnalyzer::SourceArea& sourceArea, const std::string& message,
            const std::string& line, const std::string& marker) :
                CompilerMessage(sourceArea, message, line, marker, C)
        {
        }
};


typedef CommonCompilerMessage< CompilerMessage::Categories::Warning      > CompilerWarning;
typedef CommonCompilerMessage< CompilerMessage::Categories::SyntaxError  > SyntaxError;
typedef CommonCompilerMessage< CompilerMessage::Categories::ContextError > ContextError;
typedef CommonCompilerMessage< CompilerMessage::Categories::CodeGenError > CodeGenError;
typedef CommonCompilerMessage< CompilerMessage::Categories::StateError   > StateError;
typedef CommonCompilerMessage< CompilerMessage::Categories::FileError    > FileError;
typedef CommonCompilerMessage< CompilerMessage::Categories::AsmError     > AssemblerError;


#endif



// ================================================================================