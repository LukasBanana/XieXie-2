/*
 * AttribPrefix.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_ATTRIB_PREFIX_H__
#define __XX_AST_ATTRIB_PREFIX_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class AttribPrefix : public AST
{
    
    public:
        
        //! Summarized attribute falgs.
        struct Flags
        {
            bool        isDeprecated = false;
            std::string deprecationHint;
        };

        AST_INTERFACE(AttribPrefix);

        //! Returns true if this attribute prefix has the specified attribute identifier.
        bool HasAttrib(const std::string& attribIdent) const;

        std::vector<AttribPtr>  attribs;

        // dast
        Flags                   flags;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
