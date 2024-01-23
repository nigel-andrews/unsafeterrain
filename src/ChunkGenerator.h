#ifndef CHUNK_GENERATOR_H
#define CHUNK_GENERATOR_H

#include <cstddef>
#include <memory>

#include "Program.h"
#include "TypedBuffer.h"
#include "graphics.h"
#include "utils.h"
namespace OM3D
{
    template <u32 CHUNK_SIZE = 64, u32 COMPUTE_SIZE = 1024>
    class ChunkGenerator : NonCopyable
    {
    public:
        ChunkGenerator();
        void generate(glm::ivec2 offset);

    private:
        std::shared_ptr<Program> program;
        TypedBuffer<GLfloat> buffer;
    };
} // namespace OM3D

#include "ChunkGenerator.hxx"

#endif // !CHUNK_GENERATOR_H
