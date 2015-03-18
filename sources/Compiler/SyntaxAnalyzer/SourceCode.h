/*
 * Source code header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SOURCE_CODE_H__
#define __XX_SOURCE_CODE_H__


#include "DeclPtr.h"
#include "SourcePosition.h"
#include "SourceArea.h"

#include <string>


namespace SyntaxAnalyzer
{


DECL_SHR_PTR(SourceCode);

//! Interface for a source code.
class SourceCode
{
    
    public:
        
        virtual ~SourceCode()
        {
        }

        //! Returns the next character from the source.
        virtual char Next() = 0;

        /**
        Returns the line string for the specified line number.
        If the line number is out of range, the returned string is empty.
        */
        virtual std::string FetchLine(size_t lineNumber) const = 0;

        /**
        Fetches the line and a marker at the specified source area.
        \return True if the line could be fetched. Otherwise the area is invalid or too large.
        */
        virtual bool FetchLineMarker(
            const SourceArea& area, std::string& line, std::string& marker, char markerChar = '~'
        ) const = 0;

        //! Ignores the current character.
        inline void Ignore()
        {
            Next();
        }

        //! Returns the current source position.
        inline const SourcePosition& Pos() const
        {
            return pos_;
        }
        //! Returns the current source line.
        inline const std::string& Line() const
        {
            return line_;
        }

    protected:
        
        SourceCode() = default;

        std::string     line_;
        SourcePosition  pos_;

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================