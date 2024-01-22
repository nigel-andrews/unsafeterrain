#include "ChunkGenerator.h"

// #include "GL/glew.h"
#include "glfw/src/internal.h"
#include "glm/ext/vector_float2.hpp"

namespace OM3D
{
    ChunkGenerator::ChunkGenerator()
        : program(Program::from_file("shaders/generate.comp"))
    {
        // GLint buffer_location = program->find_attrib("buffer");
    }

    void ChunkGenerator::generate(glm::vec2 chunk)
    {
        // TODO: do the thing
    }
} // namespace OM3D
