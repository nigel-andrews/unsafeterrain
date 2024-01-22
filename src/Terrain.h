#ifndef TERRAIN_H
#define TERRAIN_H

#include <cstddef>
#include <map>
#include <unordered_map>
#include <vector>

#include "Chunk.h"
#include "ChunkGenerator.h"
#include "QTree.h"
#include "graphics.h"
#include "utils.h"

namespace OM3D
{
    template <std::size_t CHUNK_SIZE>
    class Terrain : NonCopyable
    {
    public:
        Terrain() = default;
        const Chunk<CHUNK_SIZE>& fetch(const glm::vec2& pos) const;
        void add(Chunk<CHUNK_SIZE>&& chunk);

    private:
        // The quad tree gets the chunk id.
        QTree<CHUNK_SIZE> chunk_lut_;
        // The hashmap stores the corresponding chunk.
        std::unordered_map<u32, Chunk<CHUNK_SIZE>> chunks_;
        // Contains all the logic related to generating new chunks.
        ChunkGenerator generator;
    };
} // namespace OM3D

#include "Terrain.hxx"

#endif // !TERRAIN_H
