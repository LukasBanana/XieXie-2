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
static BuiltinTypeDenoterPtr&& GenBuiltinType(const BuiltinTypeDenoter::TypeNames type)
{
    return std::move(std::make_shared<BuiltinTypeDenoter>(type));
}

// Generate void type-denoter AST
static BuiltinTypeDenoterPtr&& Void()
{
    return GenBuiltinType(BuiltinTypeDenoter::TypeNames::Void);
}

// Generate boolean type-denoter AST
static BuiltinTypeDenoterPtr&& Bool()
{
    return GenBuiltinType(BuiltinTypeDenoter::TypeNames::Bool);
}

// Generate integer type-denoter AST
static BuiltinTypeDenoterPtr&& Int()
{
    return GenBuiltinType(BuiltinTypeDenoter::TypeNames::Int);
}

// Generate float type-denoter AST
static BuiltinTypeDenoterPtr&& Float()
{
    return GenBuiltinType(BuiltinTypeDenoter::TypeNames::Float);
}

// Generate pointer type-denoter AST
static PointerTypeDenoterPtr&& GenPointerType(const std::string& ident, bool isWeakRef = false)
{
    auto ast = std::make_shared<PointerTypeDenoter>();
    {
        ast->declIdent  = ident;
        ast->isWeakRef  = isWeakRef;
    }
    return std::move(ast);
}

// Generate "Object" pointer type-denoter AST
static PointerTypeDenoterPtr&& Object(bool isWeakRef = false)
{
    return GenPointerType("Object", isWeakRef);
}

// Generate "String" pointer type-denoter AST
static PointerTypeDenoterPtr&& String(bool isWeakRef = false)
{
    return GenPointerType("String", isWeakRef);
}


/*
 * Procedure and parameter generation functions
 */

// Generate literal expression AST
static ExprPtr&& GenExpr(int value)
{
    auto ast = std::make_shared<LiteralExpr>();
    {
        ast->typeDenoter    = Int();
        ast->value          = std::to_string(value);
    }
    return std::move(ast);
}

// Generate parameter AST
static ParamPtr&& GenParam(const TypeDenoterPtr& type, const std::string& ident, ExprPtr&& defaultArg = nullptr)
{
    auto ast = std::make_shared<Param>();
    {
        ast->typeDenoter    = type;
        ast->ident          = ident;
        ast->defaultArgExpr = defaultArg;
    }
    return std::move(ast);
}

// Generate procedure signature AST
static ProcSignaturePtr&& GenProcSignature(const TypeDenoterPtr& returnType, const std::string& ident, const std::vector<ParamPtr>& params)
{
    auto ast = std::make_shared<ProcSignature>();
    {
        ast->returnTypeDenoter  = returnType;
        ast->ident              = ident;
        ast->params             = params;
    }
    return std::move(ast);
}

// Generate member procedure AST
static void GenMemberProc(std::vector<StmntPtr>& declStmnts, const TypeDenoterPtr& returnType, const std::string& ident, std::vector<ParamPtr>&& params = {})
{
    auto ast = std::make_shared<ProcDeclStmnt>();
    {
        ast->procSignature = GenProcSignature(returnType, ident, params);
    }
    declStmnts.push_back(ast);
}

// Generate static procedure AST
static void GenStaticProc(std::vector<StmntPtr>& declStmnts, const TypeDenoterPtr& returnType, const std::string& ident, std::vector<ParamPtr>&& params = {})
{
    auto ast = std::make_shared<ProcDeclStmnt>();
    {
        ast->procSignature = GenProcSignature(returnType, ident, params);
        ast->procSignature->isStatic = true;
    }
    declStmnts.push_back(ast);
}

// Generate initializer procedure AST
static void GenInitProc(std::vector<StmntPtr>& declStmnts, std::vector<ParamPtr>&& params = {})
{
    auto ast = std::make_shared<InitDeclStmnt>();
    {
        ast->params = params;
    }
    declStmnts.push_back(ast);
}

