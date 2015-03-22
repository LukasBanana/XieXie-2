/*
 * LinuxConsoleManip.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ConsoleManip.h"

#include <stack>


namespace Platform
{

namespace ConsoleManip
{


/* 
 * Internal classes
 */

class IOModifier
{

    public:
    
        struct Codes
        {
            enum
            {
                Red         = 1,
                Green       = 2,
                Blue        = 4,
                
                Foreground  = 30,
                Background  = 40,
                Bright      = 60,
            };
        };
        
        IOModifier() = default;
        IOModifier(int code) :
            codeFg_{ code }
        {
        }
        IOModifier(int codeFg, int codeBg) :
            codeFg_{ codeFg },
            codeBg_{ codeBg }
        {
        }
        
        int CodeFg() const
        {
            return codeFg_;
        }
        int CodeBg() const
        {
            return codeBg_;
        }

    private:
        
        int codeFg_ = 0;
        int codeBg_ = 0;
        
};

std::ostream& operator << (std::ostream& os, const IOModifier& mod)
{
    os << "\x1b[";
    if (mod.CodeFg() && mod.CodeBg())
        os << mod.CodeFg() << ";" << mod.CodeBg();
    else if (mod.CodeFg())
        os << mod.CodeFg();
    else if (mod.CodeBg())
        os << mod.CodeBg();
    return os << "m";
}

static std::stack<IOModifier> modifierStack;

static int GetModCode(const Color::ValueType& color, bool fg)
{
    using Cd = IOModifier::Codes;
    
    int code = 0;
    
    if ((color & Color::Red) != 0)
        code += Cd::Red;
    if ((color & Color::Green) != 0)
        code += Cd::Green;
    if ((color & Color::Blue) != 0)
        code += Cd::Blue;
    if ((color & Color::Intens) != 0)
        code += Cd::Bright;
    
    code += (fg ? Cd::Foreground : Cd::Background);
    
    return code;
}


/*
 * Interface implementation
 */

void PushColor(std::ostream& stream, const Color::ValueType& front)
{
    if (IsEnabled())
    {
        auto mod = IOModifier(GetModCode(front, true));
        modifierStack.push(mod);
        stream << mod;
    }
}

void PushColor(std::ostream& stream, const Color::ValueType& front, const Color::ValueType& back)
{
    if (IsEnabled())
    {
        auto mod = IOModifier(GetModCode(front, true), GetModCode(front, false));
        modifierStack.push(mod);
        stream << mod;
    }
}

void PopColor(std::ostream& stream)
{
    if (IsEnabled() && !modifierStack.empty())
    {
        modifierStack.pop();
        if (modifierStack.empty())
            stream << IOModifier();
        else
            stream << modifierStack.top();
    }
}


} // /namespace ConsoleManip

} // /namespace Platform



// ================================================================================
