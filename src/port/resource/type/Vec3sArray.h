#pragma once

#include <cstdint>
#include <Resource.h>
#include <libultraship/libultra/types.h>

namespace SF64 {

struct Vec3sData {
    int16_t x, y, z;
    Vec3sData(int16_t x, int16_t y, int16_t z) : x(x), y(y), z(z) {
    }
};

class Vec3sArray : public Ship::Resource<Vec3sData> {
  public:
    using Resource::Resource;

    Vec3sArray() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
    }

    Vec3sData* GetPointer();
    size_t GetPointerSize();

    std::vector<Vec3sData> mData;
};
} // namespace SF64
