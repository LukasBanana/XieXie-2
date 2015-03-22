/*
 * Win32ConsoleManip.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ConsoleManip.h"

#include <Windows.h>
#include <stack>


namespace Platform
{

namespace ConsoleManip
{


static std::stack<CONSOLE_SCREEN_BUFFER_INFO> scrBufferInfoStack;

static inline HANDLE StdOut()
{
    return GetStdHandle(STD_OUTPUT_HANDLE);
}

static void PushAttrib()
{
    /* Get current console screen buffer info */
    CONSOLE_SCREEN_BUFFER_INFO bufInfo;
	GetConsoleScreenBufferInfo(StdOut(), &bufInfo);

    /* Push buffer info onto stack */
    scrBufferInfoStack.push(bufInfo);
}

void PushColor(std::ostream&, const Color::ValueType& front)
{
    if (!IsEnabled())
        return;

    /* Push color attribute onto stack */
    PushAttrib();

    /* Get current console screen buffer infor */
    CONSOLE_SCREEN_BUFFER_INFO bufInfo;
	GetConsoleScreenBufferInfo(StdOut(), &bufInfo);

    /* Setup attributes for new console color */
    WORD attrib = (bufInfo.wAttributes & 0xFFF0);
    
    if (( front & Color::Red    ) != 0) attrib |= FOREGROUND_RED;
    if (( front & Color::Green  ) != 0) attrib |= FOREGROUND_GREEN;
    if (( front & Color::Blue   ) != 0) attrib |= FOREGROUND_BLUE;
    if (( front & Color::Intens ) != 0) attrib |= FOREGROUND_INTENSITY;
    
    /* Set new console attribute */
    SetConsoleTextAttribute(StdOut(), attrib);
}

void PushColor(std::ostream&, const Color::ValueType& front, const Color::ValueType& back)
{
    if (!IsEnabled())
        return;

    /* Push color attribute onto stack */
    PushAttrib();

    /* Setup attributes for new console color */
    WORD attrib = 0;

    if (( front & Color::Red    ) != 0) attrib |= FOREGROUND_RED;
    if (( front & Color::Green  ) != 0) attrib |= FOREGROUND_GREEN;
    if (( front & Color::Blue   ) != 0) attrib |= FOREGROUND_BLUE;
    if (( front & Color::Intens ) != 0) attrib |= FOREGROUND_INTENSITY;

    if (( back & Color::Red     ) != 0) attrib |= BACKGROUND_RED;
    if (( back & Color::Green   ) != 0) attrib |= BACKGROUND_GREEN;
    if (( back & Color::Blue    ) != 0) attrib |= BACKGROUND_BLUE;
    if (( back & Color::Intens  ) != 0) attrib |= BACKGROUND_INTENSITY;

    /* Set new console attribute */
    SetConsoleTextAttribute(StdOut(), attrib);
}

void PopColor(std::ostream&)
{
    if (!IsEnabled())
        return;

    if (!scrBufferInfoStack.empty())
    {
        /* Push buffer info onto stack */
        const CONSOLE_SCREEN_BUFFER_INFO BufInfo = scrBufferInfoStack.top();
        scrBufferInfoStack.pop();

        /* Reset previous console screen buffer info */
        SetConsoleTextAttribute(StdOut(), BufInfo.wAttributes);
    }
}


} // /namespace ConsoleManip

} // /namespace Platform



// ================================================================================