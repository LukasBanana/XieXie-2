/*
 * BitMask.h
 * 
 * This file is part of the "XieXie-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_BIT_MASK_H__
#define __XX_BIT_MASK_H__


//! Base class for any flags (or rather options).
class BitMask
{
    
    public:
        
        BitMask() = default;
        BitMask(int bitMask) :
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
        BitMask& operator << (int flag)
        {
            Add(flag);
            return *this;
        }
        BitMask& operator >> (int flag)
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


inline bool operator == (const BitMask& lhs, const BitMask& rhs)
{
    return lhs.Mask() == rhs.Mask();
}

inline bool operator != (const BitMask& lhs, const BitMask& rhs)
{
    return lhs.Mask() != rhs.Mask();
}


#endif



// ================================================================================