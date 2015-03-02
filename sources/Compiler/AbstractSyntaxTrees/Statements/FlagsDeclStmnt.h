/*
 * FlagsDeclStmnt.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_FLAGS_DECL_STMNT_H__
#define __XX_AST_FLAGS_DECL_STMNT_H__


#include "Stmnt.h"
#include "PointerTypeDenoter.h"


namespace AbstractSyntaxTrees
{


class FlagsDeclStmnt : public Stmnt
{
    
    public:
        
        AST_INTERFACE_BASE(FlagsDeclStmnt);

        FlagsDeclStmnt()
        {
            thisTypeDenoter_.declRef = this;
        }
        FlagsDeclStmnt(const SourceArea& area) :
            Stmnt{ area }
        {
            thisTypeDenoter_.declRef = this;
        }

        const TypeDenoter* GetTypeDenoter() const override;

        std::string                 ident;
        std::vector<VarNamePtr>     inheritanceTypeNames;
        std::vector<std::string>    entries;

    private:

        // dast
        PointerTypeDenoter          thisTypeDenoter_; // type denoter for this flags declaration

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
