/*
 * CFGViewer.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_CFG_VIEWER_H__
#define __XX_CFG_VIEWER_H__


#include "BasicBlock.h"
#include "ClassTree.h"

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
        
        //! CFG viewer output description structure.
        struct OutputDesc
        {
            OutputDesc() { /* Can not use default constructor with clang */ }
            
            //! Specifies the font name for the graph nodes. By default "courier new".
            std::string fontName    = "courier new";
            //! Specifies the font size for the graph nodes. By default 10.
            int         fontSize    = 10;

            /**
            Specifies whether to generate a PNG file for each procedure CFG. By default true.
            \remarks This requires that the "dot" tool is installed on your platform.
            \see http://www.graphviz.org/
            */
            bool        genPNG      = true;
        };

        void ViewGraph(const BasicBlock& entryPoint, std::ostream& stream, const OutputDesc& desc = OutputDesc());
        void ViewGraph(const ClassTree& classTree, const std::string& path = "", const OutputDesc& desc = OutputDesc());

    private:
        
        void WriteLine(const std::string& line);
        void DefineBlock(const BasicBlock& block);
        void LinkBlock(const BasicBlock& block);
        std::string GetBlockID(const BasicBlock& block) const;

        std::ostream*                       stream_ = nullptr;

        std::map<const BasicBlock*, bool>   blockLinks_;

};


} // /namespace ControlFlowGraph


#endif



// ================================================================================