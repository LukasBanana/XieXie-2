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
                auto source = ast->sourceArea.GetSource();
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
        Prints all messages to log output and clears the queue.
        \param[in] printMetaInfo Specifies whether the number of errors and warnings are to be printed as well.
        */
        void Flush(Log& log, bool printMetaInfo = true);

        //! Returns true if the limit of error messages has been exceeded.
        bool ExceededErrorLimit() const;

        //! Returns the number of errors, which are currently in the report.
        size_t NumErrors() const
        {
            return numErrors_;
        }
        //! Returns true if the current report has any errors.
        bool HasErrors() const
        {
            return NumErrors() > 0;
        }

        //! Returns the number of warnings, which are currently in the report.
        size_t NumWarnings() const
        {
            return numWarnings_;
        }
        //! Returns true if the current report has any warnings.
        bool HasWarnings() const
        {
            return NumWarnings() > 0;
        }

        /* === Members === */

        //! Specifies the limit for error messages. Zero specifies infinite error messages. By default 100.
        size_t      numErrorLimit   = 100u;

        //! Show warnings configuration. By default false.
        static bool showWarnings;

    private:

        std::vector<CompilerMessage>    messages_;
        size_t                          numErrors_      = 0;
        size_t                          numWarnings_    = 0;

};


#endif



// ================================================================================