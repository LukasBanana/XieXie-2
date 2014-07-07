
# === CMake lists for the XieXie-2.0 Compiler - (09/06/2014) ===

cmake_minimum_required(VERSION 2.8)
project(XieXie2)


# === Build path ===

set(dir ${CMAKE_CURRENT_BINARY_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
set(LIBRARY_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)


# === Preprocessor definitions ===

add_definitions(-D_CRT_SECURE_NO_WARNINGS)


# === Global files ===

file(GLOB_RECURSE FilesBaseAll				${PROJECT_SOURCE_DIR}/Base/*.*)
file(GLOB_RECURSE FilesCompilerAll			${PROJECT_SOURCE_DIR}/Compiler/*.*)
file(GLOB_RECURSE FilesShellAll				${PROJECT_SOURCE_DIR}/Shell/*.*)
file(GLOB_RECURSE FilesVirtualMachineAll	${PROJECT_SOURCE_DIR}/VirtualMachine/*.*)

set(
	FilesAll
	
	${FilesBaseAll}
	${FilesCompilerAll}
	${FilesShellAll}
)


# === Source group folders ===

source_group(
	"Base" FILES
	${FilesBaseAll}
)

source_group(
	"Compiler" FILES
	${FilesCompilerAll}
)

source_group(
	"Shell" FILES
	${FilesShellAll}
)

source_group(
	"VirtualMachine" FILES
	${FilesVirtualMachineAll}
)


# === Include directories ===

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/Base")
include_directories("${PROJECT_SOURCE_DIR}/Compiler")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/Assembler")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/Expressions")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/Lists")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/Literals")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/Objects")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/Operators")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/Statements")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/TypeDenoters")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/CodeGenerator")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/ContextAnalyzer")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/SyntaxAnalyzer")
include_directories("${PROJECT_SOURCE_DIR}/Shell")
#include_directories("${PROJECT_SOURCE_DIR}/VirtualMachine")


# === Executable ===

add_executable(xxc ${FilesAll})
add_executable(xvm ${FilesVirtualMachineAll})

set_target_properties(xxc PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(xvm PROPERTIES LINKER_LANGUAGE C)


