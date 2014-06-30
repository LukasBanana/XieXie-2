/*
 * Abstract syntax tree file
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2013 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ASTIncludes.h"


namespace AbstractSyntaxTrees
{


/*
 * Static constant members
 */

/* --- AttributeList AST class --- */

const std::string AttributeList::AttribSpellings[] =
{
    "pack",
    "set",
    "get",
    "unroll"
};


/* --- FNameIdent AST class --- */

const FNameIdent::Flags::DataType FNameIdent::Flags::FlagsTypeDenoter           = (1 << 0);
const FNameIdent::Flags::DataType FNameIdent::Flags::FlagsEntry                 = (1 << 1);
const FNameIdent::Flags::DataType FNameIdent::Flags::FlagsProc                  = (1 << 2);
const FNameIdent::Flags::DataType FNameIdent::Flags::EnumEntry                  = (1 << 3);
const FNameIdent::Flags::DataType FNameIdent::Flags::EnumProc                   = (1 << 4);

const FNameIdent::InFlags::DataType FNameIdent::InFlags::AllowTemplateParams    = (1 << 0);
const FNameIdent::InFlags::DataType FNameIdent::InFlags::AllowTemplateArgs      = (1 << 1);

const FNameIdent::OutFlags::DataType FNameIdent::OutFlags::HasMultiIdent        = (1 << 0);
const FNameIdent::OutFlags::DataType FNameIdent::OutFlags::HasTemplateParams    = (1 << 1);
const FNameIdent::OutFlags::DataType FNameIdent::OutFlags::HasTemplateArgs      = (1 << 2);


/* --- Command AST class --- */

const Command::ParseFlags::DataType Command::ParseFlags::AllowDefinitionsOnly   = (1 << 0);
const Command::ParseFlags::DataType Command::ParseFlags::GlobalScope            = (1 << 1);


/*
 * Additional functions for AST classes
 */

/* --- TemplateParamList AST class --- */

TemplateParamList::TemplateParamList(ObjectPtr ObjAST) :
    AST (ObjAST->Pos()  ),
    Obj (ObjAST         )
{
}

void TemplateParamList::PrintAST()
{
    Deb("Template Parameter List");
    ScopedIndent Unused;

    if (Obj != nullptr)
        Obj->PrintAST();
            
    if (Next != nullptr)
        Next->PrintAST();
}

bool TemplateParamList::HasPattern() const
{
    if (Obj != nullptr && Obj->Type() == Object::Types::PatternObj)
        return true;
    if (Next != nullptr)
        return Next->HasPattern();
    return false;
}

void TemplateParamList::Compare(const TemplateArgList* ArgList, const ContextualAnalyzer::ScopeManager& ScopeMngr) const
{
    if (ArgList == nullptr)
    {
        /* Check if parameter has default argument */
        bool Err = true;

        if (Obj->Type() == Object::Types::PatternObj)
        {
            auto PatternObj = dynamic_cast<PatternObject*>(Obj.get());
            Err = (PatternObj->DefTDenoter == nullptr);
        }
        else if (Obj->Type() == Object::Types::VarObj)
        {
            auto VarObj = dynamic_cast<VariableObject*>(Obj.get());
            Err = (VarObj->Expr == nullptr);
        }

        if (Err)
            throw std::string("Too few arguments for template parameter list");
        return;
    }

    //!TODO! -> refactor type denoter comparision with expressions!!!
    #if 0
    /* Compare current type denoters */
    auto TDenoterAST = ArgList->Expr->GetTypeDenoter(ScopeMngr)->GetNonConstType();

    if (!Obj->TDenoter->GetNonConstType()->Compare(TDenoterAST))
        throw std::string("Incompatible type for argument expression");
    #endif

    /* Check if there are further parameters */
    if (Next != nullptr)
        Next->Compare(ArgList->Next.get(), ScopeMngr);
    else if (ArgList->Next != nullptr)
        throw std::string("Too many arguments for template parameter list");
}

TemplateParamListPtr TemplateParamList::Copy() const
{
    auto CopyObj = std::make_shared<TemplateParamList>(Obj->Copy());

    if (Next != nullptr)
        CopyObj->Next = Next->Copy();

    return CopyObj;
}


/* --- TemplateArgList AST class --- */

TemplateArgList::TemplateArgList(ExpressionPtr ExprAST) :
    AST (ExprAST->Pos() ),
    Expr(ExprAST        )
{
}
TemplateArgList::TemplateArgList(TypeDenoterPtr TDenoterAST) :
    AST     (TDenoterAST->Pos() ),
    TDenoter(TDenoterAST        )
{
}

void TemplateArgList::PrintAST()
{
    Deb("Template Argument List");
    ScopedIndent Unused;

    if (Expr != nullptr)
        Expr->PrintAST();
    if (TDenoter != nullptr)
        TDenoter->PrintAST();
    
    if (Next != nullptr)
        Next->PrintAST();
}

TemplateArgListPtr TemplateArgList::Copy() const
{
    TemplateArgListPtr CopyObj;

    if (Expr != nullptr)
        CopyObj = std::make_shared<TemplateArgList>(Expr->Copy());
    else
        CopyObj = std::make_shared<TemplateArgList>(TDenoter->Copy());

    if (Next != nullptr)
        CopyObj->Next = Next->Copy();

    return CopyObj;
}


/* --- TypeObject AST class --- */

bool TypeObject::IsClassDenoter() const
{
    /* Get custom type denoter */
    if (TDenoter->Type() != TypeDenoter::Types::CustomType)
        return false;
    auto CTDenoter = dynamic_cast<CustomTypeDenoter*>(TDenoter.get());

    /* Check if linked object is a class denoter */
    return CTDenoter->Link != nullptr ? CTDenoter->Link->IsClassDenoter() : false;
}


/* --- CallCommand AST class --- */

CommandPtr CallCommand::Copy() const
{
    auto CopyObj = std::make_shared<CallCommand>(
        FName->Copy(), ArgList != nullptr ? ArgList->Copy() : nullptr
    );

    if (Obj != nullptr)
        CopyObj->Obj = std::dynamic_pointer_cast<FunctionObject>(Obj->Copy());

    return CopyObj;
}


/* --- CustomTypeDenoter AST class --- */

size_t CustomTypeDenoter::CountPointerRefs() const
{
    /* Check if this type denoter is linked with a pattern object */
    if (Link != nullptr && Link->Type() == Object::Types::PatternObj)
    {
        auto PatternObj = dynamic_cast<PatternObject*>(Link.get());
        if (PatternObj->TDenoter != nullptr)
            return PatternObj->TDenoter->CountPointerRefs();
    }
    return 0;
}


/* --- Expression AST class --- */

void Expression::SetupCommonTypeDenoterPrim(const ScopeManager& ScopeMngr, const TypeDenoter::Types& Type)
{
    CommonTDenoter = ScopeMngr.FindType(Type).get();
}


} // /namespace AbstractSyntaxTrees



// ================================================================================
