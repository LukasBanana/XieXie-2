/*
 * CFGViewer.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_CFG_VIEWER_H__
#define __XX_CFG_VIEWER_H__


#include "BasicBlock.h"

#include <iostream>
#include <string>
#include <map>


namespace ControlFlowGraph
{


/**
Dumps the control flow graph (CFG) to a text file (in the DOT language format).
This output file can be used with 'graphviz' to visualize the graph.
\see http://www.graphviz.org/
*/
class CFGViewer
{
    
    public:
        
        void DumpGraph(
            const BasicBlock& entryPoint, std::ostream& stream,
            const std::string& fontName = "courier new", const int fontSize = 10
        );

    private:
        
        struct BlockID
        {
            size_t  id      = 0;
            bool    linked  = false;
        };

        void WriteLine(const std::string& line);
        void DefineBlock(const BasicBlock& block);
        void LinkBlock(const BasicBlock& block);
        std::string GetBlockID(const BasicBlock& block) const;

        std::ostream*                           stream_     = nullptr;

        size_t                                  idCounter_  = 0;
        std::map<const BasicBlock*, BlockID>    blockIDs_;

};


} // /namespace ControlFlowGraph


#endif



// ================================================================================