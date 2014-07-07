/*
 * Export header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2013 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_EXPORT_H__
#define __XX_EXPORT_H__


#ifdef _MSC_VER
#   define XX_EXPORT __declspec(dllexport)
#else
#   define XX_EXPORT
#endif


#endif



// ========================