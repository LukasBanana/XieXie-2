/*
 * StreamParser.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SHELL_STREAM_PARSER_H__
#define __XX_SHELL_STREAM_PARSER_H__


#include <string>
#include <istream>
#include <initializer_list>


class StreamParser
{
    
    public:
        
        StreamParser(std::istream& stream);

        std::string Accept();

        bool AcceptOption(bool& option, const std::string& flag);
        bool AcceptOption(bool& option, const std::initializer_list<const char*>& flags);

        operator bool () const;

        inline const std::string& Get() const
        {
            return tkn_;
        }

    private:
        
        std::istream&   stream_;
        std::string     tkn_;

};


#endif



// ================================================================================