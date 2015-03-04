/*
 * BasicBlock.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_CFG_BASIC_BLOCK_H__
#define __XX_CFG_BASIC_BLOCK_H__


#include "TACInst.h"

#include <vector>
#include <memory>


namespace ControlFlowGraph
{


//! Basic block of the control flow graph (CFG).
class BasicBlock
{
    
    public:
        
        std::vector<std::unique_ptr<ThreeAddressCodes::TACInst>> insts; //!< TAB instructions.

    private:
        
        std::vector<BasicBlock*> pred; //!< Predecessor reference list.
        std::vector<BasicBlock*> succ; //!< Successor reference list.

};


} // /namespace ControlFlowGraph


#endif



// ================================================================================