/*
 * FileHelper.cpp
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "FileHelper.h"
#include "StringModifier.h"

#include <fstream>


namespace FileHelper
{


bool DoesFileExist(const std::string& filename)
{
    std::ifstream file(filename);
    return file.good();
}

std::string SelectOutputFilename(std::string inputFilename, const std::string& fileExt, Log& output, bool forceOverride)
{
    std::string fileId;
    auto OutFile = [&]()
    {
        return inputFilename + fileId + '.' + fileExt;
    };

    if (!forceOverride && DoesFileExist(OutFile()))
    {
        /* Ask user to override file */
        output.Warning("output file \"" + OutFile() + "\" already exists! override? (y/n)");

        char answer = 0;
        std::cin >> answer;

        if (answer != 'y' && answer != 'Y')
        {
            /* Find available filename */
            size_t i = 0;
            do
            {
                fileId = std::to_string(++i);
            }
            while (DoesFileExist(OutFile()));
        }
    }

    return OutFile();
}


} // /namespace FileHelper



// ================================================================================