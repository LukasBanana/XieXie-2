/*
 * Dictionary.h
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_DICTIONARY_H__
#define __XX_DICTIONARY_H__


#include <string>
#include <map>


//! Class for dictionaries with string key and pointer value.
template <typename T> class Dictionary final
{
    
    public:
        
        void Register(const std::string& ident, T* entry)
        {
            map_[ident] = entry;
        }

        T* Unregister(const std::string& ident)
        {
            auto it = map_.find(ident);
            if (it != map_.end())
            {
                auto entry = it->second;
                map_.erase(it);
                return entry;
            }
            return nullptr;
        }

        void Clear()
        {
            map_.clear();
        }

        bool Has(T* entry) const
        {
            return map_.find(entry) != map_.end();
        }

        T* Find(const std::string& ident) const
        {
            auto it = map_.find(ident);
            return it != map_.end() ? it->second : nullptr;
        }

    private:
        
        std::map<std::string, T*> map_;

};


#endif



// ================================================================================
