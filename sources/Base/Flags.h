/*
 * Flags.h
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_FLAGS_H__
#define __XX_FLAGS_H__


//! Base class for any flags (or rather options).
class Flags
{
    
    public:
        
        Flags() = default;
        Flags(int bitMask) :
            bitMask_{ bitMask }
        {
        }

        bool Has(int flag) const
        {
            return (Mask() & flag) != 0;
        }
        void Add(int flag)
        {
            bitMask_ |= flag;
        }
        void Remove(int flag)
        {
            bitMask_ ^= (~flag);
        }

        bool operator () (int flag) const
        {
            return Has(flag);
        }
        Flags& operator << (int flag)
        {
            Add(flag);
            return *this;
        }
        Flags& operator >> (int flag)
        {
            Remove(flag);
            return *this;
        }

        int Mask() const
        {
            return bitMask_;
        }

        operator int () const
        {
            return Mask();
        }

    private:
        
        int bitMask_ = 0;

};


inline bool operator == (const Flags& lhs, const Flags& rhs)
{
    return lhs.Mask() == rhs.Mask();
}

inline bool operator != (const Flags& lhs, const Flags& rhs)
{
    return lhs.Mask() != rhs.Mask();
}


#endif



// ================================================================================