/*
 * ConsoleManip.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ConsoleManip.h"


namespace Platform
{

namespace ConsoleManip
{


const Color::ValueType Color::Red       = (1 << 0);
const Color::ValueType Color::Green     = (1 << 1);
const Color::ValueType Color::Blue      = (1 << 2);

const Color::ValueType Color::Intens    = (1 << 3);

const Color::ValueType Color::Black     = 0;
const Color::ValueType Color::Gray      = Color::Red | Color::Green | Color::Blue;
const Color::ValueType Color::White     = Color::Gray | Color::Intens;

const Color::ValueType Color::Yellow    = Color::Red | Color::Green | Color::Intens;
const Color::ValueType Color::Pink      = Color::Red | Color::Blue | Color::Intens;
const Color::ValueType Color::Cyan      = Color::Green | Color::Blue | Color::Intens;


static bool isEnabled = true;

void Enable(bool enable)
{
    isEnabled = enable;
}

bool IsEnabled()
{
    return isEnabled;
}


} // /namespace ConsoleManip

} // /namespace Platform



// ================================================================================