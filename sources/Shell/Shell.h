/*
 * Shell header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SHELL_H__
#define __XX_SHELL_H__


#include <string>
#include <vector>


class Shell
{
    
    public:
        
        typedef std::vector<std::string> ArgList;

        bool ExecuteCommandLine(const ArgList& args);

        void WaitForUserInput();

    private:
        
        void CmdVersion();
        void CmdHelp();

        void Pause();
        void AssembleFile(const std::string& filename);

};


#endif



// ================================================================================