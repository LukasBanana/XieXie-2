/*
 * Abstract syntax tree header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2013 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_H__
#define __XX_AST_H__


#include "SourceArea.h"
#include "Console.h"
#include "ASTDeclarations.h"
#include "Visitor.h"

#include <memory>
#include <functional>


namespace AbstractSyntaxTrees
{


using namespace SyntacticAnalyzer;


#define DefineASTVisitProc(n)                                                       \
    void Visit(Visitor* visitor, void* args = nullptr)                              \
    {                                                                               \
        visitor->Visit##n(std::dynamic_pointer_cast<n>(shared_from_this()), args);  \
    }


/**
AST (Abstract Syntax Tree) node base class.
There are several functions, which must be implemented by each child class.
The "Visit" class will be implemented by using the "DefineASTVisitProc" macro.
The "std::shared_ptr<ASTClassName> Copy() const" function must be implemented implicitly,
this function must copy the whole AST node.
*/
class AST : public std::enable_shared_from_this<AST>
{
    
    public:
        
        virtual ~AST()
        {
        }

        /**
        Virutal visitor function.
        \param[in] visitor Pass a code-generation- or contex-analysis visitor.
        */
        virtual void Visit(Visitor* visitor, void* args = nullptr) = 0;

        //! Refreshes for some AST nodes the source area (e.g. for all lists).
        virtual void RefreshSourceArea()
        {
            /* Dummy */
        }

        //! Area in the source code of this AST node.
        SourceArea sourceArea;

    protected:

        AST() = default;
        AST(const SourceArea& area) :
            sourceArea( area )
        {
        }

        template <typename T> inline std::shared_ptr<T> ThisPtr()
        {
            return std::dynamic_pointer_cast<T>(shared_from_this());
        }

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================