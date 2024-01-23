#pragma once

#include <format>

#include "ChunkGenerator.h"

namespace OM3D
{
    template <u32 CHUNK_SIZE, u32 COMPUTE_SIZE>
    ChunkGenerator<CHUNK_SIZE, COMPUTE_SIZE>::ChunkGenerator()
        : program(Program::from_file(
            "generate_extended.comp",
            { std::format("COMPUTE_SIZE {}", COMPUTE_SIZE) }))
        , buffer(nullptr, CHUNK_SIZE * CHUNK_SIZE)
    {
        // glGetProgramResourceIndex(program.get()->, GL_SHADER_STORAGE_BLOCK,
        //                           "point_pos_buffer");
        buffer.bind(BufferUsage::Storage, 0);
    }

    template <u32 CHUNK_SIZE, u32 COMPUTE_SIZE>
    void ChunkGenerator<CHUNK_SIZE, COMPUTE_SIZE>::generate(glm::ivec2 offset)
    {
        program->bind();

        DOGL(program->set_uniform("buffer_width", CHUNK_SIZE));
        DOGL(program->set_uniform("offset", offset));

        DOGL(glDispatchCompute((CHUNK_SIZE * CHUNK_SIZE) / COMPUTE_SIZE, 1, 1));

        // TODO: bind SSBO
    }
} // namespace OM3D
