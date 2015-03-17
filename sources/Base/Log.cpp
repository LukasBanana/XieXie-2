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
    stream_{ stream }
{
}

void Log::StartLn()
{
    stream_ << currentIndent_;
}

void Log::EndLn()
{
    stream_ << std::endl;
}

void Log::Print(const std::string& text)
{
    stream_ << text;
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
        PushColor(color);
        {
            log.Print(text);
        }
        PopColor();
    }
    log.EndLn();
}

static void PrintMessageColored(Log& log, const CompilerMessage& message, const Color::ValueType color)
{
    log.StartLn();
    {
        /* Print colored message category */
        PushColor(color);
        {
            log.Print(message.GetCategoryString(message.GetCategory()));
        }
        PopColor();

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
    PushColor(Color::Yellow);
    {
        if (appendPrefix)
            Message("warning: " + message);
        else
            Message(message);
    }
    PopColor();
}

void Log::Error(const std::string& message, bool appendPrefix)
{
    PushColor(Color::Red | Color::Intens);
    {
        if (appendPrefix)
            Message("error: " + message);
        else
            Message(message);
    }
    PopColor();
}

void Log::FatalError(const std::string& message, bool appendPrefix)
{
    PushColor(Color::Black, Color::Red | Color::Intens);
    {
        if (appendPrefix)
            Message("error: " + message);
        else
            Message(message);
    }
    PopColor();
}

void Log::Success(const std::string& message)
{
    PushColor(Color::Green | Color::Intens);
    {
        Message(message);
    }
    PopColor();
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