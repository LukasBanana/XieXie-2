/*
 * StreamParser.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "StreamParser.h"

#include <algorithm>


StreamParser::StreamParser(std::istream& stream) :
    // bracket initializer required for GCC (due to bug 56032)
    stream_( stream )
{
    Accept();
}

std::string StreamParser::Accept()
{
    /* Parse next token */
    auto prev = tkn_;
    stream_ >> tkn_;
    return prev;
}

bool StreamParser::AcceptOption(bool& option, const std::string& flag)
{
    if (!option && Get() == flag)
    {
        Accept();
        option = true;
        return true;
    }
    return false;
}

bool StreamParser::AcceptOption(bool& option, const std::initializer_list<const char*>& flags)
{
    if (!option)
    {
        if (std::find(flags.begin(), flags.end(), std::string(Get())) != flags.end())
        {
            Accept();
            option = true;
            return true;
        }
    }
    return false;
}

StreamParser::operator bool () const
{
    return !stream_.eof();
}



// ================================================================================
