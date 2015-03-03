/*
 * Shell.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SHELL_H__
#define __XX_SHELL_H__


#include "Log.h"
#include "StreamParser.h"

#include <istream>
#include <string>
#include <vector>


class Shell
{
    
    public:
        
        void Execute(StreamParser parser);

    private:
        
        Log log_;

};


#endif



// ================================================================================