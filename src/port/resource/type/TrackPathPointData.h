#pragma once

#include "ship/resource/Resource.h"
#include <vector>
#include <libultra/gbi.h>
#include <waypoints.h>

namespace MK64 {
// Used for binary import from torch
class TrackPathPointData : public Ship::Resource<TrackPathPoint> {
  public:
    using Resource::Resource;

    TrackPathPointData();

    TrackPathPoint* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<TrackPathPoint> TrackPathPointList;
};

// Used for xml
class Paths : public Ship::Resource<TrackPathPoint> {
  public:
    using Resource::Resource;

    Paths();

    // This struct is really ugly... Sorry
    struct PathObject {
        std::vector<TrackPathPoint> PathList;
        int32_t PathIndex;
    };

    TrackPathPoint* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<PathObject> PathObject;
};

} // namespace MK64
