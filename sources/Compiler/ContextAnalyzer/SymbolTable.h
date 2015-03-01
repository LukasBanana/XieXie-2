/*
 * SymbolTable.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SYMBOL_TABLE_H__
#define __XX_SYMBOL_TABLE_H__


#include <map>
#include <string>
#include <stack>
#include <vector>
#include <functional>


namespace ContextAnalyzer
{


//! Common symbol table class with a single scope.
template <typename OwnerType, typename SymbolType> class SymbolTable
{
    
    public:
        
        //! Override symbol callback procedure. Must return true to allow a symbol override.
        using OnOverrideProc = std::function<bool (SymbolType* symbol)>;

        SymbolTable(OwnerType& owner) :
            owner_{ owner }
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
        void Register(const std::string& ident, SymbolType* symbol, const OnOverrideProc& overrideProc = nullptr)
        {
            /* Validate input parameters */
            if (scopeStack_.empty())
                throw std::runtime_error("no active scope to register symbol");
            if (ident.empty())
                throw std::runtime_error("can not register unnamed symbol");

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
        */
        SymbolType* Fetch(const std::string& ident) const
        {
            auto it = symTable_.find(ident);
            if (it != symTable_.end() && !it->second.empty())
                return it->second.top().symbol;
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

    private:
        
        struct Symbol
        {
            SymbolType* symbol;
            size_t      scopeLevel;
        };

        //! Stores the scope stack for all identifiers.
        std::map<std::string, std::stack<Symbol>> symTable_;

        /**
        Stores all identifiers for the current stack.
        All these identifiers will be removed from "symTable_" when a scope will be closed.
        */
        std::stack<std::vector<std::string>> scopeStack_;

        OwnerType& owner_;

};


} // /namespace ContextAnalyzer


#endif



// ================================================================================