// Generate class declaration statement AST
static std::unique_ptr<ClassDeclStmnt> GenClass(const std::string& ident, ClassDeclStmnt* superClass)
{
    auto ast = MakeUnique<ClassDeclStmnt>();
    {
        ast->isBuiltin  = true;
        ast->isExtern   = true;
        ast->ident      = ident;

        if (superClass)
            ast->baseClassIdent = superClass->ident;
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
    auto ast = GenClass("Object", nullptr);
    auto& stmnts = ast->publicSegment.declStmnts;

    GenMemberProc(stmnts, Int(), "typeID");
    GenMemberProc(stmnts, Int(), "refCount");
    GenMemberProc(stmnts, Bool(), "equals", { GenParam(Object(), "rhs") });
    GenMemberProc(stmnts, String(), "toString");

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
static std::unique_ptr<ClassDeclStmnt> GenStringClass(ClassDeclStmnt* superClass)
{
    auto ast = GenClass("String", superClass);
    auto& stmnts = ast->publicSegment.declStmnts;

    GenInitProc(stmnts);
    GenInitProc(stmnts, { GenParam(String(), "src") });

    GenMemberProc(stmnts, Bool(), "equals", { GenParam(Object(), "rhs") });
    GenMemberProc(stmnts, String(), "toString");
    GenMemberProc(stmnts, String(), "copy");
    GenMemberProc(stmnts, Int(), "size");
    GenMemberProc(stmnts, Int(), "resize", { GenParam(Int(), "size") });
    GenMemberProc(stmnts, Bool(), "empty");
    GenMemberProc(stmnts, String(), "add", { GenParam(String(), "rhs") });
    GenMemberProc(stmnts, String(), "subString", { GenParam(Int(), "pos"), GenParam(Int(), "len", GenExpr(-1)) });
    GenMemberProc(stmnts, Void(), "setChar", { GenParam(Int(), "pos"), GenParam(Int(), "char") });
    GenMemberProc(stmnts, Int(), "getChar", { GenParam(Int(), "pos") });
    GenMemberProc(stmnts, Int(), "pointer");

    return ast;
}

static std::unique_ptr<ClassDeclStmnt> GenGenericArrayClass(const std::string& ident, ClassDeclStmnt* superClass)
{
    auto ast = GenClass(ident, superClass);
    auto& stmnts = ast->publicSegment.declStmnts;

    //...

    return ast;
}

static std::unique_ptr<ClassDeclStmnt> GenArrayClass(ClassDeclStmnt* superClass)
{
    return GenGenericArrayClass("Array", superClass);
}

static std::unique_ptr<ClassDeclStmnt> GenPrimArrayClass(ClassDeclStmnt* superClass)
{
    return GenGenericArrayClass("PrimArray", superClass);
}

static std::unique_ptr<ClassDeclStmnt> GenBoolArrayClass(ClassDeclStmnt* superClass)
{
    return GenGenericArrayClass("BoolArray", superClass);
}

static std::unique_ptr<ClassDeclStmnt> GenBufferClass(ClassDeclStmnt* superClass)
{
    auto ast = GenClass("Buffer", superClass);
    auto& stmnts = ast->publicSegment.declStmnts;

    //...

    return ast;
}

static std::unique_ptr<ClassDeclStmnt> GenIntrinsicsClass(ClassDeclStmnt* superClass)
{
    auto ast = GenClass("Intrinsics", superClass);
    auto& stmnts = ast->publicSegment.declStmnts;

    //...

    return ast;
}


/*
 * Global Functions
 */

void GenerateBuiltinClasses(std::vector<std::unique_ptr<ClassDeclStmnt>>& classDeclList)
{
    auto rootClass = GenObjectClass();
    auto rootClassRef = rootClass.get();

    classDeclList.push_back(std::move(rootClass));
    classDeclList.push_back(GenStringClass(rootClassRef));
    classDeclList.push_back(GenArrayClass(rootClassRef));
    classDeclList.push_back(GenPrimArrayClass(rootClassRef));
    classDeclList.push_back(GenBoolArrayClass(rootClassRef));
    classDeclList.push_back(GenBufferClass(rootClassRef));
    classDeclList.push_back(GenIntrinsicsClass(rootClassRef));
}


} // /namespace StdClassGenerator

} // /namespace ContextAnalyzer



// ================================================================================