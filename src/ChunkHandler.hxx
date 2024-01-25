#pragma once

#include <array>
#include <cstring>
#include <format>
#include <glm/geometric.hpp>

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

        auto data_shape =
            Chunk<CHUNK_SIZE>::TRIANGULATED_COUNT * sizeof(glm::vec4);
        {
            auto vbo = vbos[0];

            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            GLint pos_loc = render_program->find_attrib("position");
            assert(pos_loc != -1);

            glVertexAttribPointer(pos_loc, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(pos_loc);

            glBufferData(GL_ARRAY_BUFFER, data_shape, nullptr, GL_STREAM_DRAW);

            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_target, vbo);
        }

        {
            auto nvbo = vbos[1];

            glBindBuffer(GL_ARRAY_BUFFER, nvbo);

            GLint norm_loc = render_program->find_attrib("normal");
            assert(norm_loc != -1);

            glVertexAttribPointer(norm_loc, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(norm_loc);

            glBufferData(GL_ARRAY_BUFFER, data_shape, nullptr, GL_STREAM_DRAW);

            // No need to bind normals vbo as ssbo since the compute shader
            // doesn't write to it.
        }
    }

    template <u32 CHUNK_SIZE, u32 COMPUTE_SIZE>
    auto ChunkHandler<CHUNK_SIZE, COMPUTE_SIZE>::triangulate(
        chunk_data_t chunk_vertices) -> std::array<triangulated_data_t, 2>
    {
        triangulated_data_t vertices;
        triangulated_data_t normals;

        // std::cout << vertices.size() << " / " << chunk_vertices.size() <<
        // '\n'; std::cout << 6 * chunk_vertices.size() << '\n';

        // Create triangles from the chunk vertices.
        for (u32 i = 0; i < CHUNK_SIZE - 1; i++)
        {
            for (u32 j = 0; j < CHUNK_SIZE - 1; j++)
            {
                u32 index = i * CHUNK_SIZE + j;

                // std::cout << i << " " << j << " " << index << ' ' << 6 *
                // index
                //           << '/' << Chunk<CHUNK_SIZE>::TRIANGULATED_COUNT
                //           << '\n';

                // 0 2
                // 1
                vertices[6 * index] = chunk_vertices[index];
                vertices[6 * index + 1] = chunk_vertices[index + CHUNK_SIZE];
                vertices[6 * index + 2] = chunk_vertices[index + 1];

                //   3
                // 4 5
                vertices[6 * index + 3] = chunk_vertices[index + 1];
                vertices[6 * index + 4] = chunk_vertices[index + CHUNK_SIZE];
                vertices[6 * index + 5] =
                    chunk_vertices[index + CHUNK_SIZE + 1];
            }
        }

        auto compute_normal = [](const glm::vec4& a, const glm::vec4& b,
                                 const glm::vec4& c, bool even) -> glm::vec4 {
            auto norm = glm::vec4(
                glm::normalize(glm::cross(glm::vec3(b - a), glm::vec3(c - a))),
                1.0f);
            return even ? norm : norm;
        };

        // Compute face normals from the triangles.
        for (u32 i = 0; i < CHUNK_SIZE - 1; i++)
        {
            for (u32 j = 0; j < CHUNK_SIZE - 1; j++)
            {
                u32 index = 6 * (i * CHUNK_SIZE + j);

                glm::vec4 normal =
                    compute_normal(vertices[index], vertices[index + 1],
                                   vertices[index + 2], true);
                normals[index] = normal;
                normals[index + 1] = normal;
                normals[index + 2] = normal;

                normal =
                    compute_normal(vertices[index + 3], vertices[index + 4],
                                   vertices[index + 5], false);
                normals[index + 3] = normal;
                normals[index + 4] = normal;
                normals[index + 5] = normal;
            }
        }

        return { vertices, normals };
    }

    template <u32 CHUNK_SIZE, u32 COMPUTE_SIZE>
    auto ChunkHandler<CHUNK_SIZE, COMPUTE_SIZE>::generate(glm::ivec2 offset)
        -> std::array<triangulated_data_t, 2>
    {
        compute_program->bind();

        DOGL(compute_program->set_uniform("buffer_width", CHUNK_SIZE));
        DOGL(compute_program->set_uniform("offset", offset));

        DOGL(glDispatchCompute((CHUNK_SIZE * CHUNK_SIZE) / COMPUTE_SIZE, 1, 1));

        chunk_data_t vertices_read;
        {
            auto vbo = vbos[0];
            GLvoid* pSSBOData;
            DOGL(pSSBOData = glMapNamedBuffer(vbo, GL_READ_ONLY));
            memcpy(vertices_read.data(), pSSBOData,
                   CHUNK_SIZE * CHUNK_SIZE * sizeof(glm::vec4));

            DOGL(glUnmapNamedBuffer(vbo));
        }

        // No need to read normals from OpenGL since we don't set them in the
        // compute shader and overwrite them when triangulating.
        return triangulate(vertices_read);
    }

    template <u32 CHUNK_SIZE, u32 COMPUTE_SIZE>
    void ChunkHandler<CHUNK_SIZE, COMPUTE_SIZE>::render(
        const Chunk<CHUNK_SIZE>& chunk)
    {
        render_program->bind();

        // Vertices
        {
            auto vbo = vbos[0];

            // Write data to VBO/SSBO
            GLvoid* pSSBOData;
            DOGL(pSSBOData = glMapNamedBuffer(vbo, GL_WRITE_ONLY));
            memcpy(pSSBOData, chunk.vertices.data(),
                   chunk.vertices.size() * sizeof(glm::vec4));
            DOGL(glUnmapNamedBuffer(vbo));

            DOGL(glBindBuffer(GL_ARRAY_BUFFER, vbo));

            GLint pos_loc = render_program->find_attrib("position");
            assert(pos_loc != -1);

            DOGL(glVertexAttribPointer(pos_loc, 4, GL_FLOAT, GL_FALSE, 0,
                                       nullptr));
            DOGL(glEnableVertexAttribArray(pos_loc));
        }

        // Normals
        {
            auto nvbo = vbos[1];
            GLvoid* nSSBOData;
            DOGL(nSSBOData = glMapNamedBuffer(nvbo, GL_WRITE_ONLY));

            memcpy(nSSBOData, chunk.normals.data(),
                   chunk.normals.size() * sizeof(glm::vec4));

            DOGL(glUnmapNamedBuffer(nvbo));

            DOGL(glBindBuffer(GL_ARRAY_BUFFER, nvbo));

            GLint norm_loc = render_program->find_attrib("normal");
            assert(norm_loc != -1);

            DOGL(glVertexAttribPointer(norm_loc, 4, GL_FLOAT, GL_FALSE, 0,
                                       nullptr));
            DOGL(glEnableVertexAttribArray(norm_loc));
        }

        DOGL(glDrawArrays(GL_TRIANGLES, 0,
                          Chunk<CHUNK_SIZE>::TRIANGULATED_COUNT));
    }
} // namespace OM3D
