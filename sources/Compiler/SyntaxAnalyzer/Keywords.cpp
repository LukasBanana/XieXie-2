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
        { "true",       Ty::BoolLiteral    },
        { "false",      Ty::BoolLiteral    },
        { "null",       Ty::PointerLiteral },
        { "this",       Ty::ObjectIdent    },
        { "super",      Ty::ObjectIdent    },

        { "bool",       Ty::BuiltinType    },
        { "int",        Ty::BuiltinType    },
        { "float",      Ty::BuiltinType    },
        { "void",       Ty::Void           },
        { "var",        Ty::Var            },
        { "const",      Ty::Const          },

        { "or",         Ty::Or             },
        { "and",        Ty::And            },
        { "not",        Ty::Not            },

        { "do",         Ty::Do             },
        { "while",      Ty::While          },
        { "for",        Ty::For            },
        { "foreach",    Ty::ForEach        },
        { "forever",    Ty::ForEver        },

        { "if",         Ty::If             },
        { "else",       Ty::Else           },

        { "switch",     Ty::Switch         },
        { "case",       Ty::Case           },
        { "default",    Ty::Default        },

        { "break",      Ty::Break          },
        { "continue",   Ty::Continue       },
        { "return",     Ty::Return         },

        { "class",      Ty::Class          },
        { "extern",     Ty::Extern         },
        { "public",     Ty::Visibility     },
        { "private",    Ty::Visibility     },
        { "static",     Ty::Static         },
        { "init",       Ty::Init           },

        { "enum",       Ty::Enum           },
        { "flags",      Ty::Flags          },

        { "new",        Ty::New            },

        { "abstract",   Ty::Reserved       },
        { "package",    Ty::Reserved       },
        { "alias",      Ty::Reserved       },
        { "pattern",    Ty::Reserved       },
        { "sync",       Ty::Reserved       },
        { "byte",       Ty::Reserved       },
        { "ubyte",      Ty::Reserved       },
        { "short",      Ty::Reserved       },
        { "ushort",     Ty::Reserved       },
        { "uint",       Ty::Reserved       },
        { "long",       Ty::Reserved       },
        { "ulong",      Ty::Reserved       },
        { "double",     Ty::Reserved       },
        { "release",    Ty::Reserved       },
    };
}

static KeywordMapType keywordMap = GenerateKeywordMap();

const KeywordMapType& Keywords()
{
    return keywordMap;
}


} // /namespace SyntaxAnalyzer



// ================================================================================