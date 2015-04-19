/*
 * StdCodeFactory.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "StdCodeFactory.h"
#include "MakeUnique.h"
#include "ProcDeclStmnt.h"
#include "ProcSignature.h"
#include "Param.h"
#include "LiteralExpr.h"
#include "BuiltinTypeDenoter.h"
#include "PointerTypeDenoter.h"
#include "Attrib.h"
#include "AttribPrefix.h"


namespace ContextAnalyzer
{

namespace StdCodeFactory
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
 * Attribute generation functions
 */

static AttribPtr GenAttrib(const std::string& ident)
{
    auto attrib = std::make_shared<Attrib>();
    attrib->ident = ident;
    return attrib;
}

static AttribPrefixPtr GenAttribPrefix(const std::initializer_list<std::string>& attribIdents)
{
    auto attribPrefix = std::make_shared<AttribPrefix>();
    for (const auto& ident : attribIdents)
        attribPrefix->attribs.push_back(GenAttrib(ident));
    return attribPrefix;
}

// Generates the attribute prefix: [[final]]
static AttribPrefixPtr AttrFinal()
{
    return GenAttribPrefix({ "final" });
}

// Generates the attribute prefix: [[override]]
static AttribPrefixPtr AttrOverride()
{
    return GenAttribPrefix({ "override" });
}

// Generates the attribute prefix: [[override, final]]
static AttribPrefixPtr AttrOverrideFinal()
{
    return GenAttribPrefix({ "override", "final" });
}


/*
 * Procedure and parameter generation functions
 */

// Workaround of '{ std::shared_ptr ... }' due to MSVC12 bug!
struct ParamList
{
    ParamList() = default;
    ParamList(ParamList&& rhs) :
        list{ std::move(rhs.list) }
    {
    }
    ParamList(const ParamPtr& param)
    {
        list.push_back(param);
    }

