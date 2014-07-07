/*
 * Source area header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2013 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SOURCE_AREA_H__
#define __XX_SOURCE_AREA_H__


#include "SourcePosition.h"


namespace SyntaxAnalyzer
{


/**
This class stores a start- and an end source position.
\see SourcePosition
*/
class SourceArea
{
    
    public:
        
        SourceArea() = default;
        SourceArea(const SourcePosition& pos);
        SourceArea(const SourcePosition& startPos, const SourcePosition& endPos);

        /**
        Returns the source area as string in the format "StartRow:StartColumn - EndRow:EndColumn",
        e.g. "75:10 - 75:15" or in the format "Row:Column" if start- and end position are equal.
        */
        std::string ToString() const;

        //! Returns ture if this is a valid source position. False if start-position > end-position.
        bool IsValid() const;

        //! Returns true if this area spreads over multiple lines.
        bool IsMultiLine() const;

        //! Start- and end source positions.
        SourcePosition start, end;

        //! Invalid source area.
        static const SourceArea ignore;

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================