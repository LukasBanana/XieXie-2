/*
 * HelpPrinter.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SHELL_HELP_PRINTER_H__
#define __XX_SHELL_HELP_PRINTER_H__


#include "Log.h"

#include <string>
#include <vector>


class HelpPrinter
{
    
    public:
        
        //! Adds a new command help description.
        void Command(const std::string& grammar, const std::string& desc);

        /**
        Adds a new flag help description.
        \throws std::runtime_error If "Command" has not been called first.
        */
        void Flag(const std::string& grammar, const std::string& desc);

        //! Adds multiple flags at once.
        void Flags(const std::vector<std::pair<std::string, std::string>>& flags);

        //! Prints all help messages to log output and clears the queue.
        void Flush(Log& log, char sep = '.');

    private:
        
        struct Help
        {
            void MaxLength(size_t& maxLen, size_t offset = 0) const;
            void Print(Log& log, char sep, size_t maxLen, size_t offset = 0) const;

            std::string grammar;
            std::string desc;
        };

        struct CommandHelp
        {
            void MaxLength(size_t& maxLen, size_t flagOffset) const;
            void Print(Log& log, char sep, size_t maxLen, size_t flagOffset) const;

            Help                commandInfo;
            std::vector<Help>   flagsInfos;
        };

        std::vector<CommandHelp> commandHelps_;

};


#endif



// ================================================================================