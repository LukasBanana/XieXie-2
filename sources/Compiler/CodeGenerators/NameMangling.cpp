/*
 * NameMangling.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "NameMangling.h"
#include "Param.h"
#include "ProcSignature.h"
#include "ProcDeclStmnt.h"
#include "ClassDeclStmnt.h"
#include "BuiltinTypeDenoter.h"
#include "PointerTypeDenoter.h"
#include "ArrayTypeDenoter.h"
#include "StringModifier.h"
#include "VarName.h"

#include <exception>


using namespace AbstractSyntaxTrees;

namespace CodeGenerator
{

namespace NameMangling
{


/* --- UniqueLabel --- */

static std::string GenerateAbstractTypeLabel(const TypeDenoter* typeDenoter);

static std::string GenerateTypeLabel(const BuiltinTypeDenoter& typeDenoter)
{
    using Ty = BuiltinTypeDenoter::TypeNames;
    switch (typeDenoter.typeName)
    {
        case Ty::Void:
            return "V";
        case Ty::Bool:
            return "B";
        case Ty::Int:
            return "I";
        case Ty::Float:
            return "F";
    }
    throw std::string("invalid type denoter");
}

static std::string GenerateTypeLabel(const ArrayTypeDenoter& typeDenoter)
{
    return "A@" + GenerateAbstractTypeLabel(typeDenoter.lowerTypeDenoter.get());
}

static std::string GenerateTypeLabel(const PointerTypeDenoter& typeDenoter)
{
    return "R@" + typeDenoter.declIdent;
}

static std::string GenerateAbstractTypeLabel(const TypeDenoter* typeDenoter)
{
    if (typeDenoter)
    {
        switch (typeDenoter->Type())
        {
            case AST::Types::BuiltinTypeDenoter:
                return GenerateTypeLabel(*static_cast<const BuiltinTypeDenoter*>(typeDenoter));
            case AST::Types::ArrayTypeDenoter:
                return GenerateTypeLabel(*static_cast<const ArrayTypeDenoter*>(typeDenoter));
            case AST::Types::PointerTypeDenoter:
                return GenerateTypeLabel(*static_cast<const PointerTypeDenoter*>(typeDenoter));
            default:
                break;
        }
    }
    throw std::string("invalid type denoter");
}

static std::string GenerateSubLabel(const Param& param, size_t index)
{
    try
    {
        return GenerateAbstractTypeLabel(param.typeDenoter.get()) + '?' + param.ident;
    }
    catch (const std::string& err)
    {
        throw std::invalid_argument(err + " in parameter (" + ToStr(index) + "): \"" + param.ident + "\"");
    }
}

std::string UniqueLabel(const ProcSignature& procSignature, const std::string* classIdent)
{
    std::string label;

    label += "P" + procSignature.ident;

    if (classIdent)
        label += ",R@" + *classIdent;

    size_t i = 0;
    for (const auto& param : procSignature.params)
        label += ',' + GenerateSubLabel(*param, i++);

    return label;
}

std::string UniqueLabel(const AbstractSyntaxTrees::ProcDeclStmnt& procDecl, bool appendClassIdent)
{
    if (procDecl.procSignature)
    {
        if (appendClassIdent && procDecl.parentRef)
        {
            if (procDecl.parentRef->isModule)
            {
                /* Return simple label for module procedures */
                return procDecl.parentRef->GetIdent() + "." + procDecl.procSignature->ident;
            }
            else
            {
                /* Return label with name-mangling for standard class procedures */
                const auto& procSig = *procDecl.procSignature;
                return (
                    "C" + procDecl.parentRef->GetIdent() + "." +
                    UniqueLabel(procSig)//, procSig.isStatic ? nullptr : &(procDecl.parentRef->ident))
                );
            }
        }
        else
        {
            /* Returns only label with name-mangling for stand-alone procedures */
            return UniqueLabel(*procDecl.procSignature);
        }
    }
    return "";
}

/* --- DisplayLabel --- */

static void ExtractClass(const std::string& label, size_t& pos, std::string& str)
{
    if (++pos >= label.size())
        return;
    if (pos > 1)
        str += '.';

    auto end = label.find('.', pos);

    if (end == std::string::npos)
    {
        str += label.substr(pos);
        pos = label.size();
    }
    else
    {
        str += label.substr(pos, end - pos);
        pos = end + 1;
    }
}

