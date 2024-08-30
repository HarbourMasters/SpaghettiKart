#pragma once

#include "Limb.h"

#include <Resource.h>

namespace SF64 {
class Skeleton : public Ship::Resource<LimbData*> {
  public:
    using Resource::Resource;

    Skeleton() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
    }

    LimbData** GetPointer();
    size_t GetPointerSize();

    std::vector<LimbData*> mLimbs;
};
} // namespace SF64
