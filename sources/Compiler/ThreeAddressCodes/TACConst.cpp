/*
 * TAC constant file
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TACConst.h"
#include "StringModifier.h"


namespace ThreeAddressCodes
{


TACConst::TACConst(bool boolean) :
    type_       { ValueTypes::Boolean        },
    boolean_    { boolean                    },
    integral_   { boolean ? 1 : 0            },
    real_       { boolean ? 1.0f : 0.0f      },
    string_     { boolean ? "true" : "false" }
{
}
TACConst::TACConst(int integral) :
    type_       { ValueTypes::Integral         },
    boolean_    { integral != 0                },
    integral_   { integral                     },
    real_       { static_cast<float>(integral) },
    string_     { ToStr(integral)              }
{
}
TACConst::TACConst(float real) :
    type_       { ValueTypes::Real       },
    boolean_    { real != 0.0f           },
    integral_   { static_cast<int>(real) },
    real_       { real                   },
    string_     { ToStr(real)            }
{
}
TACConst::TACConst(const std::string& str) :
    type_       { ValueTypes::String   },
    boolean_    { str == "true"        },
    integral_   { StrToNum<int>(str)   },
    real_       { StrToNum<float>(str) },
    string_     { str                  }
{
}


} // /namespace ThreeAddressCodes



// ================================================================================