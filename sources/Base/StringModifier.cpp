/*
 * String modification file
 * 
 * This file is part of the "XieXie2-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "StringModifier.h"

#include <sstream>
#include <algorithm>
#include <limits>


/* === Templates === */

template <typename T> inline std::string ToStrTmpl(const T &Val)
{
    std::stringstream SStr;
    SStr << Val;
    return SStr.str();
}

template <typename T> inline std::string ToStrTmplPrec(const T &Val)
{
    std::stringstream SStr;
    SStr.precision(std::numeric_limits<T>::digits10);
    SStr << Val;
    return SStr.str();
}


/* === Functions === */

std::string ToStr(char Val)
{
    return std::string(1, Val);
}

std::string ToStr(short Val)
{
    return ToStrTmpl(Val);
}
std::string ToStr(unsigned short Val)
{
    return ToStrTmpl(Val);
}

std::string ToStr(int Val)
{
    return ToStrTmpl(Val);
}
std::string ToStr(unsigned int Val)
{
    return ToStrTmpl(Val);
}

std::string ToStr(long long int Val)
{
    return ToStrTmpl(Val);
}
std::string ToStr(unsigned long long int Val)
{
    return ToStrTmpl(Val);
}

std::string ToStr(float Val)
{
    return ToStrTmplPrec(Val);
}
std::string ToStr(double Val)
{
    return ToStrTmplPrec(Val);
}
std::string ToStr(long double Val)
{
    return ToStrTmplPrec(Val);
}

std::string ExtractFilename(const std::string& Filename)
{
    /* Find position with filename only */
    size_t Pos = Filename.find_last_of("/\\");

    if (Pos == std::string::npos)
        return Filename;

    /* Return filename only */
    return Filename.substr(Pos + 1);
}

std::string ExtractFilePath(const std::string& Filename)
{
    /* Return file path only */
    return Filename.substr(0, Filename.find_last_of("\\/"));
}

std::string ExtractFileExtension(const std::string& Filename)
{
    /* Return file extension only */
    auto Pos = Filename.find_last_of('.');

    if (Pos == std::string::npos)
        return Filename;

    return Filename.substr(Pos + 1, Filename.size() - Pos - 1);
}

std::string ReplaceString(
    std::string Subject, const std::string& Search, const std::string& Replace)
{
    size_t Pos = 0;

    while ( ( Pos = Subject.find(Search, Pos) ) != std::string::npos )
    {
        Subject.replace(Pos, Search.size(), Replace);
        Pos += Replace.size();
    }

    return Subject;
}

std::string RemoveWhiteSpaces(std::string Str)
{
    auto it = std::remove_if(
        Str.begin(), Str.end(),
        [](char Chr) { return Chr == ' ' || Chr == '\t'; }
    );
        
    Str.erase(it, Str.end());

    return Str;
}

std::string NumberOffset(
    size_t Num, size_t MaxNum, const char FillChar, const size_t Base)
{
    if (Num > MaxNum)
        return ToStr(Num);

    const size_t NumOrig = Num;

    /* Find number of numerics */
    size_t MaxNumerics = 0, Numerics = 0;

    while (MaxNum >= Base)
    {
        MaxNum /= Base;
        ++MaxNumerics;
    }

    while (Num >= Base)
    {
        Num /= Base;
        ++Numerics;
    }

    /* Return string with offset and number */
    return
        std::string(MaxNumerics - Numerics, FillChar) +
        (Base > 10 ? NumToHex(NumOrig, false) : ToStr(NumOrig));
}

std::string ToLower(std::string Str)
{
    for (char& Chr : Str)
    {
        if (Chr >= 'A' && Chr <= 'Z')
            Chr += 'a' - 'A';
    }
    return Str;
}

std::string ToUpper(std::string Str)
{
    for (char& Chr : Str)
    {
        if (Chr >= 'a' && Chr <= 'z')
            Chr -= 'a' - 'A';
    }
    return Str;
}



// ================================================================================