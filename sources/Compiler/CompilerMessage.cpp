/*
 * CompilerMessage.cpp
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CompilerMessage.h"


using namespace SyntaxAnalyzer;

CompilerMessage::CompilerMessage(const Categories& category) :
    category_{ category }
{
}
CompilerMessage::CompilerMessage(const std::string& message, const Categories& category) :
    category_   { category },
    message_    { message  }
{
}
CompilerMessage::CompilerMessage(
    const SourceArea& sourceArea, const std::string& message, const Categories& category) :
        category_   { category   },
        sourceArea_ { sourceArea },
        message_    { message    }
{
}
CompilerMessage::CompilerMessage(
    const SourceArea& sourceArea, const std::string& message,
    const std::string& line, const std::string& marker,
    const Categories& category) :
        category_   { category   },
        sourceArea_ { sourceArea },
        message_    { message    },
        line_       { line       },
        marker_     { marker     }
{
    /* Remove new-line character from source line */
    if (line_.size() > 0 && line_.back() == '\n')
        line_.resize(line_.size() - 1);
}
CompilerMessage::~CompilerMessage()
{
}

const char* CompilerMessage::what() const throw()
{
    return message_.c_str();
}

bool CompilerMessage::IsError() const
{
    return
        category_ == Categories::SyntaxError    ||
        category_ == Categories::ContextError   ||
        category_ == Categories::CodeGenError   ||
        category_ == Categories::StateError     ||
        category_ == Categories::FileError      ||
        category_ == Categories::AsmError;
}

bool CompilerMessage::IsWarning() const
{
    return category_ == Categories::Warning;
}

std::string CompilerMessage::Message() const
{
    return CompilerMessage::ConstructMessage(GetCategory(), GetSourceArea(), GetMessage());
}

std::string CompilerMessage::ConstructMessage(
    const Categories Category, const SourceArea& sourceArea, const std::string& message)
{
    return GetCategoryString(Category) + " (" + sourceArea.ToString() + ") -- " + message;
}

std::string CompilerMessage::GetCategoryString(const Categories Category)
{
    switch (Category)
    {
        case Categories::Message:
            break;
        case Categories::Warning:
            return "warning";
        case Categories::SyntaxError:
            return "syntax error";
        case Categories::ContextError:
            return "context error";
        case Categories::CodeGenError:
            return "code generation error";
        case Categories::StateError:
            return "state error";
        case Categories::FileError:
            return "file error";
        case Categories::AsmError:
            return "assembler error";
    }
    return "";
}



// ================================================================================