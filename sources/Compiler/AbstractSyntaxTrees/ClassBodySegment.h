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
        
        enum class Visibilities
        {
            Public, // default visibility
            Private,
        };

        AST_INTERFACE(ClassBodySegment);

        Visibilities            visibility = Visibilities::Public;
        std::vector<StmntPtr>   declStmnts;

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================