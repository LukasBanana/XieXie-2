/*
 * SearchPaths.cpp
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "SearchPaths.h"
#include "StringModifier.h"


namespace SearchPaths
{


static std::string appPath, libraryPath, modulesPath;

void Setup(const std::string& appFilename)
{
    #if !defined(XIEXIE_RELEASE_VERSION) && !defined(__clang__)
    static const std::string libBasePath = "/../repository/";
    #else
    static const std::string libBasePath = "/../";
    #endif
    
    appPath = ExtractFilePath(appFilename) + libBasePath;

    libraryPath = appPath + "library/";
    modulesPath = appPath + "modules/";
}

const std::string& AppPath()
{
    return appPath;
}

std::string& LibraryPath()
{
    return libraryPath;
}

std::string& ModulesPath()
{
    return modulesPath;
}

void MakePathRelative(std::string& path)
{
    if (path.size() >= appPath.size() && appPath.compare(0u, appPath.size(), path, 0u, appPath.size()) == 0)
        path.erase(0, appPath.size());
}


} // /namespace SearchPaths



// ================================================================================