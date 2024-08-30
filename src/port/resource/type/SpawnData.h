#pragma once

#include "resource/Resource.h"
#include <vector>
#include <libultra/gbi.h>
#include <common_structs.h>

struct ActorSpawnData {
    /* 0x00 */ Vec3s pos;
    union {
        /* 0x06 */ u16 someId; // Usually populated, but not necessarily used by all actors types
        /* 0x06 */ s16 signedSomeId;
    };
};

namespace MK64 {
class ActorSpawn : public Ship::Resource<ActorSpawnData> {
  public:
    using Resource::Resource;

    ActorSpawn();

    ActorSpawnData* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<ActorSpawnData> ActorSpawnDataList;
};
} // namespace MK64
