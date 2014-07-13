/*
 * Error reporter header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_ERROR_REPORTER_H__
#define __XX_ERROR_REPORTER_H__


#include "CompilerMessage.h"
#include <vector>


class ErrorReporter
{
    
    public:
        
        void Add(const CompilerMessage& message);

        //! Prints all messages to console and clears the message list.
        void Flush();

        bool HasErrors() const
        {
            return hasErrors_;
        }

    private:

        std::vector<CompilerMessage> messages_;

        bool hasErrors_ = false;

};


#endif



// ================================================================================