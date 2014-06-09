/*
 * Console header
 * 
 * This file is part of the "XieXie2-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_CONSOLE_H__
#define __XX_CONSOLE_H__


#include <string>
#include <initializer_list>

//class ErrorReporter;

namespace Console
{


/* === Functions === */

//void PrintErrorReport(ErrorReporter &Reporter);

void StartLn();
void EndLn();

void Print(const std::string& text);
void PrintLn(const std::string& text);

void Message(const std::string& message);
void Warning(const std::string& message, bool appendPrefix = true);
void Error(const std::string& message, bool isFatal = false, bool appendPrefix = true);
void Success(const std::string& message);

//void Message(const CompilerMessage& Msg);

void Messages(const std::initializer_list<std::string>& messages);

void UpperIndent();
void LowerIndent();

void Wait();


} // /namespace Console


/* === Classes === */

struct ScopedIndent
{
    ScopedIndent()
    {
        Console::UpperIndent();
    }
    ~ScopedIndent()
    {
        Console::LowerIndent();
    }
};


#endif



// ================================================================================