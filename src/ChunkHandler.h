#pragma once

#include <memory>

#include "Chunk.h"
#include "Program.h"
#include "utils.h"

namespace OM3D
{

    template <u32 CHUNK_SIZE = 64, u32 COMPUTE_SIZE = 1024>
    class ChunkHandler : NonCopyable
    {
        using chunk_data_t = std::array<glm::vec4, CHUNK_SIZE * CHUNK_SIZE>;
        using triangulated_data_t =
            std::array<glm::vec4, Chunk<CHUNK_SIZE>::TRIANGULATED_COUNT>;

    public:
        static ChunkHandler<CHUNK_SIZE, COMPUTE_SIZE>& GetInstance();

        auto generate(glm::ivec2 offset) -> std::array<triangulated_data_t, 2>;

        auto triangulate(chunk_data_t chunk_vertices)
            -> std::array<triangulated_data_t, 2>;

        void render(const Chunk<CHUNK_SIZE>& chunk);

    private:
        ChunkHandler();

        ChunkHandler(ChunkHandler&&) = delete;
        ChunkHandler& operator=(ChunkHandler&&) = delete;
        ChunkHandler(const ChunkHandler&) = delete;
        ChunkHandler& operator=(const ChunkHandler&) = delete;

        std::shared_ptr<Program> compute_program;
        std::shared_ptr<Program> render_program;
        GLuint vbos[2];

        GLint ssbo_target = 0;
    };
} // namespace OM3D

#include "ChunkHandler.hxx"
