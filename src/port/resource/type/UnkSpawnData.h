#pragma once

#include "resource/Resource.h"
#include <vector>
#include <libultra/gbi.h>
#include <common_structs.h>

struct UnkActorSpawnData {
    /* 0x00 */ Vec3s pos;
    // Techinically only the bottom byte of someId is the "id". The top byte is used for flags.
    /* 0x06 */ s16 someId;
    // Stores the tree's original Y position.
    /* 0x08 */ s16 unk8;
};

namespace MK64 {
class UnkActorSpawn : public Ship::Resource<UnkActorSpawnData> {
  public:
    using Resource::Resource;

    UnkActorSpawn();

    UnkActorSpawnData* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<UnkActorSpawnData> UnkActorSpawnDataList;
};
} // namespace MK64
