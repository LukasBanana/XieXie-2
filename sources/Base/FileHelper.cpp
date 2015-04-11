/*
 * FileHelper.cpp
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "FileHelper.h"

#include <fstream>


namespace FileHelper
{


//! Returns true if the specified file does exist.
bool DoesFileExist(const std::string& filename)
{
    std::ifstream file(filename);
    return file.good();
}


} // /namespace FileHelper



// ================================================================================