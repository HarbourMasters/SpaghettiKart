#pragma once

#include "include/course.h"
#include "resource/Resource.h"
#include <vector>
#include <libultra/gbi.h>

namespace MK64 {
class TrackSectionsClass : public Ship::Resource<TrackSections> {
  public:
    using Resource::Resource;

    TrackSectionsClass();

    TrackSections* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<TrackSections> TrackSectionsList;
};

class TrackSectionsO2RClass : public Ship::Resource<TrackSections> {
  public:
    using Resource::Resource;

    TrackSectionsO2RClass();

    TrackSections* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<TrackSections> TrackSectionsList;
};

} // namespace MK64
