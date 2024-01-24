#pragma once

#include <array>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "utils.h"

namespace OM3D
{
    template <u32 SIZE>
    struct Chunk : NonCopyable
    {
        u32 id;
        glm::vec2 pos;
        std::array<glm::vec4, SIZE * SIZE> vertices;
        std::array<glm::vec4, SIZE * SIZE> normals;

        Chunk(u32 id, glm::vec2 pos,
              std::array<std::array<glm::vec4, SIZE * SIZE>, 2> data)
            : id(id)
            , pos(pos)
            , vertices(std::move(data[0]))
            , normals(std::move(data[1]))
        {}

        std::array<std::vector<glm::vec4>, 2> triangulate() const;
    };
} // namespace OM3D

#include "Chunk.hxx"
