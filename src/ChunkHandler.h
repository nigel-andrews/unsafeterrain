#pragma once

#include <memory>

#include "Program.h"
#include "utils.h"

namespace OM3D
{
    template <u32 CHUNK_SIZE = 64, u32 COMPUTE_SIZE = 1024>
    class ChunkHandler : NonCopyable
    {
    public:
        static ChunkHandler<CHUNK_SIZE, COMPUTE_SIZE>& GetInstance();

        auto generate(glm::ivec2 offset)
            -> std::array<glm::vec4, CHUNK_SIZE * CHUNK_SIZE>;

    private:
        ChunkHandler();

        ChunkHandler(ChunkHandler&&) = delete;
        ChunkHandler& operator=(ChunkHandler&&) = delete;
        ChunkHandler(const ChunkHandler&) = delete;
        ChunkHandler& operator=(const ChunkHandler&) = delete;

        std::shared_ptr<Program> compute_program;
        std::shared_ptr<Program> render_program;
        GLuint vbo = 0;
        GLint ssbo_target = 0;
    };
} // namespace OM3D

#include "ChunkHandler.hxx"
