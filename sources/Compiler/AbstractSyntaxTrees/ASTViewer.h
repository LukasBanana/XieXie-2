/*
 * ASTViewer.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_VIEWER_H__
#define __XX_AST_VIEWER_H__


#include "Visitor.h"


class Log;

namespace AbstractSyntaxTrees
{


class ASTViewer : public Visitor
{
    
    public:
        
        ASTViewer(Log& log);

        void ViewProgram(Program& ast, bool showPos = false);

    private:
        
        DECL_VISITOR_INTERFACE;

        void Info(const std::string& info, const AST* ast = nullptr);
        void Value(const std::string& ident, const std::string& value);
        void String(const std::string& ident, const std::string& value);

        Log& log_;
        bool showPos_ = false;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================