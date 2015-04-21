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
#include "VarDecl.h"

#include <map>
#include <functional>


namespace ThreeAddressCodes
{


//! Variable manager to generate unique TAC variable identifiers.
class TACVarManager
{
    
        using AST = AbstractSyntaxTrees::AST;
        using VarDecl = AbstractSyntaxTrees::VarDecl;

    public:
        
        using IDType = TACVar::IDType;

        //! Restes all variables counters and references.
        void Reset();

        //! Generates new temporary variable.
        TACVar TempVar();
        /**
        Returns the 'locator-value' variable for the specified AST node.
        \see LocalVar
        \see GlobalVar
        \see MemberVar
        */
        TACVar LValueVar(const AST& ast);

        TACVar LocalVar(const AST& ast);
        TACVar GlobalVar(const VarDecl& ast);
        TACVar MemberVar(const VarDecl& ast);

        void PushVar(const TACVar& var);
        TACVar PopVar();

        //! Returns the current TAC variable at the top of the stack.
        TACVar Var();

    private:
        
        /* === Structures === */

        template <typename T, TACVar::Types varType, bool useCounter> struct VarMap
        {
            void Reset()
            {
                varCounter = 0;
                vars.clear();
            }

            TACVar& FetchVar(
                const T& ast,
                const std::function<void(TACVar& var)>& registerVarCallback = nullptr
            );

            IDType                      varCounter = 0;
            std::map<const T*, TACVar>  vars;
        };

        /* === Functions === */

        IDType GenTempVarID();
        void RemoveTempVarID(IDType id);

        /* === Members === */

        SafeStack<TACVar>                               varStack_;

        VarMap< AST,     TACVar::Types::Local,  true  > localVars_;
        VarMap< VarDecl, TACVar::Types::Global, false > globalVars_;
        VarMap< VarDecl, TACVar::Types::Member, false > memberVars_;

        std::vector<IDType>                             tempVarIDs_;

};


} // /namespace ThreeAddressCodes


#endif



// ================================================================================