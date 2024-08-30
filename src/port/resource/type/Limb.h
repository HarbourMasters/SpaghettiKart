#pragma once

#include <cstdint>
#include <Resource.h>

namespace SF64 {

struct Vec3f {
    float x, y, z;
};

struct Vec3s {
    int16_t x, y, z;
};

struct Gfx;

struct LimbData {
    /* 0x000 */ Gfx* dList;
    /* 0x004 */ Vec3f trans;
    /* 0x010 */ Vec3s rot;
    /* 0x018 */ LimbData* sibling;
    /* 0x01C */ LimbData* child;
}; // size = 0x20

class Limb : public Ship::Resource<LimbData> {
  public:
    using Resource::Resource;

    Limb() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
    }

    LimbData* GetPointer();
    size_t GetPointerSize();

    LimbData mData{};
};
} // namespace SF64
