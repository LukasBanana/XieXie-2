/*
 * Variant.h
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_VARIANT_H__
#define __XX_VARIANT_H__


#include <string>


class Variant
{
    
    public:
        
        enum class Types
        {
            Bool,
            Int,
            Float,
        };

        Variant();
        Variant(bool value);
        Variant(int value);
        Variant(float value);
        
        std::string ToString() const;

        Types Type() const
        {
            return type_;
        }

        bool Bool() const
        {
            return valueInt_ != 0;
        }
        int Int() const
        {
            return valueInt_;
        }
        float Float() const
        {
            return valueFloat_;
        }

    private:
        
        Types type_ = Types::Int;

        union
        {
            int     valueInt_;
            float   valueFloat_;
        };

};


#endif



// ================================================================================