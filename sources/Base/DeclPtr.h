/*
 * Declare shared pointer header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_DECL_PTR_H__
#define __XX_DECL_PTR_H__


#include <memory>

#define DeclPtr(n)                                  \
    class n;                                        \
    typedef std::shared_ptr<n> n##Ptr;              \
    typedef std::shared_ptr<const n> n##ConstPtr;


#endif



// ========================