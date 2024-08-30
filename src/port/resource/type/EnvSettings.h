#pragma once

#include "Limb.h"

#include <cstdint>
#include <Resource.h>

namespace SF64 {

struct EnvSettingsData {
    /* 0x00 */ int32_t type;
    /* 0x04 */ int32_t unk_04;
    /* 0x08 */ uint16_t bgColor;
    /* 0x0A */ uint16_t seqId;
    /* 0x0C */ int32_t fogR;
    /* 0x10 */ int32_t fogG;
    /* 0x14 */ int32_t fogB;
    /* 0x18 */ int32_t fogN;
    /* 0x1C */ int32_t fogF;
    /* 0x20 */ Vec3f unk_20;
    /* 0x2C */ int32_t lightR;
    /* 0x30 */ int32_t lightG;
    /* 0x34 */ int32_t lightB;
    /* 0x38 */ int32_t ambR;
    /* 0x3C */ int32_t ambG;
    /* 0x40 */ int32_t ambB;
};

class EnvSettings : public Ship::Resource<EnvSettingsData> {
  public:
    using Resource::Resource;

    EnvSettings() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
    }

    EnvSettingsData* GetPointer();
    size_t GetPointerSize();

    EnvSettingsData mSettings{};
};
} // namespace SF64
