#pragma once

#include "resource/Resource.h"
#include <vector>
#include <cstdint>

namespace MK64 {

struct MinimapTexture {
    int32_t Width;
    int32_t Height;
    int32_t Channels;
    size_t Size;
    uint8_t* Data;
};

class Minimap : public Ship::Resource<MinimapTexture> {
  public:
    using Resource::Resource;

    Minimap();

    MinimapTexture* GetPointer() override;
    size_t GetPointerSize() override;

    MinimapTexture Texture;
};

} // namespace MK64
