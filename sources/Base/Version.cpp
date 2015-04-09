/*
 * Version.cpp
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Version.h"
#include "StringModifier.h"


namespace Version
{


int Major()
{
    return 2;
}

int Minor()
{
    return 0;
}

int Revision()
{
    return 0;
}

std::string Status()
{
    return "Alpha";
}

std::string AsString()
{
    std::string str;
    
    str += ToStr(Major());
    str += '.';
    str += NumberOffset(Minor(), 99, '0');

    if (Revision() != 0)
    {
        str += " Rev. ";
        str += ToStr(Revision());
    }

    if (!Status().empty())
    {
        str += ' ';
        str += Status();
    }

    return str;
}

int AsInt()
{
    return Major() * 100 + Minor();
}


} // /namespace Version



// ================================================================================