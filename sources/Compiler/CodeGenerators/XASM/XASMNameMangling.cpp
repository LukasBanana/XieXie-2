/*
 * XASMNameMangling.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "XASMNameMangling.h"
#include "Param.h"
#include "ProcSignature.h"
#include "BuiltinTypeDenoter.h"
#include "PointerTypeDenoter.h"
#include "ArrayTypeDenoter.h"
#include "StringModifier.h"
#include "VarName.h"


using namespace AbstractSyntaxTrees;

namespace CodeGenerator
{

namespace XASM
{

namespace NameMangling
{


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
    throw std::string("invalid basic type for built-in type denoter");
}

static std::string GenerateTypeLabel(const ArrayTypeDenoter& typeDenoter)
{
    return "A@" + GenerateAbstractTypeLabel(typeDenoter.lowerTypeDenoter.get());
}

static std::string GenerateTypeLabel(const PointerTypeDenoter& typeDenoter)
{
    return "R@" + typeDenoter.declName->FullName();
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
        return GenerateAbstractTypeLabel(param.typeDenoter.get());
    }
    catch (const std::string& err)
    {
        throw std::invalid_argument(err + " in parameter (" + ToStr(index) + "): \"" + param.ident + "\"");
    }
}

std::string GenerateLabel(const ProcSignature& signature)
{
    std::string label;

    label += "P" + signature.ident;

    size_t i = 0;
    for (const auto& param : signature.params)
    {
        label += ',';
        label += GenerateSubLabel(*param, i++);
    }

    return label;
}


} // /namespace NameMangling

} // /namespace XASM

} // /namespace CodeGenerator



// ================================================================================