/*
 * ManCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"
#include "Keywords.h"
#include "ConsoleManip.h"
#include "MakeUnique.h"

#include <set>
#include <algorithm>


using namespace Platform::ConsoleManip;
using Tokens = SyntaxAnalyzer::Token::Types;

static const char* KeywordHint(const Tokens type)
{
    switch (type)
    {
        case Tokens::Macro:
            return "macro";
        case Tokens::Reserved:
            return "reserved";
    }
    return nullptr;
}

static void PrintKeywordInfo(Log& output, const std::pair<std::string, Tokens>& key, size_t maxLen)
{
    static const size_t minSepCount = 3;

    /* Get final keyword info line */
    std::string info = key.first;

    auto hint = KeywordHint(key.second);
    if (hint)
        info += std::string(maxLen - info.size() + minSepCount, ' ') + std::string(hint);

    /* Push scoped color */
    std::unique_ptr<ScopedColor> color;

    switch (key.second)
    {
        case Tokens::Macro:
            color = MakeUnique<ScopedColor>(output.stream, Color::Green | Color::Intens);
            break;
        case Tokens::Reserved:
            color = MakeUnique<ScopedColor>(output.stream, Color::Red | Color::Intens);
            break;
    }

    /* Print message */
    output.Message(info);
}

static void PrintKeywords(Log& output)
{
    /* Find longest keyword */
    size_t maxLen = 0;
    for (const auto& key : SyntaxAnalyzer::Keywords())
        maxLen = std::max(maxLen, key.first.size());

    /* Print keywords */
    output.Headline("keywords:");

    for (const auto& key : SyntaxAnalyzer::Keywords())
        PrintKeywordInfo(output, key, maxLen);
}

void ManCommand::Execute(StreamParser& input, Log& output)
{
    while (true)
    {
        if (input.Get() == "-K" || input.Get() == "--keywords")
        {
            input.Accept();
            PrintKeywords(output);
        }
        else
            break;
    }
}

void ManCommand::Help(HelpPrinter& printer) const
{
    printer.Command("man [OPT+]", "prints the compiler's manual pages");
    printer.Flag("-K, --keywords", "prints the list of all keywords");
}



// ================================================================================