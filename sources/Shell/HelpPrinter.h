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
        
        HelpPrinter() = default;
        HelpPrinter(bool briefHelp);

        //! Adds a new command help description.
        void Command(const std::string& grammar, const std::string& desc);

        /**
        Adds a new flag help description.
        \remarks If this help printer is only for brief helps, this function call has no effect.
        \throws std::runtime_error If "Command" has not been called first.
        */
        void Flag(const std::string& grammar, const std::string& desc);

        /**
        Adds multiple flags at once.
        \see Flag
        */
        void Flags(const std::vector<std::pair<std::string, std::string>>& flags);

        //! Prints all help messages to log output and clears the queue.
        void Flush(Log& log, char sep = '.');

        /**
        Compares the two help printers for a strict-weak-order (SWO).
        \remarks This is used to sort the help output.
        Only the grammar of the first two commands are compared.
        */
        static bool CompareSWO(const HelpPrinter& lhs, const HelpPrinter& rhs);

    private:
        
        /* === Structures === */

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

        /* === Functions === */

        //! Returns the string which is used to compare two help printers for a strict-weak-order (SWO).
        std::string StringForSWO() const;

        /* === Members === */

        std::vector<CommandHelp>    commandHelps_;

        bool                        briefHelp_ = false;

};


#endif



// ================================================================================