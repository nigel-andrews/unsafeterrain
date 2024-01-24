#pragma once

#include <iostream>

#include "ChunkHandler.h"
#include "Terrain.h"
#include "utils.h"

namespace OM3D
{
    template <u32 CHUNK_SIZE>
    auto Terrain<CHUNK_SIZE>::fetch(const glm::vec2& pos)
        -> const Chunk<CHUNK_SIZE>&
    {
        auto fetched = this->chunks_lut_->fetch(pos);
        auto chunk_id = fetched.unwrap_or_else([&, this]() -> u32 {
            std::cout << "Generating chunk at (" << pos.x << ", " << pos.y
                      << ")\n";

            this->chunks_lut_ =
                QTree<CHUNK_SIZE>::add_node(this->chunks_lut_, pos);

            auto chunk_id = this->chunks_lut_->chunk_count - 1;

            ChunkHandler<CHUNK_SIZE>& handler =
                ChunkHandler<CHUNK_SIZE>::GetInstance();

            // Generate chunk aligned with grid
            auto data = handler.generate(
                i32(CHUNK_SIZE)
                * glm::ivec2(pos.x / CHUNK_SIZE, pos.y / CHUNK_SIZE));
            this->chunks_.insert(
                { chunk_id, Chunk<CHUNK_SIZE>(chunk_id, pos, data) });

            return chunk_id;
        });

        return this->chunks_.at(chunk_id);
    }
    //
    // template <u32 CHUNK_SIZE>
    // inline void Terrain<CHUNK_SIZE>::add(Chunk<CHUNK_SIZE>&& chunk)
    // {
    //     // chunks.emplace_back(chunk);
    // }

    template <u32 CHUNK_SIZE>
    void Terrain<CHUNK_SIZE>::render(const glm::vec2 pos)
    {
        // glm::ivec2 aligned_pos =
        //     i32(CHUNK_SIZE) * glm::ivec2(pos / float(CHUNK_SIZE));
        auto& handler = ChunkHandler<CHUNK_SIZE>::GetInstance();
        for (float i = -2; i < 2; i++)
        {
            for (float j = -2; j < 2; j++)
            {
                auto& chunk = this->fetch(
                    pos + glm::vec2(i * CHUNK_SIZE, j * CHUNK_SIZE));
                handler.render(chunk);
            }
        }

        // for (const auto& [_, chunk] : chunks_)
        //     handler.render(chunk);
    }
} // namespace OM3D
