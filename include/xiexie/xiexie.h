/*
 * xiexie.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_XIEXIE_H__
#define __XX_XIEXIE_H__


#include <xiexie/xvm_wrapper.h>
#include <string>
#include <vector>
#include <iostream>
#include <memory>


namespace XieXie
{


/* --- Structures --- */

//! Compilation flags enumeration.
struct CompileFlags
{
    enum
    {
        Verbose     = (1 << 0),
        Warn        = (1 << 1),
        Optimize    = (1 << 2),
        ShowAST     = (1 << 3),
        ShowCFG     = (1 << 4),
        Default     = (Warn | Optimize),
    };
};

//! Compilation configuration.
struct CompileConfig
{
    std::vector<std::istream*>  inputStreams;
    std::ostream*               outputStream;
    int                         flags = CompileFlags::Default;
};


/* --- Common --- */

/**
Sets the library and modules paths.
\param[in] libraryPath Specifies the path of the "XieXie/library/" directory.
\param[in] modulesPath Specifies the path of the "XieXie/modules/" directory.
*/
void SetupPaths(
    const std::string& libraryPath,
    const std::string& modulesPath
);


/* --- Compilation Functions --- */

bool Compile(const CompileConfig& config, std::ostream* log = nullptr);

bool Assemble(std::istream& inputStream, std::ostream& outputStream, std::ostream* log = nullptr);

std::unique_ptr<VirtualMachine::ByteCode> LoadAssembly(std::istream& inputStream);


/* --- Very High-Level Functions --- */

/**
Compiles XieXie code directly from the specified code string.
\param[in] codeString Specifies the XieXie code which is to be executed.
\param[in] compileFlags Specifies the compilation flags. By default CompileFlags::Default.
\remarks This is a 'very high-level' function, to quickly execute XieXie code.
\see CompileFlags
*/
std::unique_ptr<VirtualMachine::ByteCode> CompileFromString(
    const std::string& codeString,
    int compileFlags = CompileFlags::Default,
    std::ostream* log = nullptr
);

/**
Compiles XieXie code directly from the specified file.
\param[in] codeFilename Specifies the file which contains the XieXie code to execute.
\see CompileFromString
*/
std::unique_ptr<VirtualMachine::ByteCode> CompileFromFile(
    const std::string& codeFilename,
    int compileFlags = CompileFlags::Default,
    std::ostream* log = nullptr
);

/**
Runs XieXie code directly from the specified code string.
\param[in] codeString Specifies the XieXie code which is to be executed.
\param[in] compileFlags Specifies the compilation flags. By default CompileFlags::Default.
\param[in] stackSize Specifies the size (in WORDS) for the virtual stack to execute the script.
\remarks This is a 'very high-level' function, to quickly execute XieXie code.
\see CompileFlags
*/
VirtualMachine::ExitCodes RunFromString(
    const std::string& codeString,
    int compileFlags = CompileFlags::Default,
    const size_t stackSize = VirtualMachine::Stack::defaultSize,
    std::ostream* log = nullptr
);

/**
Runs XieXie code directly from the specified file.
\param[in] codeFilename Specifies the file which contains the XieXie code to execute.
\see RunFromString
*/
VirtualMachine::ExitCodes RunFromFile(
    const std::string& codeFilename,
    int compileFlags = CompileFlags::Default,
    const size_t stackSize = VirtualMachine::Stack::defaultSize,
    std::ostream* log = nullptr
);


} // /namespace XieXie


#endif



// ================================================================================
