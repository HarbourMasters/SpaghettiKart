#pragma once

#include "resource/Resource.h"
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

    TrackPathPoint* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<std::vector<TrackPathPoint>> PathList;
};

} // namespace MK64
