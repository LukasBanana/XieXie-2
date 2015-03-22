
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

file(GLOB_RECURSE FilesBaseAll					${PROJECT_SOURCE_DIR}/Base/*.*)
file(GLOB_RECURSE FilesCompilerAll				${PROJECT_SOURCE_DIR}/Compiler/*.*)
file(GLOB_RECURSE FilesShellAll					${PROJECT_SOURCE_DIR}/Shell/*.*)
file(GLOB_RECURSE FilesVirtualMachineAll		${PROJECT_SOURCE_DIR}/VirtualMachine/*.*)

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

source_group("VirtualMachine" FILES ${FilesVirtualMachineAll})


# === Include directories ===

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
add_executable(xvm ${FilesVirtualMachineAll})

set_target_properties(xxc PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(xvm PROPERTIES LINKER_LANGUAGE C)

if(UNIX)
	target_link_libraries(xvm dl m)
	target_link_libraries(xxc dl m)
endif()


