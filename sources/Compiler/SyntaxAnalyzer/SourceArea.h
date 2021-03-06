/*
 * Source area header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SOURCE_AREA_H__
#define __XX_SOURCE_AREA_H__


#include "SourcePosition.h"

#include <memory>


namespace SyntaxAnalyzer
{


class SourceCode;

/**
This class stores a start- and an end source position.
\see SourcePosition
*/
class SourceArea
{
    
    public:
        
        /* === Functions === */

        SourceArea() = default;
        explicit SourceArea(const SourcePosition& pos, const std::shared_ptr<const SourceCode>& source = nullptr);
        SourceArea(const SourcePosition& startPos, const SourcePosition& endPos, const std::shared_ptr<const SourceCode>& source = nullptr);
        SourceArea(const SourceArea& rhs, const std::shared_ptr<const SourceCode>& source = nullptr);

        /**
        Returns the source area as string in the format "StartRow:StartColumn - EndRow:EndColumn",
        e.g. "75:10 - 75:15" or in the format "Row:Column" if start- and end position are equal.
        */
        std::string ToString() const;

        //! Increases the rows of the start and end position by 1 and sets their columns to 0.
        void IncRow();
        //! Increases the columns of the start and end position by 1.
        void IncColumn();

        //! Returns ture if this is a valid source position. False if start-position >= end-position.
        bool IsValid() const;

        //! Reste the start and end positions to (0:0).
        void Reset();

        //! Returns true if this area spreads over multiple lines.
        bool IsMultiLine() const;

        //! Returns a constant pointer to the source code.
        const SourceCode* GetSource() const;

        /* === Members === */

        //! Start- and end source positions.
        SourcePosition start, end;

        //! Invalid source area.
        static const SourceArea ignore;

    private:
        
        std::shared_ptr<const SourceCode> source_ = nullptr;

};


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================