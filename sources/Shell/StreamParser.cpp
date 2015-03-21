/*
 * StreamParser.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "StreamParser.h"


StreamParser::StreamParser(std::istream& stream) :
    // bracket initializer required for GCC (due to bug 56032)
    stream_(stream)
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

StreamParser::operator bool () const
{
    return !stream_.eof();
}



// ================================================================================
