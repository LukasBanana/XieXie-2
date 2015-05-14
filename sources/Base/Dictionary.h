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


/**
Class for dictionaries with string key and pointer value.
\tparam T Specifies the non-pointer value type.
*/
template <typename T> class Dictionary final
{
    
    public:
        
        //! Registers the specified pointer entry with the unique identifier 'ident'.
        void Register(const std::string& ident, T* entry)
        {
            map_[ident] = entry;
        }

        //! Unregisters a pointer entry with the specified identifier 'ident'.
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

        //! Clears the entire dictionary map.
        void Clear()
        {
            map_.clear();
        }

        //! Returns true if the specified pointer entry is contained in this dictionary.
        bool Has(T* entry) const
        {
            return map_.find(entry) != map_.end();
        }

        /**
        Returns a pointer to the entry with the specified registered identifier 'ident',
        or null if there is no such entry.
        */
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
