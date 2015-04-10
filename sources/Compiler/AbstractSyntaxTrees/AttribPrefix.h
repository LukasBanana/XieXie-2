/*
 * AttribPrefix.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_ATTRIB_PREFIX_H__
#define __XX_AST_ATTRIB_PREFIX_H__


#include "AST.h"

#include <vector>
#include <set>


namespace AbstractSyntaxTrees
{


class AttribPrefix : public AST
{
    
    public:
        
        AST_INTERFACE(AttribPrefix);

        //! Returns a reference to the specified attribute or null if there is no such attribute in the list.
        const Attrib* FindAttrib(const std::string& attribIdent) const;
        //! Returns true if this attribute prefix has the specified attribute identifier.
        bool HasAttrib(const std::string& attribIdent) const;

        //! Returns a list of all duplicate attributes.
        std::vector<const Attrib*> FindDuplicateAttribs() const;
        //! Returns a list of all attributes which are not allowed, i.e. not part of 'validAttribs'.
        std::vector<const Attrib*> FindInvalidAttribs(const std::set<std::string>& validAttribs) const;

        std::vector<AttribPtr>  attribs;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
