/*
 * String modification header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_STRING_MODIFIER_H__
#define __XX_STRING_MODIFIER_H__


#include <string>
#include <sstream>


/* === Inline functions === */

inline std::string ToStr(const std::string& str)
{
    return str;
}

/* === Functions === */

std::string ToStr(char val);

std::string ToStr(short val);
std::string ToStr(unsigned short val);

std::string ToStr(int val);
std::string ToStr(unsigned int val);

std::string ToStr(std::size_t val);

std::string ToStr(float val);
std::string ToStr(double val);

std::string ToStr(float val, int precision);
std::string ToStr(double val, int precision);

//! Returns the filename of the specified full path (e.g. from "C:/Program Files/Programming/App.xx" to "App.xx").
std::string ExtractFilename(const std::string& filename);
//! Returns the full path of the specified filename (e.g. from "C:/Program Files/Programming/App.xx" to "C:/Program Files/Programming").
std::string ExtractFilePath(const std::string& filename);
//! Returns the file extension part (e.g. from "C:/Program Files/Programming/App.xx" to "xx").
std::string ExtractFileExtension(const std::string& filename);
//! Returns the identifier of the specified full path (e.g. from "C:/Program Files/Programming/App.xx" to "App").
std::string ExtractFileIdent(const std::string& filename);

//! Removes the file extension part (e.g. transform "C:/Program Files/Programming/App.xx" to "C:/Program Files/Programming/App").
void RemoveFileExtension(std::string& filename);

std::string ReplaceString(
    std::string subject, const std::string& search, const std::string& replace
);

bool IsWhiteSpace(char chr);
bool HasWhiteSpaces(const std::string& str);
std::string RemoveWhiteSpaces(std::string str);

std::string NumberOffset(
    size_t num, size_t maxNum, const char fillChar = ' ', const size_t base = 10
);

char GetLower(char chr);
char GetUpper(char chr);

void ToLower(char& chr);
void ToUpper(char& chr);

std::string ToLower(std::string str);
std::string ToUpper(std::string str);

//! Returns a rough measurement for the similarities of the two strings.
size_t StringSimilarities(const std::string& lhs, const std::string& rhs);

/* === Templates === */

template <typename T> T StrToNum(const std::string& str)
{
    T val = T(0);
    std::istringstream sstr(str);
    sstr >> val;
    return val;
}

template <typename T> std::string NumToHex(T number, const size_t size, const bool prefix = true)
{
    static const char* hexAlphabet = "0123456789abcdef";

    std::string str;
    
    if (prefix)
    {
        for (int i = 2*size - 1; i >= 0; --i)
        {
            str += ToStr(
                hexAlphabet[(number >> i*4) & 0xf]
            );
        }
    }
    else
    {
        do
        {
            str = ToStr(hexAlphabet[number & 0xf]) + str;
            number >>= 4;
        }
        while (number > 0);
    }

    return str;
}

template <typename T> std::string NumToHex(const T& number, const bool Prefix = true)
{
    return NumToHex(number, sizeof(T), Prefix);
}

template <typename T> std::string NumToOct(T number)
{
    std::string str;
    
    do
    {
        str = ToStr("01234567"[number & 0x7]) + str;
        number >>= 3;
    }
    while (number > 0);
    
    return str;
}

template <typename T> std::string NumToBin(T number)
{
    std::string str;
    
    do
    {
        str = ((number & 0x1) != 0 ? "1" : "0") + str;
        number >>= 1;
    }
    while (number > 0);
    
    return str;
}

template <typename T> T HexToNum(const std::string& str)
{
    T num = T(0);
    std::stringstream sstr;
    
    sstr << std::hex << str;
    sstr >> num;

    return num;
}

template <typename T> T OctToNum(const std::string& str)
{
    T num = T(0);

    auto it = str.begin();

    if (str.size() > 2 && str[0] == '0' && str[1] == 'c')
        it += 2;

    for (; it != str.end(); ++it)
    {
        num <<= 3;
        num += ((*it) - '0') & 0x7;
    }

    return num;
}

template <typename T> T BinToNum(const std::string& str)
{
    T num = T(0);

    auto it = str.begin();

    if (str.size() > 2 && str[0] == '0' && str[1] == 'b')
        it += 2;

    for (; it != str.end(); ++it)
    {
        num <<= 1;
        if (*it != '0')
            ++num;
    }

    return num;
}


#endif



// ================================================================================