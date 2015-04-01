/*
 * SourceStream.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "SourceStream.h"

#include <fstream>


namespace SyntaxAnalyzer
{


SourceStream::SourceStream(std::unique_ptr<std::istream>&& stream) :
    stream_{ std::move(stream) }
{
}
SourceStream::SourceStream(const std::string& filename)
{
    ReadFile(filename);
}

bool SourceStream::ReadFile(const std::string& filename)
{
    /* Open file and store filename */
    filename_ = filename;
    stream_ = std::move(std::make_unique<std::ifstream>(filename, std::ios_base::in));
    return stream_->good();
}

void SourceStream::Close()
{
    stream_.release();
}

char SourceStream::Next()
{
    if (!stream_ || !stream_->good())
        return 0;

    /* Check if reader is at end-of-line */
    while (pos_.Column() >= line_.size())
    {
        /* Read new line in source file */
        std::getline(*stream_, line_);
        line_ += '\n';
        pos_.IncRow();

        /* Store line for later error messages */
        content_.push_back(line_);

        /* Check if end-of-file is reached */
        if (stream_->eof())
            return 0;
    }

    /* Increment column and return current character */
    auto chr = line_[pos_.Column()];
    pos_.IncColumn();

    return chr;
}

static bool FinalizeMarker(
    const SourceArea& area, const std::string& lineIn,
    std::string& lineOut, std::string& markerOut, char markerChar)
{
    if (area.end.Column() > lineIn.size() || area.start.Column() == 0)
        return false;

    lineOut = lineIn;
    markerOut = std::string(area.start.Column() - 1, ' ');

    for (size_t i = 0, n = markerOut.size(); i < n; ++i)
    {
        if (lineIn[i] == '\t')
            markerOut[i] = '\t';
    }

    markerOut += std::string(area.end.Column() - area.start.Column(), markerChar);

    return true;
}

std::string SourceStream::FetchLine(size_t lineNumber) const
{
    return lineNumber < content_.size() ? content_[lineNumber] : "";
}

bool SourceStream::FetchLineMarker(const SourceArea& area, std::string& line, std::string& marker, char markerChar) const
{
    if (!area.IsValid() || area.start.Row() != area.end.Row())
        return false;

    auto row = area.start.Row();

    if (row == pos_.Row())
        return FinalizeMarker(area, Line(), line, marker, markerChar);
    else if (row > 0)
        return FinalizeMarker(area, FetchLine(static_cast<size_t>(row - 1)), line, marker, markerChar);

    return false;
}


} // /namespace SyntaxAnalyzer



// ================================================================================