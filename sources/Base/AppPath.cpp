/*
 * AppPath.cpp
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "AppPath.h"


static std::string appPath;

void AppPath::Set(const std::string& path)
{
    appPath = path;
}

const std::string& AppPath::Get()
{
    return appPath;
}



// ================================================================================