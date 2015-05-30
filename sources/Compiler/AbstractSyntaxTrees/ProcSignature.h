/*
 * ProcSignature.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_AST_PROC_SIGNATURE_H__
#define __XX_AST_PROC_SIGNATURE_H__


#include "AST.h"


namespace AbstractSyntaxTrees
{


class ProcSignature : public AST
{
    
    public:
        
        AST_INTERFACE(ProcSignature);

        const TypeDenoter* GetTypeDenoter() const override;

        /**
        Returns a string representation of this procedure signature.
        \param[in] className Specifies the name of the class for this procedure.
        If this string is empty, no class name is inserted. By default "".
        */
        std::string ToString(const std::string& className = "") const;

        //! Returns the parameter with the specified identifier.
        Param* FindParam(const std::string& ident) const;
        /**
        Returns the index for the specified parameter or
        'std::numeric_limits<size_t>::max()' if the parameter is not contained in this parameter list.
        */
        size_t FindParamIndex(const std::string& ident) const;

        /**
        Returns true if the two procedure signatures are too similar, to be used as overloaded procedures.
        This ignores the return type and only compares the identifier and parameter types.
        */
        static bool AreSimilar(const ProcSignature& lhs, const ProcSignature& rhs);

        /* === Members === */

        bool                    isStatic        = false;    // is this procedure static?
        bool                    isEntryPoint    = false;    // is this procedure a main entry point?
        bool                    isCtor          = false;    // is this a constructor?
        bool                    isDtor          = false;    // is this a destructor?

        TypeDenoterPtr          returnTypeDenoter;
        std::string             ident;
        std::vector<ParamPtr>   params;

        // dast
        std::string             label;                      // final label for assembly

};


} // /namespace AbstractSyntaxTrees


#endif



// ================================================================================
