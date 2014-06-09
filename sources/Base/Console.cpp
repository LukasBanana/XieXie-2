/*
 * Console file
 * 
 * This file is part of the "XieXie2-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Console.h"

#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>


namespace Console
{


/*
 * Internal objects
 */

struct ConsoleState
{
    std::string indent; //!< Current indentation string.
    std::string indentModifier = "  ";
};

static ConsoleState consoleState;


/*
 * Functions
 */

void StartLn()
{
    std::cout << consoleState.indent;
}

void EndLn()
{
    std::cout << std::endl;
}

void Print(const std::string& text)
{
    std::cout << text;
}

void PrintLn(const std::string& text)
{
    StartLn();
    Print(text);
    EndLn();
}

void Message(const std::string& message)
{
    PrintLn(message);
}

void Warning(const std::string& message, bool appendPrefix)
{
    //ScopedColor color(Colors::Yellow);

    if (appendPrefix)
        Message("WARNING >> " + message);
    else
        Message(message);
}

void Error(const std::string& message, bool isFatal, bool appendPrefix)
{
    //PushAttrib();

    /*if (isFatal)
        SetColor(Colors::Black, Colors::Red | Colors::Intens);
    else
        SetColor(Colors::Red | Colors::Intens);*/

    if (appendPrefix)
        Message("ERROR >> " + message);
    else
        Message(message);

    //PopAttrib();
}

void Success(const std::string& message)
{
    //ScopedColor Unused(Colors::Green | Colors::Intens);
    Message(message);
}

/*void Message(const CompilerMessage& Msg)
{
    //...
}*/

void Messages(const std::initializer_list<std::string>& messages)
{
    for (const auto& msg : messages)
        Message(msg);
}

void UpperIndent()
{
    consoleState.indent += consoleState.indentModifier;
}

void LowerIndent()
{
    if (consoleState.indent.size() > consoleState.indentModifier.size())
        consoleState.indent.resize(consoleState.indent.size() - consoleState.indentModifier.size());
    else
        consoleState.indent.clear();
}

void Wait()
{
    //todo -> make this OS specific!
    system("pause");
}


} // /namespace Console



// ================================================================================