#pragma once

#include <filesystem>
#include <glm/glm.hpp>

#include "LRUCache.h"
#include "Texture.h"
#include "TypedBuffer.h"

#ifndef CACHE_SIZE
#    define CACHE_SIZE (TEXTURE_SIZE * TEXTURE_SIZE) / (PAGE_SIZE * PAGE_SIZE)
#endif
namespace OM3D
{
    using namespace glm;

    template <u32 SIZE>
    struct Page
    {
        // offset.x -> mip level, offset.y -> page index
        Page(const glm::uvec2& offset);
        std::array<u8, SIZE * SIZE> data;
        uvec2 key; // {x == mip level, y == page index}
    };

    // Tile size in pixels (equivalent to a chunk)
    template <u32 PAGE_SIZE = 64, u32 TEXTURE_SIZE = 4096>
    class VirtualTexture
    {
    public:
        VirtualTexture(const std::filesystem::path& virtual_texture_path);

    private:
        std::filesystem::path virtual_texture_path_; // On disk
        Texture page_cache_; // On GPU
        // r = fx, g = fy, b = mip level
        Texture inderection_table_;

        // TODO: One per mip level
        TypedBuffer<uvec2> page_table_; // On GPU

        // Texture streamed to GPU
        LRUCache<glm::uvec2, Page<PAGE_SIZE>, CACHE_SIZE> page_manager_;
    };
} // namespace OM3D
#undef CACHE_SIZE
