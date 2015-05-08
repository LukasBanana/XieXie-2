/*
 * Keywords header
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_KEYWORDS_H__
#define __XX_KEYWORDS_H__


#include "Token.h"

#include <map>
#include <string>


namespace SyntaxAnalyzer
{


//! Keywords map type definition.
using KeywordMapType = std::map<std::string, Token::Types>;

//! Returns the keywords map (which is an exception for identifiers).
const KeywordMapType& Keywords();


} // /namespace SyntaxAnalyzer


#endif



// ================================================================================