    ParamList& operator , (const ParamList& param)
    {
        list.insert(list.end(), param.list.begin(), param.list.end());
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
static ParamList GenParam(const TypeDenoterPtr& type, const std::string& ident, const ExprPtr& defaultArg = nullptr)
{
    auto ast = std::make_shared<Param>();
    {
        ast->typeDenoter    = type;
        ast->ident          = ident;
        ast->defaultArgExpr = defaultArg;
    }
    return ParamList(ast);
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
static void GenMemberProc(
    ClassDeclStmnt& classDecl, const TypeDenoterPtr& returnType, const std::string& ident,
    const ParamList& params = ParamList(), const AttribPrefixPtr& attribPrefix = nullptr)
{
    auto ast = std::make_shared<ProcDeclStmnt>();
    {
        ast->procSignature  = GenProcSignature(returnType, ident, params);
        ast->attribPrefix   = attribPrefix;
        ast->parentRef      = &classDecl;
    }
    classDecl.declStmnts.push_back(ast);
}

// Generate static procedure AST
static void GenStaticProc(
    ClassDeclStmnt& classDecl, const TypeDenoterPtr& returnType, const std::string& ident,
    const ParamList& params = ParamList(), const AttribPrefixPtr& attribPrefix = nullptr)
{
    auto ast = std::make_shared<ProcDeclStmnt>();
    {
        ast->procSignature              = GenProcSignature(returnType, ident, params);
        ast->procSignature->isStatic    = true;
        ast->attribPrefix               = attribPrefix;
        ast->parentRef                  = &classDecl;
    }
    classDecl.declStmnts.push_back(ast);
}

// Generate constructor procedure AST
static void GenInitProc(ClassDeclStmnt& classDecl, const ParamList& params = {})
{
    auto ast = std::make_shared<ProcDeclStmnt>();
    {
        ast->procSignature          = GenProcSignature(nullptr, "init", params);
        ast->procSignature->isCtor  = true;
        ast->parentRef              = &classDecl;
    }
    classDecl.declStmnts.push_back(ast);
}

// Generate destructor procedure AST
static void GenReleaseProc(ClassDeclStmnt& classDecl)
{
    auto ast = std::make_shared<ProcDeclStmnt>();
    {
        ast->procSignature          = GenProcSignature(nullptr, "release", ParamList());
        ast->procSignature->isDtor  = true;
        ast->parentRef              = &classDecl;
    }
    classDecl.declStmnts.push_back(ast);
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
    int refCount()
    int typeID()
    bool equals(Object rhs)
    String toString()
    int pointer()
}
*/
static std::unique_ptr<ClassDeclStmnt> GenObjectClass()
{
    auto ast = GenClass("Object");

    GenReleaseProc(*ast);
    GenInitProc(*ast);

    GenMemberProc(*ast, Int(), "refCount", ParamList(), AttrFinal());
    GenMemberProc(*ast, Int(), "typeID", ParamList(), AttrFinal());
    GenMemberProc(*ast, Bool(), "equals", GenParam(Object(), "rhs"));
    GenMemberProc(*ast, String(), "toString");
    GenMemberProc(*ast, Int(), "pointer");

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

    GenReleaseProc(*ast);
    GenInitProc(*ast);
    GenInitProc(*ast, GenParam(Int(), "size"));
    GenInitProc(*ast, GenParam(String(), "src"));

    GenMemberProc(*ast, Bool(), "equals", GenParam(Object(), "rhs"), AttrOverride());
    GenMemberProc(*ast, String(), "toString", ParamList(), AttrOverride());
    GenMemberProc(*ast, String(), "copy");
    GenMemberProc(*ast, Int(), "size");
    GenMemberProc(*ast, Void(), "resize", GenParam(Int(), "size"));
    GenMemberProc(*ast, Void(), "adjust");
    GenMemberProc(*ast, Bool(), "empty");
    GenMemberProc(*ast, String(), "add", GenParam(String(), "rhs"));
    GenMemberProc(*ast, String(), "add", GenParam(Bool(), "rhs"));
    GenMemberProc(*ast, String(), "add", GenParam(Int(), "rhs"));
    GenMemberProc(*ast, String(), "add", GenParam(Float(), "rhs"));
    GenMemberProc(*ast, String(), "subString", ( GenParam(Int(), "pos"), GenParam(Int(), "len", GenExpr(-1)) ));
    GenMemberProc(*ast, Void(), "setChar", ( GenParam(Int(), "pos"), GenParam(Int(), "char") ));
    GenMemberProc(*ast, Int(), "getChar", GenParam(Int(), "pos"));
    GenMemberProc(*ast, Int(), "pointer", ParamList(), AttrOverrideFinal());

    return ast;
}

static std::unique_ptr<ClassDeclStmnt> GenGenericArrayClass(const std::string& ident)
{
    auto ast = GenClass(ident);

    GenReleaseProc(*ast);
    GenInitProc(*ast);
    GenInitProc(*ast, GenParam(Int(), "size"));

    GenMemberProc(*ast, Bool(), "equals", GenParam(Object(), "rhs"), AttrOverride());
    //GenMemberProc(*ast, GenericArray(), "copy");
    GenMemberProc(*ast, Int(), "size");
    GenMemberProc(*ast, Int(), "resize", GenParam(Int(), "size"));
    GenMemberProc(*ast, Bool(), "empty");
    GenMemberProc(*ast, Int(), "pointer", ParamList(), AttrOverrideFinal());

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

    auto voidType   = Void();
    auto intType    = Int();
    auto floatType  = Float();

    GenStaticProc(*ast, intType, "allocMem", GenParam(intType, "size"));
    GenStaticProc(*ast, voidType, "freeMem", GenParam(intType, "ptr"));
    GenStaticProc(*ast, voidType, "copyMem", ( GenParam(intType, "destPtr"), GenParam(intType, "srcPtr"), GenParam(intType, "size") ));
    
    GenStaticProc(*ast, voidType, "sysCall", GenParam(intType, "commandPtr"));
    GenStaticProc(*ast, voidType, "clearTerm");
    GenStaticProc(*ast, voidType, "print", GenParam(intType, "textPtr"));
    GenStaticProc(*ast, voidType, "printLn", GenParam(intType, "textPtr"));
    GenStaticProc(*ast, voidType, "printInt", GenParam(intType, "value"));
    GenStaticProc(*ast, voidType, "printFloat", GenParam(floatType, "value"));
    GenStaticProc(*ast, voidType, "input", ( GenParam(intType, "stringPtr"), GenParam(intType, "maxLen") ));
    GenStaticProc(*ast, intType, "inputInt");
    GenStaticProc(*ast, floatType, "inputFloat");

    GenStaticProc(*ast, intType, "createFile", GenParam(intType, "filenamePtr"));
    GenStaticProc(*ast, intType, "deleteFile", GenParam(intType, "filenamePtr"));
    GenStaticProc(*ast, intType, "openFile", ( GenParam(intType, "filenamePtr"), GenParam(intType, "flagsPtr") ));
    GenStaticProc(*ast, voidType, "closeFile", GenParam(intType, "handlePtr"));
    GenStaticProc(*ast, intType, "fileSize", GenParam(intType, "handlePtr"));
    GenStaticProc(*ast, voidType, "fileSetPos", ( GenParam(intType, "handlePtr"), GenParam(intType, "pos") ));
    GenStaticProc(*ast, intType, "fileGetPos", GenParam(intType, "handlePtr"));
    GenStaticProc(*ast, intType, "fileEOF", GenParam(intType, "handlePtr"));
    GenStaticProc(*ast, voidType, "writeByte", ( GenParam(intType, "handlePtr"), GenParam(intType, "value") ));
    GenStaticProc(*ast, voidType, "writeInt", ( GenParam(intType, "handlePtr"), GenParam(intType, "value") ));
    GenStaticProc(*ast, voidType, "writeFloat", ( GenParam(intType, "handlePtr"), GenParam(floatType, "value") ));
    GenStaticProc(*ast, voidType, "writeBuffer", ( GenParam(intType, "handlePtr"), GenParam(intType, "bufferPtr"), GenParam(intType, "size") ));
    GenStaticProc(*ast, intType, "readByte", GenParam(intType, "handlePtr"));
    GenStaticProc(*ast, intType, "readInt", GenParam(intType, "handlePtr"));
    GenStaticProc(*ast, floatType, "readFloat", GenParam(intType, "handlePtr"));
    GenStaticProc(*ast, voidType, "readBuffer", ( GenParam(intType, "handlePtr"), GenParam(intType, "bufferPtr"), GenParam(intType, "size") ));

    GenStaticProc(*ast, floatType, "sin", GenParam(floatType, "x"));
    GenStaticProc(*ast, floatType, "cos", GenParam(floatType, "x"));
    GenStaticProc(*ast, floatType, "tan", GenParam(floatType, "x"));
    GenStaticProc(*ast, floatType, "asin", GenParam(floatType, "x"));
    GenStaticProc(*ast, floatType, "acos", GenParam(floatType, "x"));
    GenStaticProc(*ast, floatType, "atan", GenParam(floatType, "x"));
    GenStaticProc(*ast, floatType, "pow", ( GenParam(floatType, "b"), GenParam(floatType, "e") ));
    GenStaticProc(*ast, floatType, "sqrt", GenParam(floatType, "x"));
    GenStaticProc(*ast, floatType, "log", GenParam(floatType, "x"));

    GenStaticProc(*ast, intType, "randInt");
    GenStaticProc(*ast, floatType, "randFloat");
    GenStaticProc(*ast, intType, "time");
    GenStaticProc(*ast, voidType, "sleep", GenParam(intType, "duration"));

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


} // /namespace StdCodeFactory

} // /namespace ContextAnalyzer



// ================================================================================