#pragma once

#include "resource/Resource.h"
#include <vector>
#include <libultra/gbi.h>

struct TrackWaypointData {
    int16_t posX;
    int16_t posY;
    int16_t posZ;
    uint16_t trackSegment;
};

namespace MK64 {
// Used for binary import from torch
class TrackWaypoints : public Ship::Resource<TrackWaypointData> {
  public:
    using Resource::Resource;

    TrackWaypoints();

    TrackWaypointData* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<TrackWaypointData> TrackWaypointList;
};

// Used for xml
class Paths : public Ship::Resource<TrackWaypointData> {
  public:
    using Resource::Resource;

    Paths();

    TrackWaypointData* GetPointer() override;
    size_t GetPointerSize() override;

    std::vector<std::vector<TrackWaypointData>> PathList;
};

} // namespace MK64
