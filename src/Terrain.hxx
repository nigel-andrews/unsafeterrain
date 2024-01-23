#pragma once

#include <iostream>

#include "Terrain.h"

namespace OM3D
{
    template <u32 CHUNK_SIZE>
    auto Terrain<CHUNK_SIZE>::fetch(const glm::vec2& pos) const
        -> const Chunk<CHUNK_SIZE>&
    {
        // for (const Chunk<CHUNK_SIZE>& chunk : chunks)
        // {
        //     std::cout << chunk.id << ": ";
        //     if (pos.x >= chunk.origin.x && pos.x < chunk.origin.x +
        //     CHUNK_SIZE
        //         && pos.y >= chunk.origin.y
        //         && pos.y < chunk.origin.y + CHUNK_SIZE)
        //         std::cout << "In\n";
        //     else
        //         std::cout << "Not in\n";
        // }
    }

    template <u32 CHUNK_SIZE>
    inline void Terrain<CHUNK_SIZE>::add(Chunk<CHUNK_SIZE>&& chunk)
    {
        // chunks.emplace_back(chunk);
    }
} // namespace OM3D
