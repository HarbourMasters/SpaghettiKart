#pragma once

#include <cstdint>
#include <Resource.h>
#include <libultraship/libultra/types.h>

namespace SF64 {

struct Vec3s {
    int16_t x, y, z;
};

struct ColPolyData {
    /* 0x00 */ Vec3s tri;
    /* 0x06 */ int16_t unk_06;
    /* 0x08 */ Vec3s norm;
    /* 0x0E */ int16_t unk_0E;
    /* 0x10 */ int32_t dist;
    ColPolyData(Vec3s tri, int16_t unk_06, Vec3s norm, int16_t unk_0E, int32_t dist)
        : tri(std::move(tri)), unk_06(unk_06), norm(std::move(norm)), unk_0E(unk_0E), dist(dist) {
    }
}; // size = 0x14

class ColPoly : public Ship::Resource<ColPolyData> {
  public:
    using Resource::Resource;

    ColPoly() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
    }

    ColPolyData* GetPointer();
    size_t GetPointerSize();

    std::vector<ColPolyData> mColPolys;
};
} // namespace SF64
