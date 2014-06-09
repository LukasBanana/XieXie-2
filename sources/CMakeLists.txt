
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

file(GLOB_RECURSE FilesAll				${PROJECT_SOURCE_DIR}/*.*)

#file(GLOB FilesBase					Base/*.*)


# === Source group folders ===

#source_group(
#	"Base" FILES
#	${FilesBase}
#)


# === Include directories ===

include_directories("${PROJECT_SOURCE_DIR}/Base")
include_directories("${PROJECT_SOURCE_DIR}/Compiler")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/Expressions")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/Lists")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/Literals")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/Objects")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/Operators")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/Statements")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/AbstractSyntaxTrees/TypeDenoters")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/CodeGenerator")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/ContextualAnalyzer")
include_directories("${PROJECT_SOURCE_DIR}/Compiler/SyntacticAnalyzer")
include_directories("${PROJECT_SOURCE_DIR}/Shell")
include_directories("${PROJECT_SOURCE_DIR}/VirtualMachine")


# === Executable ===

add_executable(xxc ${FilesAll})

set_target_properties(xxc PROPERTIES LINKER_LANGUAGE CXX)


