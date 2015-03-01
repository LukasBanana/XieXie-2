/*
 * Source file header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SOURCE_FILE_H__
#define __XX_SOURCE_FILE_H__


#include "SourceCode.h"

#include <fstream>


namespace SyntaxAnalyzer
{


//! This class stores the position in a source code file.
class SourceFile : public SourceCode
{
    
    public:
        
        SourceFile() = default;
        SourceFile(const std::string& filename);

        bool ReadFile(const std::string& filename);

        char Next();

        inline const std::string& Filename() const
        {
            return filename_;
        }

    private:
        
        std::ifstream   stream_;
        std::string     filename_;

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================