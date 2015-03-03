/*
 * Error reporter file
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ErrorReporter.h"
#include "Log.h"
#include "StringModifier.h"


void ErrorReporter::Add(const CompilerMessage& message)
{
    messages_.push_back(message);
    if (message.IsError())
        hasErrors_ = true;
}

void ErrorReporter::Flush(Log& log, bool printMetaInfo)
{
    if (!messages_.empty())
    {
        if (printMetaInfo)
        {
            /* Print meta information */
            size_t numErrors = 0, numWarnings = 0;
        
            for (const auto& msg : messages_)
            {
                if (msg.IsError())
                    ++numErrors;
                else if (msg.IsWarning())
                    ++numWarnings;
            }

            auto info = ToStr(numErrors) + " error(s), " + ToStr(numWarnings) + " warning(s):";
            log.Message(info);
            log.Message(std::string(info.size(), '-'));
        }

        /* Print messages to log */
        for (const auto& msg : messages_)
            log.Message(msg);

        /* Reset error-bit state */
        hasErrors_ = false;
        messages_.clear();
    }
}



// ================================================================================