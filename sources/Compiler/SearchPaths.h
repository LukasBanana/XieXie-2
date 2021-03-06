/*
 * SearchPaths.h
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SEARCH_PATHS_H__
#define __XX_SEARCH_PATHS_H__


#include <string>


namespace SearchPaths
{


/**
Sets the "library/" and "moudles/" paths by the application filename,
e.g. "C:/Program Files/XieXie/bin/xxc.exe" or "/home/userXY/XieXie/bin/xxc".
\remarks This functions expects that the respective paths are under "../",
i.e. if appFilename is "XieXie/bin/xxc", then the library path will be "XieXie/library/"
and the modules path will be "XieXie/modules/".
\see LibraryPath
\see ModulesPath
*/
void Setup(const std::string& appFilename);

//! Returns the full application path.
const std::string& AppPath();

//! Returns a reference to the "library/" path.
std::string& LibraryPath();

//! Returns a reference to the "modules/" path.
std::string& ModulesPath();

/**
Makes the specified path relative to the application path (see AppPath).
\param[in,out] path Specifies the path which is to be modified. This can also be a filename.
\see AppPath
*/
void MakePathRelative(std::string& path);


} // /namespace SearchPaths


#endif



// ================================================================================