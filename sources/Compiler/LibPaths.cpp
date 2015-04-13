/*
 * LibPaths.cpp
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "LibPaths.h"
#include "StringModifier.h"


namespace LibPaths
{


static std::string libraryPath, modulesPath;

void Setup(const std::string& appPath)
{
    #ifndef XIEXIE_RELEASE_VERSION
    static const std::string libBasePath = "/../repository/";
    #else
    static const std::string libBasePath = "/../";
    #endif
    
    auto path = ExtractFilePath(appPath) + libBasePath;

    libraryPath = path + "library/";
    modulesPath = path + "modules/";
}

std::string& LibraryPath()
{
    return libraryPath;
}

std::string& ModulesPath()
{
    return modulesPath;
}


} // /namespace LibPaths



// ================================================================================