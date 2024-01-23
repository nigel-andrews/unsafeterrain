#pragma once

#include <cstddef>
#include <glm/glm.hpp>
#include <list>
#include <ostream>
#include <unordered_map>

namespace OM3D
{
    template <typename V, typename K>
    concept Cacheable = requires(const V& value, const K& key) {
        {
            value.key
        } -> std::convertible_to<K>;
    };

    template <typename K, Cacheable<K> V, std::size_t CAPACITY>
    class LRUCache
    {
    public:
        V& get(const K& offset);
        void put(const K& offset, const V& value);

        friend std::ostream& operator<<(std::ostream& os, const LRUCache& cache)
        {
            for (const auto& value : cache.list_)
                os << value.key << " -> " << value << '\n';

            return os;
        }

    private:
        std::list<V> list_;
        std::unordered_map<K, typename std::list<V>::iterator> map_;
    };
} // namespace OM3D

#include "LRUCache.hxx"
