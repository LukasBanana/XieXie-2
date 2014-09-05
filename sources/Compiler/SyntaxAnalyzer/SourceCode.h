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

#include <string>


namespace SyntaxAnalyzer
{


DeclPtr(SourceCode);

//! Interface for a source code.
class SourceCode
{
    
    public:
        
        virtual ~SourceCode()
        {
        }

        //! Returns the next character from the source.
        virtual char Next() = 0;

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