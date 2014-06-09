/*
 * Shell file
 * 
 * This file is part of the "XieXie2-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Command.h"
#include "StringModifier.h"


Command::Command(const std::string& commandLine)
{
    /* Parse argument list from command line */
    size_t prevPos = 0, pos = 0;

    bool beginArg = false;
    bool beginQuote = false;

    for (size_t len = commandLine.size(); pos < len; ++pos)
    {
        const auto& chr = commandLine[pos];

        if (chr == '\"')
        {
            if (beginQuote)
            {
                /* Add new quoted argument */
                arguments_.push_back(commandLine.substr(prevPos, pos - prevPos));
                prevPos = pos;
                beginQuote = false;
            }
            else
            {
                beginQuote = true;
                prevPos = pos + 1;
            }
        }
        else if (!beginQuote)
        {
            if (chr == ' ')
            {
                if (beginArg)
                {
                    /* Add new argument */
                    arguments_.push_back(commandLine.substr(prevPos, pos - prevPos));
                    beginArg = false;
                }
            }
            else
            {
                if (!beginArg)
                    prevPos = pos;
                beginArg = true;
            }
        }
    }

    if (beginArg)
        arguments_.push_back(commandLine.substr(prevPos));
}
Command::Command(const char* commandLine) :
    Command(std::string(commandLine))
{
}
Command::Command(const Command&& other) :
    arguments_(std::move(other.arguments_))
{
}

std::string Command::Name() const
{
    return !arguments_.empty() ? arguments_.front() : "";
}

Command::operator std::string () const
{
    /* Concatenate all arguments and separate them with a space character */
    std::string commandLine;
    size_t num = arguments_.size();

    for (const auto& arg : arguments_)
    {
        if (HasWhiteSpaces(arg))
            commandLine += "\"" + arg + "\"";
        else
            commandLine += arg;

        if (num > 1)
        {
            commandLine += ' ';
            --num;
        }
    }

    return commandLine;
}



// ========================