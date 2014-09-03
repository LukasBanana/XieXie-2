/*
 * Keywords file
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Keywords.h"


namespace SyntaxAnalyzer
{


static KeywordMapType GenerateKeywordMap()
{
    typedef Token::Types Ty;

    return
    {
        { "void",       Ty::Void     },
        { "bool",       Ty::Bool     },
        { "int",        Ty::Int      },
        { "float",      Ty::Float    },

        { "or",         Ty::Or       },
        { "and",        Ty::And      },
        { "not",        Ty::Not      },

        { "do",         Ty::Do       },
        { "while",      Ty::While    },
        { "for",        Ty::For      },
        { "forEach",    Ty::ForEach  },
        { "forEver",    Ty::ForEver  },
            
        { "if",         Ty::If       },
        { "else",       Ty::Else     },
            
        { "switch",     Ty::Switch   },
        { "case",       Ty::Case     },
        { "default",    Ty::Default  },

        { "break",      Ty::Break    },
        { "continue",   Ty::Continue },
        { "return",     Ty::Return   },

        { "class",      Ty::Class    },
        { "extern",     Ty::Extern   },
        { "public",     Ty::Public   },
        { "private",    Ty::Private  },
        { "static",     Ty::Static   },
        { "init",       Ty::Init     },
        { "release",    Ty::Release  },

        { "enum",       Ty::Enum     },
        { "flags",      Ty::Flags    },

        { "new",        Ty::New      },
    };
}

static KeywordMapType keywordMap = GenerateKeywordMap();

const KeywordMapType& Keywords()
{
    return keywordMap;
}


} // /namespace SyntaxAnalyzer



// ================================================================================