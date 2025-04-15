#pragma once

#include "resource/Resource.h"
#include <vector>
#include <libultra/gbi.h>

struct TrackWaypoint {
    int16_t posX;
    int16_t posY;
    int16_t posZ;
    uint16_t trackSegment;
};

namespace MK64 {
// Used for binary import from torch
class TrackWaypoints : public Ship::Resource<TrackWaypoint> {
  public:
    using Resource::Resource;

    TrackWaypoints();

    TrackWaypoint* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<TrackWaypoint> TrackWaypointList;
};

// Used for xml
class Paths : public Ship::Resource<TrackWaypoint> {
  public:
    using Resource::Resource;

    Paths();

    TrackWaypoint* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<std::vector<TrackWaypoint>> PathList;
};

} // namespace MK64
