#pragma once

#include "Resource.h"
#include "ResourceFactoryBinary.h"

namespace MK64 {
class ResourceFactoryBinaryTrackWaypointsV0 : public Ship::ResourceFactoryBinary {
  public:
    std::shared_ptr<Ship::IResource> ReadResource(std::shared_ptr<Ship::File> file) override;
};

} // namespace MK64
