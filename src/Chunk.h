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
            for (u32 j = 0; j < SIZE - 1; j += 2)
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

                /*
                    <-----
                    . . . .
                     \ \ \
                    . . . .
                */
                for (u32 i = SIZE - 1; i < SIZE; i--)
                {
                    u32 index = (j + 1) * SIZE + i;
                    result.push_back(data[index + SIZE]);
                    result.push_back(data[index - 1]);
                }
            }
            return result;
        }
    };
} // namespace OM3D
