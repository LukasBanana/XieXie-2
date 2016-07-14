
# === CMake lists for the XieXie-2.0 Compiler - (09/06/2014) ===

cmake_minimum_required(VERSION 2.8)
project(XieXie2)


# === Build path ===

set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/build CACHE PATH "Build Directory")
set(MODULES_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/../modules")

set(PROJECT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/../include")
set(PROJECT_MODULES_DIR "${PROJECT_SOURCE_DIR}/Modules")


# === Preprocessor definitions ===

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

macro(SETUP_LIB_OUTPUT LIB_NAME COMPILER_IDENT)
	set_target_properties(
		${LIB_NAME} PROPERTIES
		LINKER_LANGUAGE ${COMPILER_IDENT}
		DEBUG_POSTFIX "D"
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR}"
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR}"
		RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR}"
		RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR}"
	)
	target_compile_features(
		${LIB_NAME} PRIVATE
		cxx_range_for cxx_lambdas cxx_generic_lambdas
	)
endmacro()

macro(SETUP_STD_C_MODULE MODULE_NAME)
	add_library("Module_${MODULE_NAME}" SHARED "${PROJECT_MODULES_DIR}/${MODULE_NAME}/${MODULE_NAME}.c")
	set_target_properties(
		"Module_${MODULE_NAME}" PROPERTIES
		LINKER_LANGUAGE C
		DEBUG_POSTFIX "D"
		OUTPUT_NAME ${MODULE_NAME}
		RUNTIME_OUTPUT_DIRECTORY_RELEASE "${MODULES_OUTPUT_DIR}/${MODULE_NAME}"
	)
	target_link_libraries("Module_${MODULE_NAME}" xvm_module)
endmacro()

macro(SETUP_CPP_MODULE MODULE_NAME)
	set_target_properties(
		"Module_${MODULE_NAME}" PROPERTIES
		LINKER_LANGUAGE CXX
		DEBUG_POSTFIX "D"
		OUTPUT_NAME ${MODULE_NAME}
		RUNTIME_OUTPUT_DIRECTORY_RELEASE "${MODULES_OUTPUT_DIR}/${MODULE_NAME}"
	)
	target_link_libraries("Module_${MODULE_NAME}" xvm_module)
endmacro()


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

file(GLOB FilesModuleBigNumDetails				${PROJECT_MODULES_DIR}/BigNum/Details/*.*)

file(GLOB FilesPlatform ${PROJECT_SOURCE_DIR}/Platform/*.*)
if(WIN32)
	file(GLOB FilesPlatformDependent ${PROJECT_SOURCE_DIR}/Platform/Win32/*.*)
elseif(UNIX)
	file(GLOB FilesPlatformDependent ${PROJECT_SOURCE_DIR}/Platform/Linux/*.*)
else()
	file(GLOB FilesPlatformDependent ${PROJECT_SOURCE_DIR}/Platform/Null/*.*)
endif()

set(
	FilesXXC
	${FilesBaseAll}
	${FilesCompilerAll}
	${FilesPlatform}
	${FilesPlatformDependent}
)

set(
	FilesXXCShell
	${FilesShellAll}
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

set(
	FilesXVMModule
	${PROJECT_INCLUDE_DIR}/xiexie/xvm_module.h
	${PROJECT_MODULES_DIR}/xvm_module.c
)

set(FilesCoreAsmGen ${PROJECT_SOURCE_DIR}/Tools/CoreAssemblyGenerator.cpp)

set(
	FilesModuleBigNum
	${PROJECT_MODULES_DIR}/BigNum/BigNum.cpp
	${FilesModuleBigNumDetails}
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

# --- Tools ---

source_group(
	"Tools" FILES
	${FilesCoreAsmGen}
)

# --- Platform ---

source_group("Platform" FILES ${FilesPlatform} ${FilesPlatformDependent})

# --- Virtual Machine ---

source_group(
	"VirtualMachine" FILES
	${FilesXVM}
	${FilesXVMShell}
	${FilesXVMWrapper}
)

# --- Modules ---

source_group(
	"Modules" FILES
	${FilesXVMModule}
	${PROJECT_MODULES_DIR}/MsgBox/MsgBox.c
	${PROJECT_MODULES_DIR}/Socket/Socket.c
	${PROJECT_MODULES_DIR}/SysInfo/SysInfo.c
	${PROJECT_MODULES_DIR}/BigNum/BigNum.cpp
	${PROJECT_MODULES_DIR}/Audio/Audio.cpp
)

source_group(
	"Modules\\Details" FILES
	${FilesModuleBigNumDetails}
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


# === Binaries ===

add_executable(xxc ${FilesXXCShell})
add_executable(xvm ${FilesXVMShell})

add_library(xxclib STATIC ${FilesXXC})
add_library(xvmlib STATIC ${FilesXVM})
add_library(xvm_wrapper STATIC ${FilesXVMWrapper})
add_library(xvm_module STATIC ${FilesXVMModule})

SETUP_STD_C_MODULE(MsgBox)
SETUP_STD_C_MODULE(Socket)
SETUP_STD_C_MODULE(SysInfo)
SETUP_STD_C_MODULE(Audio)

add_library(Module_BigNum SHARED ${FilesModuleBigNum})
SETUP_CPP_MODULE(BigNum)

SETUP_LIB_OUTPUT(xxc CXX)
SETUP_LIB_OUTPUT(xxclib CXX)
SETUP_LIB_OUTPUT(xvmlib C)
SETUP_LIB_OUTPUT(xvm C)
SETUP_LIB_OUTPUT(xvm_wrapper CXX)
SETUP_LIB_OUTPUT(xvm_module C)

add_executable(CoreAssemblyGenerator ${FilesCoreAsmGen})


# === Additional Link Libraries ===

set_target_properties(
	CoreAssemblyGenerator PROPERTIES
	LINKER_LANGUAGE CXX
	DEBUG_POSTFIX "D"
)

target_link_libraries(xvm_wrapper xvmlib)
target_link_libraries(xxclib xvm_wrapper)
target_link_libraries(xvm xvmlib)
target_link_libraries(xxc xxclib)

if(WIN32)
	target_link_libraries(Module_Socket wsock32 ws2_32)
	target_link_libraries(Module_Audio winmm)
endif()

if(UNIX)
	target_link_libraries(xxc dl m)
	target_link_libraries(xxclib dl m)
	target_link_libraries(xvm dl m)
	target_link_libraries(xvmlib dl m)
	target_link_libraries(xvm_wrapper dl m)
	target_link_libraries(xvm_module dl m)
endif()


