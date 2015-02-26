/*
 * ClassBodySegment.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_CLASS_BODY_SEGMENT_H__
#define __XX_AST_CLASS_BODY_SEGMENT_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class ClassBodySegment : public AST
{
    
    public:
        
        AST_INTERFACE(ClassBodySegment);

        std::string             visibility; // may be empty (default visibility)
        std::vector<StmntPtr>   declStmnts;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
