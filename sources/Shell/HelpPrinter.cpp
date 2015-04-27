/*
 * HelpPrinter.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "HelpPrinter.h"

#include <algorithm>


/*
 * Help structure
 */

void HelpPrinter::Help::MaxLength(size_t& maxLen, size_t offset) const
{
    maxLen = std::max(maxLen, grammar.size() + offset);
}

void HelpPrinter::Help::Print(Log& log, char sep, size_t maxLen, size_t offset) const
{
    auto reduce = std::min(maxLen, grammar.size() + offset);
    log.Message(
        std::string(offset, ' ') + grammar + ' ' +
        std::string(maxLen - reduce, sep) + ' ' + desc
    );
}


/*
 * CommandHelp structure
 */

void HelpPrinter::CommandHelp::MaxLength(size_t& maxLen, size_t flagOffset) const
{
    commandInfo.MaxLength(maxLen);
    for (const auto& flag : flagsInfos)
        flag.MaxLength(maxLen, flagOffset);
}

void HelpPrinter::CommandHelp::Print(Log& log, char sep, size_t maxLen, size_t flagOffset) const
{
    commandInfo.Print(log, sep, maxLen);
    for (const auto& flag : flagsInfos)
        flag.Print(log, sep, maxLen, flagOffset);
}


/*
 * HelpPrinter class
 */

void HelpPrinter::Command(const std::string& grammar, const std::string& desc)
{
    CommandHelp help;
    {
        help.commandInfo = { grammar, desc };
    }
    commandHelps_.push_back(help);
}

void HelpPrinter::Flag(const std::string& grammar, const std::string& desc)
{
    if (!commandHelps_.empty())
        commandHelps_.back().flagsInfos.push_back({ grammar, desc });
    else
        throw std::runtime_error("now command added to the help printer");
}

void HelpPrinter::Flags(const std::vector<std::pair<std::string, std::string>>& flags)
{
    for (const auto& flag : flags)
        Flag(flag.first, flag.second);
}

void HelpPrinter::Flush(Log& log, char sep)
{
    static const size_t flagOffset = 2;
    static const size_t minSep = 3;

    /* Determine longest offset */
    size_t maxLen = 0;

    for (const auto& help : commandHelps_)
        help.MaxLength(maxLen, flagOffset);

    maxLen += minSep;

    /* Print help document */
    for (size_t i = 0, n = commandHelps_.size(); i < n; ++i)
    {
        const auto& help = commandHelps_[i];
        help.Print(log, sep, maxLen, flagOffset);
        if (i + 1 < n)
            log.Blank();
    }

    /* Clear command help list */
    commandHelps_.clear();
}



// ================================================================================
