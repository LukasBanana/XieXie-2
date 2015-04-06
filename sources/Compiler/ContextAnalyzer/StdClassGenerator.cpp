/*
 * StdClassGenerator.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "StdClassGenerator.h"
#include "MakeUnique.h"
#include "ProcDeclStmnt.h"
#include "ProcSignature.h"
#include "Param.h"
#include "LiteralExpr.h"
#include "BuiltinTypeDenoter.h"
#include "PointerTypeDenoter.h"
#include "InitDeclStmnt.h"


namespace ContextAnalyzer
{

namespace StdClassGenerator
{


using namespace AbstractSyntaxTrees;

/*
 * Type denoter generation functions
 */

// Generate built-in type-denoter AST
static BuiltinTypeDenoterPtr GenBuiltinType(const BuiltinTypeDenoter::TypeNames type)
{
    return std::make_shared<BuiltinTypeDenoter>(type);
}

// Generate void type-denoter AST
static BuiltinTypeDenoterPtr Void()
{
    return GenBuiltinType(BuiltinTypeDenoter::TypeNames::Void);
}

// Generate boolean type-denoter AST
static BuiltinTypeDenoterPtr Bool()
{
    return GenBuiltinType(BuiltinTypeDenoter::TypeNames::Bool);
}

// Generate integer type-denoter AST
static BuiltinTypeDenoterPtr Int()
{
    return GenBuiltinType(BuiltinTypeDenoter::TypeNames::Int);
}

// Generate float type-denoter AST
static BuiltinTypeDenoterPtr Float()
{
    return GenBuiltinType(BuiltinTypeDenoter::TypeNames::Float);
}

// Generate pointer type-denoter AST
static PointerTypeDenoterPtr GenPointerType(const std::string& ident, bool isWeakRef = false)
{
    auto ast = std::make_shared<PointerTypeDenoter>();
    {
        ast->declIdent  = ident;
        ast->isWeakRef  = isWeakRef;
    }
    return ast;
}

// Generate "Object" pointer type-denoter AST
static PointerTypeDenoterPtr Object(bool isWeakRef = false)
{
    return GenPointerType("Object", isWeakRef);
}

// Generate "String" pointer type-denoter AST
static PointerTypeDenoterPtr String(bool isWeakRef = false)
{
    return GenPointerType("String", isWeakRef);
}


/*
 * Procedure and parameter generation functions
 */

// Workaround of '{ std::shared_ptr ... }' due to MSVC12 bug!
struct ParamList
{
    ParamList() = default;
    ParamList(const ParamPtr& param)
    {
        list.push_back(param);
    }

    ParamList& operator , (const ParamPtr& param)
    {
        list.push_back(param);
        return *this;
    }

    std::vector<ParamPtr> list;
};

// Generate literal expression AST
static ExprPtr GenExpr(int value)
{
    auto ast = std::make_shared<LiteralExpr>();
    {
        ast->typeDenoter    = Int();
        ast->value          = std::to_string(value);
    }
    return ast;
}

// Generate parameter AST
static ParamPtr GenParam(const TypeDenoterPtr& type, const std::string& ident, const ExprPtr& defaultArg = nullptr)
{
    auto ast = std::make_shared<Param>();
    {
        ast->typeDenoter    = type;
        ast->ident          = ident;
        ast->defaultArgExpr = defaultArg;
    }
    return ast;
}

// Generate procedure signature AST
static ProcSignaturePtr GenProcSignature(const TypeDenoterPtr& returnType, const std::string& ident, const ParamList& params)
{
    auto ast = std::make_shared<ProcSignature>();
    {
        ast->returnTypeDenoter  = returnType;
        ast->ident              = ident;
        ast->params             = params.list;
    }
    return ast;
}

// Generate member procedure AST
static void GenMemberProc(ClassDeclStmnt& classDecl, const TypeDenoterPtr& returnType, const std::string& ident, const ParamList& params = ParamList())
{
    auto ast = std::make_shared<ProcDeclStmnt>();
    {
        ast->procSignature  = GenProcSignature(returnType, ident, params);
        ast->parentRef      = &classDecl;
    }
    classDecl.publicSegment.declStmnts.push_back(ast);
}

// Generate static procedure AST
static void GenStaticProc(ClassDeclStmnt& classDecl, const TypeDenoterPtr& returnType, const std::string& ident, const ParamList& params = ParamList())
{
    auto ast = std::make_shared<ProcDeclStmnt>();
    {
        ast->procSignature  = GenProcSignature(returnType, ident, params);
        ast->parentRef      = &classDecl;
        ast->procSignature->isStatic = true;
    }
    classDecl.publicSegment.declStmnts.push_back(ast);
}

