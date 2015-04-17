/*
 * CoreAssemblyGenerator.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <exception>
#include <algorithm>
#include <locale>
#include <cctype>
#include <functional>


static const std::string appName = "core assembly generator";
static const std::string infoPrefix = appName + ": ";

static void TrimBlanksLeft(std::string& s)
{
    s.erase(
        s.begin(),
        std::find_if(
            s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))
        )
    );
}

static void TrimBlanksRight(std::string& s)
{
    s.erase(
        std::find_if(
            s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))
        ).base(),
        s.end()
    );
}

static void TrimCommentRight(std::string& s)
{
    auto it = std::find_if(
        s.rbegin(), s.rend(),
        [](char c) { return c == ';'; }
    );
    if (it != s.rend())
    {
        s.erase(it.base(), s.end());
        s.pop_back();
    }
}

static void ResolveString(std::string& s)
{
    for (auto i = s.size(); i > 0;)
    {
        --i;
        if (s[i] == '\"')
            s.replace(i, 1, "\\\"");
    }
}

static void GenerateAssemblyIncludeFile(const std::string& assemblyInput, const std::string& assemblyOutput)
{
    /* Open input file for reading */
    std::ifstream input(assemblyInput);
    if (!input.good())
        throw std::runtime_error("reading file \"" + assemblyInput + "\" failed");

    /* Open output file for writing */
    std::ofstream output(assemblyOutput);
    if (!output.good())
        throw std::runtime_error("writing file \"" + assemblyOutput + "\" failed");

    while (!input.eof())
    {
        /* Read line */
        std::string line;
        std::getline(input, line);

        /* Parse line */
        TrimBlanksLeft(line);
        TrimCommentRight(line);
        TrimBlanksRight(line);
        ResolveString(line);

        if (line.empty())
            continue;

        /* Write line */
        output << '\"' << line << "\\n\"" << std::endl;
    }
}

int main(int argc, char* argv[])
{
    if (argc >= 2)
    {
        /* Get assembly I/O filenames */
        std::string assemblyInput = argv[1];
        std::string assemblyOutput = assemblyInput + ".h";

        if (argc >= 3)
            assemblyOutput = argv[2]; 

        /* Generate assembly include file */
        try
        {
            GenerateAssemblyIncludeFile(assemblyInput, assemblyOutput);
            std::cout << infoPrefix << "generation successful" << std::endl;
        }
        catch (const std::exception& err)
        {
            std::cerr << infoPrefix << err.what() << std::endl;
        }
    }
    else
        std::cerr << infoPrefix << "too few arguments" << std::endl;

    return 0;
}



// ================================================================================