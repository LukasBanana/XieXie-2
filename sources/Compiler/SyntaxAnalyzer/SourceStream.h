/*
 * SourceStream.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SOURCE_STREAM_H__
#define __XX_SOURCE_STREAM_H__


#include "SourceCode.h"

#include <iostream>
#include <vector>
#include <string>
#include <memory>


namespace SyntaxAnalyzer
{


class SourceStream : public SourceCode
{
    
    public:
        
        SourceStream() = default;
        SourceStream(const std::shared_ptr<std::istream>& stream);
        SourceStream(const std::string& filename);

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
        
        std::shared_ptr<std::istream>   stream_;
        std::string                     filename_;
        std::vector<std::string>        content_;

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================