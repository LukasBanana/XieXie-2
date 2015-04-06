/*
 * String modification file
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "StringModifier.h"

#include <sstream>
#include <algorithm>
#include <limits>


/* === Templates === */

template <typename T> inline std::string ToStrTmpl(const T& val)
{
    std::stringstream sstr;
    sstr << val;
    return sstr.str();
}

template <typename T> inline std::string ToStrTmplPrec(const T& val)
{
    std::stringstream sstr;
    sstr.precision(std::numeric_limits<T>::digits10);
    sstr << val;
    return sstr.str();
}

template <typename T> inline std::string ToStrTmplPrec(const T& val, int precision)
{
    std::stringstream sstr;
    sstr.precision(precision);
    sstr << std::fixed << val;
    return sstr.str();
}

/* === Functions === */

std::string ToStr(char val)
{
    return std::string(1, val);
}

std::string ToStr(short val)
{
    return ToStrTmpl(val);
}
std::string ToStr(unsigned short val)
{
    return ToStrTmpl(val);
}

std::string ToStr(int val)
{
    return ToStrTmpl(val);
}
std::string ToStr(unsigned int val)
{
    return ToStrTmpl(val);
}

std::string ToStr(float val)
{
    return ToStrTmplPrec(val);
}
std::string ToStr(double val)
{
    return ToStrTmplPrec(val);
}

std::string ToStr(float val, int precision)
{
    return ToStrTmplPrec(val, precision);
}
std::string ToStr(double val, int precision)
{
    return ToStrTmplPrec(val, precision);
}

std::string ExtractFilename(const std::string& filename)
{
    /* Find position with filename only */
    auto pos = filename.find_last_of("/\\");

    if (pos == std::string::npos)
        return filename;

    /* Return filename only */
    return filename.substr(pos + 1);
}

std::string ExtractFilePath(const std::string& filename)
{
    /* Return file path only */
    auto pos = filename.find_last_of("\\/");
    return pos != std::string::npos ? filename.substr(0, pos) : ".";
}

std::string ExtractFileExtension(const std::string& filename)
{
    /* Return file extension only */
    auto pos = filename.find_last_of('.');

    if (pos == std::string::npos)
        return filename;

    return filename.substr(pos + 1, filename.size() - pos - 1);
}

std::string ReplaceString(
    std::string subject, const std::string& search, const std::string& replace)
{
    size_t pos = 0;

    while ( ( pos = subject.find(search, pos) ) != std::string::npos )
    {
        subject.replace(pos, search.size(), replace);
        pos += replace.size();
    }

    return subject;
}

bool IsWhiteSpace(char chr)
{
    return chr == ' ' || chr == '\t' || chr == '\r' || chr == '\n';
}

bool HasWhiteSpaces(const std::string& str)
{
    for (const auto& chr : str)
    {
        if (IsWhiteSpace(chr))
            return true;
    }
    return false;
}

std::string RemoveWhiteSpaces(std::string str)
{
    auto it = std::remove_if(
        str.begin(), str.end(),
        [](char chr)
        {
            return chr == ' ' || chr == '\t';
        }
    );
        
    str.erase(it, str.end());

    return str;
}

std::string NumberOffset(
    size_t num, size_t maxNum, const char fillChar, const size_t base)
{
    if (num > maxNum)
        return ToStr(num);

    const size_t numOrig = num;

    /* Find number of numerics */
    size_t maxNumerics = 0, numerics = 0;

    while (maxNum >= base)
    {
        maxNum /= base;
        ++maxNumerics;
    }

    while (num >= base)
    {
        num /= base;
        ++numerics;
    }

    /* Return string with offset and number */
    return
        std::string(maxNumerics - numerics, fillChar) +
        (base > 10 ? NumToHex(numOrig, false) : ToStr(numOrig));
}

std::string ToLower(std::string str)
{
    for (char& chr : str)
    {
        if (chr >= 'A' && chr <= 'Z')
            chr += ('a' - 'A');
    }
    return str;
}

std::string ToUpper(std::string str)
{
    for (char& chr : str)
    {
        if (chr >= 'a' && chr <= 'z')
            chr -= ('a' - 'A');
    }
    return str;
}



// ================================================================================