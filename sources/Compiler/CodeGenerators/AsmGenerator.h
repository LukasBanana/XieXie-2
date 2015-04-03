/*
 * AsmGenerator.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_ASM_GENERATOR_H__
#define __XX_ASM_GENERATOR_H__


#include <string>
#include <iostream>


namespace CodeGenerator
{


//! Base class for all assembler code generators.
class AsmGenerator
{
    
    public:
        
        struct Configuration
        {
            bool comments       = true;
            bool blanks         = true;
            bool indentation    = true;
        };

        //! Configuration for the assembler code generation.
        Configuration config;

    protected:
        
        AsmGenerator(std::ostream& outputStream, const std::string& indentStr);
        virtual ~AsmGenerator();

        //! Throws an CodeGenError exception.
        void Error(const std::string& msg) const;
        //! Throws an InternalError exception.
        void ErrorIntern(const std::string& msg) const;

        //! Starts a new line.
        void StartLine();
        //! Ends the current line.
        void EndLine();
        
        //! Append text to the current line.
        void L(const std::string& text);
        //! Write a new line of code.
        void Line(const std::string& line);

        //! Writes a blank line.
        void Blank();
        //! Writes 'num' blank lines.
        void Blanks(size_t num);

        //! Increments the indentation.
        void IncIndent();
        //! Decrements the indentation.
        void DecIndent();

    private:
        
        std::ostream&   stream_;

        std::string     indentStr_;
        std::string     indent_;

};


} // /namespace CodeGenerator


#endif



// ================================================================================