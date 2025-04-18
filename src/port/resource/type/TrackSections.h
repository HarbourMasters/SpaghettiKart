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

typedef struct {
    std::string addr;
    uint8_t surfaceType;
    uint8_t sectionId;
    uint16_t flags;
} TrackSectionsO2R;

namespace MK64 {
class TrackSectionsClass : public Ship::Resource<TrackSectionsI> {
  public:
    using Resource::Resource;

    TrackSectionsClass();

    TrackSectionsI* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<TrackSectionsI> TrackSectionsList;
};

class TrackSectionsO2RClass : public Ship::Resource<TrackSectionsO2R> {
  public:
    using Resource::Resource;

    TrackSectionsO2RClass();

    TrackSectionsO2R* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<TrackSectionsO2R> TrackSectionsList;
};

} // namespace MK64
