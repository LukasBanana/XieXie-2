/*
 * ColorCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"
#include "ConsoleManip.h"


using namespace Platform;

void ColorCommand::Execute(StreamParser& input, Log& output)
{
    if (input.Get() == "-on")
    {
        input.Accept();
        ConsoleManip::Enable(true);
    }
    else if (input.Get() == "-off")
    {
        input.Accept();
        ConsoleManip::Enable(false);
    }
    else
        ConsoleManip::Enable(true);
}



// ================================================================================