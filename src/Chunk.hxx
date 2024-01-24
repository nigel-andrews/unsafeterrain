#pragma once

#include <glm/geometric.hpp>

#include "Chunk.h"

namespace OM3D
{

    namespace
    {

        inline glm::vec4 compute_normal(const glm::vec4& a, const glm::vec4& b,
                                        const glm::vec4& c, bool even)
        {
            auto norm = glm::vec4(
                glm::normalize(glm::cross(glm::vec3(b - a), glm::vec3(c - a))),
                1.0f);
            return even ? norm : -norm;
        }
    } // namespace

    template <u32 SIZE>
    std::array<std::vector<glm::vec4>, 2> Chunk<SIZE>::triangulate() const
    {
        std::vector<glm::vec4> result;
        std::vector<glm::vec4> normals;

        auto push = [&](const glm::vec4& vertex) {
            result.push_back(vertex);
            size_t res_size = result.size();

            const glm::vec4& a = result[res_size - 1];
            const glm::vec4& b = result[res_size - 2];
            const glm::vec4& c = result[res_size - 3];

            normals.push_back(compute_normal(a, b, c, result.size() % 2 == 0));
        };
        result.push_back(vertices[0]);
        result.push_back(vertices[SIZE]);

        for (u32 j = 0; j < SIZE - 1; j++)
        {
            /*
                ----->
                . . . .
                | | | |
                . . . .
            */
            for (u32 i = j ? 0 : 2; i < SIZE; i++)
            {
                u32 index = j * SIZE + i;
                if (index == 3)
                {
                    result.push_back(vertices[index]);
                    auto normal =
                        compute_normal(result[index - 1], result[index - 2],
                                       result[index], true);
                    normals.push_back(normal);
                    normals.push_back(normal);
                    normals.push_back(normal);
                }
                else
                    push(vertices[index]);

                push(vertices[index + SIZE]);
            }

            // Insert degenerate 0-area triangle to break up strip.
            push(vertices[j * SIZE + (SIZE - 1)]);
            push(vertices[j * SIZE + (SIZE - 1)]);
            push(vertices[j * SIZE + (SIZE - 1)]);
            push(vertices[(j + 1) * SIZE]);
        }
        return { result, normals };
    }
} // namespace OM3D
