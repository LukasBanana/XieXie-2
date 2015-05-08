/*
 * ConsoleManip.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_PLATFORM_CONSOLE_MANIP_H__
#define __XX_PLATFORM_CONSOLE_MANIP_H__


#include <iostream>


namespace Platform
{

namespace ConsoleManip
{


struct Color
{
    using ValueType = unsigned int;
    ValueType value;

    static const ValueType Red;
    static const ValueType Green;
    static const ValueType Blue;

    static const ValueType Intens;

    static const ValueType Black;
    static const ValueType Gray;
    static const ValueType White;

    static const ValueType Yellow;
    static const ValueType Pink;
    static const ValueType Cyan;
};


//! Enables or disables console manipulation. By default enabled.
void Enable(bool enable);
//! Returns true if console manipulation is enabled.
bool IsEnabled();

void PushColor(std::ostream& stream, const Color::ValueType& front);
void PushColor(std::ostream& stream, const Color::ValueType& front, const Color::ValueType& back);
void PopColor(std::ostream& stream);


class ScopedColor
{
    
    public:
    
        ScopedColor(std::ostream& stream, const Color::ValueType& front) :
            stream_( stream )
        {
            PushColor(stream_, front);
        }
        ScopedColor(std::ostream& stream, const Color::ValueType& front, const Color::ValueType& back) :
            stream_( stream )
        {
            PushColor(stream_, front, back);
        }
        ~ScopedColor()
        {
            PopColor(stream_);
        }
        
    private:
        
        std::ostream& stream_;
        
};


} // /namespace ConsoleManip

} // /namespace Platform


#endif



// ================================================================================
