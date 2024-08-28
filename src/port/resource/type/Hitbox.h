#pragma once

#include <cstdint>
#include <Resource.h>

namespace SF64 {
class Hitbox : public Ship::Resource<float> {
  public:
    using Resource::Resource;

    float* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<float> mHitbox;
};
} // namespace SF64
