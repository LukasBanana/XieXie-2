/*
 * LibPaths.h
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_LIB_PATHS_H__
#define __XX_LIB_PATHS_H__


#include <string>


namespace LibPaths
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

//! Returns a reference to the "library/" path.
std::string& LibraryPath();

//! Returns a reference to the "modules/" path.
std::string& ModulesPath();


} // /namespace LibPaths


#endif



// ================================================================================