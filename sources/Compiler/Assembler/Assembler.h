/*
 * Assembler header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2013 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_ASSEMBLER_H__
#define __XX_ASSEMBLER_H__


#include "Base/Export.h"
#include "Compiler/SyntaxAnalyzer/SourcePosition.h"

#include <fstream>
#include <string>


namespace XieXie
{


/**
XieXie Assmbler (XASM).
This class can compile XASM (XieXie Assembler) files into XBC (XieXie Byte Code) files.
*/
class XX_EXPORT Assembler
{
    
    public:
        
        bool AssembleFile(const std::string& inFilename, const std::string& outFilename);

    private:
        
        /* === Functions === */

        void ReadNextLine();
        void ParseLine();
        
        /* === Members === */

        std::ifstream inFile_;  //!< Input XASM file.
        std::ofstream outFile_; //!< Output XBC file.

        std::string line_;      //!< Current source line.

        SyntaxAnalyzer::SourcePosition sourcePos_;

};


} // /namespace XieXie


#endif



// ================================================================================