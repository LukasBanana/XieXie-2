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
#include <vector>
#include <string>


namespace SyntaxAnalyzer
{


//! This class stores the position in a source code file.
class SourceFile : public SourceCode
{
    
    public:
        
        SourceFile() = default;
        SourceFile(const std::string& filename);

        bool ReadFile(const std::string& filename);
        void Close();

        char Next() override;

        std::string FetchLine(size_t lineNumber) const override;

        bool FetchLineMarker(
            const SourceArea& area, std::string& line, std::string& marker, char markerChar = '~'
        ) const override;

        inline const std::string& Filename() const
        {
            return filename_;
        }

    private:
        
        std::ifstream               stream_;
        std::string                 filename_;
        std::vector<std::string>    content_;

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================