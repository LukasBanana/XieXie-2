
# === CMake lists for the XieXie-2.0 Compiler - (09/06/2014) ===

cmake_minimum_required(VERSION 2.8)
project(XieXie2)


# === Build path ===

set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/build CACHE PATH "Build Directory")

set(PROJECT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/../include")


# === Preprocessor definitions ===

add_definitions(-D_CRT_SECURE_NO_WARNINGS)


# === Global files ===

file(GLOB_RECURSE FilesBaseAll					${PROJECT_SOURCE_DIR}/Base/*.*)
file(GLOB_RECURSE FilesCompilerAll				${PROJECT_SOURCE_DIR}/Compiler/*.*)
file(GLOB_RECURSE FilesShellAll					${PROJECT_SOURCE_DIR}/Shell/*.*)

file(GLOB FilesCompiler							${PROJECT_SOURCE_DIR}/Compiler/*.*)
file(GLOB FilesCompilerAssembler				${PROJECT_SOURCE_DIR}/Compiler/Assembler/*.*)
file(GLOB FilesCompilerAST						${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/*.*)
file(GLOB FilesCompilerASTExprs					${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/Expressions/*.*)
file(GLOB FilesCompilerASTStmnts				${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/Statements/*.*)
file(GLOB FilesCompilerASTTypeDenoters			${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/TypeDenoters/*.*)
file(GLOB FilesCompilerCodeGenerators			${PROJECT_SOURCE_DIR}/Compiler/CodeGenerators/*.*)
file(GLOB FilesCompilerCodeGeneratorsXASM		${PROJECT_SOURCE_DIR}/Compiler/CodeGenerators/XASM/*.*)
file(GLOB FilesCompilerContextAnalyzer			${PROJECT_SOURCE_DIR}/Compiler/ContextAnalyzer/*.*)
file(GLOB FilesCompilerOptimizer				${PROJECT_SOURCE_DIR}/Compiler/Optimizer/*.*)
file(GLOB FilesCompilerSyntaxAnalyzer			${PROJECT_SOURCE_DIR}/Compiler/SyntaxAnalyzer/*.*)
file(GLOB FilesCompilerTAC						${PROJECT_SOURCE_DIR}/Compiler/ThreeAddressCodes/*.*)
file(GLOB FilesCompilerCFG						${PROJECT_SOURCE_DIR}/Compiler/ControlFlowGraph/*.*)

file(GLOB FilesShell							${PROJECT_SOURCE_DIR}/Shell/*.*)
file(GLOB FilesShellCommand						${PROJECT_SOURCE_DIR}/Shell/Command/*.*)

file(GLOB FilesPlatform ${PROJECT_SOURCE_DIR}/Platform/*.*)
if(WIN32)
	file(GLOB FilesPlatformDependent ${PROJECT_SOURCE_DIR}/Platform/Win32/*.*)
elseif(UNIX)
	file(GLOB FilesPlatformDependent ${PROJECT_SOURCE_DIR}/Platform/Linux/*.*)
else()
	file(GLOB FilesPlatformDependent ${PROJECT_SOURCE_DIR}/Platform/Null/*.*)
endif()

set(
	FilesAll
	
	${FilesBaseAll}
	${FilesCompilerAll}
	${FilesShellAll}
	${FilesPlatform}
	${FilesPlatformDependent}
)

set(
	FilesXVM
	${PROJECT_SOURCE_DIR}/VirtualMachine/xvm.c
	${PROJECT_INCLUDE_DIR}/xiexie/xvm.h
)

set(
	FilesXVMShell
	${PROJECT_SOURCE_DIR}/VirtualMachine/xvm_shell.c
)

set(
	FilesXVMWrapper
	${PROJECT_INCLUDE_DIR}/xiexie/xvm_wrapper.h
	${PROJECT_SOURCE_DIR}/VirtualMachine/xvm_wrapper.cpp
)


# === Source group folders ===

# --- Base ---

source_group("Base" FILES ${FilesBaseAll})

# --- Compiler ---

source_group("Compiler" FILES ${FilesCompiler})
source_group("Compiler\\Assembler" FILES ${FilesCompilerAssembler})

source_group("Compiler\\AbstractSyntaxTrees" FILES ${FilesCompilerAST})
source_group("Compiler\\AbstractSyntaxTrees\\Expressions" FILES ${FilesCompilerASTExprs})
source_group("Compiler\\AbstractSyntaxTrees\\Statements" FILES ${FilesCompilerASTStmnts})
source_group("Compiler\\AbstractSyntaxTrees\\TypeDenoters" FILES ${FilesCompilerASTTypeDenoters})
source_group("Compiler\\CodeGenerators" FILES ${FilesCompilerCodeGenerators})
source_group("Compiler\\CodeGenerators\\XASM" FILES ${FilesCompilerCodeGeneratorsXASM})
source_group("Compiler\\ContextAnalyzer" FILES ${FilesCompilerContextAnalyzer})
source_group("Compiler\\Optimizer" FILES ${FilesCompilerOptimizer})
source_group("Compiler\\SyntaxAnalyzer" FILES ${FilesCompilerSyntaxAnalyzer})
source_group("Compiler\\ThreeAddressCodes" FILES ${FilesCompilerTAC})
source_group("Compiler\\ControlFlowGraph" FILES ${FilesCompilerCFG})

# --- Shell ---

source_group("Shell" FILES ${FilesShell})
source_group("Shell\\Command" FILES ${FilesShellCommand})

# --- Platform ---

source_group("Platform" FILES ${FilesPlatform} ${FilesPlatformDependent})

# --- Virtual Machine ---

source_group(
	"VirtualMachine" FILES
	${FilesXVM}
	${FilesXVMShell}
	${FilesXVMWrapper}
)


# === Include directories ===

include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/Base")
include_directories("${PROJECT_SOURCE_DIR}/Compiler")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/Assembler")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/Expressions")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/Statements")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/TypeDenoters")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/CodeGenerator")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/ContextAnalyzer")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/SyntaxAnalyzer")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/ThreeAddressCodes")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/ControlFlowGraph")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/Optimizer")
include_directories("${PROJECT_SOURCE_DIR}/Platform")
include_directories("${PROJECT_SOURCE_DIR}/Shell")
include_directories("${PROJECT_SOURCE_DIR}/Shell/Command")

# === Executable ===

add_executable(xxc ${FilesAll})
add_executable(xvm ${FilesXVMShell})
add_library(xvmlib STATIC ${FilesXVM})
add_library(xvm_wrapper STATIC ${FilesXVMWrapper})

set_target_properties(
	xxc PROPERTIES
	LINKER_LANGUAGE CXX
	DEBUG_POSTFIX "D"
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR}"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR}"
	LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR}"
	LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR}"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR}"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR}"
)

set_target_properties(
	xvmlib PROPERTIES
	LINKER_LANGUAGE C
	DEBUG_POSTFIX "D"
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR}"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR}"
	LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR}"
	LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR}"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR}"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR}"
)

set_target_properties(
	xvm PROPERTIES
	LINKER_LANGUAGE C
	DEBUG_POSTFIX "D"
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR}"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR}"
	LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR}"
	LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR}"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR}"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR}"
)

set_target_properties(
	xvm_wrapper PROPERTIES
	LINKER_LANGUAGE CXX
	DEBUG_POSTFIX "D"
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR}"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR}"
	LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR}"
	LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR}"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR}"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR}"
)

target_link_libraries(xvm xvmlib)
target_link_libraries(xvm_wrapper xvmlib)
target_link_libraries(xxc xvm_wrapper)

if(UNIX)
	target_link_libraries(xvm_wrapper dl m)
	target_link_libraries(xvm dl m)
	target_link_libraries(xvmlib dl m)
	target_link_libraries(xxc dl m)
endif()


