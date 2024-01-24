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
        std::array<glm::vec4, SIZE * SIZE> data;

        Chunk(u32 id, glm::vec2 pos, std::array<glm::vec4, SIZE * SIZE> data)
            : id(id)
            , pos(pos)
            , data(std::move(data))
        {}

        std::vector<glm::vec4> triangulate() const;
    };
} // namespace OM3D

#include "Chunk.hxx"
