#pragma once

#include <stdexcept>

#include "LRUCache.h"

namespace OM3D
{

    template <typename K, Cacheable<K> V, std::size_t CAPACITY>
    V& LRUCache<K, V, CAPACITY>::get(const K& key)
    {
        const auto& it = map_.find(key);

        if (it == map_.end())
        {
            // Since Pages are cached after first pass, we already have
            // the data in memory
            throw std::runtime_error("Key not found in cache");
        }

        list_.splice(list_.begin(), list_, it->second);

        return list_.front();
    }

    template <typename K, Cacheable<K> V, std::size_t CAPACITY>
    void LRUCache<K, V, CAPACITY>::put(const K& offset, const V& value)
    {
        const auto& it = map_.find(offset);
        if (it != map_.end())
            list_.splice(list_.begin(), list_, it->second);
        else
        {
            list_.push_front(value);
            map_[offset] = list_.begin();

            if (list_.size() > CAPACITY)
            {
                const auto& last = list_.back();
                map_.erase(last.key);
                list_.pop_back();
            }
        }
    }
} // namespace OM3D
