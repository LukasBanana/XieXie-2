/*
 * Error reporter header
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_ERROR_REPORTER_H__
#define __XX_ERROR_REPORTER_H__


#include "CompilerMessage.h"
#include "SourceCode.h"
#include "AST.h"

#include <vector>


class Log;

class ErrorReporter
{
    
    public:
        
        //! Adds the specified compiler message to the report.
        void Add(const CompilerMessage& message);

        /**
        Adds the specified message to the report.
        \tparam MsgType Specifies the compiler message type.
        \param[in] msg Specifies the output message.
        \param[in] ast Specifies an optional AST node.
        \param[in] source Specifies an optional source code reference.
        */
        template <typename MsgType> void Add(const std::string& msg, const AbstractSyntaxTrees::AST* ast = nullptr)
        {
            if (ast)
            {
                if (source)
                {
                    std::string line, marker;
                    if (source->FetchLineMarker(ast->sourceArea, line, marker))
                        Add(MsgType(ast->sourceArea, msg, line, marker));
                    else
                        Add(MsgType(ast->sourceArea, msg));
                }
                else
                    Add(MsgType(ast->sourceArea, msg));
            }
            else
                Add(MsgType(msg));
        }

        /**
        Prints all messages to console and clears the message list.
        \param[in] printMetaInfo Specifies whether the number of errors and warnings are to be printed as well.
        */
        void Flush(Log& log, bool printMetaInfo = true);

        //! Returns true if the current report has any errors.
        bool HasErrors() const
        {
            return hasErrors_;
        }

        //! Optional source code reference. This can be used for better error outputs.
        const SyntaxAnalyzer::SourceCode* source = nullptr;

    private:

        std::vector<CompilerMessage> messages_;

        bool hasErrors_ = false;

};


#endif



// ================================================================================