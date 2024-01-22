#pragma once

#include <array>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "utils.h"

namespace OM3D
{
    template <u32 WIDTH>
    struct Chunk : NonCopyable
    {
        u32 id;
        glm::vec2 origin;
        std::array<GLfloat, WIDTH * WIDTH> data;
    };
} // namespace OM3D
