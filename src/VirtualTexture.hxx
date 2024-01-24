#pragma once

#include "VirtualTexture.h"

namespace OM3D
{
    template <u32 PAGE_SIZE, u32 TEXTURE_SIZE>
    VirtualTexture<PAGE_SIZE, TEXTURE_SIZE>::VirtualTexture(
        const std::filesystem::path& virtual_texture_path)
        : virtual_texture_path_(virtual_texture_path)
        , mega_texture_({ TEXTURE_SIZE, TEXTURE_SIZE },
                        ImageFormat::RGBA8_UNORM)
    {}
} // namespace OM3D
