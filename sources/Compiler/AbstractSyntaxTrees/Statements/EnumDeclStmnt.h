/*
 * EnumDeclStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_ENUM_DECL_STMNT_H__
#define __XX_AST_ENUM_DECL_STMNT_H__

// >>> temporarily disabled
#if 0


#include "ScopedStmnt.h"
#include "PointerTypeDenoter.h"


namespace AbstractSyntaxTrees
{


class EnumDeclStmnt : public ScopedStmnt
{
    
    public:
        
        AST_INTERFACE_BASE(EnumDeclStmnt);

        EnumDeclStmnt()
        {
            thisTypeDenoter_.declRef = this;
        }
        EnumDeclStmnt(const SourceArea& area) :
            ScopedStmnt{ area }
        {
            thisTypeDenoter_.declRef = this;
        }

        const TypeDenoter* GetTypeDenoter() const override;

        std::string                 ident;
        std::vector<EnumEntryPtr>   entries;

    private:

        // dast
        PointerTypeDenoter          thisTypeDenoter_; // type denoter for this enumeration declaration

};


} // /namespace AbstractSyntaxTrees


#endif

#endif



// ================================================================================
