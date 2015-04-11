/*
 * FileHelper.h
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_FILE_HELPER_H__
#define __XX_FILE_HELPER_H__


#include <string>


namespace FileHelper
{


//! Returns true if the specified file does exist.
bool DoesFileExist(const std::string& filename);


} // /namespace FileHelper


#endif



// ================================================================================