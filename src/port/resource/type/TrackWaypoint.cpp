#include "TrackWaypoint.h"
#include "libultraship/libultra/gbi.h"

namespace MK64 {
TrackWaypoints::TrackWaypoints() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
}

TrackWaypoint* TrackWaypoints::GetPointer() {
    return TrackWaypointList.data();
}

size_t TrackWaypoints::GetPointerSize() {
    return TrackWaypointList.size() * sizeof(TrackWaypoint);
}


Paths::Paths() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
}

// I don't know how to return this properly
TrackWaypoint* Paths::GetPointer() {
    return nullptr;
}

size_t Paths::GetPointerSize() {
    size_t totalWaypoints = 0;
    for (const auto& path : PathList) {
        totalWaypoints += path.size();
    }
    return totalWaypoints * sizeof(TrackWaypoint);
}

} // namespace MK64
