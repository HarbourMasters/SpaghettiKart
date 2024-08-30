#pragma once

#include "Resource.h"
#include "ResourceFactoryBinary.h"

namespace SF64 {
class ResourceFactoryBinaryVec3fV0 : public Ship::ResourceFactoryBinary {
  public:
    std::shared_ptr<Ship::IResource> ReadResource(std::shared_ptr<Ship::File> file) override;
};
}; // namespace SF64
