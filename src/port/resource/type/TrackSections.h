#pragma once

#include "resource/Resource.h"
#include <vector>
#include <libultra/gbi.h>

typedef struct {
    uintptr_t addr;
    uint8_t surfaceType;
    uint8_t sectionId;
    uint16_t flags;
} TrackSectionsI;

namespace MK64 {
class TrackSectionsClass : public Ship::Resource<TrackSectionsI> {
  public:
    using Resource::Resource;

    TrackSectionsClass();

    TrackSectionsI* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<TrackSectionsI> TrackSectionsList;
};
} // namespace MK64
