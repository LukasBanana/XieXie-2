/*
 * Assembler file
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2013 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Assembler.h"
#include "Console.h"


namespace XieXie
{


bool Assembler::AssembleFile(const std::string& inFilename, const std::string& outFilename)
{
    /* Open files */
    inFile_.open(inFilename, std::ios_base::in);
    outFile_.open(outFilename, std::ios_base::out);

    /* Read and parse input file */
    bool result = true;

    try
    {
        while (!inFile_.eof())
        {
            ReadNextLine();
            ParseLine();
        }
    }
    catch (const std::exception& err)
    {
        Console::Error("(" + sourcePos_.ToString() + ") -- " + std::string(err.what()));
        result = false;
    }
    catch (const std::string& err)
    {
        Console::Error("(" + sourcePos_.ToString() + ") -- " + err);
        result = false;
    }

    /* Close files */
    inFile_.close();
    outFile_.close();

    return true;
}


/*
 * ======= Private: =======
 */

void Assembler::ReadNextLine()
{
    std::getline(inFile_, line_);
    sourcePos_.IncRow();
}

void Assembler::ParseLine()
{
    



}


} // /namespace XieXie



// ================================================================================