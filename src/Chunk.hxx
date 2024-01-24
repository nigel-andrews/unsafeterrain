#pragma once

#include "Chunk.h"

namespace OM3D
{

    template <u32 SIZE>
    std::vector<glm::vec4> Chunk<SIZE>::triangulate() const
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
            result.push_back(data[j * SIZE + (SIZE - 1)]);
            result.push_back(data[(j + 1) * SIZE]);
            result.push_back(data[(j + 1) * SIZE]);
            result.push_back(data[(j + 1) * SIZE + 1]);
        }
        return result;
    }
} // namespace OM3D
