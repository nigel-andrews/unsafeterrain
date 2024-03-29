#ifndef TERRAIN_H
#define TERRAIN_H

#include <cstddef>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Chunk.h"
#include "ChunkHandler.h"
#include "QTree.h"
#include "graphics.h"
#include "utils.h"

namespace OM3D
{
    template <u32 CHUNK_SIZE>
    class Terrain : NonCopyable
    {
    public:
        Terrain()
        {
            auto& gen = ChunkHandler<CHUNK_SIZE>::GetInstance();
            Chunk<CHUNK_SIZE> chunk(
                static_cast<u32>(QTree<CHUNK_SIZE>::chunk_count),
                glm::vec2(0., 0.), gen.generate({ 0, 0 }));

            chunks_lut_ =
                std::make_unique<QTree<CHUNK_SIZE>>(chunk.pos, chunk.id);
            chunks_.insert({ chunk.id, std::move(chunk) });
        }

        const Chunk<CHUNK_SIZE>& fetch(const glm::vec2& pos);
        // void add(Chunk<CHUNK_SIZE>&& chunk);

        void render(const glm::vec2 pos);

    private:
        // The quad tree gets the chunk id.
        std::unique_ptr<QTree<CHUNK_SIZE>> chunks_lut_;
        // The hashmap stores the corresponding chunk.
        std::unordered_map<u32, Chunk<CHUNK_SIZE>> chunks_;
    };
} // namespace OM3D

#include "Terrain.hxx"

#endif // !TERRAIN_H
