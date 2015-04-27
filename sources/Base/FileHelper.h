/*
 * FileHelper.h
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_FILE_HELPER_H__
#define __XX_FILE_HELPER_H__


#include "Log.h"

#include <string>


namespace FileHelper
{


//! Returns true if the specified file does exist.
bool DoesFileExist(const std::string& filename);

/**
Selects an output filename for the specified input filename.
\param[in] inputFilename Specifies the input filename, e.g. "Main.xx".
\param[in] fileExt Specifies the file extension for the output filename, e.g. "xasm".
\param[in] forceOverride Specifies whether to force a file override or not. By default false.
\return The new output filename, e.g. "Main.xasm".
*/
std::string SelectOutputFilename(
    std::string inputFilename, const std::string& fileExt,
    Log& output, bool forceOverride = false
);


} // /namespace FileHelper


#endif



// ================================================================================