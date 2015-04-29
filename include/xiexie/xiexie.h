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
#include <set>


namespace XieXie
{


/* --- Structures --- */

//! Compilation flags enumeration.
struct CompileFlags
{
    enum
    {
        //! Enables additional output from the compiler.
        Verbose     = (1 << 0),

        //! Enables compiler warnings.
        Warn        = (1 << 1),

        //! Enables all optimization passes.
        Optimize    = (1 << 2),

        //! Prints the abstract-syntax-tree (AST) to the standard output.
        ShowAST     = (1 << 3),

        /**
        Shows the control-flow-graph (CFG) as PNG images.
        \remarks This option requires, that 'graphviz' is installed on your platform.
        \see http://www.graphviz.org/
        */
        ShowCFG     = (1 << 4),

        //! Prints the entire token stream to the standard output.
        ShowTokens  = (1 << 5),

        //! Default compilation flags: warning and optimization is enabled.
        Default     = (Warn | Optimize),
    };
};

//! Compilation configuration.
struct CompileConfig
{
    struct Source
    {
        std::shared_ptr<std::istream>   stream;     //!< Input source stream.
        std::string                     filename;   //!< Optional filename.
    };

    std::vector<Source>     sources;
    std::iostream*          assembly;

    int                     flags = CompileFlags::Default;

    std::string             cfgDumpPath;
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

/**
Compiles the input streams and writes the assembly to the output stream.
\param[in] config Specifies the compilation configuration.
\param[in] log Optional pointer to the log output. By default null.
\return True on successful compilation. Otherwise the errors are written to the log output.
*/
bool Compile(const CompileConfig& config, std::ostream* log = nullptr);

/**
Assembles the specified assembly code to a byte code object.
\param[in,out] assembly Specifies the assembly input stream. This must be valid XASM code.
\param[in] log Optional pointer to the log output. By default null.
\return Unique pointer to the byte code object on successful assembling.
Otherwise the errors are written to the log output.
*/
VirtualMachine::ByteCodePtr Assemble(std::istream& assembly, std::ostream* log = nullptr);


/* --- Very High-Level Functions --- */

/**
Compiles XieXie code directly from the specified code string.
\param[in] codeString Specifies the XieXie code which is to be executed.
\param[in] compileFlags Specifies the compilation flags. By default CompileFlags::Default.
\remarks This is a 'very high-level' function, to quickly execute XieXie code.
\see CompileFlags
*/
VirtualMachine::ByteCodePtr CompileFromString(
    const std::string& codeString,
    int compileFlags = CompileFlags::Default,
    std::ostream* log = nullptr
);

/**
Compiles XieXie code directly from the specified file.
\param[in] codeFilename Specifies the file which contains the XieXie code to execute.
\see CompileFromString
*/
VirtualMachine::ByteCodePtr CompileFromFile(
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
