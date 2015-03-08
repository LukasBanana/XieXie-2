/*
 * TACVarManager.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_TAC_VAR_MANAGER_H__
#define __XX_TAC_VAR_MANAGER_H__


#include "TACVar.h"
#include "SafeStack.h"
#include "AST.h"

#include <map>


namespace ThreeAddressCodes
{


class TACVarManager
{
    
        using AST = AbstractSyntaxTrees::AST;

    public:
        
        using IDType = TACVar::IDType;

        void Reset();

        TACVar TempVar();
        TACVar LocalVar(const AST& ast);

        void PushVar(const TACVar& var);
        TACVar PopVar();

        //! Returns the current TAC variable at the top of the stack.
        TACVar Var();

    private:
        
        /* === Functions === */

        IDType GenTempVarID();
        void RemoveTempVarID(IDType id);

        /* === Members === */

        IDType                          varCounterLocal_    = 0;
        IDType                          varCounterGlobal_   = 0;

        SafeStack<TACVar>               varStack_;

        std::map<const AST*, TACVar>    localVarMap_;
        std::vector<IDType>             tempVarIDs_;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================