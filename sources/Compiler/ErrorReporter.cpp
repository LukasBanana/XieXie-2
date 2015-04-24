/*
 * Error reporter file
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ErrorReporter.h"
#include "Log.h"
#include "StringModifier.h"


bool ErrorReporter::showWarnings = false;

void ErrorReporter::Add(const CompilerMessage& message)
{
    if (!message.IsWarning() || ErrorReporter::showWarnings)
    {
        messages_.push_back(message);
        if (message.IsError())
            ++numErrors_;
        if (message.IsWarning())
            ++numWarnings_;
    }
}

void ErrorReporter::Flush(Log& log, bool printMetaInfo)
{
    if (!messages_.empty())
    {
        if (printMetaInfo)
        {
            /* Print meta information */
            auto info = ToStr(numErrors_) + " error(s), " + ToStr(numWarnings_) + " warning(s):";
            log.Message(info);
            log.Message(std::string(info.size(), '-'));
        }

        /* Print messages to log */
        for (const auto& msg : messages_)
            log.Message(msg);

        /* Reset states */
        messages_.clear();
        numErrors_      = 0;
        numWarnings_    = 0;
    }
}

bool ErrorReporter::ExceededErrorLimit() const
{
    return numErrorLimit > 0 && (NumErrors() + 1) >= numErrorLimit;
}



// ================================================================================