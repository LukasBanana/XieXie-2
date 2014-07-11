/*
 * Error reporter file
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ErrorReporter.h"
#include "Console.h"


void ErrorReporter::Add(const CompilerMessage& message)
{
    messages_.push_back(message);
    if (message.IsError())
        hasErrors_ = true;
}

void ErrorReporter::Flush()
{
    if (!messages_.empty())
    {
        for (const auto& msg : messages_)
            Console::Message(msg.Message());

        hasErrors_ = false;
        messages_.clear();
    }
}



// ================================================================================