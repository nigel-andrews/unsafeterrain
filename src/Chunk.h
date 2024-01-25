#pragma once

#include <array>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "utils.h"

namespace OM3D
{
    template <u32 SIZE>
    struct Chunk : NonCopyable
    {
        // Quad lines: SIZE - 1
        // Quad columns: SIZE - 1
        // Tris per quad: 2
        // Verts per tri: 3
        static constexpr u32 TRIANGULATED_COUNT = SIZE * SIZE * 2 * 3;

        u32 id;
        glm::vec2 pos;
        std::array<glm::vec4, TRIANGULATED_COUNT> vertices;
        std::array<glm::vec4, TRIANGULATED_COUNT> normals;

        Chunk(u32 id, glm::vec2 pos,
              std::array<std::array<glm::vec4, TRIANGULATED_COUNT>, 2> data)
            : id(id)
            , pos(pos)
            , vertices(std::move(data[0]))
            , normals(std::move(data[1]))
        {}
    };
} // namespace OM3D
