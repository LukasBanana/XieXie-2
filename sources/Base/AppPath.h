/*
 * AppPath.h
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_APP_PATH_H__
#define __XX_APP_PATH_H__


#include <string>


//! Application path singleton.
class AppPath
{
    
    public:
        
        AppPath() = delete;
        AppPath(const AppPath&) = delete;

        //! Returns the applicaiton path. This path can only be set by the main function.
        static const std::string& Get();

    private:
        
        friend int main(int argc, char* argv[]);

        static void Set(const std::string& path);

};


#endif



// ================================================================================