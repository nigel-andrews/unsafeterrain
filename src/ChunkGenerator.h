#ifndef CHUNK_GENERATOR_H
#define CHUNK_GENERATOR_H

#include <memory>

#include "Program.h"
#include "glm/ext/vector_float2.hpp"
#include "graphics.h"
#include "utils.h"
namespace OM3D
{
    class ChunkGenerator : NonCopyable
    {
    public:
        ChunkGenerator();
        void generate(glm::vec2 chunk);

    private:
        std::shared_ptr<Program> program;
        GLHandle buffer;
    };
} // namespace OM3D

#endif // !CHUNK_GENERATOR_H
