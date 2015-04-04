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
#include <fstream>


namespace ControlFlowGraph
{


void CFGViewer::ViewGraph(
    const BasicBlock& entryPoint, std::ostream& stream,
    const std::string& fontName, const int fontSize)
{
    stream_ = &stream;

    WriteLine("digraph G {");
    WriteLine("node [shape=\"box\", fontname=\"" + fontName + "\", fontsize=\"" + ToStr(fontSize) + "\"];");
    WriteLine("edge [fontname=\"courier new bold\", fontsize=\"10\"];");

    DefineBlock(entryPoint);
    LinkBlock(entryPoint);

    WriteLine("}");

    blockLinks_.clear();
}

void CFGViewer::ViewGraph(
    const ClassTree& classTree, const std::string& path,
    const std::string& fontName, const int fontSize)
{
    for (const auto& block : classTree.GetRootBasicBlocks())
    {
        auto filename = path + block.second->label + ".vg";
        std::ofstream file(filename);
        ViewGraph(*block.second, file, fontName, fontSize);

        #if 1//DEBUG!!!
        auto cmd = "dot-png.bat \"" + filename + "\"";
        system(cmd.c_str());
        #endif
    }
}


/*
 * ======= Private: =======
 */

void CFGViewer::WriteLine(const std::string& line)
{
    *stream_ << line << std::endl;
}

void CFGViewer::DefineBlock(const BasicBlock& block)
{
    /* Check if block has already been traversed (for defining) */
    auto it = blockLinks_.find(&block);
    if (it != blockLinks_.end())
        return;

    /* Build label string for this block node */
    blockLinks_[&block] = false;

    std::string instrList;
    for (const auto& inst : block.insts)
        instrList += inst->ToString() + "\\l";

    /* Write graph node for this block */
    auto blockID = GetBlockID(block);
    auto blockLabel = "<" + blockID + ">";

    if (!block.label.empty())
        blockLabel += "\\n'" + block.label + "'";

    WriteLine(blockID + " [label=\"" + blockLabel + "\\n" + instrList + "\"];");

    /* Travers sucessors */
    for (const auto& succ : block.GetSucc())
        DefineBlock(*succ);
}

void CFGViewer::LinkBlock(const BasicBlock& block)
{
    /* Check if block has already been traversed (for linking) */
    auto it = blockLinks_.find(&block);
    if (it == blockLinks_.end())
        return;

    if (it->second)
        return;
    it->second = true;

    if (!block.GetSucc().empty())
    {
        /* Link to successors */
        auto blockID = GetBlockID(block);

        std::string succList;
        for (const auto& succ : block.GetSucc())
        {
            auto edge = blockID + ":s -> " + GetBlockID(*succ) + ":n";
            if (!succ.label.empty())
                edge += " [label=\"" + succ.label + "\"]";
            WriteLine(edge + ";");
        }

        /* Travers sucessors */
        for (const auto& succ : block.GetSucc())
            LinkBlock(*succ);
    }
}

std::string CFGViewer::GetBlockID(const BasicBlock& block) const
{
    return "bb" + ToStr(block.id);
}


} // /namespace ControlFlowGraph



// ================================================================================