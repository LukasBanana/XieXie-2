/*
 * MakeUnique.h
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_MAKE_UNIQUE_H__
#define __XX_MAKE_UNIQUE_H__


#include <memory>


/**
Work-around function template for the C++14 only feature "std::make_unique".
Required for GCC, optional for MSVC.
*/
template <typename T, typename... Args> std::unique_ptr<T> MakeUnique(Args&&... args)
{
    #ifdef _MSC_VER
    return std::make_unique<T>(std::forward<Args>(args)...);
    #else
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    #endif
}


#endif



// ================================================================================