/*
 * Log.h
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_LOG_H__
#define __XX_LOG_H__


#include "CompilerMessage.h"

#include <string>
#include <iostream>
#include <stack>
#include <initializer_list>


class Log
{
    
    public:
        
        class ScopedIndent
        {

            public:

                ScopedIndent(Log& log);
                ScopedIndent(ScopedIndent&& rhs);
                ~ScopedIndent();

            private:

                Log* log_ = nullptr;

        };

        Log(std::ostream& stream = std::cout);

        void StartLn();
        void EndLn();

        void Print(const std::string& text);
        void PrintLn(const std::string& text);

        void Message(const std::string& message);
        void Message(const CompilerMessage& message);
        void Messages(const std::initializer_list<std::string>& messages);

        void Warning(const std::string& message, bool appendPrefix = true);
        void Error(const std::string& message, bool appendPrefix = true);
        void FatalError(const std::string& message, bool appendPrefix = true);
        void Success(const std::string& message);

        /**
        Prints a headline with two "Message" function calls to underline the headline.
        \see Message
        */
        void Headline(const std::string& headline, char underlineChar = '-');

        void Blank();

        void IncIndent();
        void DecIndent();

        ScopedIndent Indent();
        
        /* === Members === */

        std::ostream&   stream;             //!< Output log stream.
        std::string     indent  = "  ";     //!< Indentation string.
        bool            verbose = false;    //!< Verbose state.

    private:
        
        std::stack<size_t>  indentStack_;
        std::string         currentIndent_;

};


#define SCOPED_INDENT(log) auto _scopedIndent = log.Indent()


#endif



// ================================================================================
