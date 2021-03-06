/*
 * Log.cpp
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Log.h"
#include "ConsoleManip.h"


using namespace Platform::ConsoleManip;

/*
 * ScopedIndent class
 */

Log::ScopedIndent::ScopedIndent(Log& log) :
    log_{ &log }
{
    log_->IncIndent();
}
Log::ScopedIndent::ScopedIndent(ScopedIndent&& rhs) :
    log_{ rhs.log_ }
{
    rhs.log_ = nullptr;
}
Log::ScopedIndent::~ScopedIndent()
{
    if (log_)
        log_->DecIndent();
}


/*
 * Log class
 */

Log::Log(std::ostream& stream) :
    // bracket initializer required for GCC (due to bug 56032)
    stream( stream )
{
}

void Log::StartLn()
{
    stream << currentIndent_;
}

void Log::EndLn()
{
    stream << std::endl;
}

void Log::Print(const std::string& text)
{
    stream << text;
}

void Log::PrintLn(const std::string& text)
{
    StartLn();
    Print(text);
    EndLn();
}

void Log::Message(const std::string& message)
{
    PrintLn(message);
}

static void PrintMark(Log& log, const std::string& text, const Color::ValueType color)
{
    log.StartLn();
    {
        log.Print(">> ");
        PushColor(log.stream, color);
        {
            log.Print(text);
        }
        PopColor(log.stream);
    }
    log.EndLn();
}

static void PrintMessageColored(Log& log, const CompilerMessage& message, const Color::ValueType color)
{
    log.StartLn();
    {
        /* Print colored message category */
        PushColor(log.stream, color);
        {
            log.Print(message.GetCategoryString(message.GetCategory()));
        }
        PopColor(log.stream);

        /* Print source area */
        if (message.GetSourceArea().IsValid())
            log.Print(" (" + message.GetSourceArea().ToString() + ")");

        /* Print message */
        log.Print(" -- " + message.GetMessage());
    }
    log.EndLn();

    /* Print line and marker */
    if (!message.GetLine().empty() && !message.GetMarker().empty())
    {
        PrintMark(log, message.GetLine(), color);
        PrintMark(log, message.GetMarker(), color);
    }
}

void Log::Message(const CompilerMessage& message)
{
    if (message.IsError())
        PrintMessageColored(*this, message, Color::Red | Color::Intens);
    else if (message.IsWarning())
        PrintMessageColored(*this, message, Color::Yellow);
    else
        Message(message.Message());
}

void Log::Messages(const std::initializer_list<std::string>& messages)
{
    for (const auto& msg : messages)
        Message(msg);
}

void Log::Warning(const std::string& message, bool appendPrefix)
{
    PushColor(stream, Color::Yellow);
    {
        if (appendPrefix)
            Message("warning: " + message);
        else
            Message(message);
    }
    PopColor(stream);
}

void Log::Error(const std::string& message, bool appendPrefix)
{
    PushColor(stream, Color::Red | Color::Intens);
    {
        if (appendPrefix)
            Message("error: " + message);
        else
            Message(message);
    }
    PopColor(stream);
}

void Log::FatalError(const std::string& message, bool appendPrefix)
{
    PushColor(stream, Color::Black, Color::Red | Color::Intens);
    {
        if (appendPrefix)
            Message("error: " + message);
        else
            Message(message);
    }
    PopColor(stream);
}

void Log::Success(const std::string& message)
{
    PushColor(stream, Color::Green | Color::Intens);
    {
        Message(message);
    }
    PopColor(stream);
}

void Log::Headline(const std::string& headline, char underlineChar)
{
    Message(headline);
    Message(std::string(headline.size(), underlineChar));
}

void Log::Blank()
{
    StartLn();
    EndLn();
}

void Log::IncIndent()
{
    currentIndent_ += indent;
    indentStack_.push(indent.size());
}

void Log::DecIndent()
{
    if (!indentStack_.empty())
    {
        currentIndent_.resize(currentIndent_.size() - indentStack_.top());
        indentStack_.pop();
    }
}

Log::ScopedIndent Log::Indent()
{
    return std::move(ScopedIndent(*this));
}



// ================================================================================