// Generate initializer procedure AST
static void GenInitProc(ClassDeclStmnt& classDecl, const ParamList& params = {})
{
    auto ast = std::make_shared<InitDeclStmnt>();
    {
        ast->params = params.list;
    }
    classDecl.publicSegment.declStmnts.push_back(ast);
}

// Generate class declaration statement AST
static std::unique_ptr<ClassDeclStmnt> GenClass(const std::string& ident)
{
    auto ast = MakeUnique<ClassDeclStmnt>();
    {
        ast->isBuiltin  = true;
        ast->isExtern   = true;
        ast->ident      = ident;
    }
    return ast;
}


/*
 * Built-in Classes
 */

/*
extern class Object {
    int typeID()
    int refCount()
    bool equals(Object rhs)
    String toString()
}
*/
static std::unique_ptr<ClassDeclStmnt> GenObjectClass()
{
    auto ast = GenClass("Object");

    GenMemberProc(*ast, Int(), "typeID");
    GenMemberProc(*ast, Int(), "refCount");
    GenMemberProc(*ast, Bool(), "equals", GenParam(Object(), "rhs"));
    GenMemberProc(*ast, String(), "toString");

    return ast;
}

/*
extern class String {
    init()
    init(String src)
    bool equals(Object rhs)
    String toString()
    String copy()
    int size()
    void resize(int size)
    bool empty()
    String add(String rhs)
    String subString(int pos, int len := -1)
    void setChar(int pos, int char)
    int getChar(int pos)
    int pointer()
}
*/
static std::unique_ptr<ClassDeclStmnt> GenStringClass()
{
    auto ast = GenClass("String");

    GenInitProc(*ast);
    GenInitProc(*ast, GenParam(String(), "src"));

    GenMemberProc(*ast, Bool(), "equals", GenParam(Object(), "rhs"));
    GenMemberProc(*ast, String(), "toString");
    GenMemberProc(*ast, String(), "copy");
    GenMemberProc(*ast, Int(), "size");
    GenMemberProc(*ast, Int(), "resize", GenParam(Int(), "size"));
    GenMemberProc(*ast, Bool(), "empty");
    GenMemberProc(*ast, String(), "add", GenParam(String(), "rhs"));
    GenMemberProc(*ast, String(), "subString", ( GenParam(Int(), "pos"), GenParam(Int(), "len", GenExpr(-1)) ));
    GenMemberProc(*ast, Void(), "setChar", ( GenParam(Int(), "pos"), GenParam(Int(), "char") ));
    GenMemberProc(*ast, Int(), "getChar", GenParam(Int(), "pos"));
    GenMemberProc(*ast, Int(), "pointer");

    return ast;
}

static std::unique_ptr<ClassDeclStmnt> GenGenericArrayClass(const std::string& ident)
{
    auto ast = GenClass(ident);

    GenMemberProc(*ast, Bool(), "equals", GenParam(Object(), "rhs"));
    //GenMemberProc(*ast, GenericArray(), "copy");
    GenMemberProc(*ast, Int(), "size");
    GenMemberProc(*ast, Int(), "resize", GenParam(Int(), "size"));
    GenMemberProc(*ast, Bool(), "empty");

    return ast;
}

static std::unique_ptr<ClassDeclStmnt> GenArrayClass()
{
    return GenGenericArrayClass("Array");
}

static std::unique_ptr<ClassDeclStmnt> GenPrimArrayClass()
{
    return GenGenericArrayClass("PrimArray");
}

static std::unique_ptr<ClassDeclStmnt> GenBoolArrayClass()
{
    return GenGenericArrayClass("BoolArray");
}

static std::unique_ptr<ClassDeclStmnt> GenBufferClass()
{
    auto ast = GenClass("Buffer");

    //...

    return ast;
}

static std::unique_ptr<ClassDeclStmnt> GenIntrinsicsClass()
{
    auto ast = GenClass("Intrinsics");

    //...

    return ast;
}


/*
 * Global Functions
 */

void GenerateBuiltinClasses(std::vector<std::unique_ptr<ClassDeclStmnt>>& classDeclList)
{
    classDeclList.push_back(GenObjectClass());
    classDeclList.push_back(GenStringClass());
    classDeclList.push_back(GenArrayClass());
    classDeclList.push_back(GenPrimArrayClass());
    classDeclList.push_back(GenBoolArrayClass());
    classDeclList.push_back(GenBufferClass());
    classDeclList.push_back(GenIntrinsicsClass());
}

void GenerateBuiltinClasses(AbstractSyntaxTrees::Program& program)
{
    std::vector<std::unique_ptr<ClassDeclStmnt>> classDeclList;
    GenerateBuiltinClasses(classDeclList);

    for (auto& classDecl : classDeclList)
        program.classDeclStmnts.push_back(std::move(classDecl));
}


} // /namespace StdClassGenerator

} // /namespace ContextAnalyzer



// ================================================================================