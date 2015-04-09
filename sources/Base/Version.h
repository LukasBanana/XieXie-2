/*
 * Version.h
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_VERSION_H__
#define __XX_VERSION_H__


#include <string>


namespace Version
{


int Major();
int Minor();
int Revision();
std::string Status();

//! Returns the compiler version as string, e.g. "1.00 Rev. 5 Beta".
std::string AsString();
//! Returns the compiler version as integer, e.g. 200 for version "2.00".
int AsInt();


} // /namespace Version


#endif



// ================================================================================