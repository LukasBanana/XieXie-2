/*
 * CFGViewer.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CFGViewer.h"
#include "StringModifier.h"

#include <exception>
#include <algorithm>


namespace ControlFlowGraph
{


void CFGViewer::DumpGraph(
    const BasicBlock& entryPoint, std::ostream& stream,
    const std::string& fontName, const int fontSize)
{
    stream_     = &stream;
    idCounter_  = 0;

    WriteLine("digraph G {");
    WriteLine("node [shape=\"box\", fontname=\"" + fontName + "\", fontsize=\"" + ToStr(fontSize) + "\"]");

    DefineBlock(entryPoint);
    LinkBlock(entryPoint);

    WriteLine("}");

    blockIDs_.clear();
}


/*
 * ======= Private: =======
 */

void CFGViewer::WriteLine(const std::string& line)
{
    *stream_ << line << std::endl;
}

void CFGViewer::LinkBlock(const BasicBlock& block)
{
    /* Check if block has already been traversed (for linking) */
    auto it = blockIDs_.find(&block);
    if (it == blockIDs_.end())
        return;

    if (it->second.linked)
        return;
    it->second.linked = true;

    if (!block.GetSucc().empty())
    {
        /* Link to successors */
        std::string succList;
        for (const auto& succ : block.GetSucc())
            succList += GetBlockID(*succ) + ", ";

        /* Remove last ", " entry */
        if (!succList.empty())
        {
            succList.pop_back();
            succList.pop_back();
        }

        /* Write graph links for this block */
        WriteLine(GetBlockID(block) + " -> { " + succList + " };");

        /* Travers sucessors */
        for (const auto& succ : block.GetSucc())
            LinkBlock(*succ);
    }
}

void CFGViewer::DefineBlock(const BasicBlock& block)
{
    /* Check if block has already been traversed (for defining) */
    auto it = blockIDs_.find(&block);
    if (it != blockIDs_.end())
        return;

    /* Write graph node for this block */
    auto id = ++idCounter_;
    blockIDs_[&block].id = id;

    std::string instrList;
    for (const auto& inst : block.insts)
        instrList += inst->ToString() + "\\n";

    WriteLine("bb" + ToStr(id) + " [label=\"" + instrList + "\"];");

    /* Travers sucessors */
    for (const auto& succ : block.GetSucc())
        DefineBlock(*succ);
}

std::string CFGViewer::GetBlockID(const BasicBlock& block) const
{
    auto it = blockIDs_.find(&block);
    return it != blockIDs_.end() ? "bb" + ToStr(it->second.id) : "bb";
}


} // /namespace ControlFlowGraph



// ================================================================================