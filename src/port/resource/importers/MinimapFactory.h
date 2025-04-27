#pragma once

#include "resource/Resource.h"
#include "resource/ResourceFactoryBinary.h"
#include "resource/ResourceFactoryXML.h"

namespace MK64 {
class ResourceFactoryBinaryMinimapV0 : public Ship::ResourceFactoryBinary {
  public:
    std::shared_ptr<Ship::IResource> ReadResource(std::shared_ptr<Ship::File> file,
                                                  std::shared_ptr<Ship::ResourceInitData> initData) override;
};

} // namespace MK64
