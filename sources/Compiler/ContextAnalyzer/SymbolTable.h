/*
 * SymbolTable.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SYMBOL_TABLE_H__
#define __XX_SYMBOL_TABLE_H__


#include "StringModifier.h"

#include <map>
#include <string>
#include <sstream>
#include <stack>
#include <vector>
#include <functional>


namespace ContextAnalyzer
{


//! Common symbol table class with a single scope.
template <typename Owner, typename Sym, typename PrivScope> class SymbolTable
{
    
    public:
        
        using OwnerType     = Owner;
        using SymbolType    = Sym;
        using PrivateScope  = PrivScope;

        //! Override symbol callback procedure. Must return true to allow a symbol override.
        using OnOverrideProc = std::function<bool (SymbolType* symbol)>;

        /* === Functiosn === */

        SymbolTable(OwnerType& owner) :
            // bracket initializer required for GCC (due to bug 56032)
            owner_( owner )
        {
            /* Open global scope */
            OpenScope();
        }

        //! Opens a new scope.
        void OpenScope()
        {
            scopeStack_.push({});
        }

        //! Closes the active scope.
        void CloseScope()
        {
            if (!scopeStack_.empty())
            {
                /* Remove all symbols from the table which are in the current scope */
                for (const auto& ident : scopeStack_.top())
                {
                    auto it = symTable_.find(ident);
                    if (it != symTable_.end())
                    {
                        /* Remove symbol from the top most scope level */
                        it->second.pop();
                        if (it->second.empty())
                        {
                            /* Remove symbol entry completely if it's reference list is empty */
                            symTable_.erase(it);
                        }
                    }
                }
                scopeStack_.pop();
            }
        }

        /**
        Registers the specified symbol in the current scope.
        At least one scope must be open before symbols can be registered!
        \throws std::runtime_error If an error occured.
        */
        void Register(
            std::string ident, SymbolType* symbol,
            const PrivateScope* privateScope = nullptr, const OnOverrideProc& overrideProc = nullptr)
        {
            /* Validate input parameters */
            if (scopeStack_.empty())
                throw std::runtime_error("no active scope to register symbol");
            if (ident.empty())
                throw std::runtime_error("can not register unnamed symbol");

            /* Append unique string from private scope */
            if (privateScope)
                ident += UniqueString(privateScope);

            /* Check if identifier was already registered in the current scope */
            auto it = symTable_.find(ident);
            if (it != symTable_.end() && !it->second.empty())
            {
                auto& entry = it->second.top();
                if (entry.symbol && entry.scopeLevel == ScopeLevel())
                {
                    if (overrideProc && overrideProc(entry.symbol))
                    {
                        /* Override symbol in this scope */
                        entry.symbol = symbol;
                        return;
                    }
                    else
                        throw std::runtime_error("identifier \"" + ident + "\" already declared in the current scope");
                }
            }

            /* Register new identifier */
            symTable_[ident].push({ symbol, ScopeLevel() });
            scopeStack_.top().push_back(ident);
        }

        /**
        Returns the symbol with the specified identifer which is in
        the deepest scope, or null if there is no such symbol.
        \param[in] ident Specifies the symbol's identifier.
        \param[in] privateScope Optional pointer to the current private scope. By default null.
        \param[in] allowFallbackSymTab Specifies whether the fallback symbol table can be used or not. By default true.
        */
        SymbolType* Fetch(const std::string& ident, const PrivateScope* privateScope = nullptr, bool allowFallbackSymTab = true) const
        {
            /* Search identifier in private scope */
            if (privateScope)
            {
                auto symbol = FetchSymbol(ident + UniqueString(privateScope));
                if (symbol)
                    return symbol;
            }

            /* Search identifier in this symbol table */
            auto symbol = FetchSymbol(ident);
            if (symbol)
                return symbol;

            /* Search identifier in the fallback symbol table */
            if (allowFallbackSymTab && fallbackSymTab)
                return fallbackSymTab->Fetch(ident, privateScope);

            /* No symbol found */
            return nullptr;
        }

        /**
        Tries to find a symbol with the most similar identifier.
        \return Pointer to the symbol identifier or null, if no similar symbol has been found.
        \see Fetch
        */
        const std::string* FindSimilarity(const std::string& ident, const PrivateScope* privateScope = nullptr, bool allowFallbackSymTab = true) const
        {
            /* Search identifier in private scope */
            if (privateScope)
            {
                auto symbolIdent = FindSimilarSymbol(ident + UniqueString(privateScope));
                if (symbolIdent)
                    return symbolIdent;
            }

            /* Search identifier in this symbol table */
            auto symbolIdent = FindSimilarSymbol(ident);
            if (symbolIdent)
                return symbolIdent;

            /* Search identifier in the fallback symbol table */
            if (allowFallbackSymTab && fallbackSymTab)
                return fallbackSymTab->FindSimilarity(ident, privateScope);

            /* No symbol found */
            return nullptr;
        }

        //! Returns current scope level.
        size_t ScopeLevel() const
        {
            return scopeStack_.size();
        }

        //! Returns the owner of this symbol table.
        inline OwnerType& GetOwner() const
        {
            return owner_;
        }

        /* === Members === */

        //! Fallback symbol table when an identifier was not found in this symbol table.
        SymbolTable* fallbackSymTab = nullptr;

    private:
        
        /* === Structures === */

        struct Symbol
        {
            SymbolType* symbol;
            size_t      scopeLevel;
        };

        /* === Functions === */

        SymbolType* FetchSymbol(const std::string& ident) const
        {
            auto it = symTable_.find(ident);
            if (it != symTable_.end() && !it->second.empty())
                return it->second.top().symbol;
            return nullptr;
        }

        //! Returns true if the specified identifier is similar to any symbol in this symbol table.
        const std::string* FindSimilarSymbol(const std::string& ident) const
        {
            size_t maxSim = 0;
            const std::string* symbolIdent = nullptr;

            for (auto it = symTable_.begin(); it != symTable_.end(); ++it)
            {
                auto sim = StringSimilarities(ident, it->first);
                if (sim > maxSim)
                {
                    symbolIdent = &(it->first);
                    maxSim = sim;
                }
            }

            return symbolIdent;
        }

        std::string UniqueString(const void* ptr) const
        {
            std::stringstream stream;
            stream << ptr;
            return stream.str();
        }

        /* === Members === */

        //! Stores the scope stack for all identifiers.
        std::map<std::string, std::stack<Symbol>>   symTable_;

        /**
        Stores all identifiers for the current stack.
        All these identifiers will be removed from "symTable_" when a scope will be closed.
        */
        std::stack<std::vector<std::string>>        scopeStack_;

        OwnerType&                                  owner_;

};


} // /namespace ContextAnalyzer


#endif



// ================================================================================
