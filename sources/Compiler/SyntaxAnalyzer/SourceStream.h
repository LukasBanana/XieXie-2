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
        SourceStream(const std::shared_ptr<std::istream>& stream, const std::string& name = "");
        //! \see ReadFile
        SourceStream(const std::string& filename);

        /**
        Opens the specified file for reading and stores the relative path of the filename as the name for this stream.
        \see SearchPath::MakeRelativePath
        \see Name
        */
        bool ReadFile(const std::string& filename);
        void Close();

        char Next() override;

        std::string FetchLine(size_t lineNumber) const override;

        bool FetchLineMarker(
            const SourceArea& area, std::string& line, std::string& marker, char markerChar = '~'
        ) const override;

        const std::string& Name() const override;

    private:
        
        void StoreName(const std::string& name);

        std::shared_ptr<std::istream>   stream_;
        std::string                     name_;
        std::vector<std::string>        content_;

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================