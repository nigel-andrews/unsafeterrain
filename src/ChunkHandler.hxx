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

        glGenBuffers(2, vbos);

        {
            auto vbo = vbos[0];

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

        {
            auto nvbo = vbos[1];

            glBindBuffer(GL_ARRAY_BUFFER, nvbo);

            GLint norm_loc = render_program->find_attrib("normal");
            assert(norm_loc != -1);

            glVertexAttribPointer(norm_loc, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(norm_loc);

            std::array<glm::vec4, CHUNK_SIZE * CHUNK_SIZE> normals;

            glBufferData(GL_ARRAY_BUFFER,
                         CHUNK_SIZE * CHUNK_SIZE * sizeof(glm::vec4),
                         normals.data(), GL_STREAM_DRAW);

            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_target + 1, nvbo);
        }
    }

    template <u32 CHUNK_SIZE, u32 COMPUTE_SIZE>
    auto ChunkHandler<CHUNK_SIZE, COMPUTE_SIZE>::generate(glm::ivec2 offset)
        -> std::array<chunk_data_t, 2>
    {
        compute_program->bind();

        DOGL(compute_program->set_uniform("buffer_width", CHUNK_SIZE));
        DOGL(compute_program->set_uniform("offset", offset));

        DOGL(glDispatchCompute((CHUNK_SIZE * CHUNK_SIZE) / COMPUTE_SIZE, 1, 1));

        chunk_data_t vertices_read;
        chunk_data_t normals_read;

        {
            auto vbo = vbos[0];
            GLvoid* pSSBOData;
            DOGL(pSSBOData = glMapNamedBuffer(vbo, GL_READ_ONLY));
            memcpy(vertices_read.data(), pSSBOData,
                   CHUNK_SIZE * CHUNK_SIZE * sizeof(glm::vec4));

            DOGL(glUnmapNamedBuffer(vbo));
        }

        {
            auto nvbo = vbos[1];
            GLvoid* nSSBOData;
            DOGL(nSSBOData = glMapNamedBuffer(nvbo, GL_READ_ONLY));

            memcpy(normals_read.data(), nSSBOData,
                   CHUNK_SIZE * CHUNK_SIZE * sizeof(glm::vec4));

            DOGL(glUnmapNamedBuffer(nvbo));
        }

        return { vertices_read, normals_read };
    }

    template <u32 CHUNK_SIZE, u32 COMPUTE_SIZE>
    void ChunkHandler<CHUNK_SIZE, COMPUTE_SIZE>::render(
        const Chunk<CHUNK_SIZE>& chunk)
    {
        render_program->bind();

        const auto& [tris, tri_norms] = chunk.triangulate();

        {
            auto vbo = vbos[0];
            GLvoid* pSSBOData;
            DOGL(pSSBOData = glMapNamedBuffer(vbo, GL_WRITE_ONLY));

            memcpy(pSSBOData, tris.data(),
                   CHUNK_SIZE * CHUNK_SIZE * sizeof(glm::vec4));

            DOGL(glUnmapNamedBuffer(vbo));

            DOGL(glBindBuffer(GL_ARRAY_BUFFER, vbo));

            GLint pos_loc = render_program->find_attrib("position");
            assert(pos_loc != -1);

            DOGL(glVertexAttribPointer(pos_loc, 4, GL_FLOAT, GL_FALSE, 0,
                                       nullptr));
            DOGL(glEnableVertexAttribArray(pos_loc));
        }
        {
            auto nvbo = vbos[1];
            GLvoid* nSSBOData;
            DOGL(nSSBOData = glMapNamedBuffer(nvbo, GL_WRITE_ONLY));

            memcpy(nSSBOData, tri_norms.data(),
                   CHUNK_SIZE * CHUNK_SIZE * sizeof(glm::vec4));

            DOGL(glUnmapNamedBuffer(nvbo));

            DOGL(glBindBuffer(GL_ARRAY_BUFFER, nvbo));

            GLint norm_loc = render_program->find_attrib("normal");
            assert(norm_loc != -1);

            DOGL(glVertexAttribPointer(norm_loc, 4, GL_FLOAT, GL_FALSE, 0,
                                       nullptr));
            DOGL(glEnableVertexAttribArray(norm_loc));
        }

        DOGL(glDrawArrays(GL_TRIANGLE_STRIP, 0, CHUNK_SIZE * CHUNK_SIZE));
    }
} // namespace OM3D
