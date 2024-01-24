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

        std::vector<glm::vec4> triangulate() const
        {
            std::vector<glm::vec4> result;
            for (u32 j = 0; j < SIZE - 1; j++)
            {
                /*
                    ----->
                    . . . .
                    | | | |
                    . . . .
                */
                for (u32 i = 0; i < SIZE; i++)
                {
                    u32 index = j * SIZE + i;
                    result.push_back(data[index]);
                    result.push_back(data[index + SIZE]);
                }

                // Insert degenerate 0-area triangle to break up strip.
                result.push_back(data[(j + 1) * SIZE]);
                result.push_back(data[(j + 1) * SIZE]);
            }
            return result;
        }
    };
} // namespace OM3D