static void ExtractProcedure(const std::string& label, size_t& pos, std::string& str)
{
    if (++pos >= label.size())
        return;
    if (pos > 1)
        str += '.';

    auto end = label.find(',', pos);

    if (end == std::string::npos)
    {
        str += label.substr(pos);
        pos = label.size();
    }
    else
    {
        str += label.substr(pos, end - pos);
        pos = end + 1;
    }

    str += '(';
}

static void ExtractParamBuiltin(const std::string& label, size_t& pos, std::string& str)
{
    switch (label[pos])
    {
        case 'V':
            str += "void";
            break;
        case 'B':
            str += "bool";
            break;
        case 'I':
            str += "int";
            break;
        case 'F':
            str += "float";
            break;
    }
    ++pos;
}

static void ExtractParam(
    const std::string& label, size_t& pos, std::string& str,
    bool isArray = false, bool* hasParam = nullptr
);

static void ExtractParamArray(const std::string& label, size_t& pos, std::string& str)
{
    if (label[pos] != '@')
        throw std::invalid_argument("expected '@' after array type denoter in label \"" + label + "\"");

    ++pos;
    ExtractParam(label, pos, str, true);
}

static void ExtractIdent(const std::string& label, size_t& pos, const size_t end, std::string& str)
{
    if (end == std::string::npos)
    {
        str += label.substr(pos);
        pos = label.size();
    }
    else
    {
        str += label.substr(pos, end - pos);
        pos = end;
    }
}

static void ExtractParamPointer(const std::string& label, size_t& pos, std::string& str)
{
    if (label[pos] != '@')
        throw std::invalid_argument("expected '@' after pointer type denoter in label \"" + label + "\"");

    ++pos;
    auto end = label.find('?', pos);
    if (end == std::string::npos)
        end = label.find(',', pos);

    ExtractIdent(label, pos, end, str);
}

static void ExtractParamIdent(const std::string& label, size_t& pos, std::string& str)
{
    if (label[pos] != '?')
        throw std::invalid_argument("expected '?' after parameter type in label \"" + label + "\"");

    ++pos;
    auto end = label.find(',', pos);

    str += ' ';
    ExtractIdent(label, pos, end, str);
}

static void ExtractParam(const std::string& label, size_t& pos, std::string& str, bool isArray, bool* hasParam)
{
    if (hasParam)
    {
        if (*hasParam)
            str += ", ";
        *hasParam = true;
    }

    switch (label[pos])
    {
        case 'V':
        case 'B':
        case 'I':
        case 'F':
            ExtractParamBuiltin(label, pos, str);
            break;
        case 'A':
            ++pos;
            ExtractParamArray(label, pos, str);
            break;
        case 'R':
            ++pos;
            ExtractParamPointer(label, pos, str);
            break;
    }

    if (isArray)
        str += "[]";

    if (pos < label.size() && label[pos] == '?')
        ExtractParamIdent(label, pos, str);
}

/*
example conversion:
    "CWidget.PdoSomething,I?age,A@R@String?name"
                    |
                    v
    "Widget.doSomething(int age, String[] name)"
*/
std::string DisplayLabel(const std::string& label)
{
    std::string str;
    size_t pos = 0;

    bool hasProc = false;
    bool hasParam = false;

    while (pos < label.size())
    {
        switch (label[pos])
        {
            case 'C':
                ExtractClass(label, pos, str);
                break;
            case 'P':
                ExtractProcedure(label, pos, str);
                hasProc = true;
                break;
            case 'V':
            case 'B':
            case 'I':
            case 'F':
            case 'A':
            case 'R':
                ExtractParam(label, pos, str, false, &hasParam);
                ++pos;
                break;
            default:
                return label;
        }
    }

    if (hasProc)
        str += ')';

    return str;
}

/* --- VirtualTable --- */

std::string Vtable(const AbstractSyntaxTrees::ClassDeclStmnt& classDecl)
{
    return classDecl.GetIdent() + ".vtable";
}


} // /namespace NameMangling

} // /namespace CodeGenerator



// ================================================================================