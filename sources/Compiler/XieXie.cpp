/*
 * xiexie.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "SearchPaths.h"

#include <xiexie/xiexie.h>
#include <sstream>
#include <fstream>


namespace XieXie
{


using namespace VirtualMachine;

/* --- Common --- */

void SetupPaths(const std::string& libraryPath, const std::string& modulesPath)
{
    SearchPaths::LibraryPath() = libraryPath;
    SearchPaths::ModulesPath() = modulesPath;
}


/* --- Compilation Functions --- */

bool Compile(const CompileConfig& config, std::ostream* log)
{
    return false; //todo...
}

bool Assemble(std::istream& inputStream, std::ostream& outputStream, std::ostream* log)
{
    return false; //todo...
}

std::unique_ptr<ByteCode> LoadAssembly(std::istream& inputStream)
{
    return nullptr; //todo...
}


/* --- Very High-Level Functions --- */

static std::unique_ptr<VirtualMachine::ByteCode> CompileFromStream(std::istream& inputStream, int compileFlags, std::ostream* log)
{
    /* Setup compilation configuration */
    std::stringstream outputStream, assembly;

    CompileConfig config;
    config.inputStreams = { &inputStream };
    config.outputStream = &outputStream;
    config.flags        = compileFlags;

    /* Compile code string */
    if (!Compile(config, log))
        return nullptr;

    /* Assemble code */
    if (!Assemble(outputStream, assembly))
        return nullptr;

    /* Load final assembly to byte code */
    return LoadAssembly(assembly);
}

std::unique_ptr<VirtualMachine::ByteCode> CompileFromString(const std::string& codeString, int compileFlags, std::ostream* log)
{
    std::stringstream inputStream;
    inputStream << codeString;
    return CompileFromStream(inputStream, compileFlags, log);
}

std::unique_ptr<VirtualMachine::ByteCode> CompileFromFile(const std::string& codeFilename, int compileFlags, std::ostream* log)
{
    std::ifstream inputStream(codeFilename);
    if (!inputStream.good())
        return nullptr;
    return CompileFromStream(inputStream, compileFlags, log);
}

ExitCodes RunFromString(const std::string& codeString, int compileFlags, const size_t stackSize, std::ostream* log)
{
    auto byteCode = CompileFromString(codeString, compileFlags, log);
    if (byteCode)
    {
        Stack stack(stackSize);
        return ExecuteProgram(*byteCode, stack);
    }
    return ExitCodes::InvalidByteCode;
}

ExitCodes RunFromFile(const std::string& codeFilename, int compileFlags, const size_t stackSize, std::ostream* log)
{
    auto byteCode = CompileFromFile(codeFilename, compileFlags, log);
    if (byteCode)
    {
        Stack stack(stackSize);
        return ExecuteProgram(*byteCode, stack);
    }
    return ExitCodes::InvalidByteCode;
}


} // /namespace XieXie



// ================================================================================
