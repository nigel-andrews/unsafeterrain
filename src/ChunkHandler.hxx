#pragma once

#include <array>
#include <cstddef>
#include <cstring>
#include <format>

#include "ChunkHandler.h"

namespace OM3D
{
    template <u32 CHUNK_SIZE, u32 COMPUTE_SIZE>
    auto ChunkHandler<CHUNK_SIZE, COMPUTE_SIZE>::GetInstance()
        -> ChunkHandler<CHUNK_SIZE, COMPUTE_SIZE>&
    {
        static ChunkHandler<CHUNK_SIZE, COMPUTE_SIZE> instance;
        return instance;
    }

    template <u32 CHUNK_SIZE, u32 COMPUTE_SIZE>
    ChunkHandler<CHUNK_SIZE, COMPUTE_SIZE>::ChunkHandler()
        : compute_program(Program::from_file(
            "chunk.comp", { std::format("COMPUTE_SIZE {}", COMPUTE_SIZE) }))
        , render_program(Program::from_files("chunk.frag", "chunk.vert"))
    {
        render_program->bind();

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        GLint pos_loc = render_program->find_attrib("position");
        assert(pos_loc != -1);

        glVertexAttribPointer(pos_loc, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(pos_loc);

        std::array<glm::vec4, CHUNK_SIZE * CHUNK_SIZE> vertices;
        glBufferData(GL_ARRAY_BUFFER,
                     CHUNK_SIZE * CHUNK_SIZE * sizeof(glm::vec4),
                     vertices.data(), GL_STREAM_DRAW);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_target, vbo);
    }

    template <u32 CHUNK_SIZE, u32 COMPUTE_SIZE>
    auto ChunkHandler<CHUNK_SIZE, COMPUTE_SIZE>::generate(glm::ivec2 offset)
        -> std::array<glm::vec4, CHUNK_SIZE * CHUNK_SIZE>
    {
        compute_program->bind();

        DOGL(compute_program->set_uniform("buffer_width", CHUNK_SIZE));
        DOGL(compute_program->set_uniform("offset", offset));

        DOGL(glDispatchCompute((CHUNK_SIZE * CHUNK_SIZE) / COMPUTE_SIZE, 1, 1));

        GLvoid* pSSBOData;
        DOGL(pSSBOData = glMapNamedBuffer(vbo, GL_READ_ONLY));

        std::array<glm::vec4, CHUNK_SIZE * CHUNK_SIZE> vertices_read;
        memcpy(vertices_read.data(), pSSBOData,
               CHUNK_SIZE * CHUNK_SIZE * sizeof(glm::vec4));

        // // Debug: print SSBO
        // for (u32 i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++)
        // {
        //     std::cout << vertices_read[i].x << ", " << vertices_read[i].y
        //               << ", " << vertices_read[i].z << ", "
        //               << vertices_read[i].w << std::endl;
        // }

        DOGL(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));

        return vertices_read;
    }
} // namespace OM3D
