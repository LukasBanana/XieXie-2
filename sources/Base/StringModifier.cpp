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
std::string ToStr(size_t val)
{
    return ToStrTmpl(val);
}

#ifdef __clang__
std::string ToStr(std::size_t val)
{
    return ToStrTmpl(val);
}
#endif

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
    for (size_t i = filename.size(); i > 0; --i)
    {
        auto chr = filename[i - 1];
        if (chr == '/' || chr == '\\')
            break;
        if (chr == '.')
            return filename.substr(i);
    }
    return "";
}

std::string ExtractFileIdent(const std::string& filename)
{
    /* Return file identifier only */
    auto filePart = ExtractFilename(filename);
    auto extPart = ExtractFileExtension(filename);

    if (!extPart.empty())
    {
        if (extPart.size() + 1 < filePart.size())
            filePart.resize(filePart.size() - extPart.size() - 1);
        return filePart;
    }

    return filePart;
}

void RemoveFileExtension(std::string& filename)
{
    /* Remove file extension */
    for (size_t i = filename.size(); i > 0; --i)
    {
        auto chr = filename[i - 1];
        if (chr == '/' || chr == '\\')
            break;
        if (chr == '.')
        {
            filename.resize(i - 1);
            break;
        }
    }
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

char GetLower(char chr)
{
    ToLower(chr);
    return chr;
}

char GetUpper(char chr)
{
    ToUpper(chr);
    return chr;
}

void ToLower(char& chr)
{
    if (chr >= 'A' && chr <= 'Z')
        chr += ('a' - 'A');
}

void ToUpper(char& chr)
{
    if (chr >= 'a' && chr <= 'z')
        chr -= ('a' - 'A');
}

std::string ToLower(std::string str)
{
    for (char& chr : str)
        ToLower(chr);
    return str;
}

std::string ToUpper(std::string str)
{
    for (char& chr : str)
        ToUpper(chr);
    return str;
}

size_t StringSimilarities(const std::string& lhs, const std::string& rhs)
{
    /* Check if sizes don't diverge too much */
    auto lhsSize = lhs.size();
    auto rhsSize = rhs.size();

    auto maxSizeDiff = std::max(std::size_t(1u), lhsSize/3);

    if (lhsSize > rhsSize + maxSizeDiff || lhsSize + maxSizeDiff < rhsSize)
        return 0;

    /* Check if characters don't diverge too much */
    auto n = std::min(lhsSize, rhsSize);
    auto m = n;

    auto dec = [&m, &n]() -> bool
    {
        return (--m <= n*2/3);
    };

    for (size_t i = 0; i < n; ++i)
    {
        /* Compare the current characters */
        if (lhs[i] != rhs[i])
        {
            if (GetLower(lhs[i]) != GetLower(rhs[i]) && dec())
                return 0;
        }
    }

    return m;
}



// ================================================================================
