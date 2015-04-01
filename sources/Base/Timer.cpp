/*
 * Timer.cpp
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Timer.h"

#include <chrono>
#include <sstream>
#include <iomanip>


namespace Timer
{


std::string CurrentTime()
{
    auto timePoint = std::chrono::system_clock::now();
    auto localTime = std::chrono::system_clock::to_time_t(timePoint);
    std::stringstream sstr;
    sstr << std::put_time(std::localtime(&localTime), "%Y-%m-%d %X");
    return sstr.str();
}


} // /namespace Timer



// ================================================================================