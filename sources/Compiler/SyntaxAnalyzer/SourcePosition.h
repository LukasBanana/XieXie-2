/*
 * Source position header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2013 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SOURCE_POSITION_H__
#define __XX_SOURCE_POSITION_H__


#include <string>


namespace SyntacticAnalyzer
{


//! This class stores the position in a source code file.
class SourcePosition
{
    
    public:
        
        SourcePosition() = default;
        SourcePosition(unsigned int row, unsigned int column);

        //! Returns the source position as string in the format "Row:Column", e.g. "75:10".
        std::string ToString() const;

        void IncRow();
        void IncColumn();

        //! Returns ture if this is a valid source position. False if row and column are 0.
        bool IsValid() const;

        //! Returns the row of the source position, beginning with 1.
        inline unsigned int GetRow() const
        {
            return row_;
        }
        //! Returns the colummn of the source position, beginning with 1.
        inline unsigned int GetColumn() const
        {
            return column_;
        }

        //! Invalid source position.
        static const SourcePosition ignore;

    private:
        
        unsigned int row_ = 0, column_ = 0;

};


inline bool operator == (const SourcePosition& left, const SourcePosition& right)
{
    return left.GetRow() == right.GetRow() && left.GetColumn() == right.GetColumn();
}

inline bool operator != (const SourcePosition& left, const SourcePosition& right)
{
    return !(left == right);
}

inline bool operator < (const SourcePosition& left, const SourcePosition& right)
{
    return left.GetRow() < right.GetRow() || ( left.GetRow() == right.GetRow() && left.GetColumn() < right.GetColumn() );
}

inline bool operator > (const SourcePosition& left, const SourcePosition& right)
{
    return left.GetRow() > right.GetRow() || ( left.GetRow() == right.GetRow() && left.GetColumn() > right.GetColumn() );
}

inline bool operator <= (const SourcePosition& left, const SourcePosition& right)
{
    return left < right || left == right;
}

inline bool operator >= (const SourcePosition& left, const SourcePosition& right)
{
    return left > right || left == right;
}



} // /namespace SyntacticAnalyzer


#endif



// ================================================================================