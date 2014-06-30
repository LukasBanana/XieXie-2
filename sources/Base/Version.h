/*
 * Version header
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

std::string AsString();


}


#endif



// ================================================================================