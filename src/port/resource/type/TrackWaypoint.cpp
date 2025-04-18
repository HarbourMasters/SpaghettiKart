#include "TrackWaypoint.h"
#include "libultraship/libultra/gbi.h"

namespace MK64 {
TrackWaypoints::TrackWaypoints() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
}

TrackWaypointData* TrackWaypoints::GetPointer() {
    return TrackWaypointList.data();
}

size_t TrackWaypoints::GetPointerSize() {
    return TrackWaypointList.size() * sizeof(TrackWaypointData);
}


Paths::Paths() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
}

// I don't know how to return this properly
TrackWaypointData* Paths::GetPointer() {
    printf("Do not LOAD_ASSET an XML track waypoint/path, you need to use dynamic_cast<MK64::Paths>(ResourceLoad())");
    return nullptr;
}

size_t Paths::GetPointerSize() {
    size_t totalWaypoints = 0;
    for (const auto& path : PathList) {
        totalWaypoints += path.size();
    }
    return totalWaypoints * sizeof(TrackWaypointData);
}

} // namespace MK64
