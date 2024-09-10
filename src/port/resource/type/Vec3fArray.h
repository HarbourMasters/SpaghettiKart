#pragma once

#include <cstdint>
#include <Resource.h>
#include <libultraship/libultra/types.h>

namespace SF64 {

struct Vec3fData {
    float x, y, z;
    Vec3fData(float x, float y, float z) : x(x), y(y), z(z) {
    }
};

class Vec3fArray : public Ship::Resource<Vec3fData> {
  public:
    using Resource::Resource;

    Vec3fArray() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
    }

    Vec3fData* GetPointer();
    size_t GetPointerSize();

    std::vector<Vec3fData> mData;
};
} // namespace SF64
