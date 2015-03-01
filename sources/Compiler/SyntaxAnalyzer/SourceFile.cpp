/*
 * Source file file
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "SourceFile.h"


namespace SyntaxAnalyzer
{


SourceFile::SourceFile(const std::string& filename)
{
    ReadFile(filename);
}

bool SourceFile::ReadFile(const std::string& filename)
{
    /* Open file and store filename */
    filename_ = filename;
    stream_.open(filename, std::ios_base::in);
    return stream_.good();
}

char SourceFile::Next()
{
    if (!stream_.is_open())
        return 0;

    /* Check if reader is at end-of-line */
    while (pos_.Column() >= line_.size())
    {
        /* Read new line in source file */
        std::getline(stream_, line_);
        line_ += '\n';
        pos_.IncRow();

        /* Check if end-of-file is reached */
        if (stream_.eof())
            return 0;
    }

    /* Increment column and return current character */
    auto chr = line_[pos_.Column()];
    pos_.IncColumn();

    return chr;
}


} // /namespace SyntaxAnalyzer



// ================================================================================