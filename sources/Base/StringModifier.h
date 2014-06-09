/*
 * String modification header
 * 
 * This file is part of the "XieXie2-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_STRING_MODIFIER_H__
#define __XX_STRING_MODIFIER_H__


#include <string>
#include <sstream>


/* === Inline functions === */

inline std::string ToStr(const std::string &Str)
{
    return Str;
}

/* === Functions === */

std::string ToStr(char Val);

std::string ToStr(short Val);
std::string ToStr(unsigned short Val);

std::string ToStr(int Val);
std::string ToStr(unsigned int Val);

std::string ToStr(long long int Val);
std::string ToStr(unsigned long long int Val);

std::string ToStr(float Val);
std::string ToStr(double Val);
std::string ToStr(long double Val);

//! Returns the filename of the specified full path (e.g. from "C:/Program Files/Programming/App.xx" to "App.xx").
std::string ExtractFilename(const std::string& Filename);
//! Returns the full path of the specified filename (e.g. from "C:/Program Files/Programming/App.xx" to "C:/Program Files/Programming").
std::string ExtractFilePath(const std::string& Filename);
//! Returns the file extension part (e.g. from "C:/Program Files/Programming/App.xx" to "xx").
std::string ExtractFileExtension(const std::string& Filename);

std::string ReplaceString(
    std::string Subject, const std::string& Search, const std::string& Replace
);

std::string RemoveWhiteSpaces(std::string Str);

std::string NumberOffset(
    size_t Num, size_t MaxNum, const char FillChar = ' ', const size_t Base = 10
);

std::string ToLower(std::string Str);
std::string ToUpper(std::string Str);

/* === Templates === */

template <typename T> T StrToNum(const std::string& Str)
{
    T Val = T(0);
    std::istringstream SStr(Str);
    SStr >> Val;
    return Val;
}

template <typename T> std::string NumToHex(T Number, const size_t Size, const bool Prefix = true)
{
    static const char* HexAlphabet = "0123456789abcdef";

    std::string Str;
    
    if (Prefix)
    {
        for (int i = 2*Size - 1; i >= 0; --i)
        {
            Str += ToStr(
                HexAlphabet[(Number >> i*4) & 0xF]
            );
        }
    }
    else
    {
        do
        {
            Str = ToStr(HexAlphabet[Number & 0xF]) + Str;
            Number >>= 4;
        }
        while (Number > 0);
    }

    return Str;
}

template <typename T> std::string NumToHex(const T& Number, const bool Prefix = true)
{
    return NumToHex(Number, sizeof(T), Prefix);
}

template <typename T> std::string NumToOct(T Number)
{
    std::string Str;
    
    do
    {
        Str = ToStr("01234567"[Number & 0x7]) + Str;
        Number >>= 3;
    }
    while (Number > 0);
    
    return Str;
}

template <typename T> std::string NumToBin(T Number)
{
    std::string Str;
    
    do
    {
        Str = ((Number & 0x1) != 0 ? "1" : "0") + Str;
        Number >>= 1;
    }
    while (Number > 0);
    
    return Str;
}

template <typename T> T HexToNum(const std::string& Str)
{
    T Num = T(0);
    std::stringstream SStr;
    
    SStr << std::hex << Str;
    SStr >> Num;

    return Num;
}

template <typename T> T OctToNum(const std::string& Str)
{
    T Num = T(0);

    auto it = Str.begin();

    if (Str.size() > 2 && Str[0] == '0' && Str[1] == 'c')
        it += 2;

    for (; it != Str.end(); ++it)
    {
        Num <<= 3;
        Num += ((*it) - '0') & 0x7;
    }

    return Num;
}

template <typename T> T BinToNum(const std::string& Str)
{
    T Num = T(0);

    auto it = Str.begin();

    if (Str.size() > 2 && Str[0] == '0' && Str[1] == 'b')
        it += 2;

    for (; it != Str.end(); ++it)
    {
        Num <<= 1;
        if (*it != '0')
            ++Num;
    }

    return Num;
}


#endif



// ================================================================================