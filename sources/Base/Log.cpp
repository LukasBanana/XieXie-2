/*
 * Log.cpp
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Log.h"


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

void Log::Warning(const std::string& message, bool appendPrefix)
{
    if (appendPrefix)
        Message("warning: " + message);
    else
        Message(message);
}

void Log::Error(const std::string& message, bool appendPrefix)
{
    if (appendPrefix)
        Message("error: " + message);
    else
        Message(message);
}

void Log::FatalError(const std::string& message, bool appendPrefix)
{
    if (appendPrefix)
        Message("error: " + message);
    else
        Message(message);
}

void Log::Success(const std::string& message)
{
    Message(message);
}

void Log::Messages(const std::initializer_list<std::string>& messages)
{
    for (const auto& msg : messages)
        Message(msg);
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