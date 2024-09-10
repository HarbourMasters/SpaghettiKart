#pragma once

#include <cstdint>
#include <Resource.h>
#include <libultraship/libultra/types.h>

namespace SF64 {
struct JointKey {
    /* 0x0 */ uint16_t xLen;
    /* 0x2 */ uint16_t x;
    /* 0x4 */ uint16_t yLen;
    /* 0x6 */ uint16_t y;
    /* 0x8 */ uint16_t zLen;
    /* 0xA */ uint16_t z;
}; // size = 0xC

struct AnimationData {
    /* 0x00 */ int16_t frameCount;
    /* 0x02 */ int16_t limbCount;
    /* 0x04 */ uint16_t* frameData;
    /* 0x08 */ JointKey* jointKey;
}; // size = 0xC

class Animation : public Ship::Resource<AnimationData> {
  public:
    using Resource::Resource;

    Animation() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
    }

    AnimationData* GetPointer();
    size_t GetPointerSize();

    AnimationData mData;

    std::vector<uint16_t> frameData;
    std::vector<JointKey> jointKey;
};
} // namespace SF64
